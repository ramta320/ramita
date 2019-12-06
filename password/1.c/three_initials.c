#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code.

  Compile with:
    cc -o three_initials three_initials.c -lcrypt

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./three_initials> results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$YwnKREn7s6H1JzXPaNmZgKbzie7OUMGGnZ.fq73XG79JmXSPPkrtJmDCHMYPBQU/Dv0SWB2VQMSc6cb.sxFB5/",
  "$6$KB$x.f.eqjjEfFCZzekDwrJFS.RZMLgNYQ39CkW/QqG3EfGgyitjmx1Ak4D3DjzFlQUzRGkyKyKjfvoEj80I7L2S.",
  "$6$KB$B7j5z9/F/IMhg2xVofZnNEN8pd68VRwd0X/wUMNGKx8PRi/KOVEE8onJkps0XrymM1IiKfEyzOzbWLmesjP0P0",
  "$6$KB$oUorXCNjioAaWJsTRQmoCNxy/5cobhh7l9WxpTxfxm2puw3pLIipj0KW.wbihiyNRy4J5ruiAuUwI.qCp8HPq0"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void crack(char *salt_and_encrypted){
  int r, m, t,a;     // Loop counters
  char salt[7];    // String used in hahttps://www.youtube.com/watch?v=L8yJjIGleMwshing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(r='A'; r<='Z'; r++){
    for(m='A'; m<='Z'; m++){
      for(t='A'; t<='Z'; t++){
       for(a=0; a<=99; a++){
        sprintf(plain, "%c%c%c%02d", r,m,t,a);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
}
  printf("%d solutions explored\n", count);
}


//Calculating time

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
 {
	  long long int ds =  finish->tv_sec - start->tv_sec; 
	  long long int dn =  finish->tv_nsec - start->tv_nsec; 

	  if(dn < 0 ) {
	    ds--;
	    dn += 1000000000; 
  } 
	  *difference = ds * 1000000000 + dn;
	  return !(*difference > 0);
}
int main(int argc, char *argv[])
{
  	int i;
	struct timespec start, finish;   
  	long long int time_elapsed;

  	clock_gettime(CLOCK_MONOTONIC, &start);

  	for(i=0;i<n_passwords;i<i++) 
	{
    		crack(encrypted_passwords[i]);
  	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	  time_difference(&start, &finish, &time_elapsed);
	  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		                                 (time_elapsed/1.0e9)); 
  return 0;
}
