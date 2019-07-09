#include <iostream>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h> // for M_E and M_PI
#include <GL/glut.h>
#include "supportFunctions.h"

using namespace std;

constexpr auto FPS_RATE = 120;
int windowHeight = 600, windowWidth = 600, windowDepth = 600;

void init();
void idleFunction();
void displayFunction();
double getTime();

double getTime()
{
	using Duration = std::chrono::duration<double>;
	return std::chrono::duration_cast<Duration>(
		std::chrono::high_resolution_clock::now().time_since_epoch()
		).count();
}

const double frame_delay = 1.0 / FPS_RATE;
double last_render = 0;

void init()
{
	glutDisplayFunc(displayFunction);
	glutIdleFunc(idleFunction);
	setOrtho2D(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2);
	setViewport(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void idleFunction()
{
	const double current_time = getTime();
	if ((current_time - last_render) > frame_delay)
	{
		last_render = current_time;
		glutPostRedisplay();
	}
}

void displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix(); //сохраняем матрицу в стек
	glTranslatef(-75, -75, 0); //производим манипуляции, поворот, маштабирование или перемещение
	// описываем зелёный квадрат, над ним будут производится манипуляции
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2i(-75, 75);
	glVertex2i(75, 75);
	glVertex2i(75, -75);
	glVertex2i(-75, -75);
	glEnd();
	glPopMatrix(); //восстанавливаем матрицу
	//красный квадрат
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2i(-75, 75);
	glVertex2i(75, 75);
	glVertex2i(75, -75);
	glVertex2i(-75, -75);
	glEnd();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2);
	glutCreateWindow("Window");
	init();
	glutMainLoop();
	return 0;
}
