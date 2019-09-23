#include <math.h>
#include "image.h"
#include <stdlib.h>

float absolute(float x)
{
    // return (x >= 0 ) ? x : -x;  
    if(x >= 0) return x;
    else return -x;
}

float nn_interpolate(image im, float x, float y, int c)
{
    // ALgorithmic steps 
    // what is a nn algorithm?
    // we are given a (x,y) value representing the column , row of the newly made image
    // Now suppose that the points given

    int nearest_x, nearest_y;
    // It should return the nearest value 
    nearest_x = (floor(x) < ceil(x)) ? floor(x) : ceil(x);
    nearest_y = (floor(y) < floor(y)) ? floor(y) : ceil(y);

    return get_pixel(im, nearest_x, nearest_y, c);          // return the pixel value in the image 

    // return 0;
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image new_image = make_image(w ,h, im.c);           // creates a new image of the size (w*h*im.c)
    float row_scaling_factor = h/im.h;                    // denotes the scaling factor in y direction
    float column_scaling_factor = w/im.w;                 // denotes the scaling factor in x direction
    for(int column = 0 ; column < w ; column++)
    {
        for(int row = 0; row < h; row ++)
        {
            // pixel = get_pixel(round(x), round(y), c)
            float r_new_image = nn_interpolate(im, column/column_scaling_factor, row/row_scaling_factor, 0);
            float g_new_image = nn_interpolate(im, column/column_scaling_factor, row/row_scaling_factor, 1);
            float b_new_image = nn_interpolate(im, column/column_scaling_factor, row/row_scaling_factor, 2);

            // Now set these pixels in the new_image created
            set_pixel(new_image, column, row, 0, r_new_image);
            set_pixel(new_image, column, row, 1, g_new_image);
            set_pixel(new_image, column, row, 2, b_new_image);
        }
    }

    return new_image;

}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // given float x, y it should return a float that represents the new 
    // pixel value in the original image

    // Algorithmic Steps
    // find the nearest x and y values

    int nearest_xs_available[] = {floor(x), ceil(x)};
    int nearest_ys_available[] = {floor(y), ceil(y)};

    // 3. For different quadrants the value of the location of q1 and q2 changes

    // I am writing for the first quadrant and will change values as the point lies in other quadrants  
    // FOR FIRST QUADRANT
    int nearest_x = nearest_xs_available[0];
    int farthest_x = nearest_xs_available[1];
    int nearest_y = nearest_ys_available[0];
    int farthest_y = nearest_ys_available[1];

    // Determine the quadrants that the point (x,y) is located in 
    // Now check for other quadrants
    float mid_x = (nearest_xs_available[0] + nearest_xs_available[1])/2;
    float mid_y = (nearest_ys_available[0] + nearest_ys_available[1])/2;

    int quadrant;
    if(x <= mid_x && y <= mid_y)
    {
        quadrant = 1;
    }
    else if(x >= mid_x && y < mid_y)
    {
        quadrant = 2;
    }
    else if(x < mid_x && y >= mid_y)
    {
        quadrant = 3;
    }
    else if(x >  mid_x && y > mid_y )
    {
        quadrant = 4;
    }
    
    if(quadrant == 2)
    {
        // X farthest becomes X nearest
        int *nearest_x_location = &nearest_x;
        int *farthest_x_loaction = &farthest_x;
        int temp1 = *nearest_x_location;
        *nearest_x_location = *farthest_x_loaction;
        *farthest_x_loaction = temp1;
    }
    if(quadrant == 3)
    {
        // Y farthest becomes Y nearest
        int *nearest_y_location = &nearest_y;
        int *farthest_y_loaction = &farthest_y;
        int temp2 = *nearest_y_location;
        *nearest_y_location = *farthest_y_loaction;
        *farthest_y_loaction = temp2;
    }
    if(quadrant == 4)
    {
        // X loaction changes
        int *nearest_x_location = &nearest_x;
        int *farthest_x_loaction = &farthest_x;
        int temp1 = *nearest_x_location;
        *nearest_x_location = *farthest_x_loaction;
        *farthest_x_loaction = temp1;

        // Y location changes
        int *nearest_y_location = &nearest_y;
        int *farthest_y_loaction = &farthest_y;
        int temp2 = *nearest_y_location;
        *nearest_y_location = *farthest_y_loaction;
        *farthest_y_loaction = temp2;
    }

    float first_pixel_value = get_pixel(im, nearest_x , nearest_y, c);  // V1
    float second_pixel_value = get_pixel(im, farthest_x, nearest_y, c);  // V3
    float third_pixel_value = get_pixel(im, nearest_x , farthest_y, c);   // V2
    float fourth_pixel_value = get_pixel(im, farthest_x, farthest_y, c);  // v4

    // Calculate bi-linear interpolation
    float d1 = absolute(nearest_x - x);
    float d2 = absolute(farthest_x - x);
    float q1 = first_pixel_value*d2 + second_pixel_value*d1;
    float q2 = third_pixel_value*d2 + fourth_pixel_value*d1;

    // Linear interpolation second time
    float d3 = absolute(nearest_y - y);
    float d4 = absolute(farthest_y - y);

    float final_pixel_value = q1*d4 + q2*d3;    

    // Normalize the final_pixel_value 
    final_pixel_value /= (d1 + d2)*(d3 + d4); 

    
    // FInally return this pixel value
    
    return final_pixel_value;
}

image bilinear_resize(image im, int w, int h)
{
    image new_image = make_image(w, h, im.c);

    float row_scaling_factor = h/im.h;
    float column_scaling_factor = w/im.w;

    for(int column = 0 ; column < w ; column++)
    {
        for(int row = 0; row < h; row ++)
        {
            // pixel = get_pixel(round(x), round(y), c)
            float r_new_image = bilinear_interpolate(im, column/column_scaling_factor, row/row_scaling_factor, 0);
            float g_new_image = bilinear_interpolate(im, column/column_scaling_factor, row/row_scaling_factor, 1);
            float b_new_image = bilinear_interpolate(im, column/column_scaling_factor, row/row_scaling_factor, 2);

            // Now set these pixels in the new_image created
            set_pixel(new_image, column, row, 0, r_new_image);
            set_pixel(new_image, column, row, 1, g_new_image);
            set_pixel(new_image, column, row, 2, b_new_image);
        }
    }


    return new_image;

    
}

