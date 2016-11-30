// Spring.cpp : Defines the entry point for the console application.
//

#include <windows.h> 
#include <GL/gl.h> 
#include <GL/glut.h> 
#include <stdlib.h>
#include "imageloader.h"
#include <math.h>

#define BLACK_BACKGROUND glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
#define BALL_SIZE 0.5f
#define CAMERA_START_POSITION glTranslatef(0, 0, -20.0f);

GLUquadric *quad;
GLuint steelTexture;

void display(void);
void reshape(int x, int y);



GLuint loadTexture(Image* image) {

	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
											 //Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,                            //The border of the image
		GL_RGB, GL_UNSIGNED_BYTE, image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void initGL(void) {

	Image *steelImage = loadBMP("steel.bmp");
	steelTexture = loadTexture(steelImage);

	BLACK_BACKGROUND
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Spring");

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	initGL();                       // Our own OpenGL initialization

	glutMainLoop();
	return 0;
}

void drawSpring() {
	
	GLfloat PI = 3.14159265359f;

	GLfloat t = 0.0f;
	GLfloat x, y, z;
	//glTranslatef(0, 0, 0);

	for (t = 0.0f; t <= 8 * PI; t += (PI/200)){

		x = cos(t) * (3 + cos(0));
		z = sin(t) * (3 + cos(0));
		y = 0.6 * t + sin(0);

		glPushMatrix();
			glTranslatef(x, y, z);
			gluSphere(quad, BALL_SIZE, 20, 20);
		glPopMatrix();

	}



}

void display(void)
{
	glEnable(GL_TEXTURE_2D);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the color buffer with current clearing color
	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
	glLoadIdentity();                // Reset the model-view matrix


	CAMERA_START_POSITION

	quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, steelTexture);

	drawSpring();





	glFlush();// Flush buffers to screen

}

void reshape(int x, int y)
{
	if (y == 0 || x == 0) return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLdouble)x / (GLdouble)y, 0.6, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);  //Use the whole window for rendering
}