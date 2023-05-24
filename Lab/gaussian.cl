__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST; 

__constant float filter[49] = 
{0.000036,0.000363,0.001446,0.002291,0.001446,0.000363,0.000036,
0.000363,0.003676,0.014662,0.023226,0.014662,0.003676,0.000363,
0.001446,0.014662,0.058488,0.092651,0.058488,0.014662,0.001446,
0.002291,0.023226,0.092651,0.146768,0.092651,0.023226,0.002291,
0.001446,0.014662,0.058488,0.092651,0.058488,0.014662,0.001446,
0.000363,0.003676,0.014662,0.023226,0.014662,0.003676,0.000363,
0.000036,0.000363,0.001446,0.002291,0.001446,0.000363,0.000036};


__kernel void gaussian(read_only image2d_t src_image,write_only image2d_t dst_image)
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

    /* Iterate over the rows */
   for(int i = -3; i <= 3; i++) 
   {
	  coord.y =  row + i;

      /* Iterate over the columns */
	  for(int j = -3; j <= 3; j++) 
	  {
         coord.x = column + j;

		 /* Read value pixel from the image */ 		
		 pixel = read_imagef(src_image, sampler, coord);

		 /* Acculumate weighted sum */ 		
		 sum.xyz += pixel.xyz * filter[filter_index++];
	  }
   }

   /* Write new pixel value to output */
   coord = (int2)(column, row); 
   write_imagef(dst_image, coord, sum);
}
