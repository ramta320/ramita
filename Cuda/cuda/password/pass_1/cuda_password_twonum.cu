#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>

/****************************************************************************
 * 
 *
 * Compile with:
 *   nvcc -o cudapass_twonum cuda_password_twonum.cu
 * 
 * Dr Kevan Buckley, University of Wolverhampton, January 2018
 *****************************************************************************/
__device__ int is_a_match(char *attempt){
char plain_password1[] ="AA24";
char plain_password2[] ="RA33";
char plain_password3[] ="MI22";
char plain_password4[] ="TA44";

char *r = attempt;
char *m = attempt;
char *t = attempt;
char *a = attempt;
char *r1 = plain_password1;
char *r2 = plain_password2;
char *r3 = plain_password3;
char *r4 = plain_password4;

 while(*r ==*r1){
 	if(*r == '\0')
	{
		printf("password:%s\n", plain_password1);
		break;
	}
	r++;
	r1++;
}
while(*m ==*r2){
 	if(*m == '\0')
	{
		printf("password:%s\n", plain_password2);
		break;
	}
	m++;
	r2++;
}
while(*t ==*r3){
 	if(*t == '\0')
	{
		printf("password:%s\n", plain_password3);
		break;
	}
	t++;
	r3++;
}
while(*a ==*r4){
 	if(*a == '\0')
	{
		printf("password: %s\n", plain_password4);
		return 1;
	}
	a++;
	r4++;
}
return 0;
}

__global__ void kernel(){
char n1, n2;

char password[7];
password[6] ='\0';

int i = blockIdx.x +65;
int j = threadIdx.x+65;
char firstMatch =i;
char secondMatch =j;

password[0] =firstMatch;
password[1] =secondMatch;
	for(n1='0'; n1<='9'; n1++){
		for(n2='0'; n2<='9'; n2++){
			
					password[2] =n1;
					password[3] =n2;
					
	if(is_a_match(password)){
	}
	else{
	//printf("tried: %s\n",password);
	}
     }
    }
  }



int time_difference(struct timespec *start, struct timespec *finish,long long int *difference) {
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

  kernel<<<26,26>>>();
cudaThreadSynchronize();
  

 clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 
return 0;
}
