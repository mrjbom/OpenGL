#include <iostream>
#include <chrono>
#include <GL/glut.h>
#include "camera.h"

using namespace std;

constexpr auto FPS_RATE = 120;
int windowHeight = 600, windowWidth = 600, windowDepth = 600;
float angle = 0, speedRatio = 0.0;
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

	cam.SetPosition(glm::vec3(0, 0, 350));
	cam.SetLookAt(glm::vec3(0, 0, 349));
	cam.SetClipping(.1, 1000);
	cam.SetFOV(45);
}

void displayFunction()
{
	angle += speedRatio;
	if (angle >= 360 || angle <= -360) angle = 0;
	if (camPitchAngle <= -360) camPitchAngle = 0;
	if (camPitchAngle >= 360) camPitchAngle = 0;
	if (camYawAngle <= -360) camYawAngle = 0;
	if (camYawAngle >= 360) camYawAngle = 0;


	currentCamPitchAngle += -(camPitchAngle * mouseSensitivity);
	currentCamYawAngle += -(camYawAngle * mouseSensitivity);
	cam.ChangePitch(-(camPitchAngle * mouseSensitivity));
	cam.ChangeHeading((camYawAngle * mouseSensitivity));
	camPitchAngle = 0; camYawAngle = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glm::mat4 model, view, projection;
	cam.Update();
	cam.GetMatricies(projection, view, model);
	glm::mat4 mvp = projection * view * model;
	glLoadMatrixf(glm::value_ptr(mvp));

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glRotatef(angle, 1, 0, 0);
	glRotatef(angle, 0, 1, 0);

	glColor3f(0, 1, 0);
	glutWireCube(150.0);

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	for (int i = 0; i <= 75; i += 5)
	{
		glVertex3i(i, 0, 0);
		glVertex3i(-i, 0, 0);
		glVertex3i(0, i, 0);
		glVertex3i(0, -i, 0);
		glVertex3i(0, 0, i);
		glVertex3i(0, 0, -i);
	}
	glEnd();
	glPopMatrix();

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
		speedRatio += 0.125;
		break;
	case '-': case '_':
		speedRatio -= 0.125;
		break;
	case 'w': case 'W':
		for(int z = 0; z < camMoveSpeed; ++z)
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
		angle = 0;
		speedRatio = 0;
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
