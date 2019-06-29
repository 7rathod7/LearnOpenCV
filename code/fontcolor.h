/*!the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes. */
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /*! Black */
#define RED     "\033[31m"      /*! Red */
#define GREEN   "\033[32m"      /*! Green */
#define YELLOW  "\033[33m"      /*! Yellow */
#define BLUE    "\033[34m"      /*! Blue */
#define MAGENTA "\033[35m"      /*! Magenta */
#define CYAN    "\033[36m"      /*! Cyan */
#define WHITE   "\033[37m"      /*! White */
#define BOLD    "\033[1m"       /*! Bold */
#define COLOR   "\033[2m"       /*! color text according to next seq and bit darker */
#define ITALIC  "\033[3m"       /*! italic font */
#define ULINE   "\033[4m"       /*! Underline */
#define BLINK   "\033[5m"       /*! blinking effect */
#define NOCLUE  "\033[6m"       /*! no clue as of now */
#define BGCOLOR "\033[7m"       /*! Background color */
#define INVISIBLE   "\033[8m"   /*! invisible text */
#define STRK    "\033[9m"       /*! Strike text */
#define LEFTSPACE   "\033[D"    /*! move cursor one spacing left */
/*! #define BOLDBLACK   "\033[1m\033[30m"   */      /*! Bold Black */
/*! #define BOLDRED     "\033[1m\033[31m"   */      /*! Bold Red */
/*! #define BOLDGREEN   "\033[1m\033[32m"   */      /*! Bold Green */
/*! #define BOLDYELLOW  "\033[1m\033[33m"   */      /*! Bold Yellow */
/*! #define BOLDBLUE    "\033[1m\033[34m"   */      /*! Bold Blue */
/*! #define BOLDMAGENTA "\033[1m\033[35m"   */      /*! Bold Magenta */
/*! #define BOLDCYAN    "\033[1m\033[36m"   */      /*! Bold Cyan */
/*! #define BOLDWHITE   "\033[1m\033[37m"   */      /*! Bold White */
/*! \033[0m - is the default color for the console  */
/*! \033[0;#m - is the color of the text, where # is one of the codes mentioned above   */
/*! \033[1m - makes text bold   */
/*! \033[1;#m - makes colored text bold**   */
/*! \033[2;#m - colors text according to # but a bit darker */
/*! \033[4;#m - colors text in # and underlines */
/*! \033[7;#m - colors the background according to #    */
/*! \033[9;#m - colors text and strikes it  */
/*! \033[A - moves cursor one line above (carfull: it does not erase the previously written line)   */
/*! \033[B - moves cursor one line under    */
/*! \033[C - moves cursor one spacing to the right  */
/*! \033[D - moves cursor one spacing to the left   */
/*! \033[E - don't know yet */
/*! \033[F - don't know yet */