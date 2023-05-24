#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"

int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	//Declare data and memory objects
	//Declare 2 array
	int vec1[8]{};
	int vec2[16]{};
	int output[32]{};
	cl::Buffer arrayOneBuffer;
	cl::Buffer arrayTwoBuffer;
	cl::Buffer outputBuffer;

	//For randoming purposes
	srand(time(0));

	//Initialise the arrays with values
	//Vec1
	for (int i = 0; i < 8; i++)
	{
		int random = 11 + (rand() % 9);
		vec1[i] = random;
		//std::cout << vec1[i] << std::endl;
	}

	//Vec2
	for (int i = 0; i < 8; i++)
	{
		vec2[i] = i;
		//std::cout << vec2[i] << std::endl;
	}

	for (int i = 8; i < 16; i++)
	{
		vec2[i] = i-16;
		//std::cout << vec2[i] << std::endl;
	}

	try {
		// select an OpenCL device
		if (!select_one_device(&platform, &device))
		{
			// if no device selected
			quit_program("Device not selected.");
		}

		// create a context from device
		context = cl::Context(device);

		// build the program
		if(!build_program(&program, &context, "Task1.cl")) 
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "Task1");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffers
		arrayOneBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(vec1), vec1);
		arrayTwoBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(vec2), vec2);
		outputBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(output));

		// set kernel arguments
		kernel.setArg(0, arrayOneBuffer);
		kernel.setArg(1, arrayTwoBuffer);
		kernel.setArg(2, outputBuffer);

		// enqueue kernel for execution
		queue.enqueueTask(kernel);
		std::cout << "Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(output), output);

		// output result
		std::cout << "Values in v" << std::endl;
		for (int i = 0; i < 8; i++)
		{
			std::cout << output[i] << " ";
		}

		std::cout << "\nValues in v1" << std::endl;
		for (int i = 8; i < 16; i++)
		{
			std::cout << output[i] << "  ";
		}

		std::cout << "\nValues in v2" << std::endl;
		for (int i = 16; i < 24; i++)
		{
			std::cout << output[i] << " ";
		}

		std::cout << "\nValues in results" << std::endl;
		for (int i = 24; i < 32; i++)
		{
			std::cout << output[i] << " ";
		}
		
	}
	// catch any OpenCL function errors
	catch (cl::Error e) {
		// call function to handle errors
		handle_error(e);
	}

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif

	return 0;
}
