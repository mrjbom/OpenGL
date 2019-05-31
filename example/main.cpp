#include <iostream>
#include <chrono>
#include <GL/glut.h>

using namespace std;

constexpr auto FPS_RATE = 60;
int windowHeight = 600, windowWidth = 600;

void init(); //функция инициализации
void displayFunction(); //функция отображения
void displayFunction(); //функция ожидания
double getTime(); //возвращает время

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
	glutDisplayFunc(displayFunction); //устанавливаем функцию отображения
	glutIdleFunc(idleFunction); //устанавливаем функцию ожидания
	glClearColor(0.0, 0.0, 0.0, 0.0); //белый цвет для фона
	glMatrixMode(GL_PROJECTION); //устанавливаем матрицу для проецирования 3D пространства или его участков в 2D!!!
	glLoadIdentity(); //делаем текущую матрицу единичной
	gluOrtho2D(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2); //устанавливаем ортогональную проекцию
}

void idleFunction()
{
	const double current_time = getTime();
	if ((current_time - last_render) > frame_delay)
	{
		last_render = current_time;
		glutPostRedisplay(); //обновляем экран
	}
}

void displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT); //очищаем буффер цвета
	glColor3f(1.0, 1.0, 1.0); //устанавливаем чёрный цвет
	/* рисуем */
	glutSwapBuffers(); //свапаем буффер
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); //инициализируем glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //включаем двойной буффер, RGB, буффер глубины
	glutInitWindowSize(windowWidth, windowHeight); //устанавливаем размеры
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 600) / 2, (GetSystemMetrics(SM_CYSCREEN) - 600) / 2); //позиция
	glutCreateWindow("Window"); //создаём окно
	init(); //инициализируем остальное
	glutMainLoop(); //запускаем Main
	return 0;
}
