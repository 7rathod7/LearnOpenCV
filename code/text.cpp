//! smaple program to implement write text on image
/*!
	chapter 4 exercise 6
*/
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

bool drawing_box = false;
CvRect box = cvRect( 0 , 0 , 0 , 0 );


/*!
 A little subroutine to draw a box onto a image
*/
void draw_box( IplImage *img, CvRect rect ) {
    cvRectangle(
        img,
        cvPoint( box.x, box.y ),
        cvPoint( box.x + box.width, box.y + box.height ),
        cvScalar( 0x00, 0x00, 0xff )    /* red */
    );
}

/*!
	define our callback which we will install for mouse events
*/
void my_mouse_callback( int event, int x, int y, int flags, void* param ) {
    IplImage *image = (IplImage*) param;
    switch ( event ) {
        case CV_EVENT_MOUSEMOVE:
            // if ( drawing_box ) {
            //     box.width = x - box.x;
            //     box.height = y - box.y;
            // }
            break;
        
        case CV_EVENT_LBUTTONDOWN:
            if ( !drawing_box ) {
                drawing_box = true;
                box = cvRect( x, y, 0, 40 );
            }
            break;
        
        case CV_EVENT_LBUTTONUP:
            // drawing_box = false;
            // if ( box.width < 0 ) {
            //     box.x += box.width;
            //     box.width *= -1;
            // }
            // if ( box.height < 0 ) {
            //     box.y += box.height;
            //     box.height *= -1;
            // }
            // draw_box( image, box );
        
        default:
            break;
    }
}

//! main function
int main( int argc, char ** argv ) {
	box = cvRect( -1, -1, 0, 40 );
    IplImage *image = cvCreateImage(
        cvSize( 500, 500 ),
        IPL_DEPTH_8U,
        3
    );
    cvZero( image );
    IplImage *temp = cvCloneImage( image );
	cvNamedWindow( "Text" );

	cvSetMouseCallback(
		"Text",
		my_mouse_callback,
		image
	);

	// write text and add to image
	char f[1], input_text[100]="";
    CvFont a;
    cvInitFont( &a, CV_FONT_HERSHEY_SIMPLEX, 1, 1);
    int i = 0;
	while( 1 ) {
        cvCopy( image, temp );
        if ( drawing_box ) draw_box( temp, box );
        // width of letter be assumed as 
        cvPutText( temp, input_text, cvPoint( box.x, 30 + box.y ), &a , cvScalar( 0, 0, 255 ) );
		// cvRectangle( temp, cvPoint( 10, 0 ), cvPoint( 10 + 17*i, 40 ), cvScalar( 0, 0, 255 ) );
        cvShowImage( "Text" , temp );
        // cvPutText( temp, input_text, cvPoint( 10 + (i), 30 ), &a , cvScalar( 0, 0, 0 ) );
		int key = cvWaitKey( 33 );
        // if ( key != -1 ) cout << key << endl;
        if ( key == 27 ) break; //! esc button to break the loop
        else if ( drawing_box && 'a' <= key && key <= 'z' ) {
            f[0] = key;
            // cout << f << "\n";
            // strncat( input_text, f, 1 );
            input_text[i] = key;
            box.width += 17;
            if ( i < 99 ) i++;
        }
        else if ( key == 10 ) {
            // cout << "enter\n";
            drawing_box = false;
            cvCopy( temp, image );
            box = cvRect( 0, 0, 0, 0 );
            memset( input_text, 0, 100 * sizeof( char ) );
            i = 0;
        }
        else if ( key == 65288 ) {
            if ( i > 0 ) {
                i--;
                input_text[i] = '\0';
                box.width -= 17;
            }
        }

	}
	cvDestroyWindow( "Text" );
	cvReleaseImage( &image );
	return 0;
}