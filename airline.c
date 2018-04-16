#include <stdio.h>
#include <string.h> //for strcpy
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h> //for rand test

typedef struct Seat {
        int status; //1 = avail, 0 = empty
        char passenger[32];
        char seatname[2];
        int index;
} Seat;

#define numSeats 30


Seat *seats;

void initArray();
void runProgram();


int main() {

        //Seat *seats;  
        printf("Starting ...\n");

        int flags, opt, fd; //opt?
        mode_t perms;
        size_t size;
        void *addr;


        //why don't these work without O_RDWR?
        flags = O_RDWR | O_CREAT | O_EXCL; //flags

        size = sizeof(Seat)*numSeats;
        perms = S_IRUSR | S_IWUSR;

        fd = shm_open("wtf", flags, perms); //open shared memory object
        if(fd == -1) {
                flags = O_RDWR | O_EXCL;
                fd = shm_open("wtf", flags, perms); //open shared memory object
                printf("shm_open failed with O_CREAT, now using o_excl\n");
        }
        if(ftruncate(fd, size) == -1) {
                printf("ftruncate failed\n");
        }
 seats = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //mmap file descriptor, type cast Seat*?
        if(addr == MAP_FAILED) {
                printf("mmap failed\n");
        }

        char *seatnames[] = {"1A","1B","1C","1D","1E","1F","2A","2B","2C","2D","2E","2F","3A","3B","3C","3D","3E","3F","4A","4B","4C","4D","4E","4F","5A","5B","5C","5D","5E","5F"};
       //fill seat[] 
        for(int i = 0; i < numSeats; i++) {
                seats[i].status = 0; //initialize seat as empty                
                seats[i].index = i;
                strcpy(seats[i].seatname,seatnames[i]);
        }


//      myStuff->x = 21;
/*      myStuff->y[0] = 'A';
        myStuff->y[1] = 'B';
*/
//      printf("%c\n", myStuff->y[r]);
//      printf("ind = %s\n", seats[4].seatname);


        runProgram();

        if(shm_unlink("wtf") == -1) { //close and delete shared memory object, only if u r last program running
                printf("shm_unlink failed\n");
        }
        printf("... finished.\n");
        return 0;
}


void initArray() {


}

void runProgram() {
        while(1) {
                printf("What do you want to do?\n1 List all seats.\n2 Reserve a seat.\n3 List only available seats.\n\n"); // As user what they want
                //int input;
//                scanf("%d",&input); //Wait for user input of 1, 2, or 3
          char input[1];
                scanf ("%[^\n]%*c", input);

                if(strcmp(input,"1") == 0) {
                        printf("\n");
                        for(int b=0;b<numSeats; b++) {
                                char temp_name[32];
                                if(seats[b].status == 0) {
                                        //strcpy(temp_name,"empty"); //Set passenger name to 'empty'
                                        printf("%s - empty\n", seats[b].seatname);
                                  }
                                else {
                                        //strcpy(temp_name,seats[b].passenger);
                                        printf("%s - %s\n", seats[b].seatname, seats[b].passenger);
  //                                    printf("%s\n",seats[b].passenger);
                                }
                        }
                }
                else if(strcmp(input,"2") == 0) { //Reserve a seat
                        char reserve_input[35];
                        printf("\nEnter the seat and passenger name like this: 1A, John Doe\n");
                        scanf ("%[^\n]%*c", reserve_input); //Scan in full line

        //char *token = strtok(reserve_input,","); //Get seatname token

                        char input_seatname[2];
                        char input_passenger[32];

                        strcpy(input_seatname,strtok(reserve_input,",")); //set seat name
                        //strcpy(input_passenger,strtok(NULL,"\n")); //set passenger

                        printf("seatname: %s\n",input_seatname);
//                      printf("passenger: %s\n", input_passenger);
                        //printf("token2: %s\n",token + strlen(token) - 3);

                        //Search for Seat
                        for(int b=0;b<numSeats; b++) {
                                if(strcmp(seats[b].seatname,input_seatname) == 0) { //Seat from array matches user input
//                                      printf("new seatname: %s\n",seats[b].seatname);
                                        seats[b].status = 1; //Set status to taken
                                        strcpy(seats[b].passenger,strtok(NULL,"\n"));
                                        //strcpy(seats[b].seatname,input_passenger); //Set struct.passenger to input name ***ISSUES
                                        //strcpy makes it so seatname is changed :/

//                                      printf("new seatname: %s\n",seats[b].seatname);
                                }
                        }
                        //printf("print: %s\n",reserve_input);

                }
                else if(strcmp(input,"3") == 0) { //List only available seats
                        printf("\nAvailable seats:\n");
                                for(int b=0;b<numSeats; b++) {
                                        if (seats[b].status == 0) {
                                                printf("%s\n", seats[b].seatname);
                                        }
                                 }

                }
                else {
                        printf("Invalid entry\n");
                }
                printf("\n");
        } //end while

}
