/**********************************************************************
*
*	<--- DIRECTORY STRUCTURE EXPECTED FOR TRAINER --->
*
* --/
*	 --/RAW_DATA/
*				--/TRAIN
*						 --/0 					//ABSENT - SAMPLE DATA
*						 --/1 					//PRESENT - SAMPLE DATA
*						 --/2 					//Blank? - SAMPLE DATA
*
*	<--- DIRECTORY STRUCTURE EXPECTED FOR TRAINER --->
***********************************************************************/

#ifndef KNN_TRAINER_H
#define KNN_TRAINER_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "unistd.h"

// To check for file on disk
#include <sys/stat.h>

// Exception classes
#include "Exceptions/Generic/FilePathInvalidException.h"

using namespace std;
using namespace cv;

//------------------------------------------------------------------------------------------
// Custom comparator
bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) {

    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i < j );
}
//------------------------------------------------------------------------------------------

class KNN_Trainer {

private:

    Mat _samples;			// For storing samples
    Mat _labels;			// For storing labels
    Mat sample_labels;		// For storing sample's label

    int TOTAL_SAMPLES;
    int TOTAL_CLASSES;

    // Train the KNN classifier
    void train_classifier( string  PATH_TO_TRAINING_DATA);

    // Serialize (save) the training to file.
    void serialize_training();

    // Get cropped Contour (only the contour, not extra whitespace)
    Mat getCroppedContour(Mat& segment);

    // Checks if the file exists or not.
    inline bool does_file_exist (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }
public:

    KNN_Trainer(string  PATH_TO_TRAINING_DATA, int total_samples, int total_classes) : TOTAL_SAMPLES(total_samples),  TOTAL_CLASSES(total_classes) {

        train_classifier(PATH_TO_TRAINING_DATA);

        serialize_training();
    }

    // Convert (input) integer to String (using stringstream)
    string convertIntToString(int number) {
        stringstream ss;			//create a stringstream
        ss << number;				//add number to the stream
        return ss.str();			//return a string with the contents of the stream
    }

};

/*******************************************************************/

Mat KNN_Trainer::getCroppedContour(Mat& segment) {
    vector< vector <Point> > contours; // Vector for storing contour
    vector< Vec4i > hierarchy;
    Mat croppedContour;
    Mat seg_copy = segment.clone(); //to prevent decomposition
    GaussianBlur(segment, segment, Size(5,5), 0, 0);

    findContours( segment, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    //sort the contours with a custom comparator
    sort(contours.begin(), contours.end(), compareContourAreas);

    // largest contour
    vector<Point> pt_largest;

    //take largest contour
    pt_largest = contours[contours.size()-1];

    Rect r= boundingRect(pt_largest);
    croppedContour = seg_copy(r);

    return croppedContour;
}

/********************************************************************
*							TRAIN_CLASSIFIER
*
*	Input Arguments:
*		string_to_training_data  : [string] String to training data
*		TOTAL_SAMPLES_in_dir : [int] Number of files in directory
*
*
* <-- FILE NAMING EXPECTED IN INPUT FILES -->
*
* <-- Also, there should be equal number of files, in each class -->
*
*	--/TRAIN
*			--/0
*			  	--/IMG (0).png
*			  	--/IMG (1).png
*			  	...
*			  	--/IMG (n).png
*
*
* <-- FILE NAMING EXPECTED IN INPUT FILES -->
*
*********************************************************************/

// Train the KNN classifier
void KNN_Trainer::train_classifier( string string_to_training_data) {

    try {
        // Mat for storing a single sample
        Mat sample;

        // Mat for storing CV_32FC1
        Mat sample_flattened;

        // Change directory to training data directory
        chdir( string_to_training_data.c_str() );

        char stackBuffer[512];

        for(int CURR_CLASS=0; CURR_CLASS<TOTAL_CLASSES; CURR_CLASS++) {

            for(int CURR_SAMPLE=0; CURR_SAMPLE<TOTAL_SAMPLES; CURR_SAMPLE++) {

                chdir( (convertIntToString(CURR_CLASS)).c_str());

                string filename = "IMG ("+ convertIntToString(CURR_SAMPLE+1) + ").png";
                // check if the file exists on disk
                if(!does_file_exist(filename)) {
                    throw FilePathInvalidException(filename);
                }

#if DEBUG
                cout << "current dir asdfasd"  << getcwd(stackBuffer, sizeof(stackBuffer)) << endl;
                cout << filename << endl;
#endif // DEBUG

                // Read each sample
                sample = imread(filename, 0);
                //threshold(sample,sample,200,255,THRESH_BINARY_INV); //Threshold to find contour
                cv::threshold(sample, sample, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

                sample = getCroppedContour(sample);
                //imwrite("a.png"+filename, sample);

                // Resize sample to 32,32
                resize(sample, sample, Size(32,32), 0, 0, INTER_CUBIC );

                // Convert to float - flatten the multidim to single dim (single chan)
                sample.convertTo(sample_flattened, CV_32FC1);

                // Store sample (continuous) data
                _samples.push_back(sample_flattened.reshape(1,1));

                // Store label to a mat
                sample_labels.push_back(CURR_CLASS);

            }

            // Go up 1 file directory
            chdir("../");
        }

        // Go up 2 file directories
        chdir("../..");

        // Store the data to file
        Mat sample_labels_temp;

        //make continuous
        sample_labels_temp = sample_labels.reshape(1,1);

        // Convert  to float and store in _labels
        sample_labels_temp.convertTo(_labels,CV_32FC1);
    }

    catch(FilePathInvalidException ipie) {
        std::cerr << "EXCEPTION: Input file " << ipie.getPath() << " does not exist on disk " << std::endl;
        throw ipie;
    }

    catch(...) {
        cout << "Something unusual went wrong while training " << endl;
    }

}

/********************************************************************
*						SERIALIZE_TRAINING
*
********************************************************************/

// This function serializes (writes to file) the training data.
void KNN_Trainer::serialize_training() {

    // Store the sample data in a file
    FileStorage Data("TrainedDataFiles/TrainingData.yml",FileStorage::WRITE);
    Data << "data" << _samples;
    Data.release();

    // Store the label data in a file
    FileStorage Label("TrainedDataFiles/LabelData.yml",FileStorage::WRITE);
    Label << "label" << _labels;
    Label.release();

    cout << "Training and Label data serialized successfully.... " << endl;

}
#endif // KNN_TRAINER_H
