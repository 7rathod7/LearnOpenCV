#include "Paint.h"

Paint::Paint() {
    drawing = false;   /*!< state of program */
    box = cvRect( 0, 0, 0, 0);
    endpoint = cvPoint( 0, 0 );   // for drawing lines
    circle_center = cvPoint( 0, 0 );
}

Paint::~Paint() {
}

/*
 * A little subroutine to draw a rectangles onto a image
 */
void Paint::draw_box( IplImage *img ) {
    cvRectangle(
        img,
        cvPoint( box.x, box.y ),
        cvPoint( box.x + box.width, box.y + box.height ),
        cvScalar( 0x00, 0x00, 0xff )    /* red */
    );
}

/*!
 A little subroutine to draw a lines onto a image
*/
void Paint::draw_line( IplImage *img ) {
    cvLine( img, cvPoint( box.x, box.y ), cvPoint( endpoint.x, endpoint.y ), cvScalar( 255 ) );
}

/*
 * A little subroutine to draw a circles onto a image
 */
void Paint::draw_circle( IplImage *img ) {
    cvCircle( img, circle_center, cvSqrt( ( box.width * box.width ) + ( box.height * box.height ) ), cvScalar( 0, 255, 255 ) );
}

/*
 * A little subroutine to draw a ellipses onto a image
 */
void Paint::draw_ellipse( IplImage *img ) {
    cvEllipse( img, circle_center, cvSize( abs( box.width ) , abs( box.height ) ), 0, 0, 360, cvScalar( 255, 255 ) );
}

/*
 * A little subroutine to draw a polygons onto a image
 */
void Paint::draw_polygon( IplImage *img ) {
    cvFillPoly( img, &poi, &num_poly, 1, cvScalar( 100, 100, 255 ) );
}

/*
 * A little subroutine to perform erase operation onto a image
 */
void Paint::erase( IplImage *img ) {
    cvCircle( img, circle_center, 1, cvScalar( 0, 0, 0 ), erase_thickness );
}