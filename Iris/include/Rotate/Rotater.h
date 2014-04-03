#ifndef __ROTATER_H__
#define __ROTATER_H__

// Exception classes
#include "Exceptions/Rotate/LargestContourInvalidException.h"
#include "Exceptions/Generic/FilePathInvalidException.h"

/** \brief Class Rotater()
*
*   This class find the largest contour (boundary box) in the input image, and then crops the image to that largest contour.
*   Besides doing that, it also rotates the image, to normalized position.
*/
class Rotater {
private:

    // Matrix for storing thresholded, and destination images.
    cv::Mat src, thr,dst;

    cv::Mat rotationMat, rotated, cropped;

    vector<vector<cv::Point> > contours; // Vector for storing contour
    vector<cv::Vec4i> hierarchy;

    //take second largest contour
    vector<cv::Point> pt_largest;

    void processContours();
    void preprocessInputImage();

    void getRotatedLargestContour();
    void debugLargestContour();

    // Custom comparator
    static bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) {
        double i = fabs( contourArea(cv::Mat(contour1)) );
        double j = fabs( contourArea(cv::Mat(contour2)) );
        return ( i < j );
    }

    // Checks if the file exists or not.
    inline bool does_file_exist (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }

public:
    Rotater(string filename);
    ~Rotater() {
    }

    cv::Mat getRotatedMat();
};


Rotater::Rotater(string filename) {

    try {

        // check if the image exists on disk
        if(!does_file_exist(filename)) {
            throw FilePathInvalidException(filename);
        }

        src = cv::imread(filename, 1);

        thr.create(src.rows,src.cols,CV_8UC1);
        dst.create(src.rows,src.cols,CV_8UC3);
        dst = cv::Scalar(255,255,255);

        preprocessInputImage();
        processContours();

        //take second largest contour
        pt_largest = contours[contours.size()-2];

        getRotatedLargestContour();

    }

    catch(FilePathInvalidException ipie) {
        std::cerr << "EXCEPTION: Input image " << ipie.getPath() << " does not exist on disk " << std::endl;
        throw ipie;
    }



}

// Preprocess methods - convert colour to greyscale, gauss blur, threshold (otsu)
void Rotater::preprocessInputImage() {
    cv::cvtColor(src, thr, CV_BGR2GRAY); //Convert to gray
    cv::GaussianBlur(thr, thr, cv::Size(21,21), 0, 0);
    cv::threshold(thr, thr, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

#if DEBUG
    cv::imshow("original", thr);
    cv::imwrite("10x afterblur.png", thr);
#endif
}

// This function finds and sorts the contours in cv::Matthr
void Rotater::processContours() {
    cv::findContours( thr, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    //sort the corners with a custom comparator
    std::sort(contours.begin(), contours.end(), compareContourAreas);
}

// This function rotates the largest contour; stores it in cropped.
void Rotater::getRotatedLargestContour() {

    try {
        cv::RotatedRect rr = cv::minAreaRect( cv::Mat(pt_largest));

        // get angle and size from the bounding box
        float angle = rr.angle;
        cv::Size rect_size = rr.size;

#if DEBUG
        cout << "width of largest contour" << rr.boundingRect().width << endl;
        cout << "width of largest contour" << rr.boundingRect().height << endl;
#endif // DEBUG

        if(rr.boundingRect().width < 2325 || rr.boundingRect().width > 2348) {
            throw LargestContourInvalidException();
        }

        if(rr.boundingRect().height < 3250 || rr.boundingRect().height > 3275) {
            throw LargestContourInvalidException();
        }


        //if it is tilted 90 deg
        if (rr.angle < -45.) {
            angle += 90.0;
            swap(rect_size.width, rect_size.height);
        }

        // get the rotation matrix
        rotationMat = getRotationMatrix2D(rr.center, angle, 1.0);

        // perform the affine transformation
        cv::warpAffine(src, rotated, rotationMat, src.size(), cv::INTER_CUBIC);

        // crop the resulting image
        getRectSubPix(rotated, rect_size, rr.center, cropped);

#if DEBUG
        cv::imshow("cropped_out", cropped);
        cv::imwrite("11x cropped_out.png", cropped);
#endif // DEBUG

    }

    catch(LargestContourInvalidException lcie) {
        std::cerr << "EXCEPTION: Largest Border of scanned image was not of expected 300dpi dimensions" << std::endl;
        throw lcie;
    }

}


/** \brief Function getRotatedMat()
*   This function gets the cropped and rotated main content of the scanned image.
*
*   \return cv::Mat - containing main content
*/
// Returns the cropped and rotated Mat
cv::Mat Rotater::getRotatedMat() {
    return cropped;
}

// Debugging function - extra
void Rotater::debugLargestContour() {
    cv::Scalar color( 0, 0, 255 );
    cv::drawContours( dst,contours,(contours.size()-2), color, CV_FILLED, 1, hierarchy );

    //get the bounding rectangle of the largest contour
    cv::Rect box = cv::boundingRect(pt_largest);

    //draw the bounding rectangle on the dst Mat
    cv::rectangle(dst, box, cv::Scalar(0,255,0), 1);
    cv::imshow("bounding rect", dst);

    // compute all moments
    cv::Moments mom= cv::moments(cv::Mat(pt_largest));
    // draw mass center
    cv::circle(dst,
               // position of mass center converted to integer
               cv::Point(mom.m10/mom.m00,mom.m01/mom.m00),
               20,cv::Scalar(0,255,0),-1); // draw green dot

    cout << "yolo" << endl;

    cv::imwrite("second_largest_contour.png", dst);
}


#endif // __ROTATER_H__
