#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(40, 100, 40);

	ofSetWindowPosition( 665, 30 );
	ofSetWindowShape( 640, 480 );
	ofSetFrameRate(60);

	// Set up OSC communications
	OSCout.setup( OUT_HOST, OUT_PORT );
	OSCin.setup( IN_PORT );

	initialiseJoints();
	setAllRequiredLengths( 100 );
	setAllPointHistories( 2 );

	synapseLastMessaged = ofGetElapsedTimeMillis();
	synapseMessagingInterval = 5500;

}


//--------------------------------------------------------------
void testApp::update()
{
	processOSCinput();
	processOSCoutput();
	messageSynapse();
}


//--------------------------------------------------------------
void testApp::draw()
{
	
	for( int j=0; j<15; j++ ){

		ofSetColor( 255, 0, 0 );
		//ofEllipse( ofMap( joints[j].worldPos.x, -500, 500, 0, 640), ofMap( joints[j].worldPos.y, -500, 500, 0, 480), 10, 10 );
		ofEllipse( joints[j].worldPos.x, joints[j].worldPos.y, 10, 10 );
		ofSetColor( 0, 255, 0 );
		//ofEllipse( ofMap( joints[j].screenPos.x, -500, 500, 0, 640), ofMap( joints[j].screenPos.y, -500, 500, 0, 480), 10, 10 );
		ofEllipse( joints[j].bodyPos.x, joints[j].bodyPos.y, 10, 10 );
		ofSetColor( 0, 0, 255 );
		//ofEllipse( ofMap( joints[j].bodyPos.x, -500, 500, 0, 640), ofMap( joints[j].bodyPos.y, -500, 500, 0, 480), 10, 10 );
		ofEllipse( joints[j].screenPos.x, joints[j].screenPos.y, 10, 10 );
	}

}


//--------------------------------------------------------------
void testApp::processOSCinput()
{
	while(OSCin.hasWaitingMessages())
	{
		// get the next message
		OSCmessage.clear();
		OSCin.getNextMessage( &OSCmessage );

		string address = OSCmessage.getAddress();

// ########## MAYBE UNNECESSARY ###########
/*
		if( address == "/tracking_skeleton" )
		{
			bool tracking_skeleton = OSCmessage.getArgAsInt32(0);

			//cout << "\nSkeleton - " << tracking_skeleton;

			if( tracking_skeleton )
			{
				ofBackground(0);
			}
			else
			{
				ofBackground(255);
			}
		}
*/
			if( address == "/tracking_skeleton" )
		    {
				break;
			}

	// For each joint
			bool messageMatched = false;

			for( int i=0; i<15; i++ )
			{   

			// Check for bang messages
				if( address == "/" + joints[i].jointName  )
				{
					string value = OSCmessage.getArgAsString(0);
		
					if( value == "up" ){
						// ####### HANDLE THIS SOMEHOW #############
					}
					if( value == "down" ){
						// ####### HANDLE THIS SOMEHOW #############
					}
					if( value == "left" ){
						// ####### HANDLE THIS SOMEHOW #############
					}
					if( value == "right" ){
						// ####### HANDLE THIS SOMEHOW #############
					}
					if( value == "forward" ){
						// ####### HANDLE THIS SOMEHOW #############
					}

					cout << "\nGot " << value << " from " << joints[i].jointName;
					messageMatched = true;
				}	


			// Check for new body coordinates
				else if( address == "/" + joints[i].jointName + "_pos_body"  )
				{
					//cout << "\nSetting " << joints[i].jointName << " body pos to " << OSCmessage.getArgAsFloat(0) << "," <<  OSCmessage.getArgAsFloat(1) << "," << OSCmessage.getArgAsFloat(2);
					joints[i].bodyPos.set( OSCmessage.getArgAsFloat(0), OSCmessage.getArgAsFloat(1), OSCmessage.getArgAsFloat(2) );
					messageMatched = true;
				}

			// Check for new world coordinates
				else if( address == "/" + joints[i].jointName + "_pos_world"  )
				{
					float x = OSCmessage.getArgAsFloat(0);
					
					/*
					if( x < minWorldX ) 
						minWorldX = x;
					if( x > maxWorldX ) 
						maxWorldX = x;
					if( x < minWorldY ) 
						minWorldY = x;
					if( x > maxWorldY ) 
						maxWorldY = x;
					*/

					//cout << "\nSetting " << joints[i].jointName << " world pos to " << OSCmessage.getArgAsFloat(0) << "," <<  OSCmessage.getArgAsFloat(1) << "," << OSCmessage.getArgAsFloat(2);
					joints[i].worldPos.set( OSCmessage.getArgAsFloat(0), OSCmessage.getArgAsFloat(1), OSCmessage.getArgAsFloat(2) );
					messageMatched = true;			
				}

			// Check for new screen coordinates
				else if( address == "/" + joints[i].jointName + "_pos_screen"  )
				{
					//cout << "\nSetting " << joints[i].jointName << " screen pos to " << OSCmessage.getArgAsFloat(0) << "," <<  OSCmessage.getArgAsFloat(1) << "," << OSCmessage.getArgAsFloat(2);
					joints[i].screenPos.set( OSCmessage.getArgAsFloat(0), OSCmessage.getArgAsFloat(1), OSCmessage.getArgAsFloat(2) );
					messageMatched = true;
				}
			}	

			if( !messageMatched ){
				cout << "\nunrecognised message : " << address;
			}


	}
}


