
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"

const double PI = 3.14;
const int H = 600;
const int W = 600;

double x = 0.0;
int slider_captured = 0;
double lever1 = 0.0;
double lever2 = 0.0;
double angle = 0.0;
double angle2 = 0.0;
int LeftHand = 1;
int RightHand = 2;

double speed = 0;
double angular_speed = 0;
double sight_angle = PI;
double dir[3] = { sin(sight_angle),-0.3,  cos(sight_angle) };
double eye[3] = { 0,18,15 }; // coordinates of eye


// light0 definition
float l1amb[4] = { 0.2,0.2,0.2,0 };
float l1diff[4] = { 0.7,0.7,0.7,0 };
float l1spec[4] = { 0.8,0.8,0.8,0 };
float l1pos[4] = { 0.7,1,0.4,0 };  // 0 means directional; 1 means positional

// light1 definition
float l2amb[4] = { 0.2,0.2,0.2,0 };
float l2diff[4] = { 0.9,0.7,0.5,0 };
float l2spec[4] = { 0.8,0.8,0.8,0 };
float l2pos[4] = { -0.7,-1,0.4,0 };  // 0 means directional; 1 means positional

// gold
float m1amb[4] = { 0.24,0.22,0.06,0 };
float m1diff[4] = { 0.55,0.51,0.1,0 };
float m1spec[4] = { 0.79,0.72,0.2,0 };

// silver
float m2amb[4] = { 0.3,0.3,0.3,0 };
float m2diff[4] = { 0.8,0.8,0.8,0 };
float m2spec[4] = { 0.9,0.9,0.9,0 };

// blue glass
float m3amb[4] = { 0.1,0.1,0.1,0.5 };
float m3diff[4] = { 0.3,0.4,0.6,0.5 };
float m3spec[4] = { 1,1,1,0.5 };


void init()
{
	glClearColor(0.7, 0.8, 1, 0);// color of window background

	srand(time(0));
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST); // uses Z-buffer

}

void DrawGround()
{
	int i;

	glColor3d(0, 0, 0.3);
	for (i = -50;i <= 50;i++) // lines parallel to Z
	{
		glBegin(GL_LINES);
		glVertex3d(i, 0, -50);
		glVertex3d(i, 0, 50);
		glEnd();
	}
	for (i = -50;i <= 50;i++) // lines parallel to X
	{
		glBegin(GL_LINES);
		glVertex3d(-50, 0, i);
		glVertex3d(50, 0, i);
		glEnd();
	}
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


void DrawLitCylinder(int n)
{
	double alpha, teta = 2 * PI / n;

	for (alpha = 0;alpha <= 2 * PI;alpha += teta)
	{

		glBegin(GL_POLYGON);
		glNormal3d(sin(alpha), 0, cos(alpha));
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

// Cylinder + Lighting
void DrawLitCylinder1(int n, double topR, double bottomR)
{
	double alpha, teta = 2 * PI / n;

	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{

		glBegin(GL_POLYGON);

		glNormal3d(sin(alpha),bottomR*(bottomR-topR), cos(alpha));
		glVertex3d(topR * sin(alpha), 1, topR * cos(alpha)); // upper left point

		glNormal3d(sin(alpha+teta), bottomR * (bottomR - topR), cos(alpha+teta));
		glVertex3d(topR * sin(alpha + teta), 1, topR * cos(alpha + teta)); // upper right point

		glNormal3d(sin(alpha + teta), bottomR * (bottomR - topR), cos(alpha + teta));
		glVertex3d(bottomR * sin(alpha + teta), 0, bottomR * cos(alpha + teta)); // bottom right point

		glNormal3d(sin(alpha), bottomR * (bottomR - topR), cos(alpha));
		glVertex3d(bottomR * sin(alpha), 0, bottomR * cos(alpha)); // bottom left point
		glEnd();
	}

}

void DrawSphere(int n, int slices)
{
	double beta, delta = PI / slices;

	for (beta = -PI / 2; beta <= PI / 2;beta += delta)
	{
		glPushMatrix();
		glTranslated(0, sin(beta), 0);
		glScaled(1, sin(beta+delta)-sin(beta), 1);
		DrawCylinder1(n, cos(beta + delta), cos(beta));
		glPopMatrix();
	}
}
void DrawHalfSphere(int n, int slices)
{
	double beta, delta = PI / slices;

	for (beta = 0; beta <= PI/2 ;beta += delta)
	{
		glPushMatrix();
		glTranslated(0, sin(beta), 0);
		glScaled(1, sin(beta + delta) - sin(beta), 1);
		DrawLitCylinder1(n, cos(beta + delta), cos(beta));
		glPopMatrix();
	}
}

void DrawLitSphere(int n, int slices)
{
	double beta, delta = PI / slices;
	int i;

	for (beta = -PI / 2, i = 0; beta <= PI / 2; beta += delta, i++)
	{
		glPushMatrix();
		glTranslated(0, sin(beta), 0);
		//		glRotated(i * offset*0.5, 0, 1, 0);
		glScaled(1, sin(beta + delta) - sin(beta), 1);
		DrawLitCylinder1(n, cos(beta + delta), cos(beta));
		glPopMatrix();
	}
}

void DrawLeg()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m2amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m2diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m2spec);
	glMaterialf(GL_FRONT, GL_SHININESS,80);


	glPushMatrix();
	glTranslated(0, 0, -0.5);
	glScaled(0.7,3.4,0.7);
	DrawLitCylinder1(27,0.8,1.4);
	glPopMatrix();

	glPushMatrix();
	glScaled(1.1, 1, 1.4);
	DrawHalfSphere(30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 3.5, -0.5);
	glScaled(0.7, 0.7, 0.7);
	DrawLitSphere(27, 27);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 3.7, -0.5);
	glScaled(0.5, 3.4, 0.5);
	DrawLitCylinder(40);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 7.2, -0.5);
	glScaled(0.7, 0.7, 0.7);
	DrawLitSphere(27, 27);
	glPopMatrix();
}

void DrawBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m2amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m2diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m2spec);
	glMaterialf(GL_FRONT, GL_SHININESS, 80);


	glColor3d(0.7, 0.7, 0.7);
	glPushMatrix();
	glTranslated(0, 7.2, -0.4);
	glScaled(0.7, 2.4, 0.4);
	DrawLitCylinder1(54, 4, 3);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0, 10, -0.4);
	glScaled(2.87, 1.7, 2);
	DrawLitSphere(40,40);
	glPopMatrix();



}

void DrawHead()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m2amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m2diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m2spec);
	glMaterialf(GL_FRONT, GL_SHININESS, 80);

	glColor3d(0.7, 0.7, 0.7);
	glPushMatrix();
	glTranslated(0,11.57,-0.4);
	glScaled(0.27, 1, 0.27);
	DrawLitCylinder(30);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 13.2, -0.4);
	glScaled(1.1, 1.2, 1.1);
	DrawLitSphere(40,40);
	glPopMatrix();
	 // eyes and ears
	glDisable(GL_LIGHTING);
	glColor3d(0, 0, 0);

	glPushMatrix();
	glTranslated(-0.5, 13.4, 0.62);
	glScaled(0.2, 0.2, 0.2);
	DrawLitSphere(40, 40);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5, 13.4, 0.62);
	glScaled(0.2, 0.2, 0.2);
	DrawLitSphere(40, 40);
	glPopMatrix();


	glPushMatrix();
	glTranslated(-0.77, 13.4, -0.27);
	glScaled(0.5, 0.5, 0.5);
	DrawSphere(40, 40);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.77, 13.4, -0.27);
	glScaled(0.5, 0.5, 0.5);
	DrawLitSphere(40, 40);
	glPopMatrix();


	glEnable(GL_LIGHTING);
}

void DrawArm(int hand)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m2amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m2diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m2spec);
	glMaterialf(GL_FRONT, GL_SHININESS, 80);


	glPushMatrix();
	glScaled(0.7, 3.4, 0.7);
	DrawLitCylinder1(27, 0.8, 1.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 3.5, 0);
	glScaled(0.7, 0.7, 0.7);
	DrawLitSphere(27, 27);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -0.5, 0);
	glScaled(1.25, 1.25, 1.25);
	DrawLitSphere(27, 27);
	glPopMatrix();
	
	if (lever2 != 0)
		angle2 = (lever2 + 1) * 55;
	


	glPushMatrix();
	glTranslated(0, 3.7, 0);
	if (hand == LeftHand)
		glRotated(-angle2, 0, 0, 1);
	else if (hand == RightHand)
		glRotated(angle2, 0, 0, 1);
	glPushMatrix();
	glScaled(0.5, 3.4, 0.5);
	DrawLitCylinder(40);
	glPopMatrix();
	//
	
	glPushMatrix();
	glTranslated(0, 3.7, 0);

	glScaled(0.7, 0.7, 0.7);
	DrawLitSphere(27, 27);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 4.5, 0);
	glScaled(0.2, 0.4, 0.2);
	DrawLitSphere(27, 27);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.2, 4.5, 0);
	glScaled(0.2, 0.4, 0.2);
	DrawLitSphere(27, 27);
	glPopMatrix();


	glPushMatrix();
	glTranslated(-0.2, 4.5, 0);
	glScaled(0.2, 0.4, 0.2);
	DrawLitSphere(27, 27);
	glPopMatrix();

	glPopMatrix();


}

