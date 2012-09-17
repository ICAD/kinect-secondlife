#include <XnCppWrapper.h>
#include <math.h>
class MouseControl
{
private:

	int avgX[20];
	int avgY[20];
	int avgZ[20];
	int PreZ;
	int PreX;
	int PreY;
    int tot;//=0;
	int tot2;//=0;
	int stater;//=0; //right hand satus
	int state;
public:
	MouseControl( )
	{
		tot=0;
		tot2=0;
		state=0;
		stater=0;
	}
int distance(double x1,double x2,double y1,double y2,double z1,double z2,int dimention)
	{
	
	double bd;

	if(dimention==2)
	{
		bd=(pow((x1-x2),2)+pow((y1-y2),2));
		
	}
	else if(dimention==3)
	{
		bd=(pow((x1-x2),2)+pow((y1-y2),2)+pow((z1-z2),2));
	}
	else;
	return(sqrt(bd));

}
void  Sendposition (XnUserID Player, int  id,XnSkeletonJoint Part , char  _Part [2],xn::DepthGenerator g_DepthGenerator,xn::UserGenerator g_UserGenerator )
{
	int static i,j;
	const XnDepthPixel* pDepthMap = g_DepthGenerator.GetDepthMap();


 if  (!g_UserGenerator.GetSkeletonCap().IsTracking(Player))
 {
  printf ( "not tracked! \n" );
  return ;
 }
 XnSkeletonJointPosition joint1;
 g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(Player, Part, joint1);
 XnPoint3D pt [1];
 if (joint1.fConfidence < 0.5 )
	{
		return;
	}

 pt [0] = joint1.position;
 
 g_DepthGenerator.ConvertRealWorldToProjective(1, pt, pt);

 //Scene(pt[0].X,pt[0].Y,Player);
 if(_Part[0]=='L')
 {
	 
    

	int NewZ=pt[0].Z;
	int NewX=pt[0].X;
	int NewY=pt[0].Y;
	//int kj=(int) pDepthMap[(((int)pt[0].Y) * 640) +((int)pt[0].X)];
	//cout << (int) pDepthMap[(((int)pt[0].Y) * 640) +((int)pt[0].X)] << " =?= " << ((int)pt[0].Z) << endl; 
	
	//printf ("%s (Z)%d =?= %d ",_Part,(int)pt[0].Z,kj);
	//printf ("%s (Z)% f\n",_Part,pt[0].Z);
	//printf ("%s (X)% f\n",_Part,pt[0].X);
    //printf ("%s (Y)% f\n",_Part,pt[0].Y);
	//printf ("\t%d\t%d\t%d\n",(int)pt[0].X,(int)pt[0].Y,(int)pt[0].Z);
	
	int dZ = NewZ - PreZ;
	int dX = NewX - PreX;
	int dY = NewY - PreY;
	int max=0;
	int min=0;
    static int f1=0;
	int f2=2;
	int static ZX;
	int static ZY;
	int static ZZ;
	int static flag=0;
	int static flag2=0;
	PreZ = NewZ;
	PreX = NewX;
	PreY = NewY;
	avgX[i]=pt[0].X;
	avgY[i]=pt[0].Y;
	avgZ[i]=pt[0].Z;
	if(dZ!=0 )		
	{
	if(i==20)
	{
		
		
		tot=avgZ[0]-avgZ[20];
		//printf("\
		
		i=-1;
		tot=0;
	}
	i++;
	}
	dZ=abs(dZ);
	dY=abs(dY);
	dX=abs(dX);
	if(dZ<=1 && dY<=1 && dX<=1)
	{
		//printf("Steady state\n");
		ZX=pt[0].X;
		ZY=pt[0].Y;
		ZZ=pt[0].Z;

	}
	else
	{
		tot2=((int) pDepthMap[(((int)ZY) * 640) +((int)ZX)])-ZZ;
		//printf("\nthis the prev Z = %d = ? = %d\n",(int) pDepthMap[(((int)ZY) * 640) +((int)ZX)],ZZ);
	}
	
	if((tot2>=2000))
	{
		
		flag=1;
		
	}
		
	if(tot2<=200 && tot2>=-200 )
	{
		flag2=1;
		
	}
	if(flag2==1 && flag==1)
	{
		
		if(f1==0)
		{
			f1=1;
			//printf("\nf1 was 0  \n");
			//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		}
		else if(f1==1)
		{
			//f2++;
			//printf("\nyesssssssssssssss %d \n",f2);
			
			if(f2==0)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				f2=1;
				
			}
			else if(f2==1)
			{
				mouse_event(MOUSEEVENTF_LEFTUP,  0, 0,0, 0);
				f2=0;
			}

			//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			//mouse_event(MOUSEEVENTF_LEFTUP,  0, 0,0, 0);
			f1=0;

		}
		flag2=0; flag=0;
	}
	//printf("dZ = %d \n",dZ);
	/*if(!(dY==0 && dX==0))
	{
		if(j==5)
		{
			tot2=avgY[0]-avgY[5];
			//min=minValue(avgY);
			//max=maxValue(avgY);
			tot2=abs(tot2);
			//tot2=max-min;
			if(tot2>=12 && tot2<=30 )
			{
				printf("\nyesssssssssssssss %d \n",tot2);
				
			}
			j=-1;
			tot2=0;
		}
	}
	j++;
	//tot=tot+dZ;*/
	
	
	
	
 }
 //printf ("%s (X)% f\n",_Part,pt[0].X);
 //printf ("%s (Y)% f\n",_Part,pt[0].Y);
 //printf ("%s (Z)% f\n",_Part,pt[0].Z);

 if(_Part[0]=='R'  )//&& (state==0 && stater==0 || state==1 && stater==0)
 ProcessNewHandPos(pt[0].X,pt[0].Y);
 
}
void  Sendposition1(XnUserID Player, int  id,XnSkeletonJoint Part ,XnSkeletonJoint Part1,XnSkeletonJoint Part2,xn::DepthGenerator g_DepthGenerator,xn::UserGenerator g_UserGenerator)
{
	
	const XnDepthPixel* pDepthMap = g_DepthGenerator.GetDepthMap();


 if  (!g_UserGenerator.GetSkeletonCap().IsTracking(Player))
 {
  printf ( "not tracked! \n" );
  return ;
 }
 XnSkeletonJointPosition joint,joint1,joint2;
 g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(Player, Part,  joint);
 g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(Player, Part1, joint1);
 g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(Player, Part2, joint2);
 XnPoint3D pt [3];
 if (joint1.fConfidence < 0.5 )
	{
		return;
	}

 pt [0] = joint.position;
 pt [1]= joint1.position;
 pt [2]= joint2.position;
 printf("\n%d\n",Part2);
 g_DepthGenerator.ConvertProjectiveToRealWorld(3,pt,pt);
 //printf("\n Real X = %d Y = %d Z = %d ",(int)pt[2].X,(int)pt[2].Y,(int)pt[2].Z) ; 
 int d1=distance(pt[0].X,pt[1].X,pt[0].Y,pt[1].Y,pt[0].Z,pt[1].Z,3);
 int d2=distance(pt[1].X,pt[2].X,pt[1].Y,pt[2].Y,pt[1].Z,pt[2].Z,3);
 int D=d1+d2;
 int dshtoh=distance(pt[0].X,pt[2].X,pt[0].Y,pt[2].Y,pt[0].Z,pt[2].Z,3);
 //printf(" \n D         = %d ",D);
 //printf(" \n D sh to h = %d ",dshtoh);
 if(dshtoh <=D && dshtoh >= (D-300) && Part2 == 9 )
 {
	state=1;
	
 }
 else if(dshtoh <=D && dshtoh >= (D-300) && Part2 == 15)
 {
    stater=1;
 }
 
 //int d1=ditance(
 
 
}
void ProcessNewHandPos(int NewX, int NewY)
{
	POINT CurrentMousePos; 
	GetCursorPos(&CurrentMousePos);

	static double SubPixelCarryoverX = 0, SubPixelCarryoverY=0 ;
	static double PrevX = (double)NewX, PrevY = (double)NewY; 
	double dx = (double)NewX - PrevX;
	double dy = (double)NewY - PrevY;
	double v;

	v = sqrt(dx*dx + dy*dy );
	v = 1.0*v + 0.09*v*v; //acceleration here.
	//printf("v:%lf\n", v);

	//double NewMousePosX = CurrentMousePos.x + SubPixelCarryoverX + dx*v;
	//double NewMousePosY = CurrentMousePos.y + SubPixelCarryoverY + dy*v;
	double NewMousePosX = CurrentMousePos.x + SubPixelCarryoverX + (dx*v)+(dx*v)/2;
	double NewMousePosY = CurrentMousePos.y + SubPixelCarryoverY + (dy*v)+(dy*v)/2;

	SetCursorPos((int)NewMousePosX, (int)NewMousePosY);
	
	SubPixelCarryoverX = ( dx*v - (int)dx*v );
	SubPixelCarryoverY = ( dy*v - (int)dy*v );
	
	PrevX = NewX;
	PrevY = NewY;
}

};