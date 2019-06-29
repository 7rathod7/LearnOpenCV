#include "Paint.h"

Paint *paint;
int g_switch_value = 0, cnt = 0;

/*
 * slider callback function
 */
void onTrackbarSlide( int pos ) {
    g_switch_value = pos;
}

/*
 * define our callback which we will install for mouse events
 */
void my_mouse_callback( int event, int x, int y, int flags, void* param ) {
    IplImage *image = (IplImage*) param;
    IplImage *temp = cvCreateImage(
        cvGetSize( image ),
        image->depth,
        image->nChannels
    );
    cvZero( temp );
    if ( cvGetTrackbarPos( "Mode( OR, AND, XOR )", "Paint Program" ) == 1 ) temp = cvCloneImage( image );
    char tmp[10]="";
    switch ( event ) {
        case CV_EVENT_MOUSEMOVE:
            if ( paint->drawing ) {
                if ( paint->mode == paint->RECTANGLES || paint->mode == paint->CIRCLES || paint->mode == paint->ELLIPSES ) { 
                    paint->box.width = x - paint->box.x;
                    paint->box.height = y - paint->box.y;
                }
                if ( paint->mode == paint->LINES ) {
                    paint->endpoint.x = x;
                    paint->endpoint.y = y;
                }
            }
            break;
        
        case CV_EVENT_LBUTTONDOWN:
            paint->drawing = true;
            paint->box = cvRect( x, y, 0, 0 );
            if ( paint->mode == paint->CIRCLES || paint->mode == paint->ELLIPSES ) paint->circle_center = cvPoint( paint->box.x, paint->box.y );
            if ( paint->mode == paint->POLYGON && paint->i_num_poly < paint->num_poly ) paint->poi[paint->i_num_poly++] = cvPoint( x, y );
            paint->endpoint.x = x;
            paint->endpoint.y = y;
            break;
        
        case CV_EVENT_LBUTTONUP:
            paint->drawing = false;
            if ( paint->mode == paint->RECTANGLES || paint->mode == paint->CIRCLES || paint->mode == paint->ELLIPSES ) {
                if ( paint->box.width < 0 ) {
                    paint->box.x += paint->box.width;
                    paint->box.width *= -1;
                }
                if ( paint->box.height < 0 ) {
                    paint->box.y += paint->box.height;
                    paint->box.height *= -1;
                }
                if ( paint->mode == paint->RECTANGLES ) paint->draw_box( temp );
                else if ( paint->mode == paint->CIRCLES ) paint->draw_circle( temp );
                else paint->draw_ellipse( temp );
            }
            if ( paint->mode == paint->LINES ) paint->draw_line( temp );
            if ( paint->mode == paint->POLYGON && (paint->num_poly == paint->i_num_poly) && paint->poi ) {
                paint->draw_polygon( temp );
                cout << RED << "Polygon is drawn. Press esc to enter menu\n" << RESET;
                paint->i_num_poly = 0;
            }
            // cout << cvGetTrackbarPos( "Mode( OR, AND, XOR )", "Paint Program" );
            // sprintf( tmp, "%d", cnt++ );
            // strcat( tmp, ".jpg" );
            // cvSaveImage( tmp, image );
            // sprintf( tmp, "%d", cnt++ );
            // strcat( tmp, ".jpg" );
            // cvSaveImage( tmp, temp );
            // cvNamedWindow("yo1");
            // cvShowImage("yo1",temp);
            // cvWaitKey(0);
            // cvNamedWindow("yo2");
            // cvShowImage("yo2",image);
            // cvWaitKey(0);
            // cvDestroyWindow("yo1");
            // cvDestroyWindow("yo2");
            if ( cvGetTrackbarPos( "Mode( OR, AND, XOR )", "Paint Program" ) == 0 ) cvOr( image, temp, image );
            else if ( cvGetTrackbarPos( "Mode( OR, AND, XOR )", "Paint Program" ) == 1 ) cvAnd( image, temp, image );
            else cvXor( image, temp, image );
            // switch ( cvGetTrackbarPos( "Mode( OR, AND, XOR )", "Paint Program" ) ) {
            //     case 0:
            //         cout << "0";
            //         cvOr( image, temp, image );
            //         break;

            //     case 1:
            //         cout << "1";
            //         cvAnd( image, temp, image );
            //         break;

            //     case 2:
            //         cout << "2";
            //         cvXor( image, temp, image );
            //         break;
                
            //     default:
            //         break;
            // }
            break;

        case CV_EVENT_RBUTTONDOWN:
            paint->circle_center = cvPoint( x, y );
            paint->erase( image );
            break;
        
        default:
            break;
    }
    cvReleaseImage( &temp );
}

/*
 * main function
 */
