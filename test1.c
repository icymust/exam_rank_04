// #include <unistd.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <signal.h>
// #include <errno.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdbool.h>

// int ft_popen(const char *file, char *const argv[], char type){
//     if (!file || !argv || (type != 'r' && type != 'w'))
//         return (-1);

//     int fd[2];
//     pid_t pid;

//     if (pipe(fd) == -1)
//         return (-1);

//     pid = fork();
//     if (pid < 0){
//         close(fd[0]);
//         close(fd[1]);
//         return (-1);
//     }

//     if (pid == 0){
//         if (type == 'r'){
//             if (dup2(fd[1], STDOUT_FILENO) == -1)
//                 _exit(127);
//         } else {
//             if (dup2(fd[0], STDIN_FILENO) == -1)
//                 _exit(127);
//         }
//         close(fd[0]);
//         close(fd[1]);
//         execvp(file, argv);
//         _exit(127);
//     }

//     if (type == 'r'){
//         close(fd[1]);
//         return (fd[0]);
//     } else {
//         close(fd[0]);
//         return (fd[1]);
//     }
// }

// int sandbox(void (*f)(void), unsigned int timeout, bool verbose){
//     struct sigaction sa;
//     pid_t pid;
//     int status;

//     sa.sa_handler = SIG_IGN;
//     sa.sa_flags = 0;
//     sigemptyset(&sa.sa_mask);
//     if (sigaction(SIGALRM, &sa, NULL) == -1)
//         return (-1);

//     pid = fork();
//     if (pid < 0)
//         return (-1);

//     if (pid == 0){
//         f();
//         _exit(0);
//     }

//     alarm(timeout);
//     if (waitpid(pid, &status, 0) == -1){
//         if (errno == EINTR){
//             kill(pid, SIGKILL);
//             waitpid(pid, NULL, 0);
//             if (verbose)
//                 printf("Bad function: timed out after %u seconds\n", timeout);
//             alarm(0);
//             return (0);
//         }
//         alarm(0);
//         return (-1);
//     }

//     alarm(0);

//     if (WIFEXITED(status)){
//         int code = WEXITSTATUS(status);
//         if (code == 0){
//             if (verbose)
//                 printf("Nice function!\n");
//             return (1);
//         } else {
//             if (verbose)
//                 printf("Bad function: exited with code %d\n", code);
//             return (0);
//         }
//     }

//     if (WIFSIGNALED(status)){
//         if (verbose)
//             printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
//         return (0);
//     }

//     return (-1);
// }

// int picoshell(char **cmds[]){
//     if (!cmds)
//         return (1);

//     int i = 0;
//     int prev_fd = -1;
//     pid_t pid;
//     int fd[2];

//     while (cmds[i]){
//         if (cmds[i + 1]){
//             if (pipe(fd) == -1){
//                 if (prev_fd != -1)
//                     close(prev_fd);
//                 return (1);
//             }
//         }

//         pid = fork();
//         if (pid < 0){
//             if (cmds[i + 1]){
//                 close(fd[0]);
//                 close(fd[1]);
//             }
//             if (prev_fd != -1)
//                 close(prev_fd);
//             while (wait(NULL) > 0)
//                 ;
//             return (1);
//         }

//         if (pid == 0){
//             if (prev_fd != -1){
//                 dup2(prev_fd, STDIN_FILENO);
//             }
//             if (cmds[i + 1]){
//                 dup2(fd[1], STDOUT_FILENO);
//             }
//             if (prev_fd != -1)
//                 close(prev_fd);
//             if (cmds[i + 1]){
//                 close(fd[0]);
//                 close(fd[1]);
//             }
//             execvp(cmds[i][0], cmds[i]);
//             _exit(127);
//         }

//         if (prev_fd != -1)
//             close(prev_fd);
//         if (cmds[i + 1]){
//             close(fd[1]);
//             prev_fd = fd[0];
//         } else {
//             prev_fd = -1;
//         }
//         i++;
//     }

//     if (prev_fd != -1)
//         close(prev_fd);

//     int status;
//     int result = 0; /* 0 on success, 1 if any child failed */
//     while (wait(&status) > 0){
//         if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
//             result = 1;
//         if (WIFSIGNALED(status))
//             result = 1;
//     }
//     return (result);
// }


//-------------------------------------------

// #include <unistd.h>
// #include <stdlib.h>

// int ft_popen(const char *file, char *const argv[], char type){
//     if(!file || !argv || (type != 'r' && type != 'w'))
//         return(-1);
//     int fd[2];
//     pid_t pid;
    
//     if(pipe(fd) == -1){
//         return(-1);
//     }

//     pid = fork();

//     if(pid<0){
//         close(fd[0]);
//         close(fd[1]);
//         return(-1);
//     }

