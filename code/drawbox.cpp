/*! sample box drawing Program */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

CvRect box;
bool drawing_box = false;

/*! A little subroutine to draw a box onto a image */
void draw_box( IplImage *img ) {
    cvRectangle(
        img,
        cvPoint( box.x, box.y ),
        cvPoint( box.x + box.width, box.y + box.height ),
        cvScalar( 0x00, 0x00, 0xff )    /* red */
    );
}


/*! define our callback which we will install for mouse events */
void my_mouse_callback( int event, int x, int y, int flags, void* param ) {
    IplImage *image = (IplImage*) param;
    switch ( event ) {
        case CV_EVENT_MOUSEMOVE:
            if ( drawing_box ) {
                box.width = x - box.x;
                box.height = y - box.y;
            }
            break;
        
        case CV_EVENT_LBUTTONDOWN:
            drawing_box = true;
            box = cvRect( x, y, 0, 0 );
            break;
        
        case CV_EVENT_LBUTTONUP:
            drawing_box = false;
            if ( box.width < 0 ) {
                box.x += box.width;
                box.width *= -1;
            }
            if ( box.height < 0 ) {
                box.y += box.height;
                box.height *= -1;
            }
            draw_box( image );
        
        default:
            break;
    }
}

/*! main function */
int main( int argc, char **argv ) {

    box = cvRect( -1, -1, 0, 0 );
    IplImage *image = cvCreateImage(
        cvSize( 200, 200 ),
        IPL_DEPTH_8U,
        3
    );
    cvZero( image );
    IplImage *temp = cvCloneImage( image );

    cvNamedWindow( "Box Example" );

    /*! here is the crucial moment that we actually install the callback
        Note that we set the value 'param' to be the image we are working with so
        that the callback will have the image to edit   */
    cvSetMouseCallback(
        "Box Example",
        my_mouse_callback,
        ( void * ) image
    );

    /*! the main program loop. here we copy the working image
        to the temp image and if the user is drawing, then put
        the currently contemplated box onto that temp image
        display the temp image, and wait 15ms for a keystroke the repeat */
    while( 1 ) {
        cvCopy( image, temp );
        if ( drawing_box ) draw_box( temp );
        cvShowImage( "Box Example", temp );

        if ( cvWaitKey( 15 ) == 27 ) break;
    }

    /*! Be tidy */
    cvReleaseImage ( &image );
    cvReleaseImage ( &temp );
    cvDestroyWindow ( "Box Example" );

    return 0;
}