//--------------------------------------------------------------
void testApp::processOSCoutput()
{
}


//--------------------------------------------------------------
void testApp::messageSynapse()
{
	// Periodically remind synapse of what we are and aren't tracking
	if( ofGetElapsedTimeMillis() - synapseLastMessaged > synapseMessagingInterval )
	{
		
		//setAllRequiredLengths( 20 );
		//setAllPointHistories( 5 );

		cout << "\nPOKING SYNAPSE";

		for( int i=0; i<15; i++ )
		{
			TrackableJoint j = joints[i];
			string address   = "/" + j.jointName + "_trackjointpos";

			
			if( j.trackBodyPos )
			{
				OSCmessage.clear();
				OSCmessage.setAddress( address );
				OSCmessage.addIntArg( 1 );
				OSCout.sendMessage( OSCmessage );
			}
			if( j.trackWorldPos )
			{
				OSCmessage.clear();
				OSCmessage.setAddress( address );
				OSCmessage.addIntArg( 2 );
				OSCout.sendMessage( OSCmessage );	
			}
			if( j.trackScreenPos )
			{
				OSCmessage.clear();
				OSCmessage.setAddress( address );
				OSCmessage.addIntArg( 3 );
				OSCout.sendMessage( OSCmessage );
			}

		}

		synapseLastMessaged = ofGetElapsedTimeMillis();
	}
}


//--------------------------------------------------------------
void testApp::setJointTrackModes( int jointIndex, bool bodyPos, bool worldPos, bool screenPos )
{
	joints[ jointIndex ].trackBodyPos   = bodyPos;
	joints[ jointIndex ].trackWorldPos  = worldPos;
	joints[ jointIndex ].trackScreenPos = screenPos;
}


//--------------------------------------------------------------
void testApp::setRequiredLength( int jointIndex, float value )
{
	OSCmessage.clear();
	OSCmessage.setAddress( "/" + joints[jointIndex].jointName + "_requiredlength" );
	OSCmessage.addFloatArg( value );

	//cout << "\nSENDING " << OSCmessage.getAddress() + ":" << OSCmessage.getArgAsFloat(0);

	OSCout.sendMessage( OSCmessage );
}


//--------------------------------------------------------------
void testApp::setPointHistory( int jointIndex, int value )
{
	OSCmessage.clear();
	OSCmessage.setAddress( "/" + joints[jointIndex].jointName + "_pointhistorysize" );
	OSCmessage.addIntArg( value );

	cout << "\nSENDING " << OSCmessage.getAddress() + ":" << OSCmessage.getArgAsInt32(0);

	OSCout.sendMessage( OSCmessage );
}


//--------------------------------------------------------------
void testApp::setAllRequiredLengths( float value )
{
	for( int index = 0; index < 15; index++ )
	{
		setRequiredLength( index, value );
	}
}


//--------------------------------------------------------------
void testApp::setAllPointHistories( int value )
{
	for( int index = 0; index < 15; index++ )
	{
		setPointHistory( index, value );
	}
}


//--------------------------------------------------------------
void testApp::initialiseJoints(){

	joints[0].jointName  = "righthand";
	joints[1].jointName  = "lefthand";
	joints[2].jointName  = "rightelbow";
	joints[3].jointName  = "leftelbow";
	joints[4].jointName  = "rightfoot";
	joints[5].jointName  = "leftfoot";
	joints[6].jointName  = "rightknee"; 
	joints[7].jointName  = "leftknee";
	joints[8].jointName  = "head";
	joints[9].jointName  = "torso";
	joints[10].jointName = "rightshoulder";
	joints[11].jointName = "leftshoulder";
	joints[12].jointName = "righthip";
	joints[13].jointName = "lefthip";
	joints[14].jointName = "closesthand";

	for( int i=0; i<15; i++ )
	{
		setJointTrackModes( i, false, false, true );
		joints[i].bodyPos.set(   50, 50, 50 );
		joints[i].worldPos.set(  60, 60, 60 );
		joints[i].screenPos.set( 70, 70, 70 );
	}
}


//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

	if( key == 't' )
	{
		for( int i=0; i<15; i++ )
		{
			OSCmessage.clear();
			OSCmessage.setAddress( "/" + joints[i].jointName + "_gettuninginfo");
			OSCout.sendMessage( OSCmessage );
		}
	}

	if( key == 'l' ){
		setRequiredLength( ofRandom( 0, 15 ), ofRandom(30, 40) );
	}


	if( key == '1' )
	{
		OSCmessage.clear();
		OSCmessage.setAddress( "/depth_mode");
		OSCmessage.addIntArg(0);
		OSCout.sendMessage( OSCmessage );
	}

	if( key == '2' )
	{
		OSCmessage.clear();
		OSCmessage.setAddress( "/depth_mode");
		OSCmessage.addIntArg(1);
		OSCout.sendMessage( OSCmessage );
	}

	if( key == '3' )
	{
		OSCmessage.clear();
		OSCmessage.setAddress( "/depth_mode");
		OSCmessage.addIntArg(2);
		OSCout.sendMessage( OSCmessage );
	}
}

/*

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
}

*/