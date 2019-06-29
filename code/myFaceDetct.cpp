//! face detection program
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

//! globals
//! xml from data(opencv 3.2 is incompatible) whereas xml from data_2_4(opencv 2.4 is compatible)
const char* cascadeName = "data_2_4/haarcascade_frontalface_alt.xml";
IplImage *src2;
int alpha = 0;

void mytrackbar( int pos ) {
}

//! create memory for calculations
static CvMemStorage *storage = 0;

//! create a new Haar classifier
static CvHaarClassifierCascade *cascade = 0;

//! Function to detect and draw any faces that is present in an image
void detectAndDraw( IplImage *image ) {

    int scale = 1;

    //! create a new image based on the input image
    // IplImage *temp = cvCreateImage( CvSize( image->height / scale, image->width / scale ), 8, 3 );

    //! create 2 points to represent the face locations
    CvPoint pt1, pt2;
    int i;

    // cout << "testing\n";
    // cout << "testing1\n";
    // cascade = cvLoadHaarClassifierCascade( cascadeName, cvGetSize( image ));

    //! Allocate the memory storage
    storage = cvCreateMemStorage( 0 );

    //! Clear the memory storage which was used before
    cvClearMemStorage( storage );

    //! Find whether the cascade is loaded, to find the faces. If yes, then
    if ( cascade ) {
        //! There can be more than one face in an image. So create a growable sequence of faces
        //! Detect the objects and store them in the sequence
        CvSeq *faces = cvHaarDetectObjects( image, cascade, storage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize( 40, 40 ) );

        //! Loop the number of faces found
        for( i = 0; i < ( faces ? faces->total : 0 ); i++ ) {
            //! Create a new rectangle for drawing the face
            CvRect *r = (CvRect*)cvGetSeqElem( faces, i );

            //! Find the dimensions of the face and scale it if necessary
            pt1.x = r->x * scale;
            pt2.x = ( r->x + r->width ) * scale;
            pt1.y = r->y * scale;
            pt2.y = ( r->y + r->height ) * scale;

            //! Draw the rectangle in the input image
            cvRectangle( image, pt1, pt2, CV_RGB( 255, 0, 0 ), 3, 8, 0 );
            cvSetImageROI( image, cvRect( pt1.x, pt1.y, pt2.x-pt1.x, pt2.y-pt1.y ));
            IplImage *smallImg = cvCreateImage(cvSize(pt2.x-pt1.x, pt2.y-pt1.y), IPL_DEPTH_8U, 3);
            cvResize(src2,smallImg);
            // cout << 0.1*cvGetTrackbarPos("alpha","face detection") << endl;
            double t = 0.1*cvGetTrackbarPos("alpha","face detection");
            cvSetImageROI( smallImg, cvRect( 0, 0, smallImg->width, smallImg->height ));
            cvAddWeighted( image, t, smallImg, 1.0 - t, 0.0, image );
            cvResetImageROI( image );
            // IplImage *Img = cvCreateImage(cvGetSize( image ), IPL_DEPTH_8U, 4);
            // cvCvtColor(image, Img, CV_RGB2RGBA);
            //! Show the image in the window named "result"
            cvShowImage( "face detection", image );
            cvReleaseImage( &smallImg );
        }
        cvReleaseMemStorage( &storage );
    }

    //! Release the temp image created
    // cvReleaseImage( &temp ); 
}

//! main function
int main( int argc, char **argv ) {
    // IplImage *dummy = cvCreateImage( cvSize(11, 11), IPL_DEPTH_8U, 1 );	/* Make a dummy image */
	// IplConvKernel *se = cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_ELLIPSE, NULL);	/* Create a filter */
	// cvErode(dummy, dummy, se, 1);	/* Erode */
	// cvReleaseImage( &dummy );
    // cvReleaseStructuringElement(&se);
    cvNamedWindow( "face detection", 1 );

    cvCreateTrackbar(
        "alpha",
        "face detection",
        &alpha,
        10,
        mytrackbar
    );

    CvCapture *capture = cvCaptureFromCAM( -1 );
    src2 = cvLoadImage( "res/skull2.png", 1 );

    cascade = (CvHaarClassifierCascade*)cvLoad( cascadeName, 0, 0, 0 );

    //!Load the HaarClassifierCascade
    // IplImage *temp;
    // temp = cvCreateImage( cvSize( 8, 8 ), IPL_DEPTH_8U, 3 );
    
    // cvZero( temp );
    // cvErode( temp, temp , 0, 3 );
    // cvReleaseImage( &temp );

    //! Check whether the cascade has loaded successfully. Else report and error and quit
    if ( !cascade ) {
        fprintf( stderr, "Error: Could not load classifier cascade\n");
        return -1;
    }

    IplImage *cameraImage, *newImage;
    // cvScale( src2, src2, 1 );

    while ( cameraImage = cvQueryFrame( capture ) ) {
        if ( !cameraImage ) break;

        //! flip image
        newImage = cvCreateImage( cvGetSize( cameraImage ), IPL_DEPTH_8U, 1 );
        newImage = cameraImage;
        cvFlip( newImage, newImage, 1 );

        detectAndDraw( newImage );

        // cvShowImage( "face detection", newImage );
        char c = cvWaitKey( 15 );
        if ( c == 27 ) break;
    }   

    cvReleaseImage( &src2 );
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseImage( &newImage );
    cvReleaseImage( &cameraImage );
    cvReleaseCapture( &capture );
    cvDestroyWindow( "face detection" );

    return 0;
}