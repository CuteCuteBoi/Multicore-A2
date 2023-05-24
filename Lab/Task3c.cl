__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__kernel void step2(read_only image2d_t src_image,write_only image2d_t dst_image)
{
   /* Get pixel coordinate */
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

   /* Read pixel value */
   float4 pixel = read_imagef(src_image, sampler, coord);

   float luminance = (0.299 * pixel.x) + (0.587 * pixel.y) + (0.114 * pixel.z);

   pixel.y = luminance;
   pixel.xz = 0;

   /* Write new pixel value to output */
   write_imagef(dst_image, coord, pixel);
}

__constant float filter[7] = {0.00598, 0.060626, 0.241843, 0.383103, 0.241843, 0.060626, 0.00598};

__kernel void step3Horizontal(read_only image2d_t src_image,write_only image2d_t dst_image)
{
	/* Get work-item’s row and column position */
   int column = get_global_id(0); 
   int row = get_global_id(1);

   /* Accumulated pixel value */
   float4 sum = (float4)(0.0);

   /* Filter's current index */
   int filter_index =  0;

   int2 coord;
   float4 pixel;

   coord.y = row;
   /* Iterate over the rows */
   for(int i = -3;i <= 3;i++)
   {
		coord.x = column + i;

		/* Read value pixel from the image */ 		
		pixel = read_imagef(src_image, sampler, coord);

		/* Acculumate weighted sum */ 		
		sum.xyz += pixel.xyz * filter[filter_index++];
   }

   /* Write new pixel value to output */
   coord = (int2)(column, row); 
   write_imagef(dst_image, coord, sum);
}

__kernel void step3Vertical(read_only image2d_t src_image,write_only image2d_t dst_image)
{
	/* Get work-item’s row and column position */
   int column = get_global_id(0); 
   int row = get_global_id(1);

   /* Accumulated pixel value */
   float4 sum = (float4)(0.0);

   /* Filter's current index */
   int filter_index =  0;

   int2 coord;
   float4 pixel;

   coord.x = column;
   /* Iterate over the rows */
   for(int i = -3;i <= 3;i++)
   {
		coord.y = row + i;

		/* Read value pixel from the image */ 		
		pixel = read_imagef(src_image, sampler, coord);

		/* Acculumate weighted sum */ 		
		sum.xyz += pixel.xyz * filter[filter_index++];
   }

   /* Write new pixel value to output */
   coord = (int2)(column, row); 
   write_imagef(dst_image, coord, sum);
}

__kernel void step4(read_only image2d_t src_image,write_only image2d_t dst_image, float luminance)
{
	/* Get pixel coordinate */
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

   /* Read pixel value */
   float4 pixel = read_imagef(src_image, sampler, coord);

   if((pixel.x + pixel.y + pixel.z) <= luminance)
   {
		pixel.xyz = 0;
   }

   /* Write new pixel value to output */
   write_imagef(dst_image, coord, pixel);
}

__kernel void step5(read_only image2d_t src_image,read_only image2d_t src_imageTwo,write_only image2d_t dst_image)
{
	/* Get work-item’s row and column position */
	int column = get_global_id(0); 
	int row = get_global_id(1);

	/* Accumulated pixel value */
	float4 sum = (float4)(0.0);

	float4 pixel;
	/* Get pixel coordinate */
	int2 coordinates = (int2)(get_global_id(0), get_global_id(1));
   
   	pixel = read_imagef(src_image, sampler, coordinates);
	sum.x = pixel.x;
	sum.y = pixel.y;
	sum.z = pixel.z;
 
	pixel = read_imagef(src_imageTwo, sampler, coordinates);
	pixel.x += sum.x;
	pixel.y += sum.y;
	pixel.z += sum.z;

	if((pixel.x + pixel.y + pixel.z) > 1.0)
	{
		pixel.xyz = 1;
	}

	/* Write new pixel value to output */
	write_imagef(dst_image, coordinates, pixel);
}

__kernel void step6(read_only image2d_t src_image,read_only image2d_t src_imageTwo,write_only image2d_t dst_image)
{
	/* Get work-item’s row and column position */
	int column = get_global_id(0); 
	int row = get_global_id(1);

	/* Accumulated pixel value */
	float4 sum = (float4)(0.0);

	float4 pixel;
	/* Get pixel coordinate */
	int2 coordinates = (int2)(get_global_id(0), get_global_id(1));
   
   	pixel = read_imagef(src_image, sampler, coordinates);
	sum.x = pixel.x;
	sum.y = pixel.y;
	sum.z = pixel.z;
 
	pixel = read_imagef(src_imageTwo, sampler, coordinates);
	pixel.x *= sum.x;
	pixel.y *= sum.y;
	pixel.z *= sum.z;

	/* Write new pixel value to output */
	write_imagef(dst_image, coordinates, pixel);
}