/*	TO PASS EXAM START	*/
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int    picoshell(char **cmds[])
{
	int		fds[2];
	int		pid;
	int		prev_fd = -1;
	int		i = 0;

	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fds) == -1)
			return (1);

		pid = fork();
		if (pid == 0) // child
		{
			// if not first cmd
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}

			// if not last cmd
			if (cmds[i + 1])
			{
				close(fds[0]);
				dup2(fds[1], STDOUT_FILENO);
				close(fds[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1); // fallback
		}
		// if not first cmd
		if (prev_fd != -1)
			close(prev_fd);

		// if not last cmd
		if (cmds[i + 1])
		{
			close(fds[1]);
			prev_fd = fds[0];
		}
		i++;
	}

	int		status;
	int		exit_code = 0;
	while (wait(&status) != -1)
	{
		if (WIFEXITED(status) && WIFSIGNALED(status) != 0)
			exit_code = 1;
	}
	return (exit_code);
}
/*	TO PASS EXAM END	*/ 