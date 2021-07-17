#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/fcntl.h>

#define INPUT_FILE "/tmp/command"

static void test_fgets(void)
{
    FILE *fp = NULL;
    int count = 0;
    char buf[4096];
    char *s = NULL;
    int fd = 0;

    if ((fd = creat(INPUT_FILE, 0666)) < 0) {
        perror("File creation error.");
        return;
    }
    close(fd);

    for(;;) {
        if(fp == NULL) {
            fp = fopen(INPUT_FILE, "r"); 
            if(fp == NULL) {
                break;
            }
        }    
        s = fgets(buf, sizeof(buf), fp);
        if(s != NULL) {
            count++;
            printf("%d: %s", count, s);
            fflush(stdout);
            if(0 == strcmp(s, "end\n")) {
                break;
            }
        }
        else {
            fclose(fp);
            fp = NULL;
            printf("waiting for file write. \n");
            sleep(10);
            fflush(stdout);
        }
    }

    return;
}
static void test_fifo(void)
{
    FILE *fp = NULL;
    int count = 0;
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
        count++;
        printf("%d: %s", count, s);
        fflush(stdout);
        if(0 == strcmp(s, "end\n")) {
            break;
        }
    }
    close(fd);
}

int main(int argc, char* argv[])
{   

    remove(INPUT_FILE);
    
 //   test_fgets();
    test_fifo();

    remove(INPUT_FILE);
    return 0;
}