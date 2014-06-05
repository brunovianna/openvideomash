#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetEscapeQuitsApp(true);
    ofSetBackgroundAuto(false);
    ofBackground(0);
//	#ifdef _USE_LIVE_VIDEO
//        vidGrabber.setVerbose(true);
//        vidGrabber.initGrabber(320,240);
//	#else
//        vidPlayer.loadMovie("MVI_2041.mov");
//        vidPlayer.play();
//	#endif
//
//    colorImg.allocate(1280,720);
//	grayImage.allocate(1280,720);
//	grayBg.allocate(1280,720);
//	grayDiff.allocate(1280,720);

	bLearnBakground = true;
	threshold = 80;

	//bgSubtractor = new cv::BackgroundSubtractorMOG2();


	ofFile lastVideoName;

	if (lastVideoName.doesFileExist(ofToDataPath("lastVideoName.txt"))) {
        lastVideoName.open(ofToDataPath("lastVideoName.txt"));
        lastVideoName >> fileName;
        lastVideoName.close();
        openMovie (fileName);
	}






    cout << "f - optical flow good features \n ";
    cout << "s - optical flow simple blob \n";
    cout << "b - blob tracker \n";
    cout << "c - blob finder cv \n";
    cout << "g - background extractor \n";

}

//--------------------------------------------------------------
void testApp::update(){

    if (mMovie.isLoaded())
        mMovie.update();

}

//--------------------------------------------------------------
void testApp::draw(){

    if (!mMovie.isLoaded()) {
            return;
        }


    if (!mMovie.isFrameNew()) {
        //ofIm.draw(0,0);
    } else {


        cv::Mat frame(h,w,CV_8UC3,mMovie.getPixels());

        cv::Mat resized;
        resized = myResize (frame, scaleFloat);
        ofIm.setFromPixels(resized.data,(int)w*scaleFloat,(int)h*scaleFloat, OF_IMAGE_COLOR);

        switch (effect) {
            case 'f':
                //ofIm.draw(0,0);
                {

                    goodOpticalFlow(resized);
                }
                break;
            case 's':
                //ofIm.draw(0,0);
                {

                    simpleOpticalFlow(resized);
                }
                break;
            case 'd':
                //ofIm.draw(0,0);
                {

                    denseOpticalFlow(resized);
                }
                break;
            case 't':
                //ofIm = ofImage(discreteFourierTransform(resized));
                break;
            case 'l':
                //ofIm = ofImage(houghLinesS(resized));
                break;
            case '.':
                //ofIm = ofImage(houghLinesP(resized));
                break;
            case 'b':
                blobTracker(resized);
                break;
            case 'c':
                blobFinderCV(resized);
                break;
            case 'g':

                ofIm = ofImage(bgExtract(resized));
                ofIm.draw(0,0);
                break;

            default:
                ofIm.draw(0,0);


            }


    }


}


