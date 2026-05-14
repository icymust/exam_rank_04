#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

// pipe give fd[0](read), fd[1](write)
//  if pid == 0 then child process otherwise parent

int ft_popen(const char *file, char *const argv[], char type)
{
    int fd[2]; // to store the read and write end
    pid_t pid; // to create process

    // checker for file, argv, and type (we are only accepting r or w)
    if (!file || !argv || (type != 'r' && type != 'w'))
        return (-1);

    // we will pipe the two end of fd and if it failed return the error code
    if (pipe(fd) == -1)
        return (-1);

    // we will try to create the process
    pid = fork();
    // if the process creation failed, close the two end and return the error code
    if (pid == -1)
    {
        close(fd[0]);
        close(fd[1]);
        return (-1);
    }

    // if it is the child process, we will check whether the type is read or write
    if (pid == 0)
    {

        // if the type is to read, then we will try to set the write end or else we will set the read end
        if (type == 'r')
        {
            if (dup2(fd[1], STDOUT_FILENO) == -1){
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }
        }
        else
        {
            if (dup2(fd[0], STDIN_FILENO) == -1){
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }
        }
        // finally, close both of the fds
        close(fd[0]);
        close(fd[1]);
        // then, try to push argv in the place of file
        execvp(file, argv);
        exit(1);
    }
    else
    {
        // if it is the parent process, we will check whether it is read or write
        // if read, we will close the write end and return the read end and vice versa
        if (type == 'r')
        {
            close(fd[1]);
            return (fd[0]);
        }
        else
        {
            close(fd[0]);
            return (fd[1]);
        }
    }
}
// r = команда пишет, parent читает
// w = parent пишет, команда читает

// 0 = in = read
// 1 = out = write

// int main()
// {
//     int fd;
//     char buf[128];
//     int n;
//     fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
//     if (fd == -1)
//         return (1);
//     n = read(fd, buf, 127);
//     if (n > 0)
//     {
//         buf[n] = '\0';
//         write(1, buf, n);
//     }
//     close(fd);
//     return (0);
// }

// int main(void)
// {
//     int fd;
//     char msg[] = "hello from ft_popen\n";
//     fd = ft_popen("cat", (char *const[]){"cat", NULL}, 'w');
//     if (fd == -1)
//         return (1);
//     write(fd, msg, sizeof(msg) - 1);
//     close(fd);
//     return (0);
// }
