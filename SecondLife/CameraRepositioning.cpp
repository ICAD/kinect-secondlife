// SkeleGlutv0.1.cpp : main project file.

#include "stdafx.h"
#include "glut.h"
#include <XnOpenNI.h>
#include <XnList.h>
#include <XnCppWrapper.h>
#include <XnCodecIDs.h>
#include "MouseControl.h"
#include <math.h>
#include <Windows.h>  





#define SAMPLE_XML_PATH "Sample-User.xml"




xn::Context g_Context;
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator g_UserGenerator;
MouseControl *mouse = new MouseControl ();
xn::SceneAnalyzer g_SceneAnalyzer;
double g_LastSmoothingFactor = 0.0;

int test2=0x40;
XnBool	g_bCalibrate1 = false, g_bCalibrate2 = false;
XnBool g_bShowBG = false;
XnBool g_IsSkeletonEnabled = true;
XnBool drawB=false,flyPressed=false;
XnBool CaptureFeet=false;
XnBool initHands=false;
XnBool steady=false;
float r=1.0f,g=0.0f,b=0.0f;
float angle=0.0,deltaAngle = 0.0,ratio;
float x=0.0f,y=0.0f,z=0.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f;
float cx=0.02f,cy=0.0f,cz=-1.0f;
GLint snowman_display_list;
int deltaMove = 0;
float neckX=320,neckY=240,neckZ;
float deltaXMove=0,deltaYMove=0,changeX=0,changeY=0,changeZ=0;

XnSkeletonJoint getJoint(int a);
void DrawBones(XnUserID user, xn::UserGenerator g_UserGenerator );
void SendKeyDown(char charCode);
void SendKeyUp(char charCode);
 void SendVKDown(unsigned short scanCode);
	  void SendVKUp(unsigned short scanCode);



void initScene() {

	glEnable(GL_DEPTH_TEST);


}

void orientMe(float ang) {


	lx += sin(ang);
	//lz += -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      lx, ly, lz,
			  0.0f,1.0f,0.0f);
}


void moveMeFlat(int i) {
	x = x + i*(lx)*0.1;
	z = z + i*(lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);

}
void drawJoint(XnUserID user, xn::UserGenerator g_UserGenerator,XnSkeletonJoint eJoint)
{
	static int i=0;

	
	XnStatus rc = XN_STATUS_OK;
	XnSkeletonJointPosition p;
	XnPoint3D point;
	int joint=eJoint;
	
	float X,Y;

		
		glPushMatrix();
		rc=g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, eJoint, p);
		//printf("%s failed: %d\n",xnGetStatusString(rc),joint);
		point=p.position;
		g_DepthGenerator.ConvertRealWorldToProjective(1, &point, &point);
		//printf("%f , %f\n",point.X,point.Y);
		
			
		
				
		X=(point.X-320)/320; 
		Y=-1*(point.Y-240)/240;
	glPushMatrix();
	glTranslatef(X, Y,0);
	if (eJoint==1){
		glutSolidSphere(0.05f,20,20);
		//printf("head x:%f y:%f ",X,Y);
		//printf("x:%f y:%f z:%f \n",point.X,point.Y,point.Z);
		//centerY(Y);
		//deltaAngle=X;
	}
	
	glutSolidSphere(.05f,20,20);
	glPopMatrix();
	
	i++;
	
}
	void DrawSingleUser(XnUserID user, xn::UserGenerator g_UserGenerator)
{
	static int i=0;
	i++;
	 int count=0;
	 XnSkeletonJoint j;
	
		for (count=1;count<25;count++)
		{
			
			glColor3f(count/24.0f, count/48.0f, 1.0f);
			j=getJoint(count);
			if (!g_UserGenerator.GetSkeletonCap().IsJointAvailable(j))continue;
			drawJoint(user,g_UserGenerator,j);
		}
	//if (i==1)
		//for (count=0;count<1000000;count++)gluLookAt(x,y,z,count/2000000.0f,count/2000000.0f,-1.0f,0.0f,1.0f,0.0f);
		//lx=X;ly=Y;lz=point.Z/3000;}
	}
	void getJointPoint(XnUserID user, xn::UserGenerator g_UserGenerator,XnSkeletonJoint joint,XnPoint3D* point)
   {
	   //this will return a point from the user for a specific joint
	   XnSkeletonJointPosition skeletonPosition;
	   g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, joint, skeletonPosition);
	   *point=skeletonPosition.position;

   }
