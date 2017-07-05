//CSCE 312: Lab-1 Problem-5 framework
// This is version-2; bugfix for timediff
/* ***   README   **** */
/*This file is a framework: i.e. there is no actual code whose execution time will be
measured. You need to populate the appropriate functions with the code that you wrote 
for the previous problems in order to get useful data. 

Turning in this file without your code will result in zero points being awarded for this problem.

run this file as: gcc FileName.c -o ExecutableName -lrt 

*/




#include <stdio.h>
#include <time.h>

// Macro definitions to ensure portablity between both sun.cs and linux.cs

#if defined(sun)
    #define CLOCKNAME CLOCK_HIGHRES
        
#else
    #define CLOCKNAME CLOCK_PROCESS_CPUTIME_ID
#endif
unsigned char input;

unsigned int bell;
unsigned int door_lock;
unsigned int brake;
int values[8];

enum {

			PATTERN_1 = 1,
			PATTERN_2 = 2,
			PATTERN_3 = 4,
			PATTERN_4 = 8,
			PATTERN_5 = 16,
			PATTERN_6 = 32,
			PATTERN_7 = 64,
			PATTERN_8 = 128,

	};
	
	
void read_inputs_from_ip_if(){

	//place your input code here
	//to read the current state of the available sensors
	FILE * fPointer;
	fPointer = fopen("input.txt", "r");
	int i = 0;
	while(!feof(fPointer)){
		fscanf(fPointer,"%lld", &values[i]);
		i++;
	}
	
	fclose(fPointer);
	int j = 0;
	int r = 0;
	while(j < 8){
		input |= (values[r] << j);
		j++;
		r++;
	}

}

void write_output_to_op_if(){

	//place your output code here
    //to display/print the state of the 3 actuators (DLA/BELL/BA)
	FILE * fPointer;
	fPointer = fopen("prob5_output.txt", "w");
	int i = 0;
	fprintf(fPointer, "Input: ");
	while(i < 8){
		fprintf(fPointer, "%d", values[i]);
		i++;
	}
	fprintf(fPointer, "\r\n");
	fprintf(fPointer, "bell = %d",bell);
	fprintf(fPointer, "\r\n");
	fprintf(fPointer, "door_lock = %d",door_lock);
	fprintf(fPointer, "\r\n");
	fprintf(fPointer, "brake = %d",brake);
	fclose(fPointer);
}
	
unsigned int checkBell(unsigned char passed_input) 
{
	unsigned int returned_output;
	
	//To implement  a AND b
	if ( (passed_input & PATTERN_3) == 4){
		if( (passed_input & PATTERN_2) != 2 || (passed_input & PATTERN_4) != 8)
			returned_output = 1; // output is 1 when ER = 1 and DSBF = 0
		else returned_output = 0;
	}
	else returned_output = 0; //output is 0

	return returned_output;
}

unsigned int checkLocks(unsigned char passed_input) 
{
	unsigned int returned_output;
	
	//To implement  a AND b
	if ( (passed_input & PATTERN_6) == 32){
		if( (passed_input & PATTERN_1) == 1)
			returned_output = 1; // output 1 if DLC = 1 and DOS = 1
		else if ( (passed_input & PATTERN_5) == 16){
			returned_output = 0;
		}
		else returned_output = 1;
	}
	else returned_output = 0; //output is 0

	return returned_output;
}

unsigned int checkBrakes(unsigned char passed_input) 
{
	unsigned int returned_output;
	
	//To implement  a AND b
	if ( (passed_input & PATTERN_7) == 64 && (passed_input & PATTERN_8) == 128){
		returned_output = 1; //output 1 if car is moving and brakes are pressed
	}
	else returned_output = 0; //output is 0

	return returned_output;
}

//The code segment which implements the decision logic
void control_action(){

// Put your control/decision logic code segments inside this function
// This is the actual code whose execution time which is being measure

	bell = checkBell(input);
	door_lock = checkLocks(input);
	brake = checkBrakes(input);

}

/* ---     You should not have to modify anything below this line ---------*/

/*timespec diff from
http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
*/
struct timespec diff(struct timespec start, struct timespec end)
 {
    struct timespec temp;
    //the if condition handles time stamp end being smaller than than 
    //time stamp start which could lead to negative time.

     if ((end.tv_nsec-start.tv_nsec)<0) {
          temp.tv_sec = end.tv_sec-start.tv_sec-1;
          temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
      } else {
          temp.tv_sec = end.tv_sec-start.tv_sec;
          temp.tv_nsec = end.tv_nsec-start.tv_nsec;
      }
  return temp;
 }

int main(int argc, char *argv[])
{
	unsigned int cpu_mhz;
	unsigned long long int begin_time, end_time;
	struct timespec timeDiff,timeres;
    struct timespec time1, time2, calibrationTime;
	
    clock_gettime(CLOCKNAME, &time1);
	clock_gettime(CLOCKNAME, &time2);
	calibrationTime = diff(time1,time2); //calibration for overhead of the function calls
    clock_getres(CLOCKNAME, &timeres);  // get the clock resolution data
	
    read_inputs_from_ip_if(); // get the sensor inputs
	
	clock_gettime(CLOCKNAME, &time1); // get current time
	control_action();                 // process the sensors
	clock_gettime(CLOCKNAME, &time2);   // get current time

	write_output_to_op_if();    // output the values of the actuators
	
	timeDiff = diff(time1,time2); // compute the time difference

	printf("Timer Resolution = %u nanoseconds \n ",timeres.tv_nsec);
	printf("Calibrartion time = %u seconds and %u nanoseconds \n ", calibrationTime.tv_sec, calibrationTime.tv_nsec);
    printf("The measured code took %u seconds and ", timeDiff.tv_sec - calibrationTime.tv_sec);
	printf(" %u nano seconds to run \n", timeDiff.tv_nsec - calibrationTime.tv_nsec);
	
	return 0;
}
