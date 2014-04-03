#ifndef __MAIN_PROCESSOR_H__
#define __MAIN_PROCESSOR_H__

#include "Recognizer/Recognizer.h"
#include <fstream>

using namespace std;

class MainProcessor {
private:
    static string convertIntToString(int number) {
        stringstream ss;			//create a stringstream
        ss << number;				//add number to the stream
        ss << number;				//add number to the stream
        return ss.str();			//return a string with the contents of the stream
    }
public:
    MainProcessor(ofstream& predictedOutputMatrix, cv::Mat& input_image_filled, Recognizer& recog,  vector<vector<cv::Point2i> >& TwoDimGridPoints);
};

MainProcessor::MainProcessor(ofstream& predictedOutputMatrix, cv::Mat& input_image_filled, Recognizer& recog, vector<vector<cv::Point2i> >& TwoDimGridPoints) {

    vector<vector<cv::Point2i> >::iterator grid_iter;
    grid_iter = TwoDimGridPoints.begin();
    vector<cv::Point2i>::iterator row_iter;

    cv::Mat imageROI;
    int row_count=0, col_count=0;
    int prediction;

    while(grid_iter != TwoDimGridPoints.end()) {
        vector<cv::Point2i> row_vec;
        row_vec = *grid_iter;
        row_iter = row_vec.begin();

        //re-init col_count
        col_count = 0;

        while(row_iter != row_vec.end() ) {

            cv::Point2i point1;
            point1 = *row_iter;

            //next diagonal point
            row_iter++;

            cv::Point2i point2;
            point2 = *row_iter;

            // Edge cutting of ROI (preprocessing)
            //!IMPORTANT hardcoded value (cropping)
            point1.x += 3;
            point1.y += 3;

            point2.x -= 3;
            point2.y -= 3;
            //!IMPORTANT hardcoded value (cropping)

            imageROI = input_image_filled(cv::Rect(point1, point2));
#if DEBUG
            string file_name = "img" + convertIntToString(row_count) +"-" + convertIntToString(col_count) + ".png";
            cv::imwrite(file_name, imageROI);
#endif

            prediction = recog.predict(imageROI);
            //std::cout << "The value of " << row_count << "-" << col_count << " is " <<  prediction << std::endl;
            predictedOutputMatrix << prediction << ",";

            cv::rectangle( input_image_filled,  point1  ,	point2,
                           cv::Scalar( 0, 255, 0),
                           2,
                           8 );

            col_count++;
            row_iter++;
        }
        predictedOutputMatrix << "\n";

        row_count++;
        grid_iter++;
    }

}

#endif // __MAIN_PROCESSOR_H__
