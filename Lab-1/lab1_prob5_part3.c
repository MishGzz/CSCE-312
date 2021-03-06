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

unsigned int driver_on_seat;
unsigned int seat_belt;
unsigned int engine_running;
unsigned int doors_closed;
unsigned int key_in_car;
unsigned int lock_lever;
unsigned int brake_pedal;
unsigned int car_moving;

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
		fscanf(fPointer,"%d%", &values[i]);
		i++;
	}
	fclose(fPointer);
	
driver_on_seat = values[0];
seat_belt = values[1];
engine_running = values[2];
doors_closed = values[3];
key_in_car = values[4];
lock_lever = values[5];
brake_pedal = values[6];
car_moving = values[7];
	
}

void write_output_to_op_if(){

	//place your output code here
    //to display/print the state of the 3 actuators (DLA/BELL/BA)
	FILE * fPointer;
	fPointer = fopen("prob3_output.txt", "w");
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

//The code segment which implements the decision logic
void control_action(){

// Put your control/decision logic code segments inside this function
// This is the actual code whose execution time which is being measure

	if (engine_running && !doors_closed)
		bell = 1;
	if (engine_running && !seat_belt)
		bell = 1;
	else bell = 0;
	
	if (lock_lever){
		if(key_in_car && !driver_on_seat){
			door_lock = 0;
		}
		else door_lock = 1;
	}
	else door_lock = 0;
	
	if (brake_pedal){
		if(car_moving){
			brake = 1;
		}
		else brake = 0;
	}
	else brake = 0;

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
