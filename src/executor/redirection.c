#include "minishell.h"

typedef struct s_redir_info {
    char *final_input;      // 最终的输入文件（最后一个 < file）
    char *final_output;     // 最终的输出文件（最后一个 > file）
    int   append_mode;      // 是否是追加模式
    char **heredocs;        // 所有heredoc分隔符数组
    int   heredoc_count;    // heredoc数量
} t_redir_info;

void analyze_redirections(t_redirection *redirects, int count, t_redir_info *info)
{
    // 初始化
    info->final_input = NULL;
    info->final_output = NULL; 
    info->append_mode = 0;
    info->heredoc_count = 0;
    info->heredocs = malloc(sizeof(char *) * count);
    
    // 从右到左扫描（后面的重定向覆盖前面的）
    for (int i = count - 1; i >= 0; i--) {
        switch (redirects[i].type) {
            case REDIR_INPUT:
                if (!info->final_input) {
                    info->final_input = redirects[i].file;
                }
                break;
                
            case REDIR_OUTPUT:
                if (!info->final_output) {
                    info->final_output = redirects[i].file;
                    info->append_mode = 0;
                }
                break;
                
            case REDIR_APPEND:
                if (!info->final_output) {
                    info->final_output = redirects[i].file;
                    info->append_mode = 1;
                }
                break;
                
            case REDIR_HEREDOC:
                // 记录所有heredoc分隔符（顺序重要！）
                info->heredocs[info->heredoc_count++] = redirects[i].file;
                break;
        }
    }
}

int setup_redirection_flow(t_redir_info *info)
{
    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;
    
    // 1. 处理输入流：文件输入 + heredoc串联
    if (info->final_input || info->heredoc_count > 0) {
        input_fd = create_input_stream(info);
        if (input_fd == -1) return FAILURE;
    }
    
    // 2. 处理输出流
    if (info->final_output) {
        output_fd = create_output_stream(info);
        if (output_fd == -1) return FAILURE;
    }
    
    // 3. 应用最终的重定向
    if (input_fd != STDIN_FILENO) {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != STDOUT_FILENO) {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    
    return SUCCESS;
}

int create_input_stream(t_redir_info *info)
{
    int pipefd[2];
    
    if (pipe(pipefd) == -1) return -1;
    
    // 如果有heredoc，先处理它们
    if (info->heredoc_count > 0) {
        process_heredoc_chain(pipefd[1], info->heredocs, info->heredoc_count);
    }
    
    // 如果有文件输入，追加到流中
    if (info->final_input) {
        append_file_to_stream(pipefd[1], info->final_input);
    }
    
    close(pipefd[1]);  // 关闭写入端
    return pipefd[0];  // 返回读取端
}

void process_heredoc_chain(int write_fd, char **delimiters, int count)
{
    char *line;
    
    while (1) {
        line = readline("> ");
        if (!line) {
            // EOF处理
            write(2, "minishell: warning: here-document delimited by end-of-file\n", 59);
            break;
        }
        
        // 检查是否匹配任何一个未完成的delimiter
        int matched = 0;
        for (int i = 0; i < count; i++) {
            if (delimiters[i] && strcmp(line, delimiters[i]) == 0) {
                delimiters[i] = NULL;  // 标记这个delimiter已匹配
                matched = 1;
                break;
            }
        }
        
        if (matched) {
            free(line);
            // 检查是否所有delimiter都匹配了
            if (all_delimiters_matched(delimiters, count)) break;
            continue;
        }
        
        // 写入内容到管道
        write(write_fd, line, strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
}

int handle_all_redirections(t_command *cmd)
{
    t_redir_info info;
    
    // 阶段1：分析重定向
    analyze_redirections(cmd->redirects, cmd->redirect_count, &info);
    
    // 阶段2：建立数据流
    if (setup_redirection_flow(&info) == FAILURE) {
        free(info.heredocs);
        return FAILURE;
    }
    
    free(info.heredocs);
    return SUCCESS;
}
