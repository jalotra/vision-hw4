#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    // TO normalize the sum of image across all channels to 1
    float sum_image_values = 0;
    for(int i = 0; i < im.c*im.w*im.h; i++)
    {
        sum_image_values += im.data[i];
    }

    for(int k = 0; k < im.c*im.w*im.h; k++)
    {
        im.data[k] /= sum_image_values; 
    }

}

image make_box_filter(int w)
{
    // TODO
    // return make_image(1,1,1);
    // Creates a box filter with one channel and then normalise it 
    image new_image = make_image(w, w, 1);
    // NOw set all the values to 1
    for(int channel = 0; channel < new_image.c; channel ++)
    {
        for(int row = 0; row < w ; row++)
        {
            for(int column = 0 ; column < w; column++)
            {
                set_pixel(new_image, column, row, channel, 1);
            }
        }
    }
    

    // Finally normalise the box-filter
    l1_normalize(new_image);

    return new_image;

}


float box_filter_result(image im, image filter, int row, int column, int channel, int preserve)
{
    float q = 0;
    int f_c;

    if(filter.c == 1) f_c = 0 ;
    else f_c = channel;

    for(int frow = 0; frow < filter.h; frow++) {
        for(int fcol = 0; fcol < filter.w; fcol++) {
          //  float before = q;
            float fil = get_pixel(filter, fcol, frow, f_c);
            int fx = column + fcol - filter.w/2;
            int fy = row + frow - filter.h/2;
            float im_val = get_pixel(im, fx, fy, channel);
            q = q + (fil * im_val);
        }
    }

    return q;
}

image convolve_image(image im, image filter, int preserve)
{
    // check filter is valid
    assert(filter.c == im.c || filter.c == 1);
    image f;

   if(preserve) {
        f = make_image(im.w, im.h, im.c);
   } else {
        f = make_image(im.w, im.h, 1);
    }

    if(preserve)
    {
        for(int channel = 0; channel < f.c ; channel++)
        {
            for(int row = 0; row < im.h ; row ++)
            {
                for(int col = 0; col < im.w ; col++)
                {
                    set_pixel(f, col, row, channel,  box_filter_result(im, filter, row, col, channel, preserve));
                }
            }
        }
    }
    else
    {
        for(int row = 0; row < im.h ; row ++)
            {
                for(int col = 0; col < im.w ; col++)
                {
                    set_pixel(f, col, row, 0,  (box_filter_result(im, filter, row, col, 0 , preserve)+box_filter_result(im, filter, row, col, 1 , preserve)+box_filter_result(im, filter, row, col, 2 , preserve))/3);
                }
            }
    }
    
    return f;
}


image make_highpass_filter()
{
    // TODO

    // highpass filter involves the use pf sobel filter
    image highpass_filter = make_image(3,3,1);
    // Fill the zeoes
    set_pixel(highpass_filter, 0, 0, 0, 0);
    set_pixel(highpass_filter, 2, 0, 0, 0);
    set_pixel(highpass_filter, 0, 2, 0, 0);
    set_pixel(highpass_filter, 2, 2, 0, 0);

    // Fill -1s
    set_pixel(highpass_filter, 1, 0, 0, -1);
    set_pixel(highpass_filter, 0, 1,0, -1);
    set_pixel(highpass_filter, 2, 1, 0,-1);
    set_pixel(highpass_filter, 1, 2, 0,-1);

    // fill 4
    set_pixel(highpass_filter, 1, 1,0, 4);



    return highpass_filter;
}

image make_sharpen_filter()
{
    // TODO
     image sharpen_filter = make_image(3,3,1);
    // Fill the zeoes
    set_pixel(sharpen_filter, 0, 0, 0, 0);
    set_pixel(sharpen_filter, 2, 0, 0, 0);
    set_pixel(sharpen_filter, 0, 2, 0, 0);
    set_pixel(sharpen_filter, 2, 2, 0, 0);

    // Fill -1s
    set_pixel(sharpen_filter, 1, 0, 0, -1);
    set_pixel(sharpen_filter, 0, 1,0, -1);
    set_pixel(sharpen_filter, 2, 1, 0,-1);
    set_pixel(sharpen_filter, 1, 2, 0,-1);

    // fill 4
    set_pixel(sharpen_filter, 1, 1,0, 5);



    return sharpen_filter;
}