void DrawRobot()
{
	glPushMatrix();
	glTranslated(-1.85, 0, 0);
	DrawLeg();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(1.85, 0, 0);
	DrawLeg();
	glPopMatrix();

	glPushMatrix();
	DrawBody();
	glPopMatrix();

	glPushMatrix();
	DrawHead();
	glPopMatrix();


	if (lever1 != 0)
		angle = (lever1 + 1) * 45;


	glPushMatrix();
	glTranslated(-2.7, 10.1, -0.4);
	glRotated(140 - angle , 0, 0, 1);
	glScaled(0.7, 0.7, 0.7);
	DrawArm(LeftHand);
	glPopMatrix();

	
	glPushMatrix();
	glTranslated(2.7, 10.1, -0.4);
	glRotated( -140 + angle, 0, 0, 1);
	glScaled(0.7, 0.7, 0.7);
	DrawArm(RightHand);
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
	glVertex2d(-0.2, lev + 0.2);
	glVertex2d(0.2, lev + 0.2);
	glVertex2d(0.2, lev - 0.2);
	glEnd();


	glColor3d(1, 1, 0);
	glBegin(GL_LINES);
	glVertex2d(-0.2, lev);
	glVertex2d(0.2, lev);
	glEnd();

	glLineWidth(1);

}


// all the drawings
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clean frame buffer and Z-buffer
	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // start transformations from IDENTITY
	glFrustum(-1, 1, -1, 1, 0.8, 300);

	gluLookAt(eye[0], eye[1], eye[2],  // eye coordinates
		eye[0]+dir[0], eye[1]+dir[1], eye[2]+dir[2],  // POI coordinates
		0,1, 0); // Vector UP


	glMatrixMode(GL_MODELVIEW); // defines matrix of transformations
	glLoadIdentity(); // start transformations from IDENTITY
	DrawGround();
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT0, GL_AMBIENT, l1amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l1diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l1spec);
	glLightfv(GL_LIGHT0, GL_POSITION, l1pos);

	glLightfv(GL_LIGHT1, GL_AMBIENT, l2amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l2diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l2spec);
	glLightfv(GL_LIGHT1, GL_POSITION, l2pos);



	glPushMatrix();
	//glRotated(x, 0, 1, 0);
	glScaled(1.5, 1.5, 1.5);
	DrawRobot();
	glPopMatrix();

	glDisable(GL_LIGHTING);
	
	// Draw slider
	glViewport(0, 0, 50, 100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // start transformations from IDENTITY
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW); // defines matrix of transformations
	glLoadIdentity(); // start transformations from IDENTITY
	glDisable(GL_DEPTH_TEST);

	DrawSlider(lever1);

	glViewport(W - 50, 0, 50, 100);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // start transformations from IDENTITY
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW); // defines matrix of transformations
	glLoadIdentity(); // start transformations from IDENTITY
	glDisable(GL_DEPTH_TEST);

	DrawSlider(lever2);


	glEnable(GL_DEPTH_TEST);
	glutSwapBuffers(); // show all
}

void MouseDrag(int x, int y)
{
	int yy = H - y - 1;
	double nl;

	if (slider_captured == 1)
	{
		int slider_y_start = 0;
		int slider_hight = 100;
		nl = 2.0 * (yy - slider_y_start) / slider_hight - 1.0;
		if (nl >= -1 && nl <= 1)
			lever1 = nl;
	}

	if (slider_captured == 2)
	{
		int slider_y_start = 0;
		int slider_hight = 100;
		nl = 2.0 * (yy - slider_y_start) / slider_hight - 1.0;
		if (nl >= -1 && nl <= 1)
			lever2 = nl;
	}
}

void MouseClick(int button, int state, int x, int y)
{
	int yy = H - y - 1;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//lever1
		int left1 = 15;
		int right1 = 35;
		int midY1 = 100 * (lever1 + 1) / 2.0;
		int bottom1 = midY1 - 20;
		int top1 = midY1 + 20;

		//lever2
		int left2 = W - 50 + 15;
		int right2 = W - 50 + 35;
		int midY2 = 100 * (lever1 + 1) / 2.0;
		int bottom2 = midY1 - 20;
		int top2 = midY1 + 20;

		if (left1 < x && x < right1 && bottom1 < yy && yy < top1)
		{
			slider_captured = 1;
		}
		else if (left2 < x && x < right2 && bottom2 < yy && yy < top2)
		{
			slider_captured = 2;
		}
		else
		{
			slider_captured = 0;
		}

	}
}
// all changes must be here
void idle() 
{
	int i, j;

	x += 1;
	// locomotion or ego motion
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
	glutCreateWindow("Exam");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseDrag);
	glutSpecialFunc(SpecialKeys);
	

	init();

	glutMainLoop();
}