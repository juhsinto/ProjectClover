#include <iostream>

// Custom headers for Grid Sub-Modules
#include "Grid/GridMaker/GridMaker.h"
#include "Grid/GridFilter/GridFilter.h"
#include "Grid/GridProcessor/GridProcessor.h"
#include "QR/QRDecoder.h"
#include "Rotate/Rotater.h"
#include "Main/MainProcessor.h"


//#include <fstream>


using namespace std;

int main() {


    try {

        // This is the path of the template; you could possibly add more templates
        // But then you would need to change the Exception classes as well.
        const string default_blank_template = "Templates/template-clean-new.png";

        GridMaker gm(default_blank_template);
        cv::Mat GridPointsBin = gm.getGrid();

        // After getting grid intersection points
        GridFilter gf(GridPointsBin, 820, 670, 2130, 3155);
        set<int> set_x = gf.getSetX();
        set<int> set_y = gf.getSetY();

        // dealing with the center of contours
        GridProcessor gp(set_x, set_y);
        vector<vector<cv::Point2i> > TwoDimGridPoints = gp.getProcessedGridPoints();
/*
//<< UNIT TEST OF QR >>
        // This is where the actual images are read
        string filename = "bbc.png";
        cv::Mat input_qr = cv::imread(filename, 1);
//<< UNIT TEST OF QR >>


//<< UNIT TEST OF RECOG >>
        cv::Mat imageROI  = cv::imread("InputImages/p.png", 1);
        int prediction = recog.predict(imageROI);

        cout << "prediction is " << prediction << endl;
//<< UNIT TEST OF RECOG >>

*/

        // This is the path of the scanned image - ideally you want to loop from this point on - for all the scanned images
        const string filename = "original-rot.jpg";
        cv::Mat input_image_filled;
        cv::Mat input_image;

        //Crop and Rotate the image
        Rotater rot(filename);
        input_image_filled = rot.getRotatedMat(); imwrite("roatatedthing.png", input_image_filled);
/*        input_image_filled.copyTo(input_image);

        QRDecoder qr(input_image);
        string qr_header = qr.getQRData();
        cout << "header of page is " << qr_header << endl;

        //Initialize the recognizer (KNN)
        Recognizer recog;

        //Change current working directory - for output images.
        #if DEBUG
            char stackBuffer[512];
            cout << "current dir "  << getcwd(stackBuffer, sizeof(stackBuffer)) << endl;
            cout << "success or fail getting into /Output " << chdir("Output") << endl;
            cout << "current dir "  << getcwd(stackBuffer, sizeof(stackBuffer)) << endl;
        #else
            chdir("Output");
        #endif

        //make a text file for storing the prediction of segmented handwritten character
        // here i am storing the output txt file name as the inputfilename.png + ".txt"
        string predictedOutFile = filename + ".txt";
        ofstream predictedOutputMatrix( predictedOutFile.c_str() );
        predictedOutputMatrix << qr_header << "\n";

        //where the main grid iteration and recognition takes place
        MainProcessor mp(predictedOutputMatrix, input_image_filled, recog, TwoDimGridPoints);

        predictedOutputMatrix.close();

        //go up a directory level
        chdir("..");

        //showImage("final overlay", input_image_filled);
        cv::imwrite("Output_overlayed.png", input_image_filled);

        cout << "----------------------------------------------------- " << endl
             << "DONE" << endl;
   */ }

    // all exceptions should come here

    catch(...) {
        cout << "exception occurred" << endl;
    }

    cv::waitKey(0);
    return 0;
}
