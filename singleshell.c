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

#define MY_FILE_SIZE 1024

#define MY_SHARED_FILE_NAME "/sharedlogfile"



char *addr = NULL;

int fd = -1;



int initmem()

{

    fd = shm_open(MY_SHARED_FILE_NAME, O_RDWR, 0);

    if (fd < 0){

        perror("singleshell.c:fd:line31");

        exit(1);

    }

    addr = mmap(NULL, MY_FILE_SIZE,

                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (addr == NULL){

        perror("singleshell.c:mmap:");

        close(fd);

        exit(1);

    }

    return 0;

}



int main(int argc, char *argv[])

{

    initmem();

    

    char inbuf[256] = {'\0'};

    int nbyte;

    time_t t = time(NULL);

    struct tm *local_time = localtime(&t);

    

    int saat = (local_time->tm_hour)+7;    /*türkiye saatine göre düzenlendi*/

    int dakika = local_time->tm_min;

    int saniye = local_time->tm_sec;

    int gun = local_time->tm_mday;

    int ay = local_time->tm_mon + 1;

    int yil = local_time->tm_year + 1900;

   





    while (1) {


        write(1, "$", 2);



        if ((nbyte = read(0, inbuf, 255)) <= 0) {

            perror("input <=0 byte");

        } else {

            inbuf[nbyte - 1] = '\0';

        }






        if (strncmp(inbuf, "exit", 4) == 0) {

            printf("Child process id: %d\n", getpid());

            printf("Parent process id: %d\n", getppid());

            printf("Saat:%d.%d.%d \n", saat, dakika, saniye);

            printf("Tarih : %d/%d/%d\n", gun, ay, yil);

            exit(0);

        }





        if (strncmp(inbuf, "cd ", 3) == 0) {

            char *directory = &inbuf[3];

            if (chdir(directory) == -1) {

                perror("cd");

            }

            continue;

        }



        int child_pc = fork();



        if (child_pc == 0) {



            char *args[256];

            char *token = strtok(inbuf," ");

            int i = 0;

            while(token != NULL){

                args[i++]=token;

                token = strtok(NULL, " ");

            }

            args[i]=NULL;



  

            int result = execvp(args[0], args);



        

            if (result == -1) {

                char command[256] = "/bin/";

                strncat(command, args[0], 256);

                args[0] = command;

                execvp(args[0], args);

                perror("error");

                exit(1);

            }

        } else {

            waitpid(child_pc, NULL, 0);

        }

    }



    munmap(addr, 1024);


     close(fd);

}
