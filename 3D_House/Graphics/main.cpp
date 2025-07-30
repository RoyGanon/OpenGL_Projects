
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"

const double PI = 3.14;
const int GSZ = 100;

double ground[GSZ][GSZ] = {0};

double speed = 0;
double angular_speed = 0;
double sight_angle = PI;
double dir[3] = { sin(sight_angle),-0.3,  cos(sight_angle) };
double eye[3] = {7,18,25 }; // coordinates of eye
double offset = 0;
const int H = 600;
const int W = 600;
double lever = 0.0;
double lever2 = 0.0;

int slider_captured = 0;


void init()
{//                    R   G    B
	glClearColor(0.2,0.7,1,0);// color of window background
	glEnable(GL_DEPTH_TEST); 
}

void DrawGround()
{
	int i,j;
	glColor3d(0.4, 0.7, 0.4);
	glBegin(GL_POLYGON);
	glVertex3d(-GSZ / 2, 0, -GSZ / 2);
	glVertex3d(-GSZ / 2, 0, GSZ / 2);
	glVertex3d(GSZ / 2, 0, GSZ / 2);
	glVertex3d(GSZ / 2, 0, -GSZ / 2);
	glEnd();

}


void DrawCylinder(int n)
{
	double alpha, teta = 2 * PI / n;

	for (alpha = 0;alpha <= 2 * PI;alpha += teta)
	{
		
		glBegin(GL_POLYGON);
		glVertex3d(sin(alpha), 1, cos(alpha)); // upper left point
		glVertex3d(sin(alpha + teta), 1, cos(alpha + teta)); // upper right point
		glVertex3d(sin(alpha + teta), 0, cos(alpha + teta)); // bottom right point
		glVertex3d(sin(alpha), 0, cos(alpha)); // bottom left point
		glEnd();
	}

}


void DrawCylinder1(int n,double topR, double bottomR)
{
	double alpha, teta = 2 * PI / n;

	for (alpha = 0;alpha <= 2 * PI;alpha += teta)
	{

		glBegin(GL_POLYGON);

		glVertex3d(topR*sin(alpha), 1, topR*cos(alpha)); // upper left point
		glVertex3d(topR * sin(alpha + teta), 1, topR * cos(alpha + teta)); // upper right point
		glVertex3d(bottomR*sin(alpha + teta), 0, bottomR * cos(alpha + teta)); // bottom right point
		glVertex3d(bottomR * sin(alpha), 0, bottomR * cos(alpha)); // bottom left point
		glEnd();
	}

}