void DrawBoundaries (XnPoint3D refL,XnPoint3D refR,XnPoint3D Left,XnPoint3D Right)
{	// draw boundary box
		float zLDiff,zRDiff;
		static float zL=1;
		g_DepthGenerator.ConvertRealWorldToProjective(1, &refL, &refL);
		g_DepthGenerator.ConvertRealWorldToProjective(1, &refR, &refR);
		g_DepthGenerator.ConvertRealWorldToProjective(1, &Left, &Left);
		g_DepthGenerator.ConvertRealWorldToProjective(1, &Right, &Right);

		zLDiff=refL.Z-Left.Z;
		zRDiff=refR.Z-Right.Z;
	glLineWidth(3.0);
	//--Top Left HanD
		glBegin(GL_LINES);
			glVertex3f( (refL.X-345)/320,-1*(refL.Y-200)/240, -1.0f);
			glVertex3f( (refL.X-295)/320,-1*(refL.Y-200)/240, -1.0f);
		glEnd();
		//--Bottom
		glBegin(GL_LINES);
			glVertex3f( (refL.X-345)/320,-1*(refL.Y-270)/240, -1.0f);
			glVertex3f( (refL.X-295)/320,-1*(refL.Y-270)/240, -1.0f);
		glEnd();
		//--Left
		glBegin(GL_LINES);
			glVertex3f( (refL.X-345)/320,-1*(refL.Y-210)/240, -1.0f);
			glVertex3f( (refL.X-345)/320,-1*(refL.Y-270)/240, -1.0f);
		glEnd();
		//--Right
		glBegin(GL_LINES);
			glVertex3f( (refL.X-345)/320,-1*(refL.Y-210)/240, -1.0f);
			glVertex3f( (refL.X-345)/320,-1*(refL.Y-270)/240, -1.0f);
		glEnd();
		
		glBegin(GL_LINES);
			glVertex3f( (refL.X-295)/320,-1*(refL.Y-210)/240, -1.0f);
			glVertex3f( (refL.X-295)/320,-1*(refL.Y-270)/240, -1.0f);
		glEnd();
		//--RIght Hand Top
		glBegin(GL_LINES);
			glVertex3f( (refR.X-335)/320,-1*(refR.Y-210)/240, -1.0f);
			glVertex3f( (refR.X-305)/320,-1*(refR.Y-210)/240, -1.0f);
		glEnd();
		//--Bottom
		glBegin(GL_LINES);
			glVertex3f( (refR.X-345)/320,-1*(refR.Y-270)/240, -1.0f);
			glVertex3f( (refR.X-295)/320,-1*(refR.Y-270)/240, -1.0f);
		glEnd();
		//--Left
		glBegin(GL_LINES);
			glVertex3f( (refR.X-345)/320,-1*(refR.Y-210)/240, -1.0f);
			glVertex3f( (refR.X-345)/320,-1*(refR.Y-270)/240, -1.0f);
		glEnd();
		//--Right
		glBegin(GL_LINES);
			glVertex3f( (refR.X-295)/320,-1*(refR.Y-210)/240, -1.0f);
			glVertex3f( (refR.X-295)/320,-1*(refR.Y-270)/240, -1.0f);
		glEnd();

			
		glBegin(GL_LINES);
			glVertex3f( 0,.9, -1.0f);
			glVertex3f( 0,.7, -1.0f);
		glEnd();
	
		
			glPushMatrix();
	glTranslatef(0, 0.8f,0);
	glutSolidSphere(.025f,20,20);
	glPopMatrix();

		
}
	void handsBodyMovementLogic(XnPoint3D refL,XnPoint3D refR,XnPoint3D Left,XnPoint3D Right)
	{
		
		float xLDiff,xRDiff,zLDiff,zRDiff,yLDiff,yRDiff;
		XnPoint3D cL,cR,cLR,cRR; // change these!!! once testing is done
		
		g_DepthGenerator.ConvertRealWorldToProjective(1, &refL, &cLR);
		g_DepthGenerator.ConvertRealWorldToProjective(1, &refR, &cRR);
		g_DepthGenerator.ConvertRealWorldToProjective(1, &Left, &cL);
		g_DepthGenerator.ConvertRealWorldToProjective(1, &Right, &cR);
		//printf("RL: %f   L: %f \n RR: %f    R:  %f\n",cLR.Z,cL.Z,cRR.Z,cR.Z);
		xLDiff=cLR.X-cL.X;
		xRDiff=cRR.X-cR.X;
		zLDiff=cLR.Z-cL.Z;
		zRDiff=cRR.Z-cR.Z;
		yLDiff=cLR.Y-cL.Y;
		yRDiff=cRR.Y-cR.Y;
	

		if (xLDiff>20 && xRDiff<-20)
		{
			

		
			SendKeyDown('w');
			SendKeyUp('w');
		
	

		}
		else if (xLDiff<-20 && xRDiff>20)
		{
			
	
			SendKeyDown('s');
			SendKeyUp('s');
		}
		if (zLDiff>100  )
		{
	
		SendKeyUp('d');
		    SendKeyDown('a');
			
			

		}
		else if (zRDiff>100 )
		{
		
		SendKeyUp('a');
		SendKeyDown('d');
		
			
	
		}
		else
		{
			SendKeyUp('a');
		SendKeyUp('d');
		}
		if (yLDiff>20 && yRDiff>20 )
		{
		SendVKDown(VK_PRIOR);
		SendVKUp(VK_NEXT);	
			//printf("up\n");
		}
		else if (yRDiff<-20 && yLDiff<-20)
		{
		//printf("Down\n");
		SendVKDown(VK_NEXT);
		SendVKUp(VK_PRIOR);	
			
	
		}
		else{
			SendVKUp(VK_PRIOR);	
			SendVKUp(VK_NEXT);
		}
		
		/**zLDiff=abs(refL.Z)-abs(Left.Z);
		zRDiff=abs(refR.Z)-abs(Right.Z);
		//printf ("L:  %f\n R: %f\n",abs(refL.X),Right.X);
		if (xLDiff>20 &&xRDiff>20 ||xLDiff<-20 &&xRDiff<-20)
		{
			//printf("ldiff %f rdiff %f \N refl %f  left %f \nrefr %f right %f \n %d \n",xLDiff,xRDiff,refL.X,Left.X,refR.X,Right.X,refL.X>Left.X);
			if (refL.X>Left.X && refR.X<Right.X)
			{
				printf("going forward");
					r=0.0f;
					b=1.0f;
					g=0.0f;
			}
			else
				g=1.0f;
		}**/
		
	}
	void handsLocationLogic(XnUserID user, xn::UserGenerator g_UserGenerator)
	{
		//-----!!!!! One thing to think about is that if we move forward or back that we might want to reset the initHands values could change...
		static int count=0;

		static XnPoint3D refLeftHand,refRightHand;
		XnPoint3D lPoint,rPoint,head,torso;
		
		getJointPoint(user,g_UserGenerator,XN_SKEL_LEFT_HAND,&lPoint);
		getJointPoint(user,g_UserGenerator,XN_SKEL_RIGHT_HAND,&rPoint);
		getJointPoint(user,g_UserGenerator,XN_SKEL_HEAD,&head);
		getJointPoint(user,g_UserGenerator,XN_SKEL_TORSO,&torso);

	
		if (lPoint.Y>(torso.Y-50)&&lPoint.Y<(head.Y+10) && rPoint.Y>(torso.Y-50)&&rPoint.Y<(head.Y+10))
		{
			//This has been modded from original design to only look for both hands in position
			if (!steady)
			{
				count++;
				if (count>8)
				{
					steady=true;
					count=0;
				}
			}
			else
			{
			//***-- it is now steady; check to see if this is first time through set ref hand points  think about using states in the class for this//
				if (!initHands)
				{
					refLeftHand=lPoint;
					refRightHand=rPoint;
					initHands=true;
					
				}
	
					r=0;
					b=1.0f;
					g=0.0f;
					handsBodyMovementLogic(refLeftHand,refRightHand,lPoint,rPoint);
					DrawBoundaries (refLeftHand,refRightHand,lPoint,rPoint);

				
				
				
			}
			flyPressed=false;
			
		}
		
		// Checks if both hands are above head if so switch on flying
		else if (lPoint.Y>(head.Y+20) && rPoint.Y>(head.Y+20))
		{
		 if (!flyPressed)
			{g=1.0f;
			b=1.0f;
			r=0.0f;
			
			steady=false;
			count=0;
			initHands=false;
			flyPressed=true;
			printf("FLY");
			SendKeyDown('f');
			SendKeyUp('f');
			SendVKUp(VK_PRIOR);	
			SendVKUp(VK_NEXT);

		 }
		}//-- We also reset the values of false and steady as well as initHands, again a states could probably increase readability as well as redundancy.
		//-- Also need to make sure that the up down and left right buttons are sent back up so not 
		else
		{

			steady=false;
			count=0;
			initHands=false;
			flyPressed=false;
			SendVKUp(VK_PRIOR);	
			SendVKUp(VK_NEXT);
			SendKeyUp('a');
			SendKeyUp('d');
		}

	
		
		
	}

