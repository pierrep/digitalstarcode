#include "testApp.h"
#include "ofxIIDCSettings.h"

//--------------------------------------------------------------
void testApp::setup(){

	camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;
	appWidth        = ofGetWidth();
	appHeight       = ofGetHeight();
	mytimeThen		= 0.0f;

	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_SILENT);

    //vidGrabber.setDeviceID("814436102632562");
    //vidGrabber.setDeviceID(0);


    Libdc1394Grabber *sdk = new Libdc1394Grabber;
	sdk->setFormat7(true);
	sdk->listDevices();
	sdk->setDiscardFrames(true);
	sdk->set1394bMode(false);
	//sdk->setROI(x,y,width,height);*/

	//vidGrabber.setVerbose(true);

    bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV422, VID_FORMAT_RGB, -1, true, sdk );

    //bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV422, VID_FORMAT_RGB, 30 );
	// or like this:
	//bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 30, true, new Libdc1394Grabber);
	// or like this:
	//bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV411, VID_FORMAT_RGB, 30, true, new Libdc1394Grabber, new ofxIIDCSettings);


	if(result) {
	    ofLog(OF_LOG_VERBOSE,"Camera successfully initialized.");
	} else {
	    ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
	}

}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(100,100,100);

	vidGrabber.update();


	if (vidGrabber.isFrameNew()){

        calculateCaptureFramerate();
	}

    sprintf(buf,"App framerate : %f",ofGetFrameRate());

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	vidGrabber.draw(appWidth - camWidth,0);


    /* Framerate display */
	ofDrawBitmapString(buf,30,appHeight - 40);
	ofDrawBitmapString(buf2,30,appHeight - 20);
}

//--------------------------------------------------------------
void testApp::calculateCaptureFramerate()
{
    mytimeNow = ofGetElapsedTimef();
    if( (mytimeNow-mytimeThen) > 0.05f || myframes == 0 ) {
        myfps = myframes / (mytimeNow-mytimeThen);
        mytimeThen = mytimeNow;
        myframes = 0;
        myframeRate = 0.9f * myframeRate + 0.1f * myfps;
        sprintf(buf2,"Capture framerate : %f",myframeRate);
    }
    myframes++;
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
	}

}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

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
