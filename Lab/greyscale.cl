__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST; 

__kernel void greyscale(read_only image2d_t src_image,write_only image2d_t dst_image)
{
   /* Get pixel coordinate */
   int2 coord = (int2)(get_global_id(0), get_global_id(1));

   /* Read pixel value */
   float4 pixel = read_imagef(src_image, sampler, coord);

   float luminance = (0.299 * pixel.x) + (0.587 * pixel.y) + (0.114 * pixel.z);

   pixel.xyz = luminance;

   /* Write new pixel value to output */
   write_imagef(dst_image, coord, pixel);
}
