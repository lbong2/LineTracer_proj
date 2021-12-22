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

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <wiringPi.h>

#include "svm.h"

#define WIDTH 640
#define HEIGHT 480

using namespace cv;
using namespace cv::ml;
using namespace std;

int main()
{
    // -------------- video ------------------
    // Classifier
    CascadeClassifier cascade1;
    CascadeClassifier cascade2;
    cascade1.load("xml/stop_signal.xml");
    cascade2.load("xml/kmSignal_fin.xml");

    vector<Rect> st;
    vector<Rect> km;

    // video from cam
    VideoCapture capture("/dev/video0");
    if (!capture.isOpened())
    {
        printf("Couldn¡¯t open the web camera¡¦\n");
        return 0;
    }
    Mat frame, img;
    
    // count
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    
    // number
    Mat digit = Mat::zeros(40, 60, CV_8UC3);
    
    HOGDescriptor hog(Size(20, 20), Size(10, 10), Size(5, 5), Size(5, 5), 9);
    
    Ptr<SVM> svm = train_hog_svm(hog);

    if (svm.empty()) {
        cerr << "Training failed!" << endl;
        return -1;
    }
    int res = 0;
    
    // wave
    float dist, s, e;
    
    // ------------- motor control -------------
    char buff_motor[4] = {0,};
	
    int dev_motor = open("/dev/my_motor", O_RDWR);
    if(dev_motor == -1){
	    printf("motor Opening was not possible!\n");
	    return -1;
    }
    printf("motor Opening was successful!\n");
	
    // flags & pwm
    // default == forward
    char flag_m1A = '1';
    char flag_m1B = '0';
    char flag_m2A = '1';
    char flag_m2B = '0';
    unsigned int pwm1 = 0, pwm2 = 1000;
    
    // ------------------ wave ------------------
    char buff_wave[2] = {0,};
	
    int dev_wave = open("/dev/my_wave", O_RDWR);
    if(dev_wave == -1){
	    printf("wave Opening was not possible!\n");
	    return -1;
    }
    printf("wave Opening was successful!\n");
    
    int count_wave = 0;
    // ----------------- buzzer -----------------
    char buff_buzzer = '0';
	
    int dev_buzzer = open("/dev/my_buzzer", O_RDWR);
    if(dev_buzzer == -1){
	    printf("buzzer Opening was not possible!\n");
	    return -1;
    }
    printf("buzzer Opening was successful!\n");
    
    char flag_buzzer = '0';	
    char flag_buzzer_wave = '0';	
    
    // ----------------- sensor -----------------
    char buff_sensor[2] = {0, 0};
    
    int dev_sensor = open("/dev/my_sensor", O_RDWR);
    if(dev_sensor == -1){
	    printf("sensor Opening was not possible!\n");
	    return -1;
    }
    printf("sensor Opening was successful!\n");
    
    char flag_sensor1 = '0';
    char flag_sensor2 = '0';
    
    // ------------------ work ------------------
    	
	//----------------------Default--------------------
            
	
	
	
    while (1)
    {
        capture >> frame;
        resize(frame, img, Size(WIDTH, HEIGHT));
	
        Rect bounds(0, 0, img.cols, img.rows);

        cascade1.detectMultiScale(img, st, 1.1, 4, 0 | CASCADE_SCALE_IMAGE, Size(60, 60));
        cascade2.detectMultiScale(img, km, 1.1, 4, 0 | CASCADE_SCALE_IMAGE, Size(110, 110));
        
        // stop signal
        if (st.size())
        {
            for (int y = 0; y < st.size(); y++)
            {
                Point lb(st[y].x + st[y].width, st[y].y + st[y].height);
                Point tr(st[y].x, st[y].y);
                rectangle(img, lb, tr, Scalar(0, 255, 0), 3, 8, 0);
            }
            count1++;
        }
        // speed signal
        else if (km.size())
        {
            for (int y = 0; y < km.size(); y++)
            {
                Point lb(km[y].x + km[y].width, km[y].y + km[y].height);
                Point tr(km[y].x, km[y].y);
                rectangle(img, lb, tr, Scalar(255, 0, 0), 3, 8, 0);
            }
            count2++;
        }
	// default
	else
	{
	    count1 = 0;
	    count2 = 0;
	    count3++;
	}

        // visualizing
        imshow("Face", img);
        imshow("digit", digit);

        // ESC
        if (waitKey(30) >= 0)
            break;
	    	    
	// sensor control
	read(dev_sensor, buff_sensor, 2);
	flag_sensor1 = buff_sensor[0];
	flag_sensor2 = buff_sensor[1];
	//printf("%c %c\n", buff_sensor[0], buff_sensor[1]); 
	
	if (flag_sensor1 == '0' && flag_sensor2 == '1')
	{
	    flag_m1A = '1';
	    flag_m1B = '0';
	    flag_m2A = '0';
	    flag_m2B = '0';    
	}
	if (flag_sensor1 == '1' && flag_sensor2 == '0')
	{
	    flag_m1A = '0';
	    flag_m1B = '0';
	    flag_m2A = '1';
	    flag_m2B = '0';    
	}
	if (flag_sensor1 == '0' && flag_sensor2 == '0')
	{
	    flag_m1A = '0';
	    flag_m1B = '1';
	    flag_m2A = '0';
	    flag_m2B = '1';  
	}
	if (flag_sensor1 == '1' && flag_sensor2 == '1')
	{
	    flag_m1A = '1';
	    flag_m1B = '0';
	    flag_m2A = '1';
	    flag_m2B = '0';   
	}
	    
	// case
        if (count1 >= 10)
        {
            printf("Stop signal\n");
            count1 = 0;
	    
	    // motor	    
	    flag_m1A = '0';
	    flag_m1B = '0';
	    flag_m2A = '0';
	    flag_m2B = '0';  
	     
	    
	    // buzzer
	    flag_buzzer = '1';
        }
        else if (count2 >= 10)
        {
            printf("Speed signal\n");
            count2 = 0;
	    
	    for (int y = 0; y < km.size(); y++)
	    {
		Point lb(km[y].x + km[y].width / 4, km[y].y + km[y].height / 4);
		Point tr(km[y].x + km[y].width / 1.7, km[y].y + km[y].height * 3 / 4);
		//rectangle(img, lb, tr, Scalar(255, 0, 0), 3, 8, 0);
            
		//printf("dddd\n");
	    
		Rect sq(km[y].x + km[y].width / 3, km[y].y + km[y].height / 4, km[y].width / 5, km[y].height / 2);
		
		digit = img(bounds & sq).clone();

		inRange(digit,  Scalar(0, 0, 0), Scalar(150, 150, 150), digit);
		
		
		resize(digit, digit, Size(40,60), 0, 0, INTER_AREA);
		vector<float> desc;
		hog.compute(digit, desc);

		Mat desc_mat(desc);
		res = cvRound(svm->predict(desc_mat.t()));
		
		//cout << res << endl;
	    }
	    
	    // motor
	    if (res == 0){
		pwm1 = 39990;
		pwm2 = 10;
	    }
	    else if (res == 1){		
		pwm1 = 0;
		pwm2 = 40000;
	    }
	    else
	    {
		pwm1 = 0;
		pwm2 = 40000;
	    }		
	    
	    // buzzer
	    flag_buzzer = '0';
        }
	else if(count3 >= 10)
	{
	    printf("Default\n");
            //printf("%c	%c\n", flag_sensor1, flag_sensor2);
	    
	    count3 = 0;
	    // motor	    
	    flag_m1A = '1';
	    flag_m1B = '0';
	    flag_m2A = '1';
	    flag_m2B = '0';  
	    	    
	    // buzzer
	    flag_buzzer = '0';
	}

	    
	// wave control
	buff_wave[0] = '0';
	write(dev_wave, &buff_wave[0], 1);
	buff_wave[0] = '1';
	write(dev_wave, &buff_wave[0], 1);
	delayMicroseconds(10);
	buff_wave[0] = '0';
	write(dev_wave, &buff_wave[0], 1);
	
	read(dev_wave, &buff_wave[1], 1);
	while(buff_wave[1] == '0')
	{	
	    read(dev_wave, &buff_wave[1], 1);
	}
	    s = micros();
	    
	while(buff_wave[1] == '1')	
	{	
	    read(dev_wave, &buff_wave[1], 1);
	}
	    e = micros();
	    
	dist = (e-s)/58;
	
	if(dist <= 30)
	{	
	    //printf("dist = %f\n", dist); 
	    flag_buzzer_wave = '1';	    
	}
	else
	{
	    flag_buzzer_wave = '0';
	}
	    
	// buzzer control
	if (flag_buzzer == '1' || flag_buzzer_wave == '1')
	{
	    buff_buzzer = '1';	    
	    write(dev_buzzer, &buff_buzzer, 1);
	    flag_m1A = '0';
	    flag_m1B = '0';
	    flag_m2A = '0';
	    flag_m2B = '0';  
	}
	else
	{
	    buff_buzzer = '0';
	    write(dev_buzzer, &buff_buzzer, 1);
	}
	
	// motor control
	buff_motor[0] = '0';
	buff_motor[1] = '0';
	buff_motor[2] = '0';
	buff_motor[3] = '0';
	write(dev_motor, buff_motor, 4);
	delayMicroseconds(pwm1);
	//printf("%c %c %c %c\n", flag_m1A, flag_m1B, flag_m2A, flag_m2B);
	buff_motor[0] = flag_m1A;
	buff_motor[1] = flag_m1B;
	buff_motor[2] = flag_m2A;
	buff_motor[3] = flag_m2B;
	write(dev_motor, buff_motor, 4);
	delayMicroseconds(pwm2);	    
    }
	
    // --------------- close program ------------------
    // motor
    buff_motor[0] = '0';
    buff_motor[1] = '0';
    buff_motor[2] = '0';
    buff_motor[3] = '0';
    write(dev_motor, buff_motor, 4);
    close(dev_motor);
    
    // buzzer
    buff_buzzer = '0';
    write(dev_buzzer, &buff_buzzer, 1);
    close(dev_buzzer);
    
    // cam
    capture.release();
    destroyAllWindows();
	
    return 0;
}
