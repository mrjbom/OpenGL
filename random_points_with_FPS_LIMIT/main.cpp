#include <iostream>
#include <chrono>
#include <GL/glut.h>
#include <ctime>

constexpr auto FPS_RATE = 60;
int windowHeight = 600, windowWidth = 600;

using namespace std;

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
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-300, 300, -300, 300);
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
	glColor3f(0, 0, 0);
	glPointSize(3.0);
	glBegin(GL_POINTS);
	//генерация точек и их отрисовка
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
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - windowHeight) / 2, (GetSystemMetrics(SM_CYSCREEN) - windowWidth) / 2);
	glutCreateWindow("Window");
	srand((unsigned)time(NULL));
	init();
	glutMainLoop();
	return 0;
}
