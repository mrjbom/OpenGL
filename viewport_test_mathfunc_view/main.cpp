// Translate world window to view window
// The world window has a size(orthogonal projection) from -16 to 16 by x and -2 to 2 by y(the graph ranges from -1 to 1) this size is optimal for displaying it in the middle
// The viewport is the size of the window in pixels
#include <iostream>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h> //for M_E and M_PI
#include <GL/glut.h>

using namespace std;

constexpr auto FPS_RATE = 60;
int windowHeight = 600, windowWidth = 600, windowDepth = 600;

float yBiggest = 0;
float xBiggest = 0;

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
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2, -windowDepth / 2, windowDepth / 2);
	gluOrtho2D(-16, 16, -2, 2);
	glViewport(0, 0, windowWidth, windowHeight);
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
	glBegin(GL_LINE_STRIP);

	for (float x = -16; x <= 16; x += 0.05)
	{
		float y = sin(x);
		glVertex2f(x, y);
	}

	glEnd();
	glDisable(GL_POLYGON_STIPPLE);
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2);
	glutCreateWindow("Window");
	init();
	glutMainLoop();
	return 0;
}