void draw_stickfigure()
{
	
	float fNewColor = 0;
	xn::DepthMetaData dm;
	g_DepthGenerator.GetMetaData(dm);
	XnPoint3D corner = xnCreatePoint3D(dm.XRes(), dm.YRes(), dm.ZRes());


	XnUserID users[10];
	XnUInt16 nUsers = 10;
	
	glPushMatrix();
	g_UserGenerator.GetUsers(users, nUsers);
	for (int i = 0; i < nUsers; ++i)
	{
		
		if (g_UserGenerator.GetSkeletonCap().IsTracking(users[i]))
		{
			DrawSingleUser(users[i], g_UserGenerator);

			if(drawB)
			DrawBones(users[i],g_UserGenerator);
			handsLocationLogic(users[i],  g_UserGenerator);
			/*mouse->Sendposition1(users[i],i,XN_SKEL_LEFT_SHOULDER,XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND,g_DepthGenerator,g_UserGenerator);
			mouse->Sendposition1(users[i],i,XN_SKEL_RIGHT_SHOULDER,XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND,g_DepthGenerator,g_UserGenerator);
			mouse->Sendposition(users[i],i,XN_SKEL_RIGHT_HAND,"RH",g_DepthGenerator,g_UserGenerator);
			mouse->Sendposition(users[i],i,XN_SKEL_LEFT_HAND,"LH",g_DepthGenerator,g_UserGenerator);*/
		}
	}

		glPopMatrix();
		
	
	
}
DWORD WINAPI EEThreadProc(LPVOID lpThreadParameter)
{
	{
		g_Context.WaitAndUpdateAll();
	
	}
	return 0;
}
void renderScene(void) {

	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Draw ground

	glColor3f(0.9f, 0.0f, 0.0f);
	//glBegin(GL_QUADS);
		//glVertex3f(-100.0f, 0.0f, -100.0f);
		//glVertex3f(-100.0f, 0.0f,  100.0f);
		//glVertex3f( 100.0f, 0.0f,  100.0f);
	//glVertex3f( 100.0f, 0.0f, -100.0f);
	//glEnd();
		
	//glPushMatrix();
	glBegin(GL_QUADS);
		glVertex3f(-10.0f,-1*cy +0.01f, -1.0f);
		glVertex3f(-10.0f,-1*cy-0.01f,  -1.0f);
		glVertex3f( 10.0f, -1*cy-0.01f,  -1.0f);
		glVertex3f( 10.0f, -1*cy+0.01f, -1.0f);
		glEnd();

	glBegin(GL_QUADS);
		glVertex3f( cx+0.01,-10.0f, -1.0f);
		glVertex3f(cx-0.01f,-10.0f,  -1.0f);
		glVertex3f( cx-0.01f,10.0f,   -1.0f);
		glVertex3f( cx+0.01f,10.0f,  -1.0f);
		glEnd();
	
	


		
		
	


		
	//glPopMatrix();
	EEThreadProc(NULL);
	draw_stickfigure();
// Draw 36 SnowMen


	glutSwapBuffers();
}
void pressKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : 
			printf("LEFT\n");break;
		case GLUT_KEY_RIGHT : 
			printf("Right\n");break;
		case GLUT_KEY_UP : 
			printf("UP\n");break;
		case GLUT_KEY_DOWN : 
			printf("SOWN\n");break;
		case GLUT_KEY_F5: 
			printf("Skeleton");
			drawB =!drawB;break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : 
			printf("UNDONE LEFT\n");break;
		case GLUT_KEY_RIGHT : 
			printf("UNDONE RIGHT\n");;break;
		case GLUT_KEY_UP : printf("UNDONE UP\n");break;
		case GLUT_KEY_DOWN : printf("UNDONE Down\n");break;
		
	}
}

	void XN_CALLBACK_TYPE NewUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	printf("New user identified: %d\n", user);
	g_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", user);
}