void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){





    switch (key) {
        case '0':
			bLearnBakground = true;
			effect = '0';
			break;
		case '=':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;


        case 'o':
        case 'O':
            {

            if (mMovie.isLoaded()) {

                    mMovie.stop();
                    mMovie.close();


            }

            //ofFileDialogResult ofSystemLoadDialog(string windowTitle="", bool bFolderSelection=false, string defaultPath="")
            ofFileDialogResult openFileResult = ofSystemLoadDialog("select a movie");

            //Check if the user opened a file
            if (openFileResult.bSuccess) {

                //We have a file, check it and process it
                openMovie(openFileResult.getPath());
                fileName = openFileResult.getPath();

                }
            }
            break;

        case 'e':
        case 'E':

            //TODO
            //cleanUpLastEffect();
            //setupEffect(effect);


            exportMovie();

            break;


        case ' ':
            {
                if (mMovie.isLoaded()) {

                    if (mMovie.getIsMovieDone()) {
                        mMovie.firstFrame();
                    } else {

                        if (mMovie.isPaused())
                            mMovie.setPaused(false);
                        else
                            mMovie.setPaused(true);
                    }
                } else {
                    ofSystemAlertDialog
                    ("please load a movie first");
                }
            }
            break;



        case 's':
        case 'S':
            {
                bgSubtractor = cv::BackgroundSubtractorMOG2 (60, 26.0, true);

                lastPointsOpticalFlow = std::vector<cv::Point2f>(MAX_FEATURES);

                cv::SimpleBlobDetector::Params params;
                params.minThreshold = 50.0f; //ants 50
                params.maxThreshold = 100.0f; //ants 100
                //params.minDistBetweenBlobs = 5.0f; //ants 0
                params.filterByInertia = true;
                params.maxInertiaRatio = 10.0f; //ants 10
                //params.minInertiaRatio = 1.0f; //ants 0
                params.filterByConvexity = false;
                params.filterByColor = true;
                params.blobColor = 255.0f; //ants 255
                params.filterByCircularity = false;
                params.filterByArea = true;
                params.minArea = 20.0f;  //ants 20
                params.maxArea = 150.0f; //ants 150
                simpleBlobDetector  = cv::SimpleBlobDetector(params);

                effect = 's';
                ofClear(0);
            }
            break;

        case 'f':
        case 'F':
            {

                bgSubtractor = cv::BackgroundSubtractorMOG2 (60, 26.0, true);
                lastPointsOpticalFlow = std::vector<cv::Point2f>(MAX_FEATURES);
                goodFeatureDetector = cv::GoodFeaturesToTrackDetector (70);
                effect = 'f';
                ofClear(0);
            }
            break;

        case 'd':
        case 'D':
            {

                bgSubtractor = cv::BackgroundSubtractorMOG2 (60, 26.0, true);
                lastPointsOpticalFlow = std::vector<cv::Point2f>(MAX_FEATURES);
                denseFeatureDetector = cv::DenseFeatureDetector ();
                effect = 'd';
                ofClear(0);
            }
            break;


        case 'b':
        case 'B':

            {
                //cv::Mat frame(h,w,CV_8UC3,mMovie.getPixels());

                //bgFrame = myResize (frame, scaleFloat);

                bgFrame = cv::Mat(h,w,CV_8UC3,mMovie.getPixels());
                bgSubtractor = cv::BackgroundSubtractorMOG2 (60, 26.0, true);
                ofClear(0);
                effect = 'b';
            }
            break;
        case 'c':
            {
                ofClear(0);
                effect = 'c';

            }
            break;
        case 't':
        case 'T':
            effect = 't';
            break;

        case 'l':
        case 'L':
            effect = 'l';
            break;

        case '.':
            effect = '.';
            break;

        case 'g': {
            bgFrame = cv::Mat(h,w,CV_8UC3,mMovie.getPixels());
            bgSubtractor = cv::BackgroundSubtractorMOG2 (60, 26.0, true);

            //ofBackground(0,50);
            //ofEnableBlendMode (OF_BLENDMODE_MULTIPLY);
            //cv::Mat bgFrame(sh,sw,CV_8UC1);
            //cv::cvtColor(myResize(colorFrame, scaleFloat), bgFrame, CV_RGB2GRAY);


            effect = 'g';
            ofBackground(0);
            //ofClear(0);
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);

            break;
        }
        case '1':
            scaleFloat = 1.0;
            sw = w;
            sh = h;
            break;

        case '2':
            scaleFloat = 0.5;
            sw = (int)w * 0.5;
            sh = (int)h * 0.5;
            break;

        case '3':
            scaleFloat = 0.33;
            sw = (int)w * 0.33;
            sh = (int)h * 0.33;
            break;

        case '4':
            scaleFloat = 0.25;
            sw = (int)w * 0.25;
            sh = (int)h * 0.25;
            break;


        case '7':
            scaleFloat =  0.7f;
            sw = (int)w * 0.7f;
            sh = (int)h * 0.7f;
            break;


    }


}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}


void testApp::exit() {

    // create or open a file
    ofFile pFile (ofToDataPath("lastVideoName.txt"),ofFile::WriteOnly);

    //pFile.remove();
    //pFile.create();

    // add some lines of text
    pFile << fileName;

    // save file
    pFile.close();


}


