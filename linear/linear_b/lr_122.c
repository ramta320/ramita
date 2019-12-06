#include <stdio.h>

/******************************************************************************
 * This program enables a user to specify a slope and intercept, which are 
 * used to output a series of datapoints that can be plotted in a spreadsheet.
 * This program should be used to visualise your estimate of the solution to
 * your linear regression problem.
 * 
 * To compile:
 *   cc -o lr_122 lr_122.c
 * 
 * To run and direct the output into a file:
 *   ./lr_122
 * 
 * To load results into a spreadsheet:
 *   libreoffice lr01_results.csv
 * 
 * Dr Kevan Buckley, University of Wolverhampton, 2018
 *****************************************************************************/

int main(int argc, char **argv) {
  int i,j;
  double m;
  double c;
  double x;
  double y;
  
 if(argc != 3) {
    fprintf(stderr, "You need to specify a slope and intercept\n");
    return 1;
  }
for ( j=1; j<=3;j++)
{

  sscanf(argv[1], "%lf", &m);
  sscanf(argv[2], "%lf", &c);

}
for ( j=1; j<=3;j++)
{

  for(i=0; i<100; i++) {
    x = i;
    y = (m * x) + c;
    printf("%0.2lf,%0.2lf\n", x, y);
}

  }
  
  return 0;
}