image make_emboss_filter()
{
    // TODO
    image emboss_filter = make_image(3,3,1);
    // Fill the zeoes
    set_pixel(emboss_filter, 0, 0, 0, -2);
    set_pixel(emboss_filter, 2, 0, 0, 0);
    set_pixel(emboss_filter, 0, 2, 0, 0);
    set_pixel(emboss_filter, 2, 2, 0, 2);

    // Fill -1s
    set_pixel(emboss_filter, 1, 0, 0, -1);
    set_pixel(emboss_filter, 0, 1,0, -1);
    set_pixel(emboss_filter, 2, 1, 0,1);
    set_pixel(emboss_filter, 1, 2, 0,-1);

    // fill 4
    set_pixel(emboss_filter, 1, 1,0, 1);



    return emboss_filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    // return make_image(1,1,1);
    // The probability distribution function looks like soetjimg 

    // define the size of the filter // the filter will be a odd*odd filter
    int s = 6*sigma ;
    s = (s%2 == 0) ? s+1 : s; 
    // Check the git repo for the function 
    image gaussian_filter = make_image(s,s,1);

    for(int row = 0; row < gaussian_filter.h ; row++)
    {
        for(int col = 0; col < gaussian_filter.w; col++)
        {
            int x = col - gaussian_filter.w/2;
            int y = row - gaussian_filter.h/2;

            float value = (1/(TWOPI*sigma*sigma))*exp(-(x*x + y*y)/(2*sigma*sigma));

            set_pixel(gaussian_filter, col, row, 0, value);
        }
    }
    // Final step is to normalize the gasussian function
    l1_normalize(gaussian_filter);

    return gaussian_filter;
}

image add_image(image a, image b)
{
    // TODO
    // return make_image(1,1,1);

    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image new_image = make_image(a.w, a.h, a.c);

    for(int channel = 0; channel < a.c ; channel++)
    {
        for(int row = 0; row < a.h ; row++)
        {
            for(int col = 0; col < a.w ; col++)
            {
                float final_value = get_pixel(a, col, row, channel) + get_pixel(b, col, row, channel);
                set_pixel(new_image, col, row, channel, final_value);
            }
        }
    }

    return new_image;
}

image sub_image(image a, image b)
{
    // TODO
    // return make_image(1,1,1);
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image new_image = make_image(a.w, a.h, a.c);

    for(int channel = 0; channel < a.c ; channel++)
    {
        for(int row = 0; row < a.h ; row++)
        {
            for(int col = 0; col < a.w ; col++)
            {
                float final_value = get_pixel(a, col, row, channel) - get_pixel(b, col, row, channel);
                set_pixel(new_image, col, row, channel, final_value);
            }
        }
    }

    return new_image;
}

image make_gx_filter()
{
    // TODO
    image gx_filter = make_image(3,3,1);

    set_pixel(gx_filter, 0, 0, 0,-1);
    set_pixel(gx_filter, 0, 1, 0 ,-2);
    set_pixel(gx_filter, 0, 2, 0 ,-1);

    set_pixel(gx_filter, 1, 0, 0, 0);
    set_pixel(gx_filter, 1, 1, 0 , 0);
    set_pixel(gx_filter, 1, 2, 0 , 0);

    set_pixel(gx_filter, 2, 0, 0, 1);
    set_pixel(gx_filter, 2, 1, 0 ,2);
    set_pixel(gx_filter, 2, 2, 0 ,1);

    // l1_normalize(gx_filter);

    return gx_filter;
}

image make_gy_filter()
{
    // TODO
    // return make_image(1,1,1);
    image gy_filter = make_image(3,3,1);

    set_pixel(gy_filter, 0, 0, 0,-1);
    set_pixel(gy_filter, 1, 0, 0 ,-2);
    set_pixel(gy_filter, 2, 0, 0 ,-1);

    set_pixel(gy_filter, 0, 1, 0, 0);
    set_pixel(gy_filter, 1, 1, 0 , 0);
    set_pixel(gy_filter, 2, 1, 0 , 0);

    set_pixel(gy_filter, 0, 2, 0, 1);
    set_pixel(gy_filter, 1, 2, 0 ,2);
    set_pixel(gy_filter, 2, 2, 0 ,1);


    return gy_filter;
}

