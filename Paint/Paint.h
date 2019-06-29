#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "fontcolor.h"

using namespace std;
//! A Paint Class
/*!
  This is part of my learning of OpenCV. I have implemented this simple paint
  program as part of my exercise.
*/
class Paint {

public:

    //! Drawing modes
    /*! These modes decide which shape is being drawn on Window */
    enum Drawing_Mode {
        LINES,      /*!< sets drawing mode to lines */
        CIRCLES,    /*!< sets drawing mode to circles */
        RECTANGLES, /*!< sets drawing mode to rectangles */
        ELLIPSES,   /*!< sets drawing mode to ellipses */
        POLYGON,    /*!< sets drawing mode to polygon */
        ERASE       /*!< erase mode */
    };
    Drawing_Mode mode = LINES;

    bool drawing;   /*!< state of program */
    CvRect box;
    CvPoint endpoint;   // for drawing lines
    CvPoint circle_center;
    CvPoint *poi;
    int num_poly, i_num_poly;
    int erase_thickness;

    Paint();
    void draw_box( IplImage *img );
    void draw_line( IplImage *img );
    void draw_circle( IplImage *img );
    void draw_ellipse( IplImage *img );
    void draw_polygon( IplImage *img );
    void erase( IplImage *img );
    ~Paint();
};
