#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define IN_PORT  12347
#define OUT_PORT 12346
#define OUT_HOST "localhost"

#define righthand      0;
#define lefthand       1;
#define rightelbow     2;
#define leftelbow      3;
#define rightfoot      4;
#define leftfoot       5;
#define rightknee      6;
#define leftknee       7;
#define head           8;
#define torso          9;
#define rightshoulder 10;
#define leftshoulder  11;
#define righthip      12;
#define lefthip       13;
#define closesthand   14;


struct TrackableJoint 
{
	string jointName;

	bool trackBodyPos;
	bool trackWorldPos;
	bool trackScreenPos;
	
	ofVec3f bodyPos;
	ofVec3f worldPos;
	ofVec3f screenPos;
};



//--------------------------------------------------------
class testApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void processOSCinput();
		void processOSCoutput();
		
		void setRequiredLength( int jointIndex, float value );
		void setPointHistory(   int jointIndex, int   value );
		void setJointTrackModes( int jointIndex, bool bodyPos, bool worldPos, bool screenPos );

		void setAllRequiredLengths( float value );
		void setAllPointHistories(  int   value );
		
		ofxOscSender   OSCout;
		ofxOscReceiver OSCin;
		ofxOscMessage  OSCmessage;
	
		TrackableJoint joints[15];
		void initialiseJoints();

		void messageSynapse();
		int synapseLastMessaged;
		int synapseMessagingInterval;

		void keyPressed(int key);



		/* ### UNUSED PUBLIC ofBaseApp METHODS ###
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		*/

};

