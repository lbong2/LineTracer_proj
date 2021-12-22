#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/photo.hpp>

#define WIDTH 640
#define HEIGHT 480

using namespace cv;
using namespace cv::ml;
using namespace std;

int main()
{
    // Classifier
    CascadeClassifier cascade1;
    CascadeClassifier cascade2;
    cascade1.load("xml/stop_signal.xml");
    cascade2.load("xml/kmSignal_fin.xml");

    vector<Rect> faces1;
    vector<Rect> faces2;

    // video from cam
    VideoCapture capture("/dev/video0");
    if (!capture.isOpened())
    {
        printf("Couldn¡¯t open the web camera¡¦\n");
        return 0;
    }
    Mat frame, img;

    // count & flags
    int count1 = 0;
    int count2 = 0;
    
    // work
    while (1)
    {
        capture >> frame;
        resize(frame, img, Size(WIDTH, HEIGHT));

        cascade1.detectMultiScale(img, faces1, 1.1, 4, 0 | CASCADE_SCALE_IMAGE, Size(60, 60));
        cascade2.detectMultiScale(img, faces2, 1.1, 4, 0 | CASCADE_SCALE_IMAGE, Size(110, 110));
        
        // stop signal
        if (faces1.size())
        {
            for (int y = 0; y < faces1.size(); y++)
            {
                Point lb(faces1[y].x + faces1[y].width, faces1[y].y + faces1[y].height);
                Point tr(faces1[y].x, faces1[y].y);
                rectangle(img, lb, tr, Scalar(0, 255, 0), 3, 8, 0);
            }
            count1++;
        }
        // speed signal
        if (faces2.size())
        {
            for (int y = 0; y < faces2.size(); y++)
            {
                Point lb(faces2[y].x + faces2[y].width, faces2[y].y + faces2[y].height);
                Point tr(faces2[y].x, faces2[y].y);
                rectangle(img, lb, tr, Scalar(255, 0, 0), 3, 8, 0);
            }
            count2++;
        }

        if (count1 >= 20)
        {
            count1 = 0;
            printf("Stop signal\n");
        }
        if (count2 >= 20)
        {
            count2 = 0;
            printf("Speed signal\n");
        }

        // visualizing
        imshow("Face", img);

        // ESC
        if (waitKey(30) >= 0)
            break;
    }

	capture.release();
	destroyAllWindows();
    return 0;
}
