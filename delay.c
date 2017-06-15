#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

void delay(long int t)
{ 
	sleep(abs(t/1000));	
}