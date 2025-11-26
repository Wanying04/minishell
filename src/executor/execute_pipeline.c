#include "minishell.h"

int	execute_pipeline(t_command *cmd, t_env *env)
{
	
}

// 在 execute_pipeline 中处理：
// - 不完整管道 "cmd |"
// - 空命令管道 " | cmd"  
// - 多级管道 "cmd1 | cmd2 | cmd3"
// - 管道错误处理

// 管道函数处理所有管道相关边界情况
int execute_pipeline(t_command *first_cmd, t_env *env)
{
    // 检查管道完整性
    // 创建进程和管道
    // 处理管道错误
    // 等待子进程
    // 返回统一状态码
}

int execute_pipeline(t_command *first_cmd, t_env *env)
{
    t_command *current = first_cmd;
    t_command *last_valid_cmd = NULL;
    
    // 找到管道中最后一个有效的命令
    while (current)
    {
        if (current->argv && current->argv[0])
            last_valid_cmd = current;
        
        // 如果遇到管道符但下一个命令无效
        if (current->is_piped && 
            (!current->next || !current->next->argv || !current->next->argv[0]))
        {
            // 模仿 bash：执行到最后一个有效命令，然后报错
            if (last_valid_cmd)
            {
                // 执行有效的管道部分
                return execute_partial_pipeline(first_cmd, last_valid_cmd, env);
            }
            else
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
                return (2);
            }
        }
        current = current->next;
    }
    
    // 正常执行完整的管道
    return execute_full_pipeline(first_cmd, env);
}