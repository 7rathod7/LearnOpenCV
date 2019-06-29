//! smoothing program
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main( int argc, char **argv ) {
	if ( argc != 2) {
		fprintf( stderr, "Error : input number of argument , plz provide only 2 command line arg\n" );
		return -1;
	}
	IplImage *input_image = cvLoadImage( argv[1] ), *output_image;
	cvNamedWindow( "original image", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "mutated image", CV_WINDOW_AUTOSIZE );
	cvMoveWindow( "mutated image", input_image->width + 200, 80 );

	output_image = cvCreateImage( 
		cvGetSize( input_image ),
		IPL_DEPTH_8U,
		3
	);

	// cvSmooth( input_image, output_image, CV_GAUSSIAN, 13 );

	cvShowImage( "original image", input_image );

	cvSmooth( input_image, output_image, CV_BILATERAL, 13, 13 );

	cvShowImage( "mutated image", output_image );
	cvWaitKey( 0 );

	cvReleaseImage( &input_image );
	cvReleaseImage( &output_image );
	cvDestroyWindow( "original image" );
	cvDestroyWindow( "mutated image" );
	return 0;
}