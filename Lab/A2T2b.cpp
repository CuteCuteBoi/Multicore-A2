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

	//Store the output of the read into this vector
	std::vector<char> plainText;

	bool isFilePresent = false;
	std::string fileName = "plaintext.txt";
	std::fstream newFile;
	char charOne;
	int shift;

	newFile.open(fileName, std::ios::in);
	if (newFile.fail())
	{
		std::cout << "plaintext.txt does not exist!" << std::endl;
	}
	else
	{
		isFilePresent = true;
	}

	if (isFilePresent == true)
	{
		if (newFile.is_open())
		{
			while (newFile.get(charOne))
			{
				if (charOne >= 'a' && charOne <= 'z')
				{
					charOne = charOne - 32;
					plainText.push_back(charOne);
				}
				else
				{
					plainText.push_back(charOne);
				}
			}

			newFile.close();
		}
	}

	bool isValid = false;

	//Asks for user input for shift value and validating input
	std::cout << "Please enter the shift value: ";
	std::cin >> shift;

	while (isValid == false)
	{
		if (shift <= 26 && shift >= -26)
		{
			isValid = true;
		}

		if (std::cin.fail() == true || shift >= 26 || shift <= -26)
		{
			std::cin.clear();
			std::cin.ignore(100, '\n');
			std::cout << "Invalid shift value!!" << std::endl;
			std::cout << "Re-enter a value between -26 and 26: ";
			std::cin >> shift;
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(100, '\n');
			break;
		}
	}

	std::cout << "You've entered a shift value of " << shift << std::endl;
	std::cout << std::endl;

	int oriSize = plainText.size();

	//Create buffer and vector
	cl::Buffer befBuffer, aftBuffer;
	std::vector<char> cipherTextStore(oriSize);
	std::vector<char> decipherTextStore(oriSize);

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
		if (!build_program(&program, &context, "Cipher.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "Cipher");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffers
		befBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(char) * oriSize, &plainText[0]);
		aftBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(char) * oriSize);

		// set kernel arguments
		kernel.setArg(0, shift);
		kernel.setArg(1, befBuffer);
		kernel.setArg(2, aftBuffer);

		// enqueue kernel for execution
		cl::NDRange offset(0);
		cl::NDRange globalSize(oriSize);
		queue.enqueueNDRangeKernel(kernel,offset,globalSize);

		std::cout << "Cipher Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(aftBuffer, CL_TRUE, 0, sizeof(char) * oriSize, &cipherTextStore[0]);

		// output result (DEBUG)
		/*for (int i = 0; i < oriSize; i++)
		{
			std::cout << cipherTextStore[i];
		}*/

		//Save the output to a txt file
		std::string fileName = "ciphertext.txt";
		std::ofstream outFile;
		outFile.open(fileName);
		for (int i = 0; i < cipherTextStore.size(); i++)
		{
			outFile << cipherTextStore[i];
		}
		outFile.close();
		std::cout << "Cipher file written to " << fileName << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		//-----------------------------------------------------------------------------------------------------------
		//Decipher

		// build the program
		if (!build_program(&program, &context, "Decipher.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "Decipher");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffers
		befBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(char) * oriSize, &cipherTextStore[0]);
		aftBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(char) * oriSize);

		// set kernel arguments
		kernel.setArg(0, shift);
		kernel.setArg(1, befBuffer);
		kernel.setArg(2, aftBuffer);

		// enqueue kernel for execution
		queue.enqueueNDRangeKernel(kernel, offset, globalSize);

		std::cout << "Decipher Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(aftBuffer, CL_TRUE, 0, sizeof(char)* oriSize, &decipherTextStore[0]);


		// output result (DEBUG)
		/*for (int i = 0; i < oriSize; i++)
		{
			std::cout << decipherTextStore[i];
		}*/

		//Save the output to a txt file
		fileName = "decipher.txt";
		outFile.open(fileName);
		for (int i = 0; i < decipherTextStore.size(); i++)
		{
			outFile << decipherTextStore[i];
		}
		outFile.close();
		std::cout << "Decipher file written to " << fileName << std::endl;

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
