/*! detecting dominating points within an image program */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
/*! 
	
*/

int main( int argc, char **argv ) {

	if ( argc != 2 ) {
		fprintf( stderr, "Error : invalid number of arguments passed to program\n" );
		exit( -1 );
	}

	/*! Load an interesting image	*/
	IplImage *image = cvLoadImage( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    /*! create output image */
    IplImage *output = cvCreateImage(
		cvGetSize( image ),
		image->depth,
		1
	);
    /*! creating gray scale image for edge detection */
    IplImage *edge = cvCreateImage(
		cvGetSize( image ),
		image->depth,
		1
	);

	cvNamedWindow( "Input1", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Input1", image );
	cvNamedWindow( "Output1", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Output2", CV_WINDOW_AUTOSIZE );

    /*! threshold the input image */
    cvThreshold( image, edge, 230, 255, CV_THRESH_BINARY );
    cvShowImage( "Output1", edge );
	
    /*! generate the contours */
    CvMemStorage *storage = cvCreateMemStorage();
    CvSeq *contours = NULL;
    int Nc = cvFindContours( edge, storage, &contours, sizeof( CvContour ), CV_RETR_LIST );

    /*! display the contours */
    printf( "Total contours found = %d\n", Nc );
    cvDrawContours( output, contours, cvScalarAll( 255 ), cvScalarAll( 255 ), 10 );

    /*! generate the dominant points */ // fix here, cvfindodminatpoints
    CvMemStorage *dominantstorage = cvCreateMemStorage();
    CvSeq *dominant = cvApproxPoly( contours, sizeof( CvContour ), dominantstorage, CV_POLY_APPROX_DP, 3, 1 );

    printf( "dominant total = %d\n", dominant->total );

    /*! display the dominant points */
    int i, idx;
    CvPoint p;
    for( i = 0; i < dominant->total; i++ ) {
        idx = *(int *) cvGetSeqElem( dominant, i );
        p = *(CvPoint *) cvGetSeqElem( contours, idx );
        cvDrawCircle( output, p, 1, CV_RGB( 255, 0, 0 ) );
        printf( "%d %d %d\n", idx, p.x, p.y );
    }

    /*! show output */
    cvShowImage( "Output2", output );

    cvWaitKey( 0 );
    cvReleaseMemStorage( &storage );
    cvReleaseMemStorage( &dominantstorage );
	cvReleaseImage( &image );
    cvReleaseImage( &edge );
	cvReleaseImage( &output );
	cvDestroyWindow( "Input1" );
	cvDestroyWindow( "Output1" );
	cvDestroyWindow( "Output2" );
	

	return 0;
}