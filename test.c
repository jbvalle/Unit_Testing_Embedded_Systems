#include <criterion/criterion.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct PFT_args_t{

    char test_bin[256];
    char test_func[256];

}PFT_args_t;


void *PFT_write_pipe(void *arg){

    printf("WRITE SEQ START\n");
    PFT_args_t *pft_args = (PFT_args_t *)arg;

    char cmd[1024];

    for(int i = 0; i < sizeof(cmd); i++){

        cmd[i] = '\0';
    }

    strcpy(cmd, "python call_gdb.py ");
    strcat(cmd, pft_args->test_bin);
    strcat(cmd, " ");
    strcat(cmd, pft_args->test_func);

    system(cmd);

    return NULL;
}

void *PFT_read_pipe(void *arg){

    // Wait to allow PFT_write_pip() to fetch function return value
    usleep(2000000);

    printf("READ SEQ START\n");
    // Declare file descriptor to pipe
    int fd = open("/tmp/my_pipe", O_RDONLY);

    // Read Pipe message
    char buffer[1024];

    int num_read = read(fd, buffer, sizeof(buffer));

    int *ret_val = (int *)malloc(sizeof(int));

    if(num_read > 0){

        *ret_val = strtol(buffer, NULL, 10);
    }

    close(fd);

    return ret_val;
}

int TEST_func(char *func){

    /*******************************************/
    /* Set input argument for python test func */
    /*******************************************/
    PFT_args_t pft_args;
    strcpy(pft_args.test_bin, "main");
    strcpy(pft_args.test_func, func);

    pthread_t PFT_read_thread, PFT_write_thread;

    pthread_create(&PFT_write_thread, NULL, PFT_write_pipe, (void *)&pft_args);
    pthread_create(&PFT_read_thread, NULL, PFT_read_pipe, NULL);

    int *ret_val;

    pthread_join(PFT_write_thread, NULL);
    pthread_join(PFT_read_thread, (void **)&ret_val);

    return *ret_val;
}

Test(TEST, test1){
    
    cr_expect(TEST_func("add") == 50);
}