//     if(pid == 0){
//         if(type == 'r'){
//             dup2(fd[1], STDOUT_FILENO);
//         }else{
//             dup2(fd[0], STDIN_FILENO);
//         }

//         close(fd[0]);
//         close(fd[1]);
//         execvp(file, argv);
//         exit(0);
//     }

//     if(type == 'r'){
//         close(fd[1]);
//         return(fd[0]);
//     }else{
//         close(fd[0]);
//         return(fd[1]);
//     }

// }


// #include <stdbool.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <signal.h>
// #include <errno.h>

// void sig_handler(int sig){
//     (void)sig;
// }

// int sandbox(void (*f)(void), unsigned int timeout, bool verbose){
//     int status;
//     pid_t pid;
//     struct sigaction sa;

//     sa.sa_handler = sig_handler;
//     sa.sa_flags = 0;
//     sigemptyset(&sa.sa_mask);

//     sigaction(SIGALRM, &sa, 0);

//     pid = fork();

//     if(pid <0)
//         return(-1);
//     if(pid == 0){
//         f();
//         exit(0);
//     }

//     alarm(timeout);

//     if(waitpid(pid, &status, 0) == -1){
//         if(errno == EINTR){
//             kill(pid, SIGKILL);
//             waitpid(pid, NULL, 0);
//             if(verbose)
//                 printf("Bad function: timed out after %u seconds\n", timeout);
//             return(0);
//         }
//         return(-1);
//     }

//     if(WIFEXITED(status)){
//         if(WEXITSTATUS(status) == 0){
//             if(verbose)
//                 printf("Nice function!\n");
//             return(1);
//         }
//         else{
//             if(verbose)
//                 printf("Bad function: exited with code %d\n", WIFEXITED(status));
//             return(0);
//         }
//     }

//     if(WIFSIGNALED(status)){
//         if(verbose)
//             printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
//         return(0);
//     }
//     return(-1);

// }

// #include <unistd.h>
// #include <stdlib.h>

// int    picoshell(char **cmds[]){
//     int i = 0;
//     int prev_fd = -1;
//     int fd[2];
//     pid_t pid;

//     while(cmds[i]){
//         if(cmds[i+1] && pipe(fd) == -1){
//             return(1);
//         }

//         pid = fork();
//         if(pid <0)
//         {
//             close(fd[0]);
//             close(fd[1]);
//             return(1);
//         }

//         if(pid == 0){
//             if(prev_fd != -1){
//                 dup2(prev_fd, STDIN_FILENO);
//                 close(prev_fd);
//             }

//             if(cmds[i+1]){
//                 close(fd[0]);
//                 dup2(fd[1], STDOUT_FILENO);
//                 close(fd[1]);
//             }

//             execvp(cmds[i][0], cmds[i]);
//             exit(0);
//         }

//         if(prev_fd != -1){
//             close(prev_fd);
//         }

//         if(cmds[i+1]){
//             close(fd[1]);
//             prev_fd = fd[0];
//         }
//         i++;
//     }

//     int status;
//     int exit_code = 0;

//     while(wait(&status) != -1){
//         if(WIFEXITED(status) && WEXITSTATUS(status)!= 0)
//             exit_code = 1;
//         if(WIFSIGNALED(status))
//             exit_code = 1; 
//     }
//     return(exit_code);
// }



#include <stdio.h>
#include <stdlib.h> // change this to <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

// node *n(**s);

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

//...
node *parse_sum(char **s);
node *parse_multi(char **s);

node *parse_union(char **s){
    if(**s >= '0' && **s <= '9'){
        node *n = new_node((node){VAL, **s-'0', NULL, NULL});
        (*s)++;
        return(n);
    }
    else if (accept(s, '(')){
        node *lhs = parse_sum(s);
        if(!expect(s, ')')){
            destroy_tree(lhs);
            return(NULL);
        }
        return(lhs);
    }
    unexpected(**s);
    return(NULL);
}

node *parse_multi(char **s){
    node *lhs = parse_union(s);
    if(!lhs)
        return(NULL);
    while(accept(s, '*')){
        node *rhs = parse_union(s);
        if(!rhs){
            destroy_tree(lhs);
            return(NULL);
        }
        lhs = new_node((node){MULTI, 0, lhs, rhs});
    }
    return(lhs);
}

node *parse_sum(char **s){
    node *lhs = parse_multi(s);
    if(!lhs)
        return(NULL);
    while(accept(s, '+')){
        node *rhs = parse_multi(s);
        if(!rhs){
            destroy_tree(lhs);
            return(NULL);
        }
        lhs = new_node((node){ADD, 0, lhs, rhs});
    }
    return(lhs);
}

node    *parse_expr(char *s)
{
    //...
    node *ret = parse_sum(&s);
    if(!ret)
        return(NULL);

    if (*s) 
    {
        unexpected(*s);
        destroy_tree(ret);
        return (NULL);
    }
    return (ret);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}