#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxVideoRecorder.h"


#include "opencv2/video/background_segm.hpp"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif



        ofxCvContourFinder 	contourFinder;

		int 				threshold;
		bool				bLearnBakground;

		char effect = 'p';
		char scale ='1';
		float scaleFloat = 1;

        void openMovie (string result);
        int w, h, sw, sh;
        string fileName;
        string fileExt;
        ofImage ofIm, ofImTmp;

        ofxVideoRecorder    vidRecorder;
        void exportMovie ();

        ofVideoPlayer 		mMovie;



        cv::Mat myResize(cv::Mat src, float s);
        cv::Mat bgFrame;
        cv::Mat oldBinary;

        ofPixelsRef bgExtract(cv::Mat color_img);
        void blobTracker(cv::Mat color_img);
        void blobFinderCV(cv::Mat color_img);
        cv::BackgroundSubtractorMOG2 bgSubtractor;
        vector<ofxCvBlob>  lastBlobs;


};

