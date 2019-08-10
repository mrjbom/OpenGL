#include <iostream>
#include <chrono>
#include <GL/glut.h>
#include <soil.h>
#include "camera.h"

using namespace std;

constexpr auto FPS_RATE = 120;
int windowHeight = 600, windowWidth = 1000, windowDepth = 600;
struct MyPoint3f
{
	float x;
	float y;
	float z;
};
MyPoint3f lastMousePos = { };
bool mouseButtonWasPressed = false;
float mouseSensitivity = 0.00125;
int camMoveSpeed = 1;
float camPitchAngle = 0, camYawAngle = 0;
float currentCamPitchAngle = 0, currentCamYawAngle = 0;
Camera cam;

int imageWidth = 0, imageHeight = 0;
unsigned char* image = 0;
GLuint texture;

void init();
void displayFunction();
void idleFunction();
void reshapeFunction(int, int);
void keyboardFunction(unsigned char, int, int);
void specialKeysFunction(int, int, int);
void mouseFunc(int, int, int, int);
void motionFunction(int, int);
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
	image = SOIL_load_image("treeTexture.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
	glGenTextures(0, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glutDisplayFunc(displayFunction);
	glutIdleFunc(idleFunction);
	glutReshapeFunc(reshapeFunction);
	glutKeyboardFunc(keyboardFunction);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunction);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2, -windowDepth / 2, windowDepth / 2);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cam.SetPosition(glm::vec3(0, 300, 350));
	cam.SetLookAt(glm::vec3(0, 300, 349));
	cam.SetClipping(.1, 2000);
	cam.SetFOV(45);
}

void displayFunction()
{
	if (abs(currentCamPitchAngle + -(camPitchAngle * mouseSensitivity)) < 0.90)
		cam.ChangePitch(-(camPitchAngle * mouseSensitivity));
	cam.ChangeHeading((camYawAngle * mouseSensitivity));
	if (abs(currentCamPitchAngle + -(camPitchAngle * mouseSensitivity)) < 0.90)
		currentCamPitchAngle += -(camPitchAngle * mouseSensitivity);
	currentCamYawAngle += -(camYawAngle * mouseSensitivity);
	camPitchAngle = 0; camYawAngle = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model, view, projection;
	cam.Update();
	cam.GetMatricies(projection, view, model);
	glm::mat4 mvp = projection * view * model;
	glLoadMatrixf(glm::value_ptr(mvp));
	glMatrixMode(GL_MODELVIEW);

	//grid
	///*
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	for (int i = -1000; i <= 1000; i += 25)
	{
		glVertex3i(i, 0, -1000);
		glVertex3i(i, 0, 1000);
	}
	for (int i = -1000; i <= 1000; i += 25)
	{
		glVertex3i(-1000, 0, i);
		glVertex3i(1000, 0, i);
	}
	glEnd();
	//*/



	/*
	glPushMatrix();
	glTranslatef(0, 300, 0);
	//glutWireTorus(100, 150, 100, 100);
	glutSolidCube(150);
	glPopMatrix();
	*/

	///*
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glTexCoord2f(0, 1);
	glVertex3f(-150, 150, 0);
	glTexCoord2f(1, 1);
	glVertex3f(150, 150, 0);
	glTexCoord2f(1, 0);
	glVertex3f(150, 450, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-150, 450, 0);
	glEnd();
	//*/

	if (GetAsyncKeyState(VK_LSHIFT))
	{
		for (int z = 0; z < camMoveSpeed; ++z)
			cam.Move(UP);
	}
	if (GetAsyncKeyState(VK_LCONTROL))
	{
		for (int z = 0; z < camMoveSpeed; ++z)
			cam.Move(DOWN);
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
	}
	if (GetAsyncKeyState(VK_UP))
	{
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
	}

	glutSwapBuffers();
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

void reshapeFunction(int w, int h)
{
	cam.SetViewport(0, 0, windowWidth, windowHeight);
}

void keyboardFunction(unsigned char key, int w, int h)
{
	switch (key)
	{
	case '+': case '=':
		break;
	case '-': case '_':
		break;
	case 'w': case 'W':
		for (int z = 0; z < camMoveSpeed; ++z)
			cam.Move(FORWARD);
		break;
	case 'a': case 'A':
		for (int z = 0; z < camMoveSpeed; ++z)
			cam.Move(LEFT);
		break;
	case 's': case 'S':
		for (int z = 0; z < camMoveSpeed; ++z)
			cam.Move(BACK);
		break;
	case 'd': case 'D':
		for (int z = 0; z < camMoveSpeed; ++z)
			cam.Move(RIGHT);
		break;
	case 'q': case 'Q':
		for (int z = 0; z < camMoveSpeed; ++z)
			cam.Move(DOWN);
		break;
	case 'e': case 'E':
		for (int z = 0; z < camMoveSpeed; ++z)
			cam.Move(UP);
		break;
	case 27:
		currentCamPitchAngle = 0;
		currentCamYawAngle = 0;
		break;
	default:
		cout << key << endl;
		break;
	}
}

void specialKeysFunction(int key, int x, int y)
{
	cout << key << endl;
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseButtonWasPressed = true;
		lastMousePos.x = x;
		lastMousePos.y = y;
	}
}

void motionFunction(int mousePosX, int mousePosY)
{
	if (mousePosX >= 0 && mousePosX < windowWidth && mousePosY >= 0 && mousePosY < windowHeight)
	{
		if (mouseButtonWasPressed)
		{
			camPitchAngle += -mousePosY + lastMousePos.y;
			camYawAngle += mousePosX - lastMousePos.x;
			lastMousePos.x = mousePosX;
			lastMousePos.y = mousePosY;
		}
	}
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