void testApp::openMovie (string result) {

//    if (mMovie.isLoaded())
//        mMovie.closeMovie();


    if (mMovie.loadMovie(result)) {
        w = mMovie.getWidth();
        h = mMovie.getHeight();
        sw = w;
        sh = h;

        ofIm.allocate(h,w, OF_IMAGE_COLOR);
        ofImTmp.allocate(h,w, OF_IMAGE_COLOR);



        mMovie.play();

        //mMovie.pause();
	} else {
        ofSystemAlertDialog("the movie couldn't be loaded");
	}

}


void testApp::exportMovie() {

        float rate;

        if (!mMovie.isLoaded()) {
            ofSystemAlertDialog("please load a movie first");
            return;
        }

        mMovie.setPaused(true);
        mMovie.firstFrame();

        if ((mMovie.getTotalNumFrames()!=0)&&(mMovie.getDuration()!=0)) {
            rate =  mMovie.getTotalNumFrames() / mMovie.getDuration() ;

        } else {

            rate =  29.97 ;

        }


        //cout << "rate: " << rate;

        string saveFileName = "/dados/_SESC/timelapses/testMovie";
        fileExt = ".avi"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats

        // override the default codecs if you like
        // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
        //vidRecorder.setVideoCodec("copy");
        //vidRecorder.setVideoCodec("copy");
        vidRecorder.setVideoCodec("msmpeg4v2");
        vidRecorder.setPixelFormat("rgb24");
        //vidRecorder.setVideoCodec("mjpeg");
        vidRecorder.setVideoBitrate("50000k");
        vidRecorder.setAudioCodec("libmp3lame");

        vidRecorder.setAudioBitrate("192k");

        vidRecorder.setup(saveFileName+ofGetTimestampString()+fileExt, round(w*scaleFloat), round(h*scaleFloat), rate,44100,0);

        mMovie.nextFrame();
        mMovie.update();
        while (mMovie.getCurrentFrame()< mMovie.getTotalNumFrames()) {
            if (mMovie.isFrameNew()) {

                cv::Mat frame(h,w,CV_8UC3,mMovie.getPixels());
                cv::Mat resized;
                resized = myResize (frame, scaleFloat);

                draw();

                switch (effect) {
                    case 'p':
                        //ofIm = ofImage(opticalFlow(resized));
                        ofIm.draw(0,0);
                        break;
                    case 'b':
                        {
                        blobTracker(resized);
                        ofImage screen;
                        screen.grabScreen(0,0,1920,1080);
                        vidRecorder.addFrame(screen);
                        break;

                        }
                    case 'c':
                        {
                            blobFinderCV(resized);
                            //ofIm.draw(0,0);
                        }
                        break;
                    case 'l':
                        //ofIm = ofImage(houghLinesS(resized));
                        break;
                    case '.':
                        //ofIm = ofImage(houghLinesP(resized));
                        break;
                    case 't':
                        //ofIm = ofImage(discreteFourierTransform(resized));
                        break;
                    case 'g':
                        ofIm = ofImage(bgExtract(resized));
                        vidRecorder.addFrame(ofIm.getPixelsRef());
                        ofIm.draw(0,0);
                        break;
                    }


                //redraw();

                mMovie.nextFrame();
                mMovie.update();
                cout << mMovie.getCurrentFrame() << "\n";
            } else {
                mMovie.update();
                //ofSleepMillis(1);
            }
        }


        vidRecorder.close();
        ofSystemAlertDialog("done");


}

cv::Mat testApp::myResize (cv::Mat src, float s) {

            cv::Mat dst;

            if (s!=1.0) {
                    cv::resize (src, dst, cv::Size(), s, s );
                    return dst;

                } else {

                    return src.clone();

                }



}



