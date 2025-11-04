#include "minishell.h"

int	handle_redirections()
{

}

int	execute_external_command(t_command *cmd, t_env *env)
{
	char	*input;

	pid_t	pid;

	pid = fork();

	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		command_ls();
		printf("Hi! I am child process %d\n", pid);
	}
		
	else
		printf("Hi! Parent is here, %d\n", pid);
	printf("Hi! Process ID is %d\n", pid);
	return (0);
}

// execute_external_command()
// 负责「真正执行命令」：
// fork();
// execve();
// 错误处理、退出状态；
// 父进程等待子进程。


