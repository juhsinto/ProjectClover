#ifndef __QR_DECODER_H__
#define __QR_DECODER_H__

#include <iostream>

//ZBar header
#include "zbar.h"

// Exception classes
#include "Exceptions/QRException/NoQRDataException.h"
#include "Exceptions/QRException/NoQRException.h"


using namespace std;
using namespace zbar;


/** \brief Class QRDecoder()
*
*   This class is responsible for reading QR data within an image.
*/
class QRDecoder {
private:
    cv::Mat grey_input;
    ImageScanner scanner;
    Image* image;

public:
    QRDecoder(cv::Mat& input_qr);
    ~QRDecoder() { delete image; }
    //Gets the data from the QR
    string getQRData();

    // Show the scanned QR
    void showScannedQR();
};


/** \brief Function showScannedQR()
*   This function shows the QR scanned within a boundary box.
*
*/
void QRDecoder::showScannedQR()  {
    cv::imshow("ScannedQR", grey_input);
}

QRDecoder::QRDecoder(cv::Mat& input_qr)  {

        cv::cvtColor(input_qr,input_qr,CV_BGR2GRAY);
        cv::GaussianBlur(input_qr, input_qr, cv::Size(5,5), 0, 0);
        cv::threshold(input_qr,input_qr,200, 255, CV_THRESH_BINARY);

        image = new Image(input_qr.cols, input_qr.rows, "Y800", input_qr.data, input_qr.cols * input_qr.rows);
        //set initial configuration to scanner
        scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
        cv::cvtColor(input_qr,grey_input,CV_GRAY2RGB);
}


/** \brief Function getQRData()
*   This function retrieves the content within the QR code.
*
*   \return string - text within the QR code.
*/
string QRDecoder::getQRData() {


    try {
        // scan the image for bar-codes/qr-codes
        int num_Of_QR_Found = scanner.scan(*image);

        // check If no QR is found
        if(num_Of_QR_Found < 1) {
            throw NoQRException();
        }


        std::string qrText;

        // extract results
        for(Image::SymbolIterator symbol = image->symbol_begin();
                symbol != image->symbol_end();
                ++symbol) {

#if DEBUG
            vector<cv::Point> vp;
            cout  << "QR contains \"" << symbol->get_data() << '"' << " " << endl;
            int n = symbol->get_location_size();
            for(int i=0; i<n; i++) {
                vp.push_back(cv::Point(symbol->get_location_x(i),
                                       symbol->get_location_y(i)) );
            }
            cv::RotatedRect r = cv::minAreaRect(vp);
            cv::Point2f pts[4];
            r.points(pts);
            for(int i=0; i<4; i++) {
                line(grey_input, pts[i], pts[(i+1)%4], cv::Scalar(255,0,0), 3);
            }
            cout << "Angle of QR is : "<< r.angle << endl;

            cv::imwrite("12x qr_code_rect.png", grey_input);

#endif  // DEBUG

            qrText = symbol->get_data();

            // check If there is no data in the QR
            if(qrText == "") {
                throw NoQRDataException();
            }

        }
        return qrText;

    }

    catch(NoQRDataException ndqe) {
        std::cerr << "EXCEPTION: There was no Data within the QR code " << std::endl;
        throw ndqe;
    }

    catch(NoQRException nqe) {
        std::cerr << "EXCEPTION: No was QR found or QR is corrupted" <<  std::endl;
        throw nqe;
    }
}


#endif // __QR_DECODER_H__
