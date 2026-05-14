#include <unistd.h>
#include <stdlib.h>

/*	TO PASS EXAM START	*/
int ft_popen(const char *file, char *const argv[], char type)
{
	int		fds[2]; // fds[read, write]
	int		pid;

	if (!file || !argv || (type != 'w' && type != 'r'))
		return (-1);
	if (pipe(fds) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close(fds[0]), close(fds[1]), -1);
	if (pid == 0) //child
	{
		if (type == 'r')
			dup2(fds[1], STDOUT_FILENO);
		else // type == 'w'
			dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		close(fds[1]);
		execvp(file, argv);
		// end of program (or exit) defaultly closes standard fds that means fd on index 0-2
		// no need to close them manualy
		exit(42); // exit child
	}
	// main process
	if (type == 'r')
		return (close(fds[1]), fds[0]);
	else // type == 'w'
		return (close(fds[0]), fds[1]);
}

/*	TO PASS EXAM END	*/