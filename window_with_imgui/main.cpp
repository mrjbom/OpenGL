#include <iostream>
#include <chrono>
#include <GL/glut.h>
#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

using namespace std;

constexpr auto FPS_RATE = 120;
int windowHeight = 600, windowWidth = 600;

void init();
void displayFunction();
void idleFunction();
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
	gluOrtho2D(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2);
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

static bool showWindow = true;

void displayFunction()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	
	if (showWindow)
	{
		ImGui::Begin("Another Window", &showWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			showWindow = false;
		ImGui::End();
	}

	/**/
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(238, 130, 238);
	glBegin(GL_POLYGON);
	glVertex3i(-150, 150, 0);
	glVertex3i(150, 150, 0);
	glVertex3i(150, -150, 0);
	glVertex3i(-150, -150, 0);
	glEnd();

	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glEnd();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2);
	glutCreateWindow("Window");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	init();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	glutMainLoop();

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