void XN_CALLBACK_TYPE LostUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	printf("User %d lost\n", user);
}

void XN_CALLBACK_TYPE CalibrationStart(xn::SkeletonCapability& skeleton, XnUserID user, void* pCookie)
{
	printf("Calibration start for user %d\n", user);
}
void XN_CALLBACK_TYPE CalibrationEnd(xn::SkeletonCapability& skeleton, XnUserID user, XnBool bSuccess, void* pCookie)
{
	printf("Calibration complete for user %d: %s\n", user, bSuccess?"Success":"Failure");
	if (bSuccess)
	{
		skeleton.StartTracking(user);
	}
	else
	{
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", user);
	}
}
void XN_CALLBACK_TYPE PoseDetected(xn::PoseDetectionCapability& poseDetection, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	printf("Pose '%s' detected for user %d\n", strPose, nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, FALSE);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
}
#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
		return rc;													\
	}
int main(int argc, char **argv)
{
		
	XnStatus rc = XN_STATUS_OK;

	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH);
	CHECK_RC(rc, "InitFromXml");

	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, "Find depth generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
	CHECK_RC(rc, "Find user generator");

	XnCallbackHandle h;

	g_UserGenerator.RegisterUserCallbacks(NewUser, LostUser, NULL, h);
	g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

	XnCallbackHandle hCalib;
	XnCallbackHandle hPose;
	g_UserGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(&CalibrationStart, &CalibrationEnd, NULL, hCalib);
	g_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(&PoseDetected, NULL, NULL, hPose);

	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

	xn::DepthMetaData depthMD;
	g_DepthGenerator.GetMetaData(depthMD);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Wyatt-Skeleton Tracking");

	initScene();

	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);



	//glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}

