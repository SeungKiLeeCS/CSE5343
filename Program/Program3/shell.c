// Seung Ki Lee

/*
Below are links to Research Materials I have looked at:

https://brennan.io/2015/01/16/write-a-shell-in-c/
https://www.geeksforgeeks.org/making-linux-shell-c/
https://stackoverflow.com/questions/28502305/writing-a-simple-shell-in-c-using-fork-execvp
https://www.quora.com/What-are-some-good-tutorials-for-writing-a-shell-in-C
https://gist.github.com/parse/966049
https://codereview.stackexchange.com/questions/67746/simple-shell-in-c
https://cboard.cprogramming.com/c-programming/141949-writing-c-shell.html
https://news.ycombinator.com/item?id=13112589
https://stackoverflow.com/questions/34565028/cmpfunc-in-qsort-function-in-c
*/

// LIBRARIES
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

// FUNCTION DECLARATION
void shell_loop(void);
char* read_line(void);
char** parse_line(char* line);
int execute_shell(char** args);
int launch_shell(char** args, int i);

int cmd_clr();
int cmd_dir();
int cmd_environ();
int cmd_quit();
void* cmd_frand(void* vptr);
void* cmd_fsort(void* vptr);
int filesizecheck(char* filename);
int util_compare(const void* first, const void* second);

// CONST & EXTERN
const char* cmd_arr[6] = {"clr", "dir", "environ", "quit", "frand", "fsort"};
extern char** environ;

// MAIN
int main(int argc, char** argv) {
    // seed
    srand(time(0));
    // clear
    system("clear");
    // Run Loop
    printf("Welcome to Seung Ki's Shell.\n");
    shell_loop();

    return EXIT_FAILURE;
}

// FUNCTION IMPLEMENTATION

// MAIN LOOP
void shell_loop(void) {
    char* command;
    char** commandarr;
    int status;

    do {
        printf("LEE$ ");
        command = read_line();
        commandarr = parse_line(command);
        status = execute_shell(commandarr);

        // Free Dynamically Allocated Memory
        free(commandarr);

    } while (status);
} 

// READ LINE
char* read_line(void) {
    char* line = NULL;
    ssize_t buffersize=0;
    getline(&line, &buffersize, stdin);
    return line;
}

