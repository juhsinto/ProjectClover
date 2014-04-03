#ifndef __GRIDMAKER_H__
#define __GRIDMAKER_H__

//openCV headers
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// To check for file on disk
#include <sys/stat.h>

// Custom headers
#include "Grid/GridMaker/LineFinder.h"
#include "Grid/GridMaker/SobelDerrivative.h"

// Exception classes
#include "Exceptions/Generic/FilePathInvalidException.h"
#include "Exceptions/GridMaker/ImageEmptyException.h"
#include "Exceptions/GridMaker/ImageDimensionInvalidException.h"


/** \brief Class GridMaker()
*
*   This class performs Sobel Derivative on x, and y axes, and gets the lines that are present in both directions.
*   Then it performs an intersection of the x, and y lines, to get grid points.
*/
class GridMaker {
private:
    cv::Mat atemplate;
    cv::Mat blurred;

    cv::Mat horizontal_bin_lines;
    cv::Mat vertical_bin_lines;

    cv::Mat GridPoints;

    cv::Mat getHorizontalLines(cv::Mat& blurred_template);
    cv::Mat getVerticalLines(cv::Mat& blurred_template);

    // Checks if the file exists or not.
    inline bool does_file_exist (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }

public:
    GridMaker(std::string filename);
    ~GridMaker() {}

    cv::Mat getGrid();
};

GridMaker::GridMaker(std::string filename) {

    try {
        // check if the file exists on disk
        if(!does_file_exist(filename)) {
            throw FilePathInvalidException(filename);
        }

        //set the template to the clean image.
        atemplate = cv::imread(filename, 1);

        // check if the image is empty (contains no pixels)
        if(atemplate.empty()) {
            throw ImageEmptyException(filename);
        }

        // check if image dimensions are 300dpi
        if (!atemplate.rows > 3000 && atemplate.cols < 2000) {
            throw ImageDimensionInvalidException(filename);
        }

        //remove noise
        cv::GaussianBlur(atemplate, blurred, cv::Size(3,3), 0, 0);

        //Create Mat with size of [input template], and type [8bit Single Channel] - for (Mat) GridPoints
        GridPoints.create(atemplate.size(), CV_8UC1);

        //get horizontal bin lines
        horizontal_bin_lines = getHorizontalLines(blurred);
        vertical_bin_lines = getVerticalLines(blurred);

    }

    catch(FilePathInvalidException ipie) {
        std::cerr << "EXCEPTION: Input file " << ipie.getPath() << " does not exist on disk " << std::endl;
        throw ipie;
    }

    catch(ImageEmptyException iee) {
        std::cerr << "EXCEPTION: Input image " << iee.getImageName() << " is empty (no pixel data)" << std::endl;
        throw iee;
    }

    catch(ImageDimensionInvalidException idie) {
        std::cerr << "EXCEPTION: Input image " << idie.getImageName() << " is not of the right dimensions" << std::endl;
        throw idie;
    }

    catch(...) {
        std::cerr << "EXCEPTION: Unknown Exception occurred" << std::endl;
    }
}

/** \brief  Function getGrid()
*
*   \return Mat - with intersection points.
*/
cv::Mat GridMaker::getGrid() {

    cv::Mat	IntersectionPoints(atemplate.size(),CV_8UC3);
    IntersectionPoints.setTo(cv::Scalar(0,0,0));

    //get the intersection of vertical and horizontal lines. (bitwise AND)
    bitwise_and(horizontal_bin_lines,vertical_bin_lines, IntersectionPoints);

    cv::threshold(IntersectionPoints, GridPoints, 190, 200, CV_THRESH_BINARY);

#if DEBUG
    cv::imshow("grid",GridPoints);
    cv::imwrite("7x IntersectionPoints.jpg", GridPoints);
#endif

    return GridPoints;
}

/** \brief  Function getHorizontalLines()
*
*   \param  blurred_template Mat
*   \return Mat - which has only horizontal lines
*/
cv::Mat GridMaker::getHorizontalLines(cv::Mat& blurred_template) {
    //=================================================================
    // Sobel Derrivative to truncate pixels of horizontal/vertical lines
    SobelDerrivative sd1;
    sd1.setDerrivativeY(2);
    cv::Mat SobelY = sd1.getSobelDerrivativeX(blurred_template);

#if DEBUG
    cv::imshow("sobel Image horizontal", SobelY);
    cv::imwrite("1x horizontal.jpg", SobelY);
#endif

    cv::GaussianBlur(SobelY, SobelY, cv::Size(3,3), 0, 0);

    // Apply canny for edge detection
    //cannyContours = applyCanny(SobelY, 150, 200);
    Canny(SobelY, SobelY, 150, 200);

#if DEBUG
    cv::imshow("Canny Image horizontal", SobelY);
    cv::imwrite("2x canny_horizontal.jpg", SobelY);
#endif
    //=================================================================
    // Create LineFinder instance
    LineFinder finder;

    //linelength => min length ; gap => max gap allowed between a line
    finder.setLineLengthAndGap(600,300);
    //Points that need to be in direction of line, to consider as line
    finder.setMinVote(80);

    // Detect lines and draw them
    std::vector<cv::Vec4i> lines= finder.findLines(SobelY);

    cv::Mat horizontal_bin(atemplate.size(), CV_8UC1);
    finder.drawDetectedLines(horizontal_bin);

#if DEBUG
    cv::imshow("horizontal lines", horizontal_bin);
    cv::imwrite("3x horizontal_bin.jpg", horizontal_bin);
#endif
    //=====================================================================
    return horizontal_bin;
}

/** \brief  Function getVerticalLines()
*
*   \param  blurred_template Mat
*   \return Mat - which has only vertical lines
*/
cv::Mat GridMaker::getVerticalLines(cv::Mat& blurred_template) {
    //=================================================================
    // Sobel Derrivative to truncate pixels of horizontal/vertical lines
    SobelDerrivative sd2;
    sd2.setDerrivativeX(2);
    cv::Mat SobelX = sd2.getSobelDerrivativeX(blurred_template);

#if DEBUG
    cv::imshow("sobel Image vertical", SobelX);
    cv::imwrite("4x vertical.jpg", SobelX);
#endif

    cv::GaussianBlur(SobelX, SobelX, cv::Size(3,3), 0, 0);

    // Apply canny for edge detection
    Canny(SobelX, SobelX, 150, 200);

#if DEBUG
    cv::imshow("Canny Image vertical", SobelX);
    cv::imwrite("5x canny_vertical.jpg", SobelX);
#endif
    //=================================================================
    // Create LineFinder instance
    LineFinder finder;

    //line-length => min length ; gap => max gap allowed between a line
    finder.setLineLengthAndGap(1000,300);
    //Points that need to be in direction of line, to consider as line
    finder.setMinVote(80);

    // Detect lines and draw them
    std::vector<cv::Vec4i> lines= finder.findLines(SobelX);

    cv::Mat vertical_bin(atemplate.size(), CV_8UC1);
    finder.drawDetectedLines(vertical_bin);
#if DEBUG
    cv::imshow("vertical lines", vertical_bin);
    cv::imwrite("6x vertical_bin.jpg", vertical_bin);
#endif
    //=====================================================================
    return vertical_bin;
}

#endif // __GRIDMAKER_H__
