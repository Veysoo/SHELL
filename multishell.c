/**

 
 Veysel Göç
 Mustafa Sevgili


*/

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/wait.h>

#include <sys/mman.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <time.h>

#include <string.h>



#define INBUF_SIZE 256



/* these should be the same as singleshell.c */

#define MY_FILE_SIZE 1024

#define MY_SHARED_FILE_NAME "/sharedlogfile"



char *addr = NULL;

int fd = -1;



/* function to initialize shared memory */



void xtermolusturma(int terminalsayisi) {



    for (int i = 0; i < terminalsayisi; i++) {


        int child_pid = fork();

        if (child_pid == 0) {



            char terminalno[5];

            snprintf(terminalno, 5, "%d", i+1);

            char *args[] = {"xterm", "-hold", "-e", "/bin/bash", "-c", "cd /home/kali/Desktop/proje1 && ./a.out", "--", terminalno, NULL};

            execvp(args[0], args);



        } else if (child_pid == -1) {

            perror("error: ");

        }



    }

}



int initmem()

{

    fd = shm_open(MY_SHARED_FILE_NAME, O_RDWR, 0);

    if (fd < 0){

        perror("multishell.c:fd:line34");

        exit(1);

    }

    addr = mmap(NULL, MY_FILE_SIZE,

                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (addr == NULL){

        perror("multishell.c:mmap:");

        close(fd);

        exit(1);

    }

    return 0;

}



void cikti(char *veri)

{

    time_t t;

    struct tm *tarih;

    char zaman[20];

    time(&t);

    tarih = localtime(&t);

    strftime(zaman, 20, "%Y-%m-%d", tarih);/*localtime ý  belirli bir þekilde formatkayarak zamana stringine koyduk ardýndan logdoyaismin eklicez*/



    char logdosyaismi[30];

    snprintf(logdosyaismi, 30, "shelllog-%s.txt", zaman);



    int fd = open(logdosyaismi, O_CREAT | O_WRONLY, 0644);



    if (fd == -1) {

        perror("error: ");

        exit(1);

    }



    write(fd, veri, strlen(veri));



    close(fd);

}









int main(int argc, char *argv[])

{



    int sayi = atoi(argv[1]);



    /* initialize shared memory */

    initmem();



    xtermolusturma(sayi);



    char veri[50]="addr ile dosyanýn içini dolduramadýk";

    cikti(veri);





    /* unmap shared memory */

    munmap(addr, MY_FILE_SIZE);



    /* close shared memory file */

    close(fd);



    return 0;

}
