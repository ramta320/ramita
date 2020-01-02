#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <mpi.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    mpicc -o PassCrack_fournum PassCrackMPI_fournum.c -lrt -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    mpirun -n 3 ./PassCrack_fournum

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *passwords_enc[] = {
  "$6$KB$XxrFI/n2PbyeuU/fo9EhOur88MWUsXCPntbPW6.6GTd24iPbFR3RrGqLr7rKkE6W3FPuqwaivlHxzpN1oQE2s1",
  "$6$KB$HEIqRDWaQq6NjXB7LW8Ti9Fdcq6e45jWNN5C0pUYzbMuQw4cNVp7qXLIE.ZXjJ0bLMNkcd9kCdu4uxSvn4qT1/",
  "$6$KB$vY.cdC10gcTiTKkwRDmum9qPl83/mxIjzPrZon05gp4d1WdA6TTslTlAruHqzkduY9GASo/50b2uEo9ZqCVz61",
  "$6$KB$sQ.2lnhed9NrzLwAFzSQE7jo3Zp5ogfrc.OGrEW4/MEEaeIx60IsZ2hN5EHB1FndFc/ISy8iMqjUxWghK1D68."
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void function_1(char *salt_and_encrypted){
  int s,a, t;    
  char salt[7];
  char plain[7];
  char *enc;
  int count = 0;

  substr(salt, salt_and_encrypted, 0, 6);

  for(s='A';s<='M'; s++){
    for(a='A'; a<='Z'; a++){
      
	for(t=0; t<=9999; t++){
	
	//printf("Instance 1:");
        sprintf(plain, "%c%c%04d", s,a,t); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } 
	
      }
    }
  }


  printf("%d solutions explored\n", count);
}

void function_2(char *salt_and_encrypted){
  int s, a, t, m;
  char salt[7];
  char plain[7];
  char *enc;
  int count = 0;

  substr(salt, salt_and_encrypted, 0, 6);

  for(s='N';s<='Z'; s++){
    for(a='A'; a<='Z'; a++){
      for(t=0; t<=9999; t++){
	
	//printf("Instance 1:");
        sprintf(plain, "%c%c%04d", s,a,t); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } 
	
      }
    }
  }

  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main() {

  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

  int size, rank;
  int i;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3){
    if (rank ==0){
	printf("this program needs to run on exactly 3 process to run\n ");
	}
}else{
 if(rank == 0){

	int r;
	MPI_Send(&r, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	MPI_Send(&r, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

}else if(rank==1){
 int num;
 MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  for(i=0; i<n_passwords; i<i++){
    function_1(passwords_enc[i]);
   }
  } else{
    int num;
    MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for(i=0; i<n_passwords; i<i++){
          function_2(passwords_enc[i]);
   }
  }
}
 MPI_Finalize();
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
 



if (rank ==0){

	 printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

}
  return 0;
}
