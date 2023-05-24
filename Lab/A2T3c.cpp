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
#include "bmpfuncs.h"

int main(void)
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	unsigned char* inputImage;
	unsigned char* inputImageTwo;
	unsigned char* outputImage;
	int imgWidth, imgHeight, imageSize;

	cl::ImageFormat imgFormat;
	cl::Image2D inputImgBuffer, inputImgBufferTwo, outputImgBuffer;

	float userLuminance = 0.0f;
	bool isValid = false;

	//Asks for user input for shift value and validating input
	std::cout << "Please enter the luminance value between 0 and 1: ";
	std::cin >> userLuminance;

	while (isValid == false)
	{
		if (userLuminance <= 1.0f && userLuminance >= 0.0f)
		{
			isValid = true;
		}

		if (std::cin.fail() == true || userLuminance < 0.0f || userLuminance > 1.0f)
		{
			std::cin.clear();
			std::cin.ignore(100, '\n');
			std::cout << "Invalid Luminance value!!" << std::endl;
			std::cout << "Re-enter a value between 0 and 1: ";
			std::cin >> userLuminance;
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(100, '\n');
			break;
		}
	}

	std::cout << "You entered a luminance value of " << userLuminance << endl;
	std::cout << "--------------------" << std::endl;

	std::cout << endl;

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
		if (!build_program(&program, &context, "Task3c.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "step2");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// read input image
		inputImage = read_BMP_RGB_to_RGBA("mandrill.bmp", &imgWidth, &imgHeight);

		// allocate memory for output image
		imageSize = imgWidth * imgHeight * 4;
		outputImage = new unsigned char[imageSize];

		// image format
		imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

		// set kernel arguments
		kernel.setArg(0, inputImgBuffer);
		kernel.setArg(1, outputImgBuffer);

		// enqueue kernel
		cl::NDRange offset(0, 0);
		cl::NDRange globalSize(imgWidth, imgHeight);

		queue.enqueueNDRangeKernel(kernel, offset, globalSize);

		std::cout << "Part 2 enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read image from device to host memory
		cl::size_t<3> origin, region;
		origin[0] = origin[1] = origin[2] = 0;
		region[0] = imgWidth;
		region[1] = imgHeight;
		region[2] = 1;

		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

		// output results to image file
		write_BMP_RGBA_to_RGB("A2T3c1.bmp", outputImage, imgWidth, imgHeight);


		//--------------------------------------------------------------------------------------------------------------
		// Part 3 (Horizontal pass)
		//Create kernel
		kernel = cl::Kernel(program, "step3Horizontal");

		// read input image
		inputImage = read_BMP_RGB_to_RGBA("A2T3c1.bmp", &imgWidth, &imgHeight);

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

		// set kernel arguments 
		kernel.setArg(0, inputImgBuffer);
		kernel.setArg(1, outputImgBuffer);

		queue.enqueueNDRangeKernel(kernel, offset, globalSize);

		std::cout << "Part 3 (Horizontal pass) enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

		// output results to image file
		write_BMP_RGBA_to_RGB("A2T3c2.bmp", outputImage, imgWidth, imgHeight);

		//--------------------------------------------------------------------------------------------------------------
		// Part 3 (Vertical pass)
		//Create kernel
		kernel = cl::Kernel(program, "step3Vertical");

		// read input image
		inputImage = read_BMP_RGB_to_RGBA("A2T3c2.bmp", &imgWidth, &imgHeight);

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

		// set kernel arguments 
		kernel.setArg(0, inputImgBuffer);
		kernel.setArg(1, outputImgBuffer);

		queue.enqueueNDRangeKernel(kernel, offset, globalSize);

		std::cout << "Part 3 (Vertical pass) enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

		// output results to image file
		write_BMP_RGBA_to_RGB("A2T3c3.bmp", outputImage, imgWidth, imgHeight);

		//--------------------------------------------------------------------------------------------------------------
		// Part 4
		//Create kernel
		kernel = cl::Kernel(program, "step4");

		// read input image
		inputImage = read_BMP_RGB_to_RGBA("A2T3c1.bmp", &imgWidth, &imgHeight);

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

		// set kernel arguments 
		kernel.setArg(0, inputImgBuffer);
		kernel.setArg(1, outputImgBuffer);
		kernel.setArg(2, userLuminance);

		queue.enqueueNDRangeKernel(kernel, offset, globalSize);

		std::cout << "Part 4 enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

		// output results to image file
		write_BMP_RGBA_to_RGB("A2T3c4.bmp", outputImage, imgWidth, imgHeight);

		//--------------------------------------------------------------------------------------------------------------
		// Part 5
		//Create kernel
		kernel = cl::Kernel(program, "step5");

		// read input image
		inputImage = read_BMP_RGB_to_RGBA("A2T3c3.bmp", &imgWidth, &imgHeight);
		inputImageTwo = read_BMP_RGB_to_RGBA("A2T3c4.bmp", &imgWidth, &imgHeight);

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
		inputImgBufferTwo = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImageTwo);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

		// set kernel arguments 
		kernel.setArg(0, inputImgBuffer);
		kernel.setArg(1, inputImgBufferTwo);
		kernel.setArg(2, outputImgBuffer);

		queue.enqueueNDRangeKernel(kernel, offset, globalSize);

		std::cout << "Part 5 enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

		// output results to image file
		write_BMP_RGBA_to_RGB("A2T3c5.bmp", outputImage, imgWidth, imgHeight);

		//--------------------------------------------------------------------------------------------------------------
		// Part 5
		//Create kernel
		kernel = cl::Kernel(program, "step6");

		// read input image
		inputImage = read_BMP_RGB_to_RGBA("A2T3c5.bmp", &imgWidth, &imgHeight);
		inputImageTwo = read_BMP_RGB_to_RGBA("noise.bmp", &imgWidth, &imgHeight);

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
		inputImgBufferTwo = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImageTwo);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

		// set kernel arguments 
		kernel.setArg(0, inputImgBuffer);
		kernel.setArg(1, inputImgBufferTwo);
		kernel.setArg(2, outputImgBuffer);

		queue.enqueueNDRangeKernel(kernel, offset, globalSize);

		std::cout << "Part 6 enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

		// output results to image file
		write_BMP_RGBA_to_RGB("A2T3c6.bmp", outputImage, imgWidth, imgHeight);

		std::cout << "Done." << std::endl;

		// deallocate memory
		free(inputImage);
		free(inputImageTwo);
		free(outputImage);
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
