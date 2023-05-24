__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__constant float filter[7] = {0.00598, 0.060626, 0.241843, 0.383103, 0.241843, 0.060626, 0.00598};

__kernel void gaussianVert(read_only image2d_t src_image,write_only image2d_t dst_image)
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

__kernel void gaussianHori(read_only image2d_t src_image,write_only image2d_t dst_image)
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