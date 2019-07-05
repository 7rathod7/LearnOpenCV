//! smoothing program
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main( int argc, char **argv ) {

	if ( argc != 2) {
		fprintf( stderr, "Error : input number of argument , plz provide only 2 command line arg\n" );
		return -1;
	}

	IplImage *input_image = cvLoadImage( argv[1] ), *output_image;

	if ( input_image == NULL ) {
        fprintf( stderr, "Error : failed to load image\n");
        exit( 1 );
	}

	output_image = cvCreateImage(
		cvGetSize( input_image ),
		IPL_DEPTH_8U,
		3
	);

	if ( output_image == NULL ) {
        fprintf( stderr, "Error : failed to create image\n");
        exit( 1 );
	}

	// cvSmooth( input_image, output_image, CV_GAUSSIAN, 13 );
    cvNamedWindow( "original image", CV_WINDOW_AUTOSIZE );
	cvShowImage( "original image", input_image );

	cvSmooth( input_image, output_image, CV_BILATERAL, 10, 10, 120, 10 );
    cvNamedWindow( "mutated image", CV_WINDOW_AUTOSIZE );
	cvMoveWindow( "mutated image", input_image->width + 200, 80 );
	//cvAbsDiff( input_image, output_image, output_image );
	cvShowImage( "mutated image", output_image );
	cvWaitKey( 0 );

	cvReleaseImage( &input_image );
	cvReleaseImage( &output_image );
	cvDestroyWindow( "original image" );
	cvDestroyWindow( "mutated image" );
	return 0;
}