//TODO: Add a default case to control logic flow
XnSkeletonJoint getJoint(int a)
{

	switch (a){
	case 1:
		return XN_SKEL_HEAD;
			break;
	case 2:
		return XN_SKEL_NECK	;
			break;
			
	case 3:
		return XN_SKEL_TORSO;	
		break;
	case 4:
		return XN_SKEL_WAIST;	
		break;
	case 5:
		return XN_SKEL_LEFT_COLLAR;	
			break;
	case 6:
		return XN_SKEL_LEFT_SHOULDER;	
			break;
	case 7:
		return XN_SKEL_LEFT_ELBOW	;
			break;
	case 8:	
		return XN_SKEL_LEFT_WRIST	;	
			break;
	case 9:
		return XN_SKEL_LEFT_HAND	;	
			break;
	case 10:
		return XN_SKEL_LEFT_FINGERTIP	;
			break;
	case 11:
		return XN_SKEL_RIGHT_COLLAR	;
			break;
	case 12:
		return XN_SKEL_RIGHT_SHOULDER	;
			break;
	case 13:
		return XN_SKEL_RIGHT_ELBOW	;	
			break;
	case 14:
		return XN_SKEL_RIGHT_WRIST	;	
			break;
	case 15:
		return XN_SKEL_RIGHT_HAND	;
			break;
	case 16:
		return XN_SKEL_RIGHT_FINGERTIP	;
			break;
	case 17:
		return XN_SKEL_LEFT_HIP	;
			break;
	case 18:	
		return XN_SKEL_LEFT_KNEE	;
			break;
	case 19:
		return XN_SKEL_LEFT_ANKLE	;	
			break;
	case 20:
		return XN_SKEL_LEFT_FOOT	;
			break;
	case 21:
		return XN_SKEL_RIGHT_HIP	;
			break;
	case 22:	
		return XN_SKEL_RIGHT_KNEE	;
			break;
	case 23:
		return XN_SKEL_RIGHT_ANKLE	;
			break;
	case 24:
		return XN_SKEL_RIGHT_FOOT	;
			break;
	//TODO: reassess logic default
	default:
		return XN_SKEL_HEAD;
			break;
	}
}

