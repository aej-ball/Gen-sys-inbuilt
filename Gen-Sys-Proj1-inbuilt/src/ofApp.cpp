#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    camWidth = 320;
    camHeight = 240;
    
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    
    b_showDebug = true; //shows images from colour to blackout white
    ofSetBackgroundColor(255, 255, 255);
    currentTime = ofGetElapsedTimeMillis();
    waitTime = 5000; //amount to wait in ms
    
    
    colorImg.allocate(camWidth, camHeight);
    grayImg.allocate(camWidth, camHeight);
    backgroundGray.allocate(camWidth,camHeight);
    backgroundCol.allocate(camWidth,camHeight);
    grayDiff.allocate(camWidth, camHeight);
    contoured.allocate(camWidth, camHeight);
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    bLearnBackground = true;
    threshold = 70;
    string ofGetTimestampString("%A %B %Y %R");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    line.clear();
    bool bNewFrame = false;
    
    //grabs new data from camera
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    blobpoints.clear();
    
    if (vidGrabber.isFrameNew());
    
    if (vidGrabber.isFrameNew()){
        colorImg.setFromPixels(vidGrabber.getPixels());
    
        backgroundGray = backgroundCol; //makes background image grayscale
        grayImg = colorImg; //makes live footage grayscale
    
        if (bLearnBackground == true){
            backgroundGray = grayImgPic;
            bLearnBackground = false;
        }
    
    grayDiff.absDiff( backgroundGray, grayImgPic);
    grayDiff.threshold(threshold);
    contourFinder.findContours(grayDiff, 20, (camWidth*camHeight)/4, 1, false, true);
    }
    

    if (ofGetElapsedTimeMillis() > currentTime + waitTime) {
        // this is the loop that waits and does stuff every waitTime millisecond
        cout << "it's time !!!!" << endl;
    
        
        grayImgPic = grayImg; // update captured image
        
        currentTime = ofGetElapsedTimeMillis();
        ofImage screenShot;
        screenShot.grabScreen(0, 0, camWidth, camHeight); // grabs screenshot
        screenShots.push_back(screenShot);

//        screenShot.save("screenshot_"+ ofGetTimestampString() +".png"); //to save images to the bin folder
        
    }
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (b_showDebug){               // show or hide the debug stuff
    backgroundCol.draw(0,100 + camWidth, camHeight/4, camHeight/4);
    backgroundGray.draw(0+(camWidth/4), 100 + camHeight, camWidth/4, camHeight/4);
    colorImg.draw(0,100 + (camHeight+((camHeight/4))), camWidth/4, camHeight/4);
    grayImg.draw(0+(camWidth/4),100 + (camHeight+((camHeight/4))), camWidth/4, camHeight/4);
    grayImgPic.draw(0+((camWidth/4)*2), 100 + (camHeight+(camHeight/4)), camWidth/4, camHeight/4);
    grayDiff.draw(0+((camWidth/4)*3), 100 + (camHeight+(camHeight/4)), camWidth/4, camHeight/4);
    }

    //finds contours and then draws on the line
    ofPushMatrix();
    if (contourFinder.nBlobs > 0){
        for (int k = 0; k < contourFinder.nBlobs; k++){
            for (int i = 0; i < contourFinder.blobs[k].pts.size(); i++){
                ofVec3f point(contourFinder.blobs[k].pts[i]);
                blobpoints.push_back(point);
            }
        }
        for (int j = 0; j < blobpoints.size(); j++) {
            line.curveTo(blobpoints[j]);
        }
        line.close();
        
        ofSetLineWidth(3.0);
        ofSetColor(0, 0, 0);
        line.draw();
        ofSetColor(255);

    }
    ofPopMatrix();
    
    //draws thumbnails of screenshots to screen
    int thumbWidth = 100;

                for (int i = 0; i < screenShots.size(); i++) {
                    screenShots[i].draw(i * thumbWidth, camHeight, thumbWidth, thumbWidth * 0.75);
                    }
    


    //Shows the time and date on the screen
    ofSetColor(0, 0, 0);
    ofDrawBitmapString(ofGetTimestampString("%H:%M - %d/%m/%y"), 0+10, camHeight-30);
    ofSetColor(255);

    
    
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'b':
            // sets background image
            backgroundCol.setFromPixels(vidGrabber.getPixels());
            break;
            
        case 'p':
            //can overide forground auto image
            grayImgPic = grayImg;
            break;
            
        case 'q':
            //alters thresholh +
            threshold++;
            break;
        
        case 'a':
            //alters threshold -
            threshold--;
            break;
            
        case 'd':
            // show or hide the debug stuff
            b_showDebug = !b_showDebug;
        break;
            
        default:
            break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
