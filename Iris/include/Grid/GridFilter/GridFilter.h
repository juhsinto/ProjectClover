#ifndef GRID_PROCESSOR_H
#define GRID_PROCESSOR_H

// openCV headers
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// Exception classes
#include "Exceptions/GridFilter/ExceedsMaxValueException.h"
#include "Exceptions/GridFilter/ExceedsMinValueException.h"

using namespace std;


/** \brief Class GridFilter()
*
*   This class filters points from the intersection points obtained from GridMaker,
*   of those points which lie in the specified ROI
*/
class GridFilter {
private:
    //to store the grid obtained from GridMaker
    cv::Mat template_grid;

    //to store contours
    vector<vector<cv::Point> > contours;

    //to store hierarchy information of contours
    vector<cv::Vec4i> hierarchy;

    //set to store x,y co-ordinates of center points
    set<int> set_x;
    set<int> set_y;

    //this function processes the mass centers of the contours - only the contours within the ROI is taken
    void processContourMassCenters(int, int, int, int);

    void applyCannyToTemplate(int thresh1, int thresh2);

    void findContoursInTemplate();

public:
    GridFilter(cv::Mat& GridPointsBin, int xMin, int yMin, int xMax, int yMax);
    ~GridFilter() {}

    //void setBoundaryROI(int, int, int, int);

    set<int> getSetX();
    set<int> getSetY();
};

GridFilter::GridFilter(cv::Mat& GridPointsBin, int xMin, int yMin, int xMax, int yMax)  {

    try {
        // check if the points are LESS than min allowed values for the 300dpi template
        if(xMin < 0 || yMin < 0 || xMax < 0 || yMax < 0) {
            throw ExceedsMinValueException();
        }

        // check if the points are MORE than min allowed values for the 300dpi template
        if(xMin > 820 || yMin > 670 || xMax < 2130 || yMax < 3155) {
            throw ExceedsMaxValueException();
        }

        template_grid = GridPointsBin;

        findContoursInTemplate();

        //! IMP - these are hardcoded values but can be adjusted
        processContourMassCenters(xMin, xMax, yMin, yMax);

    }

    catch(ExceedsMinValueException eminve) {
        std::cerr << "EXCEPTION: input values for ROI within template are not valid - exceeds MinValue" << std::endl;
        throw eminve;
    }

    catch(ExceedsMaxValueException emaxve) {
        std::cerr << "EXCEPTION: input values for ROI within template are not valid - exceeds MaxValue" << std::endl;
        throw emaxve;
    }

    catch(...) {
        std::cerr << "EXCEPTION: Unknown Exception occurred" << std::endl;
    }

}

void GridFilter::applyCannyToTemplate(int thresh1, int thresh2) {

    //threshold1 – first threshold for the hysteresis procedure.
    //threshold2 – second threshold for the hysteresis procedure.
    Canny(template_grid, template_grid, thresh1, thresh2);
}

void GridFilter::findContoursInTemplate() {

    findContours( template_grid, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cv::Point(0, 0) );

#if DEBUG
    cout << "Size of contours  [vector] is " << contours.size() << endl;
    cout << "Size of hierarchy [vector] is " << hierarchy.size() << endl;

    // Draw contours
    cv::Mat drawing = cv::Mat::zeros( template_grid.size(), CV_8UC3 );
    for( unsigned int i = 0; i< contours.size(); i++ ) {
        //cout << i << endl;
        cv::Scalar color(0,0,255);
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
    }

    cv::imshow("template_grid", drawing);
    cv::imwrite("8x template_grid123.png", drawing);
#endif
}

// Boundary co-ordinate pairs, where main gridpoints are to be extracted
// xMin, xMax; yMin, yMax
void GridFilter::processContourMassCenters(int xMin, int xMax, int yMin, int yMax) {

    vector<cv::Moments> mu(contours.size() );
    for( unsigned int i = 0; i < contours.size(); i++ ) {
        mu[i] = moments( contours[i], false );
    }

    vector<cv::Point2f> mc( contours.size() );
    for( unsigned int i = 0; i < contours.size(); i++ ) {
        mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );


        //!IMPORTANT THIS IS WHERE THE CRITERA FOR MAIN GRID IS
        if(mc[i].x < xMin  || mc[i].x > xMax)
            continue;

        if(mc[i].y < yMin  || mc[i].y > yMax)
            continue;
        //!IMPORTANT THIS IS WHERE THE CRITERA FOR MAIN GRID IS

        set_x.insert(mc[i].x);
        set_y.insert(mc[i].y);
    }

#if DEBUG
    cv::Mat drawing = cv::Mat::zeros( template_grid.size(), CV_8UC3 );
    for( unsigned int i = 0; i < contours.size(); i++ ) {
        //cout << mc[i].x << ", " << mc[i].y << endl;

        cv::Scalar color(0,0,255);
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
        circle( drawing, mc[i], 3, cv::Scalar(0,255,0), -1, 8, 0 );
    }
    imwrite("9x overlayed.png", drawing);
#endif
}

/** \brief  Function getSetX()
*
*   \return set<int> - which has non-repeating points on X axis
*/
set<int> GridFilter::getSetX() {
    return set_x;
}

/** \brief  Function getSetY()
*
*   \return set<int> - which has non-repeating points on Y axis
*/
set<int> GridFilter::getSetY() {
    return set_y;
}

#endif // GRID_PROCESSOR_H
