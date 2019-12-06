#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
/******************************************************************************
 * This program takes an initial estimate of m and c and finds the associated 
 * rms error. It is then as a base to generate and evaluate 8 new estimates, 
 * which are steps in different directions in m-c space. The best estimate is 
 * then used as the base for another iteration of "generate and evaluate". This 
 * continues until none of the new estimates are better than the base. This is
 * a gradient search for a minimum in mc-space.
 * 
 * To compile:
 *   cc -o r122e 122e.c -lm -pthread
 * 
 * To run:
 *   ./r122e
 * 
 * Dr Kevan Buckley, University of Wolverhampton, 2018
 *****************************************************************************/

 int i;
  double bm = 1.3;
  double bc = 10;
  double be;
  double dm[8];
  double dc[8];
  double e[8];
  double step = 0.01;
  double best_error = 999999999;
  int best_error_i;
  int minimum_found = 0;
 
  double om[] = {0,1,1, 1, 0,-1,-1,-1};
  double oc[] = {1,1,0,-1,-1,-1, 0, 1};

typedef struct point_thread {
  double x;
  double y;
} point_thread;

int n_data = 1000;
point_thread data[];

double residual_error(double x, double y, double m, double c) {
  double e = (m * x) + c - y;
  return e * e;
}

double rms_error(double m, double c) {
  int i;
  double mean;
  double error_sum = 0;
 
  for(i=0; i<n_data; i++) {
    error_sum += residual_error(data[i].x, data[i].y, m, c);  
  }
 
  mean = error_sum / n_data;
 
  return sqrt(mean);
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


void *thread_linear() {
 //int *a = args;
 //int i =*a;
 
 

 
      dm[i] = bm + (om[i] * step);
      dc[i] = bc + (oc[i] * step);    
     
      e[i] = rms_error(dm[i], dc[i]);
      if(e[i] < best_error) {
        best_error = e[i];
        best_error_i = i;
	pthread_exit(NULL);
      }
}
     




int main(int argc, char *argv[]){
   struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
 
  pthread_t p_thread[8];
  be = rms_error(bm, bc);

 
 while(!minimum_found) {
    for(i=0;i<8;i++) {
pthread_create(&p_thread[i],NULL,(void*)thread_linear,&i);
pthread_join(p_thread[i],NULL);

printf("minimum m,c is %lf,%lf with error %lf\n", bm, bc, be);
}

if(best_error < be) {
      be = best_error;
      bm = dm[best_error_i];
      bc = dc[best_error_i];
    } else {
      minimum_found = 1;
    }
  }
  printf("minimum m,c is %lf,%lf with error %lf\n", bm, bc, be);
   clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
  }

point_thread data[] = {
  {84.11,145.57},{65.53,115.04},{77.35,154.41},{85.60,136.95},
  {82.90,119.00},{75.84,115.15},{72.16,128.47},{65.76,93.73},
  {78.60,143.18},{22.21,52.70},{31.27,73.05},{96.16,134.88},
  {72.24,126.22},{71.06,120.91},{66.51,130.82},{78.78,146.99},
  {91.89,144.12},{37.23,97.80},{52.44,105.43},{89.42,146.53},
  {27.52,62.83},{42.52,98.87},{77.52,138.96},{11.06,62.58},
  {30.34,70.14},{33.82,107.01},{23.65,54.64},{85.31,147.83},
  {98.99,154.24},{24.48,71.25},{38.62,90.34},{12.05,36.56},
  {46.50,103.33},{96.68,158.81},{45.85,100.69},{73.91,138.58},
  {67.38,122.99},{46.98,89.49},{72.20,111.06},{53.53,117.84},
  {20.44,57.66},{20.56,59.69},{62.44,104.42},{36.11,95.92},
  {89.71,153.40},{46.96,96.54},{38.70,80.15},{16.98,72.63},
  { 0.20,42.40},{99.83,155.96},{54.29,98.39},{46.63,103.13},
  {37.91,77.29},{32.99,81.81},{65.78,111.88},{12.67,57.51},
  {19.69,62.67},{48.96,100.37},{53.03,88.67},{45.30,99.11},
  {15.32,48.05},{62.15,112.51},{50.03,108.92},{17.70,41.10},
  {39.98,82.48},{35.04,87.19},{35.18,99.07},{13.24,64.66},
  {63.71,106.99},{92.08,129.34},{20.25,63.84},{10.04,53.33},
  {41.89,108.63},{86.56,134.15},{60.41,116.67},{93.71,170.14},
  {93.87,154.40},{66.57,107.11},{98.98,182.02},{39.61,72.97},
  {93.35,151.86},{64.58,136.32},{74.82,125.53},{71.48,120.65},
  {15.84,57.71},{75.72,123.11},{65.06,92.14},{57.31,110.87},
  {41.27,83.71},{84.27,152.66},{16.87,55.23},{97.63,160.42},
  {62.53,116.35},{79.48,136.62},{37.77,91.34},{12.41,70.61},
  {21.88,57.77},{43.49,90.93},{20.05,83.56},{96.74,140.56},
  { 2.50,66.69},{48.98,89.73},{67.23,106.14},{ 2.36,42.85},
  {82.45,129.93},{50.51,92.63},{57.14,105.14},{51.37,100.32},
  { 7.05,45.92},{61.26,85.14},{78.19,142.42},{54.42,110.77},
  {26.40,72.47},{ 0.56,43.60},{ 4.87,59.40},{35.87,88.07},
  {27.12,76.30},{12.75,57.97},{76.32,132.82},{54.01,109.61},
  {59.66,122.59},{56.25,113.34},{ 1.91,42.95},{89.39,163.90},
  {87.28,138.89},{33.08,77.15},{84.99,128.53},{79.89,159.20},
  {30.88,76.37},{73.86,123.57},{63.41,120.75},{21.08,63.19},
  {61.83,93.78},{82.68,144.78},{91.69,162.42},{ 2.29,25.55},
  {59.12,108.80},{ 5.27,60.02},{54.90,103.48},{93.09,145.55},
  {68.20,123.35},{10.36,49.99},{74.23,118.46},{55.22,101.34},
  {61.46,113.67},{19.08,60.79},{52.77,114.92},{35.42,66.87},
  {25.98,65.10},{10.87,53.36},{23.20,59.40},{14.33,42.21},
  { 8.34,45.69},{30.97,75.86},{92.60,148.38},{ 3.09,34.39},
  {50.10,92.16},{33.42,86.76},{18.57,51.71},{19.86,63.32},
  { 4.34,56.34},{27.81,82.48},{99.68,159.62},{30.02,84.96},
  {45.42,71.55},{70.52,129.03},{84.76,168.69},{16.09,53.61},
  {72.05,128.53},{42.45,91.63},{31.69,67.36},{10.62,62.28},
  {35.85,79.37},{24.04,86.65},{16.80,51.94},{11.77,35.38},
  {71.54,130.82},{50.39,95.50},{36.28,77.46},{14.91,55.11},
  {29.08,69.94},{47.46,101.57},{48.96,95.80},{90.03,137.80},
  { 6.77,48.94},{90.08,132.00},{ 7.16,55.89},{69.33,123.00},
  {13.35,54.03},{98.65,151.31},{82.60,133.95},{57.03,106.31},
  {98.08,155.76},{75.08,136.69},{91.61,144.00},{32.19,76.43},
  {99.56,174.36},{86.84,142.51},{21.28,79.22},{70.86,116.26},
  { 8.50,46.37},{17.69,64.58},{70.96,111.71},{43.00,87.86},
  {87.32,129.43},{78.17,140.13},{79.99,138.94},{22.42,70.64},
  {21.68,77.16},{71.30,142.78},{64.26,116.42},{99.73,146.83},
  {18.95,76.47},{35.94,78.80},{77.76,134.01},{25.26,84.35},
  {38.80,104.16},{35.70,84.36},{98.92,163.12},{ 1.88,40.13},
  {85.27,156.81},{21.30,61.07},{52.37,124.65},{91.86,143.02},
  {86.64,147.96},{58.21,99.66},{92.31,147.83},{52.97,102.49},
  {75.75,130.56},{12.54,45.38},{82.18,133.26},{96.73,159.55},
  {45.63,99.09},{83.00,126.78},{31.84,69.60},{79.43,136.76},
  {76.44,118.58},{93.98,165.67},{39.51,86.70},{44.87,96.51},
  { 9.35,44.98},{60.19,115.52},{86.76,143.80},{96.88,145.04},
  {94.92,148.50},{51.29,102.25},{74.86,145.14},{85.02,136.40},
  {77.75,138.13},{69.26,101.71},{60.23,100.29},{95.54,159.69},
  {73.42,117.17},{20.08,55.26},{64.39,116.15},{54.03,108.39},
  {66.50,122.92},{15.58,52.18},{ 1.27,62.22},{23.07,83.72},
  {62.51,116.75},{59.87,123.89},{92.38,152.91},{59.68,127.42},
  {82.89,130.97},{63.26,112.78},{66.55,131.16},{64.34,134.24},
  {81.13,122.40},{ 6.54,51.44},{92.93,142.71},{17.17,56.48},
  {85.96,146.11},{39.39,86.79},{67.71,117.80},{22.26,63.76},
  {28.56,87.48},{93.90,146.75},{86.22,144.56},{33.23,80.05},
  {56.38,118.72},{15.20,60.64},{97.02,169.75},{85.90,144.08},
  {82.05,136.33},{62.26,113.11},{12.73,49.86},{ 9.09,51.38},
  {12.05,62.73},{49.31,110.36},{26.02,69.96},{33.55,78.21},
  { 7.56,60.68},{44.63,79.44},{33.75,92.22},{40.18,73.19},
  { 8.05,54.90},{30.03,76.88},{66.22,130.10},{53.21,100.86},
  {36.58,97.06},{ 3.53,55.31},{57.90,111.60},{75.70,137.00},
  {35.54,85.87},{13.40,36.19},{83.05,156.64},{39.82,90.17},
  {17.63,79.38},{ 0.81,37.35},{ 4.19,28.20},{66.61,116.02},
  {29.41,78.65},{92.90,165.47},{22.92,80.65},{70.12,130.57},
  {20.41,67.50},{82.90,127.87},{ 2.98,46.93},{66.05,114.01},
  {33.84,67.54},{82.67,147.59},{57.23,108.57},{ 4.34,47.93},
  {36.02,97.03},{17.98,47.93},{55.05,98.65},{ 9.32,60.33},
  {46.40,87.74},{32.61,87.50},{89.56,136.15},{74.98,138.71},
  {83.53,134.06},{80.88,134.93},{15.06,66.08},{67.89,118.85},
  {49.09,94.50},{38.60,87.76},{32.87,81.86},{95.35,149.97},
  {43.42,79.27},{65.50,109.72},{53.19,92.11},{41.65,106.82},
  {84.99,130.32},{69.69,119.12},{85.44,160.67},{80.30,143.82},
  {72.12,132.77},{79.07,128.88},{68.61,120.14},{88.91,142.59},
  {11.01,46.12},{98.32,174.28},{27.59,70.56},{49.08,107.19},
  { 1.15,33.74},{71.95,137.52},{96.53,156.41},{95.31,150.48},
  {75.76,109.50},{ 9.58,44.36},{31.75,76.40},{ 5.20,53.45},
  { 0.43,41.20},{25.64,64.03},{41.72,100.44},{84.36,128.64},
  {80.52,132.28},{57.26,103.54},{21.15,63.33},{57.68,116.99},
  {93.88,147.27},{41.04,80.34},{94.03,147.21},{70.84,146.21},
  {44.94,86.21},{ 2.45,33.95},{70.83,101.85},{93.06,153.50},
  {44.83,84.66},{79.27,154.18},{ 3.37,43.67},{77.34,145.28},
  {36.64,66.12},{42.31,84.81},{98.28,159.12},{ 8.42,41.04},
  {61.94,88.50},{68.83,123.39},{ 4.50,63.14},{49.00,113.05},
  {94.83,152.88},{ 2.81,45.34},{26.88,84.70},{46.91,97.86},
  {52.29,95.93},{92.78,141.22},{ 6.14,61.62},{37.99,90.25},
  {14.20,59.38},{20.92,63.94},{22.53,51.96},{20.04,52.42},
  {98.04,164.36},{98.93,146.70},{80.56,132.44},{29.40,74.89},
  {18.64,68.82},{46.83,106.53},{90.38,159.50},{49.52,120.68},
  {93.43,144.32},{67.22,126.88},{31.79,68.30},{86.98,141.00},
  {82.95,142.56},{48.10,99.81},{32.30,81.72},{ 7.08,44.62},
  { 5.34,41.57},{31.65,96.58},{83.31,135.51},{93.42,162.65},
  {52.52,112.22},{12.13,77.62},{78.17,135.49},{59.03,115.76},
  {63.34,121.80},{87.95,155.57},{33.06,78.76},{27.73,72.65},
  {66.04,117.97},{87.84,151.65},{83.93,143.20},{74.19,135.15},
  {29.39,61.18},{10.50,73.69},{93.13,158.57},{30.13,77.18},
  { 9.42,48.99},{97.66,166.60},{90.79,148.80},{23.53,67.28},
  {97.95,160.39},{83.45,146.33},{63.05,119.00},{13.96,62.14},
  {95.99,142.45},{97.25,161.02},{33.84,89.22},{70.46,133.95},
  {81.75,141.51},{51.25,111.27},{42.03,85.75},{93.43,159.27},
  {43.44,92.29},{33.98,82.81},{84.75,147.92},{34.91,105.49},
  {15.12,64.03},{31.84,81.26},{79.57,149.04},{ 9.72,45.50},
  {47.94,115.64},{22.62,69.48},{76.13,130.94},{60.08,131.58},
  {35.30,71.65},{14.10,54.87},{ 8.98,52.53},{52.00,119.93},
  {69.73,139.68},{ 3.00,45.08},{51.86,112.37},{15.45,49.68},
  {92.20,154.40},{65.49,111.62},{47.41,94.41},{15.67,73.91},
  {72.81,133.31},{15.88,59.19},{29.18,85.03},{57.11,96.99},
  {23.79,70.55},{60.13,103.64},{51.92,107.56},{67.81,119.60},
  {95.25,157.11},{48.39,104.25},{63.51,127.00},{12.08,62.26},
  {30.41,65.37},{23.38,70.24},{61.15,111.04},{60.97,115.10},
  { 4.33,36.10},{58.09,109.74},{98.30,153.05},{90.58,151.08},
  {86.29,124.95},{25.01,68.22},{67.26,116.66},{32.31,73.73},
  { 7.15,38.84},{80.17,140.91},{26.51,80.51},{65.93,117.24},
  {93.63,154.46},{37.11,85.22},{53.48,102.36},{29.41,87.18},
  {64.05,119.77},{84.23,158.46},{98.28,158.61},{90.93,154.10},
  {96.67,154.16},{21.97,73.29},{46.02,103.08},{86.43,143.05},
  {54.64,115.72},{38.49,84.97},{43.28,104.85},{25.61,72.66},
  {43.58,101.00},{96.50,147.67},{93.98,162.05},{84.06,134.23},
  {29.69,81.32},{ 4.69,36.44},{15.91,53.83},{64.72,110.73},
  {38.84,77.74},{34.16,92.88},{18.74,66.52},{78.76,127.67},
  {46.02,105.73},{43.58,90.20},{74.19,127.97},{93.15,158.87},
  {32.04,90.52},{48.97,88.46},{72.58,109.78},{12.32,52.60},
  {23.67,65.19},{ 3.61,25.72},{29.49,108.49},{19.95,74.75},
  { 5.80,42.80},{83.83,151.43},{13.19,33.61},{20.01,75.52},
  {75.90,144.00},{60.90,113.59},{35.40,92.68},{15.42,53.87},
  {34.88,87.06},{43.16,92.47},{25.96,89.15},{77.58,123.16},
  {71.83,120.04},{ 7.90,48.03},{80.07,133.79},{74.54,133.18},
  {63.77,104.84},{28.43,72.23},{21.48,65.41},{ 2.94,55.96},
  {73.38,146.96},{37.95,71.08},{ 1.03,41.22},{12.19,70.59},
  { 7.74,45.60},{51.94,93.29},{55.55,107.75},{31.41,58.78},
  {78.41,126.94},{42.22,73.71},{22.04,88.47},{27.76,79.96},
  {78.28,137.28},{76.63,142.18},{89.63,152.72},{ 2.29,56.84},
  {92.69,148.15},{66.84,121.22},{69.92,129.48},{94.32,157.96},
  {67.19,129.41},{ 8.87,58.31},{45.84,87.84},{77.37,130.70},
  {58.32,110.64},{17.43,65.18},{65.78,117.68},{38.32,90.86},
  {35.51,78.07},{18.64,51.85},{70.34,104.68},{34.02,68.14},
  {28.66,71.62},{63.30,120.10},{83.14,123.36},{52.41,107.03},
  { 7.79,38.47},{38.10,88.87},{62.17,133.37},{ 5.47,52.20},
  {68.15,133.79},{21.58,56.53},{35.35,98.17},{45.43,79.58},
  {44.38,101.73},{63.20,117.19},{28.03,85.57},{53.62,84.34},
  {71.28,132.65},{27.11,74.93},{19.60,81.50},{69.73,119.51},
  {60.23,111.97},{97.51,151.50},{49.10,86.19},{19.06,89.27},
  {57.92,113.35},{62.52,94.08},{25.18,64.13},{61.26,120.43},
  {25.07,74.82},{44.01,86.79},{37.09,90.57},{94.69,147.81},
  {36.05,73.95},{45.52,94.25},{ 7.51,41.64},{36.80,91.39},
  {65.77,107.56},{81.82,133.93},{98.40,161.59},{26.03,63.51},
  {23.40,94.34},{19.79,57.21},{14.98,64.15},{ 2.43,35.25},
  {61.18,119.94},{96.58,142.39},{92.45,155.38},{70.88,120.94},
  {66.69,113.96},{ 1.51,26.53},{46.04,91.46},{56.07,112.14},
  {33.34,70.10},{52.87,104.03},{43.74,94.41},{89.25,141.49},
  {11.71,45.14},{84.97,141.45},{34.84,79.12},{99.33,186.03},
  {85.52,160.12},{ 5.29,50.99},{50.67,109.04},{22.47,77.02},
  {98.48,161.98},{22.71,66.37},{56.38,109.40},{82.45,137.62},
  {89.12,151.74},{66.75,103.45},{71.70,102.00},{25.95,74.57},
  {61.78,119.79},{15.99,45.43},{39.61,82.62},{99.67,161.15},
  {64.94,106.95},{72.95,137.16},{22.07,68.83},{44.88,100.11},
  {84.39,155.65},{73.74,117.08},{95.43,142.47},{ 9.90,45.83},
  {64.89,125.56},{34.98,96.39},{75.26,129.25},{97.33,158.52},
  {59.31,99.37},{59.15,108.84},{10.10,61.27},{23.80,57.12},
  {81.90,130.81},{25.90,67.94},{57.58,101.84},{88.91,158.55},
  {43.80,101.64},{ 2.07,40.16},{45.15,94.54},{82.88,165.96},
  {94.19,155.67},{93.35,148.13},{55.68,114.07},{29.00,78.76},
  {64.21,130.67},{36.56,83.95},{ 0.03,34.22},{78.64,141.60},
  {71.16,119.35},{47.22,107.86},{12.61,53.05},{14.59,65.55},
  {16.58,60.49},{31.41,67.96},{16.62,45.98},{80.16,133.89},
  {13.24,62.55},{24.44,54.03},{28.83,85.97},{12.92,58.85},
  {28.83,73.23},{36.12,88.70},{87.22,140.84},{43.17,105.75},
  {20.18,67.73},{ 2.63,28.83},{77.45,128.90},{17.18,57.78},
  {25.09,76.89},{60.79,112.18},{ 6.33,42.00},{71.72,115.59},
  {63.42,117.55},{11.40,39.65},{60.95,110.89},{70.60,120.60},
  {78.81,127.09},{42.07,98.76},{33.80,74.03},{34.10,70.54},
  {85.16,136.45},{54.47,117.61},{98.62,150.32},{99.56,150.16},
  {67.31,122.94},{99.36,154.99},{82.18,123.25},{68.03,128.56},
  {76.32,142.47},{54.70,104.26},{71.75,115.77},{15.89,54.72},
  {96.58,151.88},{91.17,133.56},{97.49,157.08},{79.86,139.90},
  {27.84,69.66},{24.29,51.15},{29.15,60.16},{22.04,61.39},
  {57.22,110.63},{42.54,73.88},{34.29,66.70},{69.56,128.52},
  {27.13,77.42},{87.06,160.10},{35.15,101.36},{60.32,116.06},
  {53.30,97.68},{82.05,131.50},{80.45,152.71},{84.24,126.45},
  {27.53,59.98},{31.62,73.47},{ 4.02,48.26},{ 9.26,54.36},
  {41.48,100.87},{11.53,43.45},{26.60,66.07},{30.71,89.83},
  {68.77,138.83},{61.35,101.36},{25.61,72.68},{75.51,133.55},
  {53.01,122.43},{42.08,80.79},{17.69,59.22},{94.58,156.70},
  {88.98,146.47},{50.01,102.71},{49.46,79.53},{70.39,144.56},
  {60.48,132.09},{97.49,143.27},{87.23,159.77},{17.37,62.66},
  {65.82,114.70},{ 8.15,45.78},{59.37,123.81},{82.76,131.84},
  {76.36,133.21},{18.85,78.15},{84.26,153.78},{10.14,31.15},
  {36.58,100.33},{99.71,175.00},{77.74,131.34},{ 9.38,58.88},
  { 1.85,39.26},{39.31,82.78},{56.18,117.65},{99.01,164.36},
  {10.90,59.33},{88.56,127.24},{26.46,68.90},{52.29,110.22},
  {76.39,149.92},{49.73,103.88},{ 8.76,48.06},{49.93,96.20},
  {83.06,146.41},{41.15,100.06},{15.17,58.34},{45.30,80.62},
  {53.03,99.03},{43.13,97.60},{39.83,88.30},{44.34,92.93},
  {91.11,141.13},{45.26,86.78},{11.56,54.49},{49.37,101.10},
  {62.22,110.96},{98.58,158.88},{86.24,146.22},{35.43,89.11},
  {99.25,149.43},{17.57,41.28},{79.16,132.52},{93.52,156.19},
  {17.91,56.69},{83.19,133.88},{ 3.13,39.94},{43.65,98.27},
  {38.62,85.08},{32.57,94.28},{27.01,70.53},{23.35,75.63},
  {11.25,43.73},{74.50,147.39},{40.44,82.83},{31.76,88.14},
  {37.28,74.80},{83.24,145.00},{ 6.01,64.26},{62.95,124.78},
  {78.75,128.50},{62.81,110.47},{90.98,152.61},{30.23,80.51},
  {60.60,114.37},{ 7.93,53.40},{75.51,120.92},{60.55,92.67},
  {15.65,60.61},{74.35,113.68},{32.80,84.02},{42.62,71.51},
  {68.38,104.60},{28.51,74.49},{42.65,83.60},{ 5.30,58.68},
  {78.88,131.30},{ 9.13,54.24},{ 6.48,55.23},{34.62,85.68},
  {89.63,156.54},{50.85,102.34},{30.73,78.53},{47.63,110.93},
  {33.08,97.34},{21.68,66.39},{60.82,106.83},{20.71,93.39},
  {67.60,134.05},{ 4.56,40.02},{61.67,106.28},{95.04,142.65},
  {26.52,67.86},{34.94,100.00},{89.20,122.25},{71.37,136.59},
  {27.50,58.65},{43.86,87.16},{ 4.77,25.70},{49.23,108.90},
  {71.07,117.24},{50.50,92.30},{83.40,153.12},{ 9.89,43.94},
  {21.75,70.55},{86.26,150.46},{96.67,173.73},{ 1.97,32.11},
  {44.79,86.15},{70.33,132.92},{ 5.92,41.52},{38.78,76.93},
  {91.64,142.59},{83.46,119.18},{81.86,143.48},{47.85,97.06},
  {33.34,75.26},{ 3.36,36.57},{63.01,124.74},{33.74,76.04},
  {35.16,61.78},{57.21,106.61},{22.17,58.30},{31.46,80.30},
  {32.84,90.44},{62.10,119.18},{43.31,86.37},{71.54,119.44},
  {74.44,111.38},{21.76,75.50},{90.95,155.81},{85.99,160.71},
  {17.73,74.41},{34.69,80.37},{65.07,99.80},{89.40,143.88},
  {47.30,109.71},{38.98,90.11},{54.63,115.55},{57.21,109.43},
  {75.37,116.79},{62.73,114.08},{48.61,79.26},{10.06,81.21},
  {91.80,146.42},{40.74,96.38},{19.39,46.56},{79.63,121.47},
  {25.33,73.44},{42.18,96.96},{66.07,124.22},{17.51,74.52},
  {84.30,145.63},{53.89,106.36},{93.32,156.13},{68.08,122.57},
  {88.50,160.72},{87.62,160.29},{ 0.79,31.42},{59.53,125.34},
  {57.11,99.28},{31.29,72.30},{84.10,134.21},{ 6.27,61.47},
  {41.30,106.69},{22.71,58.41},{23.36,59.89},{50.02,99.64},
  {29.72,73.95},{82.18,162.90},{78.21,116.64},{15.76,54.01},
  {32.39,87.75},{16.99,61.33},{49.34,90.71},{78.18,124.54},
  {41.99,99.33},{45.57,81.61},{64.80,130.42},{38.91,83.27},
  {47.29,92.09},{97.98,160.30},{95.29,149.15},{16.58,73.58},
  {25.42,64.77},{58.35,106.16},{78.08,137.74},{15.74,72.88},
  {55.08,116.51},{10.99,57.64},{11.39,47.61},{79.74,122.13},
  {57.23,112.24},{97.61,160.82},{64.77,118.66},{37.13,102.96},
  {53.72,92.50},{51.89,112.21},{21.42,60.72},{36.00,75.53},
  {95.01,164.40},{65.01,123.74},{78.82,144.12},{93.08,135.09}
};
