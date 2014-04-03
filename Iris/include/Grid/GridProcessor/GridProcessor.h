#ifndef GRID_FILTER_H
#define GRID_FILTER_H

//openCV headers
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//Custom headers
#include "ProcessPoints.h"

#include "Exceptions/GridProcessor/EmptySetException.h"
#include "Exceptions/GridProcessor/GridNumberOfColsException.h"
#include "Exceptions/GridProcessor/GridNumberOfRowsException.h"

using namespace std;

/** \brief Class GridProcessor()
*
*   This class processes the set of points into a stack, and then makes an iteratable zig-zag 2D vector
*/
class GridProcessor {
private:

    // Vectors containing x,y co-ordinates of GridPoints
    vector<int> x_values;
    vector<int> y_values;

    // Vector for temporarily storing zig-zag co-ordinates for one row
    vector<cv::Point2i> colPoints;

    // 2dim vector, of rows containing zig-zag co-ordinates of GridPoints
    vector<vector<cv::Point2i> > TwoDimGridPoints;
    cv::Point2i point;

public:
    GridProcessor(set<int>& x_points, set<int>& y_points);
    ~GridProcessor() {}

    void processGridPoints();
    vector<vector<cv::Point2i> > getProcessedGridPoints();
};

GridProcessor::GridProcessor(set<int>& x_points, set<int>& y_points)  {

    try {
        //check if set is empty
        if(x_points.size() == 0 || y_points.size() == 0) {
            throw EmptySetException();
        }

        ProcessPoints pp1;
        pp1.push_data_to_stack(x_points);
        pp1.processStack();
        x_values = pp1.get_processed_points();

        ProcessPoints pp2;
        pp2.push_data_to_stack(y_points);
        pp2.processStack();
        y_values = pp2.get_processed_points();

#if DEBUG
        cout << endl;
        cout << "number of x points in x stack " << x_values.size() << endl;
        cout << "number of y points in y stack " << y_values.size() << endl;
#endif

        //check if number of points in x is proper for default template
        if (x_values.size() != 14) {
            throw GridNumberOfColsException(x_values.size());
        }

        //check if number of points in y is proper for default template
        if (y_values.size() != 29) {
            throw GridNumberOfRowsException(y_values.size());
        }

        processGridPoints();

    }

    catch(EmptySetException ese) {
        std::cerr << "EXCEPTION: set is empty - exceeds MaxValue" << std::endl;
        throw ese;
    }

    catch(GridNumberOfColsException gnoce) {
        std::cerr << "EXCEPTION: Grid is not default, num of pts currently is " << gnoce.getNumberOfCols() << " should be 14" <<  std::endl;
        throw gnoce;
    }

    catch(GridNumberOfRowsException gnore) {
        std::cerr << "EXCEPTION: Grid is not default, num of pts currently is " << gnore.getNumberOfRows() << " should be 29"<<  std::endl;
        throw gnore;
    }

}

/** \brief Function processGridPoints()
*   This function makes an iteratable zig-zag 2D vector, see diagram for simple explanation
*/
void GridProcessor::processGridPoints() {

    std::vector<int>::iterator it_x, it_y;
    it_y = y_values.begin();
    while(it_y != y_values.end()-1 )	{

#if DEBUG
        std::cout << "\n------------------------------------------------" << endl;
#endif

        it_x = x_values.begin();
        while(it_x != x_values.end()-1 ) {

#if DEBUG
            cout << *it_x << "," << *it_y << endl;
#endif

            point.x = *it_x;
            point.y = *it_y;
            //points.push_back(point);
            colPoints.push_back(point);

            //iterate through x one step
            if(it_x != x_values.end()) {

                it_x++;
                it_y++;

#if DEBUG
                cout << *it_x << "," << *it_y << endl;
#endif

                point.x = *it_x;
                point.y = *it_y;
                //points.push_back(point);
                colPoints.push_back(point);
            }

            it_y--;
        }
        TwoDimGridPoints.push_back(colPoints);
        colPoints.clear();

        it_y++;
    }
}

/** \brief Function getProcessedGridPoints()
*           This function makes an iteratable zig-zag 2D vector, see diagram for simple explanation
*
*   \return vector<vector<cv::Point2i> > - which is a 2d vector -  RowColumn Grid - of zig-zag points.
*/
vector<vector<cv::Point2i> > GridProcessor::getProcessedGridPoints() {
    return TwoDimGridPoints;
}

#endif // GRID_FILTER_H