void drawStickPoint(XnPoint3D point)								
{	g_DepthGenerator.ConvertRealWorldToProjective(1, &point, &point);																	
	float fX = ((point.X-320)/ 320);								
	float fY = -((point.Y-240) /240);								
	float fU = fX + 0.5;												
	float fV = fY + 0.5;												
	glVertex3f(fX, fY, 0.0f);	
}
void drawStickLine(XnUserID user, xn::UserGenerator userGenerator, XnSkeletonJoint joint1, XnSkeletonJoint joint2)
{
	XnSkeletonJointPosition pos1, pos2;
	userGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, joint1, pos1);
	userGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, joint2, pos2);
	if (pos1.fConfidence == 1 &&
		pos2.fConfidence == 1)
	{
		glColor3f(1.0f,0.5f,0.3f);
	
	}
	else
	{
		glColor3f(0.5,0.5,0.5);

		if ((pos1.position.X == 0 && pos1.position.Y == 0 && pos1.position.Z == 0) ||
			(pos2.position.X == 0 && pos2.position.Y == 0 && pos2.position.Z == 0))
		{
			return;
		}
	}
		drawStickPoint(pos1.position);
	drawStickPoint(pos2.position);
		
}
void DrawBones(XnUserID user, xn::UserGenerator g_UserGenerator ){
	glPushMatrix();
		glLineWidth(3.0);
	glBegin(GL_LINES);
	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_HAND, XN_SKEL_LEFT_ELBOW );
 	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_SHOULDER);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_SHOULDER, XN_SKEL_RIGHT_SHOULDER);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_TORSO, XN_SKEL_RIGHT_SHOULDER);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_NECK, XN_SKEL_HEAD);
 
	drawStickLine(user, g_UserGenerator, XN_SKEL_TORSO, XN_SKEL_LEFT_HIP);
	drawStickLine(user, g_UserGenerator, XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP);

	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP);

	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE);
	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT);
	drawStickLine(user, g_UserGenerator, XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE);
	drawStickLine(user, g_UserGenerator, XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT);
	glEnd();
	glPopMatrix();
}



 void SendVKUp(unsigned short scanCode)
		{
		
			INPUT inputUp;
			inputUp.type = INPUT_KEYBOARD;
			inputUp.ki.dwFlags = KEYEVENTF_KEYUP;
			inputUp.ki.wVk = scanCode;
			SendInput( 1, &inputUp, sizeof(inputUp) );
		}

 void SendVKDown(unsigned short scanCode)
		{
		
			INPUT inputDown;
			inputDown.type = INPUT_KEYBOARD;
			inputDown.ki.dwFlags = 0;
			inputDown.ki.wVk = scanCode;
			SendInput( 1, &inputDown, sizeof(inputDown) );
		}
	
 void SendVKCode(unsigned short scanCode)
		{
			SendVKDown(scanCode);
			SendVKUp(scanCode);
		}
void SendKeyDown(char charCode)
		{
			SendVKDown((unsigned short)VkKeyScanA(charCode));
		}

 void SendKeyUp(char charCode)
		{
			SendVKUp((unsigned short)VkKeyScanA(charCode));
		}

	
