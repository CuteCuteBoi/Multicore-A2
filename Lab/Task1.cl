__kernel void Task1(__global int* input1, __global int* input2,__global int* outputArray)
{
	//Initialise int8 variables
	__local int8 v, v1, v2;

	//Copy content of input1 into an int8 vector
	v = vload8(0,input1);

	//Copy content of input2 into 2 int8 vector
	v1 = vload8(0,input2);
	v2 = vload8(1,input2);

	//Initialise results
	int8 results = 0;

	//Check if the int8 contains elements > 16
	if(any(v > 16))
	{
		results = select(v1,v2,v > 16);
	}
	else
	{
		results.s0123 = v1.s0123;
		results.s4567 = v2.s0123;
	}

	//Send data back to program
	vstore8(v,0,outputArray);
	vstore8(v1,1,outputArray);
	vstore8(v2,2,outputArray);
	vstore8(results,3,outputArray);
}