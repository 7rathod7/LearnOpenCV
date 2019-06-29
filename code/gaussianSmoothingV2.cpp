/*! smoothing program */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;
/*! 
	1.Smooth this image with a 5-by-5 Gaussian filter and display the results. What
	did you find?
	Answer : the intensity of single pixel is now shared among group of pixel
	with low intensity with some space occupancy( I mean non zero pixels ).
	2.Smooth this image with a 9-by-9 Gaussian filter and display the results. What
	did you find?
	Answer : the intensity of single pixel is now shared among group of pixel
	with much low intensity with wider space occupancy( I mean non zero pixels ).
	3.What does it look like if you start over and smooth the image twice with the
	5-by-5 filter? Compare this with the 9-by-9 results. Are they nearly the same?
	Why or why not?
	Answer : there is much difference in intensities but space occupancy is more
	in 11-by-11 because it  might be clear from formula.
*/

int main( int argc, char **argv ) {

	/*! create 1 channel image of 100-by-100 pixel empty image	*/
	IplImage *image = cvCreateImage( cvSize( 100, 100 ), IPL_DEPTH_8U , 1 );
	IplImage *output = cvCreateImage( cvSize( 100, 100 ), IPL_DEPTH_8U , 1 );
	cvZero( image );

	/*! center pixel should have value 255	*/
	uchar *pixel = (uchar*) image->imageData;
	pixel[ 50 * image->widthStep + 50 ] = 255;
	// pixel[ 50 * image->widthStep + 51 ] = 255;
	// pixel[ 50 * image->widthStep + 52 ] = 255;
	// pixel[ 50 * image->widthStep + 53 ] = 255;
	// pixel[ 50 * image->widthStep + 54 ] = 255;
	// pixel[ 51 * image->widthStep + 50 ] = 255;
	// pixel[ 51 * image->widthStep + 51 ] = 255;
	// pixel[ 51 * image->widthStep + 52 ] = 255;
	// pixel[ 51 * image->widthStep + 53 ] = 255;
	// pixel[ 51 * image->widthStep + 54 ] = 255;

	/*!	display image	*/
	cvNamedWindow( "smoothing", CV_WINDOW_AUTOSIZE );
	cvShowImage( "smoothing", image );
	cvWaitKey( 0 );

	/*! apply gaussian filter 5-by-5 ? 
		after applying output image is complete dark */
	cvSmooth( image, output, CV_GAUSSIAN, 5, 5 );
	cvSmooth( output, output, CV_GAUSSIAN, 5, 5 );
	cvShowImage( "smoothing", output );
	cvWaitKey( 0 );
	pixel = (uchar*)output->imageData;

	// int count = 0;
	// for ( int i = 0; i < 100; i++) {
	// 	for ( int j = 0; j < 100; j++) {
	// 		if ( pixel[ i * image->widthStep + j ] ) count++;
	// 	}
	// }

	// cout << (int)pixel[ 50*output->widthStep + 50 ] << "\t" << count << "\n";

	/*! apply gaussian filter 9-by-9 ? 
		after applying output image is complete dark */
	cvSmooth( image, output, CV_GAUSSIAN, 9, 9 );
	pixel = (uchar*)output->imageData;
	cvShowImage( "smoothing", output );
	cvWaitKey( 0 );
	// count = 0;
	// for ( int i = 0; i < 100; i++) {
	// 	for ( int j = 0; j < 100; j++) {
	// 		if ( pixel[ i * image->widthStep + j ] ) count++;
	// 	}
	// }

	// cout << (int)pixel[ 50*output->widthStep + 50 ] << "\t" << count << "\n";
	cvReleaseImage( &image );
	cvReleaseImage( &output );
	cvDestroyWindow( "smoothing" );

	return 0;
}