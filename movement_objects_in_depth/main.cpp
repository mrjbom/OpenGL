#include <iostream>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h> // for M_E and M_PI
#include <GL/glut.h>

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
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2, -windowDepth / 2, windowDepth / 2);
	/* пояснение по поводу оси Z, можно подумать, что near ортогональной системы должен быть положительным,
	   а far в ней, отрицательным, но т.к. в правосторонней системе(используемой в OpenGL) ось Z вылезает за экран(viewport),
	   то OpenGL её инвертирует, поэтому параметр near должен быть отрицательным, а far положительным */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST); //включаем глубину
	glDepthFunc(GL_LEQUAL); //устанавливаем функцию глубины
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//смещаем красный квадрат на передний план
	glTranslatef(-32.5, -32.5, 2);
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3i(-150, 150, 0);
	glVertex3i(150, 150, 0);
	glVertex3i(150, -150, 0);
	glVertex3i(-150, -150, 0);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	//перемещаем зелёный квадрат на задний план
	glTranslatef(32.5, 32.5, -2);
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3i(-150, 150, 0);
	glVertex3i(150, 150, 0);
	glVertex3i(150, -150, 0);
	glVertex3i(-150, -150, 0);
	glEnd();
	glPopMatrix();
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