// PARSE LINE
char** parse_line(char* line) {
    int buffersize = 64;
    int index = 0;
    char** tokenarr = malloc(buffersize * sizeof(char*));
    char* token;

    // Memory Allocation
    if(!tokenarr) {
        fprintf(stderr, " Malloc Error \n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \n");

    while(token != NULL) {
        tokenarr[index] = token;
        index++;

        if (index >= buffersize) {
            buffersize += realloc(tokenarr, buffersize * sizeof(char*));
            if (!tokenarr) {
                fprintf(stderr, " Malloc Error \n");
                exit(EXIT_FAILURE);                
            }
        }

        token = strtok(NULL, " \n");
    }

    tokenarr[index] = NULL;

    return tokenarr;
}

// CHECK COMMANDS
int execute_shell(char** args) {
    // Edge Case for just Enter
    if (args[0] == NULL) {
        return 1;
    }

    int systemcall = -1;

    for(int i=0; i<6; i++) {
        // if the argument provided matches list of comments
        if(strcmp(args[0], cmd_arr[i]) == 0) {
            launch_shell(args, i);
            return 1;
        }
    }

    // else call system
    system(args[0]);
    return 1;
}

int launch_shell(char** args, int i) {
    if(cmd_arr[i] == "clr") {
        return cmd_clr();
    }
    else if(cmd_arr[i] == "dir") {
        return cmd_dir();
    }
    else if (cmd_arr[i] == "environ") {
        return cmd_environ();
    }
    else if (cmd_arr[i] == "quit") {
        return cmd_quit();
    }
    else if (cmd_arr[i] == "frand") {
        // spawn thread
        pthread_t tid1;
        pthread_create(&tid1, NULL, cmd_frand, &args);
        pthread_join(tid1, NULL);
        printf("File Created\n");
    }
    else if (cmd_arr[i] == "fsort") {
        pthread_t tid2;
        pthread_create(&tid2, NULL, cmd_fsort, &args);
        pthread_join(tid2, NULL);
        printf("File Sorted\n");
    }
}

// COMMAND IMPLEMENTATION
// CLR
int cmd_clr() {
    system("clear");
    return 1;
}

// DIR
int cmd_dir() {
    system("ls -al");
    return 1;
}

// ENVIRON
int cmd_environ() {
    char** env = environ;
    while(*env) {
        printf("%s\n", *env++);
    }
    return 1;
}

// QUIT
int cmd_quit() {
    exit(0);
}

// FRAND
void* cmd_frand(void *vptr) {
    char*** paramarr = (char***) vptr;
    char** param = *paramarr;

    FILE* fp;
    fp = fopen(param[1], "w");
    int size = atoi(param[2]);

    for(int i=0; i<size; i++) {
        int r_int = rand();
        fprintf(fp, "%d\n", r_int);
    }

    fclose(fp);

    return NULL;
}

// FSORT
void* cmd_fsort(void* vptr) {
    char*** paramarr = (char***) vptr;
    char** param = *paramarr;

    int filesize = filesizecheck(param[1]);
    int nparamarr [filesize];
    
    FILE* fp = fopen(param[1], "r");
    int fscan_iter = 0;

    // Check for the length of the file first for quicksort
    for(int i=0;fscanf(fp, "%d", &fscan_iter) && !feof(fp); i++) {
        nparamarr[i] = fscan_iter;
    }

    fclose(fp);

    // Calculate the size 
    int arrlen = sizeof(nparamarr) / sizeof(int);

    // Built in Quicksort
    qsort(nparamarr, arrlen, sizeof(int), util_compare);

    // Now write to the file
    fp = fopen(param[1], "w");
    for (int i=0; i < arrlen; i++) {
        fprintf(fp, "%d\n", nparamarr[i]);
    }

    fclose(fp);
}

// UTILITY FUNCTIONS -> to use built in qsort()

// FILESIZE
int filesizecheck(char* filename) {

    // open the file
    FILE* fp = fopen(filename, "r");

    int num_count = 0;
    int check;

    // Count New line Chars to see individual numbers
    while (EOF != (check=getc(fp))) {
        if('\n' == check) {
            num_count ++;
        }
    }

    fclose(fp);
    return num_count;
}

// UTIL_COMPARE
int util_compare(const void* first, const void* second) {
    return (*(int*)first - *(int*)second);
}

// RESULTS

// seungkil@genuse34.engr.smu.edu$ ./a.out
// Welcome to Seung Ki's Shell.
// LEE$ clr
// clr

// LEE$ dir
// dir

// total 32
// drwx------ 2 seungkil cseugrad  4096 Mar  9 19:47 .
// drwx------ 3 seungkil cseugrad  4096 Mar  9 18:22 ..
// -rwx------ 1 seungkil cseugrad 13268 Mar  9 19:47 a.out
// -rw------- 1 seungkil cseugrad  6849 Mar  9 19:47 shell.c

// LEE$ environ
// environ
// HOSTNAME=genuse34.engr.smu.edu
// TERM=xterm
// SHELL=/bin/bash
// HISTSIZE=1000
// SSH_CLIENT=10.8.2.85 3811 22
// MORE=-c
// QTDIR=/usr/lib64/qt-3.3
// QTINC=/usr/lib64/qt-3.3/include
// SSH_TTY=/dev/pts/2
// CDPATH=.:..:../..:/users7/cse/seungkil
// USER=seungkil
// LD_LIBRARY_PATH=/usr/local/es6/lib64:/usr/local/es6/lib
// LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=01;05;37;41:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lz=01;31:*.xz=01;31:*.bz2=01;31:*.tbz=01;31:*.tbz2=01;31:*.bz=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.rar=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=01;36:*.au=01;36:*.flac=01;36:*.mid=01;36:*.midi=01;36:*.mka=01;36:*.mp3=01;36:*.mpc=01;36:*.ogg=01;36:*.ra=01;36:*.wav=01;36:*.axa=01;36:*.oga=01;36:*.spx=01;36:*.xspf=01;36:
// MAIL=/var/spool/mail/seungkil
// PATH=/usr/local/es6/bin:/usr/local/jdk1.8.0_31/bin:/usr/local/synopsys/wrapper:/usr/local/cds2008/wrapper:/usr/local/mentor/wrapper:/usr/local/x86_64/bin:/usr/local/bin:/usr/bin:/bin:/usr/bin/X11:/usr/games:/usr/local/tex/bin/i686-pc-linux-gnu:/usr/local/eclipse:/usr/local/cad/bin:/usr/local/ncl/bin:/usr/local/cwm:/usr/local/ant/bin
// EXINIT=:set ai aw ic sw=2 sm redraw nowarn dir=/usr/tmp
// PWD=/users7/cse/seungkil/OS/Program3
// NCARG_ROOT=/usr/local/ncl
// JAVA_HOME=/usr/local/jdk1.8.0_31
// EDITOR=vi
// LANG=en_US.UTF-8
// MODULEPATH=/usr/share/Modules/modulefiles:/etc/modulefiles
// LOADEDMODULES=
// ignoreeof=
// SSH_ASKPASS=/usr/libexec/openssh/gnome-ssh-askpass
// HISTCONTROL=ignoredups
// SHLVL=1
// HOME=/users7/cse/seungkil
// noclobber=
// LOGNAME=seungkil
// VISUAL=vi
// MGLS_LICENSE_FILE=1717@genuse32
// QTLIB=/usr/lib64/qt-3.3/lib
// CVS_RSH=ssh
// SSH_CONNECTION=10.8.2.85 3811 129.119.123.64 22
// MODULESHOME=/usr/share/Modules
// LESSOPEN=||/usr/bin/lesspipe.sh %s
// G_BROKEN_FILENAMES=1
// NAG_KUSARI_FILE=/usr/local/lib/NAG_Fortran/nag.license
// BASH_FUNC_module()=() {  eval `/usr/bin/modulecmd bash $*`
// }
// _=./a.out
// OLDPWD=/users7/cse/seungkil/OS

// LEE$ frand this.txt 10
// frand
// this.txt
// 10
// LEE$ quit
// quit
// seungkil@genuse34.engr.smu.edu$ ls
// a.out  shell.c  this.txt

// seungkil@genuse34.engr.smu.edu$ nano this.txt
// 1893261680
// 273255261
// 1127421514
// 127786426
// 1300852523
// 362875276
// 1573504802
// 1708657152
// 2003836874
// 1494563684

// Welcome to Seung Ki's Shell.

// LEE$ fsort this.txt
// fsort
// this.txt

// LEE$ quit
// quit

// seungkil@genuse34.engr.smu.edu$ nano this.txt

// 127786426
// 273255261
// 362875276
// 1127421514
// 1300852523
// 1494563684
// 1573504802
// 1708657152
// 1893261680
// 2003836874
