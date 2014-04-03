#include <iostream>
#include "Modules/KNN_Trainer.h"
#include "Modules/KNN_Tester.h"

int main() {

    try {
        KNN_Trainer train_knn_obj("RAW_DATA/TRAIN/", 944, 2);
        KNN_Tester test_knn_obj("RAW_DATA/TEST/", 944, 2);

        cv::waitKey(0);
        return 0;
    }

    catch(...) {
        cout << "Exception occurred " << endl;
    }

}
