//File named “lab5_prob6.c”
#include <stdio.h>
int very_fast_function(int i){
asm(
	"movl %1,%%ebx;"
    "sall $6,%%ebx;"
    "addl $1,%%ebx;"
    "cmpl $1024,%%ebx;"
    "jle .L2;"
    "addl $1,%0;"
    "jmp .L3;"
".L2:;"
	"movl $0,%0;"
".L3:;"
    : "=r" (i) : "r" (i) : "ebx" );
	
return i;
}
int main(int argc, char *argv[])
{
int i;
i=40;
printf("The function value of i is %d\n", very_fast_function(i) );
return 0;
}