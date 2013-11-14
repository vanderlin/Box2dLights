#include "App.h"

//--------------------------------------------------------------
void App::setup(){
    
    ofBackground(0);
    ofDisableArbTex();  // <-- Very Important

	

    lightRender.setup(720, 480);
    lightRender.setRenderFuction(this, &App::renderObjects);
    
    lightRender.addLight(255, 255);
    
    
    box2d.init();
	box2d.setGravity(0, 0.0);
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
    
}

//--------------------------------------------------------------
void App::update(){
    box2d.update();

    for(int i=0; i<circles.size(); i++) {
        ofVec2f p = circles[i].getPosition();
        if(!ofRectangle(0, 0, lightRender.width, lightRender.height).inside(p)) {
            circles[i].setPosition(ofRandom(lightRender.width), 10);
            circles[i].addImpulseForce(ofVec2f(ofRandomf()*5, ofRandomf()*5), ofVec2f(1,1));
        }
    }
    
    lightRender.renderLights();
    
    if(lightRender.lights.size()) lightRender.lights[0].pos.set(ofGetMouseX(), ofGetMouseY());
}

//--------------------------------------------------------------
void App::renderObjects() {
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0x90d4e3);
        ofCircle(circles[i].getPosition(), circles[i].getRadius());
	}
}

//--------------------------------------------------------------
void App::draw() {
    
    lightRender.draw();
    renderObjects();

    // debug draw
    lightRender.debugDraw(lightRender.width, 0);
    
    ofSetColor(255);
    ofDrawBitmapString("Space to add circles\nClick to add a light\nc to clear everything\n"+ofToString(ofGetFrameRate(),0)+ "fps", 20, lightRender.height+40);
}

//--------------------------------------------------------------
void App::keyPressed(int key){
    if(key == ' ') {
		float r = ofRandom(2, 5);		
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), mouseX, mouseY, r);
        circle.addImpulseForce(ofVec2f(ofRandomf()*5, ofRandomf()*5), ofVec2f(1,1));
		circles.push_back(circle);
	}
    if(key == 'c') {
        lightRender.lights.clear();
        circles.clear();
        fixedCircles.clear();
    }
}

//--------------------------------------------------------------
void App::keyReleased(int key){

}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){
    lightRender.addLight(x, y);
    
    ofxBox2dCircle circle;
    circle.setup(box2d.getWorld(), x, y, 10);
    circle.setPhysics(0.0, 0.53, 0.1);
    fixedCircles.push_back(circle);
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void App::windowResized(int w, int h){

}

//--------------------------------------------------------------
void App::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void App::dragEvent(ofDragInfo dragInfo){ 

}
