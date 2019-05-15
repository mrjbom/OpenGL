#include <iostream>
#include <conio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void init(); //функция инициализации
void displayFunction(); //функция отображения

void init()
{
	glutDisplayFunc(displayFunction); //устанавливаем функцию отображения
	glClearColor(1.0, 1.0, 1.0, 0.0); //белый цвет для фона
	glMatrixMode(GL_PROJECTION); //устанавливаем матрицу для проецирования 3D пространства или его участков в 2D!!!
	glLoadIdentity(); //делаем текущую матрицу единичной
	gluOrtho2D(-300, 300, -300, 300); //устанавливаем ортогональную проекцию
}

void displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT); //очищаем буффер цвета
	glColor3f(0.0, 0.0, 0.0); //устанавливаем чёрный цвет
	glBegin(GL_POLYGON); //рисуем полигон
	glVertex2f(150, -150); //устанавливаем вершины
	glVertex2f(150, 150);
	glVertex2f(-150, 150);
	glVertex2f(-150, -150);
	glEnd(); //закончили рисовать
	glutSwapBuffers(); //свапаем буффер
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); //инициализируем glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //включаем двойной буффер, RGB, буффер глубины
	glutInitWindowSize(600, 600); //устанавливаем размеры
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 600) / 2, (GetSystemMetrics(SM_CYSCREEN) - 600) / 2); //позиция
	glutCreateWindow("Window"); //создаём окно
	init(); //инициализируем остальное
	glutMainLoop(); //запускаем Main
	return 0;
}
