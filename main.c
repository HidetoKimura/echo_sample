#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/fcntl.h>

static char* INPUT_FILE=NULL;
static char* OUTPUT_FILE=NULL;

typedef void (*parse_hook_t)(int argc, char *argv[]);

static void parse_command(char* s, parse_hook_t hook)
{
    #define     DELIM   " \t\n\r"
    #define     ARG_MAX 32
    int32_t     argc;
    char*       argv[ARG_MAX];
    uint32_t    i;
    char*       tok;

    tok = strtok(s, DELIM); 
    if(tok == NULL) {
        return;
    }
        
    argv[0] = tok;
    for(i = 1; i < (sizeof(argv)/sizeof(argv[0])); i++) {
        if((tok = strtok(NULL, DELIM)) == NULL) break;
            argv[i] = tok;
    }
    argc = i;
    
    hook(argc, argv);

    return;
}

static void output_file(char* filename, char* str)
{
    FILE *file;
    file = fopen(filename, "w");

    if (file == NULL) {
        perror("output_file:");
        return;
    }

    fputs(str, file);

    fclose(file);
 }

// usage
// ./echo_sample /tmp/haltp-command &
// echo "dtv-tuneup /tmp/haltp/dtv/channel" > /tmp/haltp-command
// echo "quit" > /tmp/haltp-command
static void external_command_main(int argc, char *argv[])
{
    char buf[4096];

    if(argc < 1) return;

    if(strcmp(argv[0], "dtv-tuneup") == 0) {
        char  out_file[4096];
        char* dir;
        if(argc != 2) {
            printf("usage: echo \"dtv-tuneup [output_tmp_file]\" > [input_fifo_file]\n");
            return;
        }

        OUTPUT_FILE = argv[1];
        snprintf(out_file, sizeof(out_file), "%s", OUTPUT_FILE);
        dir = dirname(out_file);

        snprintf(buf, sizeof(buf), "mkdir -p %s", dir);
        system(buf);

        snprintf(buf, sizeof(buf), "12ch:TV OSaka\n");
        output_file(OUTPUT_FILE, buf);
    }

    return;
}

static void read_fifo(void)
{
    FILE *fp = NULL;
    char buf[4096];
    char* s;
    int fd;
    int len;
   
    if(0 != mkfifo(INPUT_FILE, 0666)) {
        perror("can't create fifo file");
        return;
    }
    
    fd = open(INPUT_FILE, O_RDONLY);
    if(fd == -1){
        perror("can't open fifo file");
        return;
    }

    for(;;) {
        len = read(fd, buf, sizeof(buf)-1);
        if(len == 0) continue;
        s = buf; 
        s[len]='\0';
        printf("read : %s", s);        
        if(strcmp(s, "quit\n") == 0) break;
        parse_command(s, external_command_main);
    }
    close(fd);
}

int main(int argc, char* argv[])
{   
    if(argc!= 2) {
        printf("usage : echo_sample <input_fifo_file> & \n");
        return 0;        
    }

    INPUT_FILE = argv[1];

    remove(INPUT_FILE);
    
    read_fifo();

    remove(INPUT_FILE);

    return 0;
}