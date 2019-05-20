#include <iostream>
#include <GL/glut.h>
#include <ctime>

#define FPS_RATE 60

using namespace std;

void init();
void idleFunction();
void displayFunction();
void timerFunction(int number);

void init()
{
	glutDisplayFunc(displayFunction);
	glutIdleFunc(idleFunction);
	glutTimerFunc(1000 / FPS_RATE, timerFunction, 0);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-300, 300, -300, 300);
}

void idleFunction()
{
	//glutPostRedisplay();
}

void timerFunction(int number)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS_RATE, timerFunction, 0);
}

void displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	glPointSize(3.0);
	glBegin(GL_POINTS);
	//генерирование точек и их отрисовка
	for (unsigned int i = 0; i < 500; ++i)
	{
		bool sign = bool(rand() % 2);
		int x = 0 + rand() % 301;
		int y = 0 + rand() % 301;
		if (!sign)
			x = -x;
		sign = bool(rand() % 2);
		if (!sign)
			y = -y;
		glVertex2i(x, y);
	}
	glEnd();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 600) / 2, (GetSystemMetrics(SM_CYSCREEN) - 600) / 2);
	glutCreateWindow("Window");
	srand((unsigned)time(NULL));
	init();
	glutMainLoop();
	return 0;
}