ofPixelsRef testApp::bgExtract (cv::Mat color_img) {

    int th = color_img.rows;
    int tw = color_img.cols;

    cv::Mat imgReturn, bgMask;
    cv::Mat grey_img, blurred;

    cv::cvtColor(color_img, grey_img, CV_RGB2GRAY);


    bgSubtractor(grey_img,bgMask);

    //adaptiveThreshold(img, imgBinary, 255, cv::ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,3,5.0 );
    //cv::absdiff (img,resizeBgFrame, result);
    //cv::threshold(result, imgBinary, 15,100,cv::THRESH_BINARY);

    //if (firstFrame) {
    //    firstFrame = false;
    //    oldBinary = imgBinary.clone();
    //}
    //cv::subtract(imgBinary,grey,imgBinary);


    //oldBinary *=0.8;

    //imgBinary +=oldBinary;

    blurred = bgMask.clone();
    for ( int i = 1; i < 5; i = i + 2 )
         cv::GaussianBlur( bgMask, blurred, cv::Size( i, i ), 0,0);
         //cv::medianBlur( bgMask, blurred, i);
         //cv::bilateralFilter( bgMask, blurred, i, i*2, i/2);

    cv::cvtColor(blurred, imgReturn, CV_GRAY2RGB);
    //cv::add (color_img,imgReturn,imgReturn,imgBinary);

    //oldBinary = imgBinary.clone();


    //ofImTmp.setFromPixels(color_img.data,imgReturn.cols,imgReturn.rows, OF_IMAGE_COLOR);
    ofImTmp.setFromPixels(imgReturn.data,imgReturn.cols,imgReturn.rows, OF_IMAGE_COLOR);
    return ofImTmp.getPixelsRef();

}

void testApp::blobTracker (cv::Mat color_img) {

    int th = color_img.rows;
    int tw = color_img.cols;

    cv::Mat imgReturn, bgMask;
    cv::Mat img, resizeBgFrame, greyBgFrame;
/*
	cv::CvSeq* contour_list = NULL;
    cv::CvMemStorage*           contour_storage;
    cv::CvMemStorage*           storage;
    cv::CvMoments*              myMoments;
    vector<cv::CvSeq*>          cvSeqBlobs;  //these will become blobs

    contour_storage = cv::cvCreateMemStorage( 1000 );
*/
    //cv::cvtColor(color_img, img, CV_RGB2GRAY);
    cv::cvtColor(bgFrame, greyBgFrame, CV_RGB2GRAY);

    //resizeBgFrame = myResize(greyBgFrame, scaleFloat);

    //cv::cvtColor(resizeBgFrame, greyBgFrame, CV_RGB2GRAY);

    bgSubtractor(greyBgFrame,bgMask);

    //cv::findContours( bgMask, contour_storage, &contour_list,
    //                sizeof(cv::CvContour), cv::CV_RETR_EXTERNAL, cv::CV_CHAIN_APPROX_SIMPLE  );


    ofxCvGrayscaleImage ofxGrIm;
    ofxGrIm.setFromPixels(bgMask.data, sw, sh);
    float maxSize = sh * sw / 4;
    contourFinder.findContours( ofxGrIm, 4, maxSize, 30, true );
    for(int i = 0; i < contourFinder.nBlobs; i++) {
        ofxCvBlob blob = contourFinder.blobs.at(i);
        //if (lastBlobs.size() > 0)
         //   ofLine (lastBlobs.at(i).centroid.x, lastBlobs.at(i).centroid.y,
         //           blob.centroid.x,blob.centroid.y);

        //cv::Point p (blob.centroid.x, blob.centroid.y );
        //cv::circle(color_img,p,5, cv::Scalar(230,230,230));
        ofCircle (blob.centroid.x, blob.centroid.y,4);
    }

    lastBlobs.assign(contourFinder.blobs.begin(), contourFinder.blobs.end());

    //ofImTmp.setFromPixels(color_img.data,color_img.cols,color_img.rows, OF_IMAGE_COLOR);
    //ofImTmp.setFromPixels(ofxGrIm.getPixels(),color_img.cols,color_img.rows, OF_IMAGE_GRAYSCALE);

    //return ofImTmp.getPixelsRef();
    //return ofxCoIm.getPixelsRef();
}


