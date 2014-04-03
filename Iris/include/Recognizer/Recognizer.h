#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "unistd.h"

// To check for file on disk
#include <sys/stat.h>

// Exception classes
#include "Exceptions/Generic/FilePathInvalidException.h"
#include "Exceptions/Recognizer/InvalidPredictionException.h"

using namespace std;


/** \brief Class Recognizer()
*
*   This class deals with prediction using kNN classification algorithm; which is part of the openCV machine learning library.
*   It predicts the input segmented Mat against the training data, which cluster it falls under.
*/
class Recognizer {

private:
    void deserialize_training();
    cv::Mat getCroppedContour(cv::Mat& segment);

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
    cv::KNearest knn;

    Recognizer() {

        deserialize_training();
    }
    ~Recognizer() {}

    int predict(cv::Mat &sample);
};

cv::Mat Recognizer::getCroppedContour(cv::Mat& segment) {
    vector<vector<cv::Point> > contours; // Vector for storing contour
    vector<cv::Vec4i> hierarchy;
    cv::Mat croppedContour;
    cv::Mat seg_copy = segment.clone(); //to prevent decomposition
    cv::GaussianBlur(segment, segment, cv::Size(5,5), 0, 0);

    cv::findContours( segment, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    //sort the corners with a custom comparator
    sort(contours.begin(), contours.end(), compareContourAreas);

    // largest contour
    vector<cv::Point> pt_largest;

    //take largest contour
    pt_largest = contours[contours.size()-1];

    cv::Rect r= cv::boundingRect(pt_largest);
    croppedContour = seg_copy(r);

    return croppedContour;
}


void Recognizer::deserialize_training() {

    try {
        cv::Mat _samples;
        cv::Mat _labels;
        // Read stored sample and label for training

        const string training_data_file_path = "RecognitionData/TrainingData.yml";
        const string label_data_file_path = "RecognitionData/LabelData.yml";

        // check if the file exists on disk
        if(!does_file_exist(training_data_file_path)) {
            throw FilePathInvalidException(training_data_file_path);
        }

        // check if the file exists on disk
        if(!does_file_exist(label_data_file_path)) {
            throw FilePathInvalidException(label_data_file_path);
        }

        cv::FileStorage Data(training_data_file_path, cv::FileStorage::READ); // Read training data to a Mat
        Data["data"] >> _samples;
        Data.release();

        cv::FileStorage Label(label_data_file_path, cv::FileStorage::READ); // Read label data to a Mat
        Label["label"] >> _labels;
        Label.release();

        knn.train(_samples, _labels); // Train with sample and responses
#if DEBUG
        cout << "finished reading training data " << endl;
#endif


    }

    catch(FilePathInvalidException ipie) {
        std::cerr << "EXCEPTION: Input File " << ipie.getPath() << " does not exist on disk " << std::endl;
        throw ipie;
    }

}


/** \brief Function predict()
*   This function gives the classification of the input Image - to which cluster it belongs
*
*   \return int - 0 (A) or 1 (P)
*/
int Recognizer::predict(cv::Mat &sample) {

    try {
        cvtColor(sample, sample, CV_BGR2GRAY);

        cv::Mat sample_flattened;

        //threshold(sample,sample,200,255,THRESH_BINARY_INV);
        cv::threshold(sample, sample, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

        sample = getCroppedContour(sample);

        // Resize sample to 32,32
        cv::resize(sample, sample, cv::Size(32,32), 0, 0, cv::INTER_CUBIC );

        // Convert to float - flatten the multi-dim to single dim (single channel)
        sample.convertTo(sample_flattened, CV_32FC1);

        float p = knn.find_nearest(sample_flattened.reshape(1,1), 1);

        if(p < 0 || p > 1) {
            throw InvalidPredictionException();
        }

        return static_cast<int>(p);
    }

    catch(InvalidPredictionException ipe) {
        std::cerr << "EXCEPTION: Prediction was not 0 or 1 " << std::endl;
        throw ipe;
    }

}


#endif // RECOGNIZER_H
