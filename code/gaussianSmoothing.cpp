/*! smoothing program */ 
#include <opencv/cv.h>
#include <opencv/highgui.h>

/*!
	Are the output results nearly the same by smoothing the image twice with a
	5-by-5 Gaussian filter as when you smooth once with two 11-by-11 filters? Why
	or why not?
	Answer : output results are different. Smoothing is more in case of 2 11-by-11
			 because kernel size is more.
 */

int main( int argc, char **argv ) {
	if ( argc != 2) {
		fprintf( stderr, "Error : input number of argument , plz provide only 2 command line arg\n" );
		return -1;
	}

	/*! 3, 5, 9, 11 */ 
	IplImage *input_image = cvLoadImage( argv[1], CV_LOAD_IMAGE_UNCHANGED ), *output_image;
	cvNamedWindow( "original image", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "3x3", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "5x5", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "9x9", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "11x11", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "2x5x5", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "2x11x11", CV_WINDOW_AUTOSIZE );
	cvMoveWindow( "3x3", input_image->width, 80 );
	cvMoveWindow( "5x5", 2*input_image->width, 80 );
	cvMoveWindow( "9x9", 3*input_image->width, 80 );
	cvMoveWindow( "11x11", 0 , 80 + input_image->height );
	cvMoveWindow( "2x5x5", input_image->width, 80 + input_image->height );
	cvMoveWindow( "2x11x11", 2*input_image->width, 80 + input_image->height );

	output_image = cvCreateImage( 
		cvGetSize( input_image ),
		IPL_DEPTH_8U,
		3
	);

	/*! reduces region / brightness decreases  */

	cvShowImage( "original image", input_image );

	/*! expands region / brightness increases  */

	cvSmooth( input_image, output_image, CV_GAUSSIAN, 3, 3 );

	cvShowImage( "3x3", output_image );

	cvSmooth( input_image, output_image, CV_GAUSSIAN, 5, 5 );

	cvShowImage( "5x5", output_image );

	cvSmooth( input_image, output_image, CV_GAUSSIAN, 9, 9 );

	cvShowImage( "9x9", output_image );

	cvSmooth( input_image, output_image, CV_GAUSSIAN, 11, 11 );

	cvShowImage( "11x11", output_image );

	cvSmooth( input_image, output_image, CV_GAUSSIAN, 5, 5 );
	cvSmooth( output_image, output_image, CV_GAUSSIAN, 5, 5 );

	cvShowImage( "2x5x5", output_image );

	cvSmooth( input_image, output_image, CV_GAUSSIAN, 11, 11 );
	cvSmooth( output_image, output_image, CV_GAUSSIAN, 11, 11 );

	cvShowImage( "2x11x11", output_image );		
	
	cvWaitKey( 0 );

	cvReleaseImage( &input_image );
	cvReleaseImage( &output_image );
	cvDestroyWindow( "original image" );
	cvDestroyWindow( "3x3" );
	cvDestroyWindow( "5x5" );
	cvDestroyWindow( "9x9" );
	cvDestroyWindow( "11x11" );
	cvDestroyWindow( "2x5x5" );
	cvDestroyWindow( "2x11x11" );
	return 0;
}