void testApp::blobFinderCV (cv::Mat color_img) {

    cv::Mat img, imgBinary;

    cv::cvtColor(color_img, img, CV_RGB2GRAY);


    //adaptiveThreshold(img, imgBinary, 255, cv::ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,3,5.0 );
    cv::threshold(img, imgBinary, 10,255,cv::THRESH_BINARY);

    vector < vector<cv::Point> > contours;

    vector<cv::Vec4i> hierarchy;
    cv::findContours( imgBinary, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    // iterate through all the top-level contours,
    // draw each connected component with its own random color


    for(int idx = 0 ; idx < contours.size() ; idx ++ )
    {
        //cv::Scalar color( 255,255,255);
        //cv::drawContours( color_img, contours, idx, color, 2, 8, hierarchy );

        cv::Rect bounding = cv::boundingRect(contours.at(idx));
        float x = bounding.x + bounding.width / 2;
        float y = bounding.y + bounding.height / 2;



        ofCircle (x, y, 1);


    }

}

void testApp::goodOpticalFlow (cv::Mat color_img) {


    vector<uchar> status;
    vector<float> errors;

    cv::Mat grey_img, bgMask;

    // beautiful glitch when initiliaze this way cv::Mat blank(h,w,CV_8UC3);
    //cv::Mat blank(color_img.rows,color_img.cols,CV_8UC3, cv::Scalar(50,50,50));

    cv::cvtColor(color_img, grey_img, CV_RGB2GRAY);
    bgSubtractor(grey_img,bgMask);

    if (firstFrameOpticalFlow) {
        lastImgOpticalFlow = bgMask.clone();
        firstFrameOpticalFlow = false;
    } else {
        //lastpoints = points;
        goodFeatureDetector.detect( lastImgOpticalFlow, keyPointsOpticalFlow );
        //points.resize(keypoints.size());
        for (unsigned int i=0;i<keyPointsOpticalFlow.size();i++) {
            lastPointsOpticalFlow[i].x = keyPointsOpticalFlow[i].pt.x;
            lastPointsOpticalFlow[i].y = keyPointsOpticalFlow[i].pt.y;
        }
        std::vector<cv::Point2f> points(MAX_FEATURES); // 500 corners as max

        cv::TermCriteria tc (cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01);

        cv::calcOpticalFlowPyrLK(lastImgOpticalFlow,bgMask,lastPointsOpticalFlow,points,status, errors, cv::Size(11,11),
                                 3,tc,0,0.1);

        lastImgOpticalFlow= bgMask.clone();

        for (unsigned int i=0; i<points.size(); i++ ) {
            if( status[i]==0|| errors[i]>550 ) {
                continue;
            }

            //cv::Point p = lastpoints[i];
            //cv::Point q = points[i];
            //line(blank, p, q, cv::Scalar(230,230,230),1,CV_AA);
            //circle (color_img, p, 3, cv::Scalar(255),1,CV_AA);

            ofVec2f startPoint (lastPointsOpticalFlow[i].x, lastPointsOpticalFlow[i].y);
            ofVec2f endPoint ( points[i].x, points[i].y);
            if (startPoint.distance(endPoint) <  color_img.cols /50.0)
                ofLine (lastPointsOpticalFlow[i].x, lastPointsOpticalFlow[i].y, points[i].x, points[i].y);
        }



    }

    //ofImTmp.setFromPixels(blank.data,blank.cols,blank.rows, OF_IMAGE_COLOR);

    //return ofImTmp.getPixelsRef();


}

void testApp::simpleOpticalFlow (cv::Mat color_img) {


    vector<uchar> status;
    vector<float> errors;

    cv::Mat grey_img, bgMask;

    // beautiful glitch when initiliaze this way cv::Mat blank(h,w,CV_8UC3);
    //cv::Mat blank(color_img.rows,color_img.cols,CV_8UC3, cv::Scalar(50,50,50));

    cv::cvtColor(color_img, grey_img, CV_RGB2GRAY);
    bgSubtractor(grey_img,bgMask);

    if (firstFrameOpticalFlow) {
        lastImgOpticalFlow = bgMask.clone();
        firstFrameOpticalFlow = false;
    } else {
        //lastpoints = points;
        simpleBlobDetector.detect( lastImgOpticalFlow, keyPointsOpticalFlow );
        //points.resize(keypoints.size());
        for (unsigned int i=0;i<keyPointsOpticalFlow.size();i++) {
            lastPointsOpticalFlow[i].x = keyPointsOpticalFlow[i].pt.x;
            lastPointsOpticalFlow[i].y = keyPointsOpticalFlow[i].pt.y;
        }
        std::vector<cv::Point2f> points(MAX_FEATURES); // 500 corners as max

        cv::TermCriteria tc (cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01);

        cv::calcOpticalFlowPyrLK(lastImgOpticalFlow,bgMask,lastPointsOpticalFlow,points,status, errors, cv::Size(11,11),
                                 3,tc,0,0.1);

        lastImgOpticalFlow= bgMask.clone();

        for (unsigned int i=0; i<points.size(); i++ ) {
            if( status[i]==0|| errors[i]>550 ) {
                continue;
            }

            //cv::Point p = lastpoints[i];
            //cv::Point q = points[i];
            //line(blank, p, q, cv::Scalar(230,230,230),1,CV_AA);
            //circle (color_img, p, 3, cv::Scalar(255),1,CV_AA);

            ofVec2f startPoint (lastPointsOpticalFlow[i].x, lastPointsOpticalFlow[i].y);
            ofVec2f endPoint ( points[i].x, points[i].y);
            if (startPoint.distance(endPoint) <  color_img.cols /50.0)
                ofLine (lastPointsOpticalFlow[i].x, lastPointsOpticalFlow[i].y, points[i].x, points[i].y);
        }



    }

    //ofImTmp.setFromPixels(blank.data,blank.cols,blank.rows, OF_IMAGE_COLOR);

    //return ofImTmp.getPixelsRef();


}

void testApp::denseOpticalFlow (cv::Mat color_img) {


    vector<uchar> status;
    vector<float> errors;

    cv::Mat grey_img, bgMask;

    // beautiful glitch when initiliaze this way cv::Mat blank(h,w,CV_8UC3);
    //cv::Mat blank(color_img.rows,color_img.cols,CV_8UC3, cv::Scalar(50,50,50));

    cv::cvtColor(color_img, grey_img, CV_RGB2GRAY);
    bgSubtractor(grey_img,bgMask);

    if (firstFrameOpticalFlow) {
        lastImgOpticalFlow = bgMask.clone();
        firstFrameOpticalFlow = false;
    } else {
        //lastpoints = points;
        denseFeatureDetector.detect( lastImgOpticalFlow, keyPointsOpticalFlow );
        //points.resize(keypoints.size());
        for (unsigned int i=0;i<keyPointsOpticalFlow.size();i++) {
            lastPointsOpticalFlow[i].x = keyPointsOpticalFlow[i].pt.x;
            lastPointsOpticalFlow[i].y = keyPointsOpticalFlow[i].pt.y;
        }
        std::vector<cv::Point2f> points(MAX_FEATURES);

        cv::TermCriteria tc (cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01);

        cv::calcOpticalFlowPyrLK(lastImgOpticalFlow,bgMask,lastPointsOpticalFlow,points,status, errors, cv::Size(11,11),
                                 3,tc,0,0.1);

        lastImgOpticalFlow= bgMask.clone();

        for (unsigned int i=0; i<points.size(); i++ ) {
            if( status[i]==0|| errors[i]>550 ) {
                continue;
            }

            //cv::Point p = lastpoints[i];
            //cv::Point q = points[i];
            //line(blank, p, q, cv::Scalar(230,230,230),1,CV_AA);
            //circle (color_img, p, 3, cv::Scalar(255),1,CV_AA);

            ofVec2f startPoint (lastPointsOpticalFlow[i].x, lastPointsOpticalFlow[i].y);
            ofVec2f endPoint ( points[i].x, points[i].y);
            if (startPoint.distance(endPoint) <  color_img.cols /50.0)
                ofLine (lastPointsOpticalFlow[i].x, lastPointsOpticalFlow[i].y, points[i].x, points[i].y);
        }



    }

    //ofImTmp.setFromPixels(blank.data,blank.cols,blank.rows, OF_IMAGE_COLOR);

    //return ofImTmp.getPixelsRef();


}

