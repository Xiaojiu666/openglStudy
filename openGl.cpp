#include<gl/glut.h>

void renderWindow() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,0,0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-.5f,-.5f);
	glVertex2d(.5f,.5f);
	glVertex2f(.0f,.0f);
	glEnd();
	glFlush();
}

int main(int argc,char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowPosition(1, 1);
	glutInitWindowSize(720,720);
	glutCreateWindow("hello opengl");
	glutDisplayFunc(&renderWindow);
	glutMainLoop();
}