image create_gradientX_filter()
{
    image gradient_filter = make_image(3,1,1);
    set_pixel(gradient_filter, 0, 0, 0, -1);
    set_pixel(gradient_filter, 1, 0, 0, 0);
    set_pixel(gradient_filter, 2, 0, 0, 1);


    return gradient_filter;

}
image create_gradientY_filter()
{
    image gradient_filter = make_image(1,3,1);
    set_pixel(gradient_filter, 0, 0, 0, -1);
    set_pixel(gradient_filter, 0, 1, 0, 0);
    set_pixel(gradient_filter, 0, 2, 0, 1);


    return gradient_filter;

}

float three_way_max_finder(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min_finder(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void feature_normalize(image im)
{
    // TODO
    float min_r = get_pixel(im, 0, 0, 0);
    float min_g = get_pixel(im, 0, 0, 1);
    float min_b = get_pixel(im, 0, 0, 2);

    float max_r = get_pixel(im, 0, 0, 0);
    float max_g = get_pixel(im, 0, 0, 1);
    float max_b = get_pixel(im, 0, 0, 2);

    float r_value, g_value, b_value;
    for(int row = 0; row < im.h; row++)
    {
        for(int col = 0; col < im.w; col++)
        {   
            r_value = get_pixel(im, col, row, 0);
            g_value = get_pixel(im, col, row, 1);
            b_value = get_pixel(im, col, row, 2);

            min_r = ( r_value <= min_r) ? r_value : min_r;
            min_g = ( g_value <= min_g) ? g_value : min_g;
            min_b = ( b_value <= min_b) ? b_value : min_b;

            max_r = ( r_value >= max_r) ? r_value : max_r;
            max_g = ( g_value >= max_g) ? g_value : max_g;
            max_b = ( b_value >= max_b) ? b_value : max_b;

        }
    }

    float min_across_all_channels = three_way_min_finder(min_r, min_b, min_g);
    float max_across_all_channels = three_way_max_finder(max_b, max_g, max_r);

    for(int row = 0; row < im.h ; row++)
    {
        for(int col = 0; col < im.w ; col++)
        {
            set_pixel(im, col, row, 0, (get_pixel(im, col , row, 0) - min_across_all_channels)/(max_across_all_channels - min_across_all_channels));
            set_pixel(im, col, row, 1, (get_pixel(im, col , row, 1) - min_across_all_channels)/(max_across_all_channels - min_across_all_channels));
            set_pixel(im, col, row, 2, (get_pixel(im, col , row, 2) - min_across_all_channels)/(max_across_all_channels - min_across_all_channels));
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    // return calloc(2, sizeof(image));

    image *return_pointer = calloc(2, sizeof(image));        // creates two objects of the type image 

    return_pointer[0] = make_image(im.w, im.h, im.c);
    return_pointer[1] = make_image(im.w, im.h, im.c);

    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();

    // Now I have to run these filters over the image im and calculate the magnitude 
    // and angle between 

    image gx = convolve_image(im, gx_filter, 0);
    image gy = convolve_image(im, gy_filter, 0);

    for(int channel = 0 ; channel < im.c; channel ++)
    {
        for(int row = 0; row < im.h ; row++)
        {
            for(int col = 0; col < im.w; col++)
            {
                float x_value = get_pixel(gx, col, row, channel);
                float y_value = get_pixel(gy, col, row, channel);

                float val = x_value*x_value + y_value*y_value;
                set_pixel(return_pointer[0], col, row, channel, sqrt(val));

                float angle_value = atan(y_value/x_value);
                set_pixel(return_pointer[1], col, row, channel, angle_value); 
            }
        }
    }

    return return_pointer;

}

image colorize_sobel(image im)
{
    // TODO
    // return make_image(1,1,1);

    image new_image = make_image(im.w, im.h, im.c);

    // Create a new image of size
    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();

    // Now I have to run these filters over the image im and calculate the magnitude 
    // and angle between 

    image gx = convolve_image(im, gx_filter, 1);
    image gy = convolve_image(im, gy_filter, 1);

    for(int channel = 0 ; channel < im.c; channel ++)
    {
        for(int row = 0; row < im.h ; row++)
        {
            for(int col = 0; col < im.w; col++)
            {
                float x_value = get_pixel(gx, col, row, channel);
                float y_value = get_pixel(gy, col, row, channel);

                float val = x_value*x_value + y_value*y_value;
                set_pixel(new_image, col, row, channel, sqrt(val));

            }
        }
    }

    return new_image;
}