void DrawFloor(int numOfWindows)
{
	glColor3d(1.0, 0.85, 0.7);
	glPushMatrix();
	glScaled(1, 0.3, 1);
	DrawCylinder(4);
	glPopMatrix();

	/////////

	double alpha, teta = 2 * PI / 4;
	int i;

	for (alpha = 0;alpha <= 2 * PI;alpha += teta)
	{
		for (i = 0;i <= 2 * numOfWindows + 1;i += 2)
		{
			double ratio1 = (double)i / (2 * numOfWindows + 1);
			double ratio2 = (double)(i + 1) / (2 * numOfWindows + 1);

			double x1 = (1 - ratio1) * sin(alpha) + ratio1 * sin(alpha + teta);
			double z1 = (1 - ratio1) * cos(alpha) + ratio1 * cos(alpha + teta);

			double x2 = (1 - ratio2) * sin(alpha) + ratio2 * sin(alpha + teta);
			double z2 = (1 - ratio2) * cos(alpha) + ratio2 * cos(alpha + teta);

			glBegin(GL_POLYGON);
			glColor3d(0.8, 0.7, 0.5);
			glVertex3d(x1, 0.7, z1);
			glVertex3d(x2, 0.7, z2);
			glVertex3d(x2, 0.3, z2);
			glVertex3d(x1, 0.3, z1);
			glEnd();
		}
	}
	glPushMatrix();
	glTranslated(0, 0.7, 0);
	glColor3d(1.0, 0.85, 0.7);
	glScaled(1, 0.3, 1);
	DrawCylinder(4);
	glPopMatrix();

}
void DrawHouse(int numOfWindows , int numOfStores)
{
	
	for (int i = 0; i < numOfStores; i++)
	{
		glPushMatrix();
		glTranslated(0, i, 0);
		DrawFloor(numOfWindows);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, i, 0);
		glColor3d(0.1, 0.1, 0.5);
		glScaled(0.8, 1, 0.8);
		DrawCylinder(4);
		glPopMatrix();
	}
	glColor3d(0.6, 0.2, 0.2);
	glPushMatrix();
	glTranslated(0, numOfStores, 0);
	glScaled(1, 1.7, 1);
	DrawCylinder1(4, 0, 1.1);
	glPopMatrix();


}
void DrawSlider(double lev)
{
	glColor3d(1, 1, 0);
	// Background
	glBegin(GL_POLYGON);
	glVertex2d(-1, -1);
	glVertex2d(-1, 1);
	glVertex2d(1, 1);
	glVertex2d(1, -1);
	glEnd();

	// slider
	glLineWidth(2);
	glColor3d(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(0, -1);
	glVertex2d(0, 1);
	glEnd();

	glColor3d(0.4, 0.2, 0);
	glBegin(GL_POLYGON);
	glVertex2d(-0.2, lev - 0.2);
	glVertex2d(-0.2,lev + 0.2);
	glVertex2d(0.2, lev + 0.2);
	glVertex2d(0.2, lev - 0.2);
	glEnd();


	glColor3d(1, 1, 0);
	glBegin(GL_LINES);
	glVertex2d(-0.2,lev + 0);
	glVertex2d(0.2, lev + 0);
	glEnd();

	glLineWidth(1);

}
// all the drawings
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clean frame buffer and Z-buffer
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // start transformations from IDENTITY
	glFrustum(-1, 1, -1, 1, 0.8, 300);

	gluLookAt(eye[0], eye[1], eye[2],  // eye coordinates
		eye[0]+dir[0], eye[1]+dir[1], eye[2]+dir[2],  // POI coordinates
		0,1, 0); // Vector UP


	glMatrixMode(GL_MODELVIEW); // defines matrix of transformations
	glLoadIdentity(); // start transformations from IDENTITY

	DrawGround();
	glPushMatrix();
	glRotated(offset, 0, 1, 0);
	glScaled(7, 7, 7);
	int numWindows = (int)((lever + 1.0) * 0.5 * 9.0) + 1;
	int numFloors = (int)((lever2 + 1.0) * 0.5 * 4.0) + 1; 
	DrawHouse(numWindows,numFloors);
	glPopMatrix();

	 // Backround slider
	glViewport(0.7 * W, 0, 0.3* W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // start transformations from IDENTITY
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW); // defines matrix of transformations
	glLoadIdentity(); // start transformations from IDENTITY
	glColor3d(0.57, 0.57, 0.57);
	glBegin(GL_POLYGON);
	glVertex2d(-1, -1); glVertex2d(-1, 1);
	glVertex2d(1, 1); glVertex2d(1, -1);
	glEnd();
	
	// Draw slider1
	glViewport(0.75 *W, 0.7 * H, 100, 100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // start transformations from IDENTITY
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW); // defines matrix of transformations
	glLoadIdentity(); // start transformations from IDENTITY
	glDisable(GL_DEPTH_TEST);

	DrawSlider(lever);
	

	// Draw slider2
	glViewport(0.75 * W, 0.25 * H, 100, 100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // start transformations from IDENTITY
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW); // defines matrix of transformations
	glLoadIdentity(); // start transformations from IDENTITY
	glDisable(GL_DEPTH_TEST);
	DrawSlider(lever2);

	glViewport(0, 0, 600, 600);
	glEnable(GL_DEPTH_TEST);
	glutSwapBuffers(); // show all
}
void MouseClick(int button, int state, int x, int y) {
	int yy = H - y - 1;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// lever1
		int sx1 = 0.75 * W , sy1 = 0.7 * H;
		int left1 = sx1 + 30, right1 = sx1 + 70;
		int midY1 = sy1 + 100*(lever + 1) / 2.0; // lever->pixel
		int bottom1 = midY1 - 20, top1 = midY1 + 20;
		

		// lever2
		int sx2 = 0.75 * W, sy2 = 0.25 * H;
		int left2 = sx2 + 30, right2 = sx2 + 70;
		int midY2 = sy2 + 100*(lever2 + 1) /2.0;
		int bottom2 = midY2 - 20, top2 = midY2 + 20;

		if (left1 < x && x < right1 && bottom1 < yy && yy < top1)
			slider_captured = 1;
		else if (left2 < x && x < right2 && bottom2 < yy && yy < top2)
			slider_captured = 2;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		slider_captured = 0;
	}
}

void MouseDrag(int x, int y) {
	int yy = H - y - 1; //y -  pos of the mouse from bottom to top // transfer from botom to top -> top to bottom
	double nl;

	if (slider_captured == 1) {
		nl = 2.0 * (yy - 0.7 * H) / 100.0 - 1.0;
		if (nl >= -1 && nl <= 1) lever = nl;
	}
	else if (slider_captured == 2) {
		nl = 2.0 * (yy - 0.25 * H) / 100.0 - 1.0;
		if (nl >= -1 && nl <= 1) lever2 = nl;
	}
}
// all changes must be here
void idle() 
{
	int i, j;

	double dist;
	offset += 0.01;
	sight_angle += angular_speed;
	dir[0] = sin(sight_angle);
	dir[1] = -0.3;
	dir[2] = cos(sight_angle);

	eye[0] += speed * dir[0]; // x
	eye[1] += 0;// y
	eye[2] += speed * dir[2];  // z

	glutPostRedisplay(); // indirect call to display
}

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		speed -= 0.01;
		break;
	case GLUT_KEY_UP:
		speed += 0.01;
		break;
	case GLUT_KEY_LEFT:
		angular_speed += 0.001;
		break;
	case GLUT_KEY_RIGHT:
		angular_speed -= 0.001;
		break;
	case GLUT_KEY_PAGE_UP:
		eye[1] += 0.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		eye[1] -= 0.1;
		break;
	}
}


void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE|GLUT_DEPTH); // defines memory for each pixel in window and second buffer 
	glutInitWindowSize(W, H);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("3D House");

	glutDisplayFunc(display);
	glutIdleFunc(idle);


	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
	glutSpecialFunc(SpecialKeys);

	init();

	glutMainLoop();
}