int main( int argc, char **argv) {

    paint = new Paint();

    // box = cvRect( 0, 0, 0, 0 );
    // endpoint.x = 0;
    // endpoint.y = 0;
    // create image and set to zero and display it
    IplImage *image = cvCreateImage(
        cvSize( 1200, 800 ),
        IPL_DEPTH_8U,
        3
    );
    cvZero( image );
    IplImage *temp = cvCloneImage( image );
    cvNamedWindow( "Paint Program", CV_WINDOW_AUTOSIZE );
    cvCreateTrackbar(
            "Mode( OR, AND, XOR )",
            "Paint Program",
            &g_switch_value,
            2,
            onTrackbarSlide
    );

    // attach mouse callback listener
    cvSetMouseCallback(
        "Paint Program",
        my_mouse_callback,
        ( void * ) image
    );

    // draw lines, circles, ellipses, polygons on image using left click is held
    // create eraser function is right button is held

    // cvLine( image, cvPoint( 0, 0 ), cvPoint( 100, 100 ), cvScalar( 255 ) ); // blue line
    // cvRectangle( image, cvPoint( 100, 100 ), cvPoint( 200, 200 ), cvScalar( 0, 255 ) );
    // cvCircle( image, cvPoint( 150, 150 ), 50, cvScalar( 0, 0, 255 ) );
    // cvEllipse( image, cvPoint( 500, 500 ), cvSize( 300, 200 ), 0, 0, 360, cvScalar( 255, 255 ) );
    // int j = 4;
    // poi = new CvPoint[j];
    // poi[0] = cvPoint( 400, 400 );
    // poi[1] = cvPoint( 500, 500 );
    // poi[2] = cvPoint( 500, 400 );
    // poi[3] = cvPoint( 400, 300 );

    // cvFillPoly( image, &poi, &j, 1, cvScalar( 255 ) );
    // cvFillConvexPoly( image, poi, j, cvScalar( 255 ) );
    // cvPolyLine( image, &poi, &j, 1, 1, cvScalar( 255, 255 ) );
    int i;
    cout << BGCOLOR << YELLOW << "|-------------------- Paint Program -------------------------|\n" << RESET;
    char menu[200] = "";
    strcat( menu, "1. Lines(default)\n" );
    strcat( menu, "2. Rectangle\n" );
    strcat( menu, "3. Cirlce\n" );
    strcat( menu, "4. Ellipse\n" );
    strcat( menu, "5. Polygon\n" );
    strcat( menu, "6. Erase\n" );
    strcat( menu, "7. Save\n" );
    strcat( menu, "8. Exit\n" );
    strcat( menu, "Input >> " );
    while ( 1 ) {
        cout << BLUE << menu << RESET;
        cin >> i;
        cout << YELLOW << "|------------------------------------------------------------|\n" << RESET;
        if ( i == 1 ) {
            // code for drawing line
            paint->mode = paint->LINES;
            while ( 1 ) {
                cvCopy( image, temp );  
                if ( paint->drawing ) paint->draw_line( temp );
                cvShowImage( "Paint Program", temp );

                if ( cvWaitKey( 33 ) == 27 ) break;
            }
        }
        else if ( i == 2 ) {
            paint->mode = paint->RECTANGLES;
            // rectangle
            while ( 1 ) {
                cvCopy( image, temp );
                if ( paint->drawing ) paint->draw_box( temp );
                cvShowImage( "Paint Program", temp );

                if ( cvWaitKey( 33 ) == 27 ) break;
            }
        }
        else if ( i == 3 ) {
            // code for circle
            paint->mode = paint->CIRCLES;
            while ( 1 ) {
                cvCopy( image, temp );
                if ( paint->drawing ) paint->draw_circle( temp );
                // cvXor( image, temp, temp );
                cvShowImage( "Paint Program", temp );

                if ( cvWaitKey( 33 ) == 27 ) break;
            }
        }
        else if ( i == 4 ) {
            // code for ellipse
            paint->mode = paint->ELLIPSES;
            while ( 1 ) {
                cvCopy( image, temp );
                if ( paint->drawing ) paint->draw_ellipse( temp );
                cvShowImage( "Paint Program", temp );

                if ( cvWaitKey( 33 ) == 27 ) break;
            }
        }
        else if ( i == 5 ) {
            // code for polygon
            paint->mode = paint->POLYGON;
            cout << WHITE << "enter number of points : " << RESET;
            cin >> paint->num_poly;
            paint->poi = new CvPoint[paint->num_poly];
            while ( 1 ) {
                cvCopy( image, temp );
                if ( paint->drawing && ( paint->i_num_poly == paint->num_poly) ) paint->draw_polygon( temp );
                cvShowImage( "Paint Program", temp );
                if ( cvWaitKey( 33 ) == 27 ) break;
            }
            delete[] paint->poi;
            paint->poi = NULL;
        }
        else if ( i == 6 ) {
            // code for circle to erase
            paint->mode = paint->ERASE;
            cout << WHITE << "enter thickness of erase : " << RESET;
            cin >> paint->erase_thickness;
            while ( 1 ) {
                cvCopy( image, temp );
                if ( paint->drawing ) paint->erase( temp );
                cvShowImage( "Paint Program", temp );

                if ( cvWaitKey( 33 ) == 27 ) break;
            }
        }
        else if ( i == 7 ) cvSaveImage( "paint.jpg", image ); 
        else if ( i == 8 ) break;
    }

    cout << BGCOLOR << YELLOW << "|------------------------------------------------------------|\n" << RESET;

    // allow "logical drawing" using slider with modes AND, OR, XOR

    cvReleaseImage( &image );
    cvReleaseImage ( &temp );
    cvDestroyWindow( "Paint Program" );
    return 0;
}