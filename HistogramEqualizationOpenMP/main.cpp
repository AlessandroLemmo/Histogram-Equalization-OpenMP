#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <omp.h>

using namespace std;
using namespace cv;


// make the histogram and convert from RGB to YUV creating a coloumn vector yuv_vector with col*row*3 dimension
void RGB_to_YCbCr(unsigned char* ptr_image, int* histogram, int width, int height)
{
    // initialize all intensity values to 0
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }

    // all variables are shared with all threads 
    omp_set_dynamic(0);
    #pragma omp parallel num_threads(32) default(shared)
    {
        #pragma omp for schedule(static) collapse(2)
        
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {

                    int pixel_idx = (i * width + j) * 3;

                    int r = ptr_image[pixel_idx];
                    int g = ptr_image[pixel_idx + 1];
                    int b = ptr_image[pixel_idx + 2];

                    // Convert from RGB into YUV
                    int Y = 0.299 * r + 0.587 * g + 0.114 * b; // Y is the luminous level
                    int Cb = 128 - 0.168736 * r - 0.331264 * g + 0.5 * b;
                    int Cr = 128 + 0.5 * r - 0.418688 * g - 0.081312 * b;

                    histogram[Y]++;

                    // memorize in a column vector all the pixels of the image in RGB format ( the yuv_vector is initialized like rows*cols*3 )
                    ptr_image[pixel_idx] = Y;
                    ptr_image[pixel_idx + 1] = Cb;
                    ptr_image[pixel_idx + 2] = Cr;
                }
            }     
    }
}


// compute the cdf(named cumulative_histogram) and normalize it (named equalized)
void equalize(int* histogram, int* histogram_eq, int width, int height)
{
    //calculate of cdf
    int sum = 0;
    int cdf[256] = { 0 };
    cdf[0] = histogram[0];

    #pragma omp parallel for
    for (int i = 1; i < 256; i++)
    {
        sum += histogram[i];
        cdf[i] = sum;
    }

    #pragma omp parallel for
    for (int i = 1; i < 256; i++) {
        histogram_eq[i] = (int)(((float)cdf[i] - cdf[0]) / ((float)width * height - 1) * 255);
    }
}


void YCbCr_to_RGB(unsigned char* ptr_image, int* equalized, int width, int height)
{
    // edit Y channel with equalized vector and reconvert from YUV to RGB 
    omp_set_dynamic(0);
    #pragma omp parallel num_threads(32) default(shared)
    {
        #pragma omp for schedule(static) collapse(2)
        
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {

                    int index = (i * width + j) * 3;

                    int Y = equalized[ptr_image[index]];
                    int Cb = ptr_image[index + 1];
                    int Cr = ptr_image[index + 2];

                    int R = (int)max(0, min(255, (int)(Y + 1.402 * (Cr - 128))));
                    int G = (int)max(0, min(255, (int)(Y - 0.344136 * (Cb - 128) - (0.714136 * (Cr - 128)))));
                    int B = (int)max(0, min(255, (int)(Y + 1.772 * (Cb - 128))));

                    ptr_image[index] = R;
                    ptr_image[index + 1] = G;
                    ptr_image[index + 2] = B;
                }
            }   
    }
}



int main() {

    // Load the image
    Mat image = imread("images/image5.jpg");
    
    namedWindow("Original Image", 0);
    resizeWindow("Equalized Image", 800, 600);
    imshow("Original Image", image);

    unsigned char* ptr_image = image.ptr();
    int height = image.rows;
    int width = image.cols;

    int* histogram = (int*)malloc(256 * sizeof(int));
    int* histogram_eq = (int*)malloc(256 * sizeof(int));

    double start = omp_get_wtime();

    RGB_to_YCbCr(ptr_image, histogram, width, height);
    equalize(histogram, histogram_eq, width, height);
    YCbCr_to_RGB(ptr_image, histogram_eq, width, height);

    double end = omp_get_wtime();
    cout << "\nTime with OpenMP: " << (end - start) * 1000 << endl << endl;

    cout << "Saving equalized image..." << endl;
    imwrite("images/image_equalized_openmp.jpg", image);
    cout << "Image saved" << endl;

    // Display equalized image
    namedWindow("Equalized Image", 0);
    resizeWindow("Equalized Image", 800, 600);
    imshow("Equalized Image", image);

    waitKey(0);

    return 0;
}
