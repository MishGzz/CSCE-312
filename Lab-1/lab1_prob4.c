// CSCE312: Lab-1 Problem-4

/* ***   README   **** */

/*

run this file as : gcc filename.c -o executableName

*/


#include <stdio.h> //This is useful to do i/o to test the code 

unsigned int input;

unsigned int bell;
unsigned int door_lock;
unsigned int brake;
int values[8];
int endOfFile = 0;
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
	//input will consist of a file containing 8 integers (1 or 0)
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
	fPointer = fopen("prob4_output.txt", "w");
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

	bell = checkBell(input);
	door_lock = checkLocks(input);
	brake = checkBrakes(input);
	
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
