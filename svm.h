#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace cv::ml;
using namespace std;

Ptr<SVM> train_hog_svm(const HOGDescriptor& hog)
{
    Mat digits = imread("G_Th_F.jpg", IMREAD_GRAYSCALE);
    
		resize(digits, digits, Size(2000,240), 0, 0, INTER_AREA);

    if (digits.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }

    Mat train_hog, train_labels;

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 50; i++) {
            Mat roi = digits(Rect(i * 40, j * 60, 40, 60));

            vector<float> desc;
            hog.compute(roi, desc);

            Mat desc_mat(desc);
            train_hog.push_back(desc_mat.t());
            if (j <= 1)
                train_labels.push_back(0);
            else
                train_labels.push_back(1);
        }
    }

    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::Types::C_SVC);
    svm->setKernel(SVM::KernelTypes::LINEAR);
    svm->setC(2.5);
    svm->setGamma(0.50625);
    svm->train(train_hog, ROW_SAMPLE, train_labels);

    return svm;
}


