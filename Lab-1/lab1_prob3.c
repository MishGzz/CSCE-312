// CSCE312: Lab-1 Problem-3 framework

/* ***   README   **** */

/*
This example program sounds the bell when driver is on seat 
AND haven't closed the doors. Use the general framework and the function 
shells, replace the code inside  the control_action() function with your 
own code to do problems 3 and 4.

Note: This code is designed to run in an infinite loop to mimic a real control system. 
If you prefer to read the inputs from a file, modify the code appropriately to terminate the
loop when all the inputs have been processed.

Turning in this file without your code will result in zero points being awarded for this problem.

run this file as : gcc filename.c -o executableName

*/


#include <stdio.h> //This is useful to do i/o to test the code 

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

void read_inputs_from_ip_if(){
	//printf("running program 1");
	//place your input code here
	//to read the current state of the available sensors
	//input will consist of a file containing 8 integers (1 or 0)
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
	/*
	The code given here sounds the bell when driver is on seat 
	AND hasn't closed the doors. (Requirement-2)
 	Replace this code segment with your own code to do problems 3 and 4.
	*/

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

int main(int argc, char *argv[])
{
	
	/*The main control loop which keeps the system alive and responsive for ever, 
	until the system is powered off */
	//for (; ; )
	//{
		read_inputs_from_ip_if();
		control_action();
		write_output_to_op_if();
	//}
	
	return 0;
}
