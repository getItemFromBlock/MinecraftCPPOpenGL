#include "Core/App/Input.hpp"

using namespace Core::App;

float mouseScroll = 0;
int LastKey = 0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouseScroll += (float)yoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		LastKey = key;
}

Core::Maths::IVec2 screenDim = Core::Maths::IVec2();
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	screenDim = Core::Maths::IVec2(width, height);
}

void Inputs::InitInputs(GLFWwindow* window, Core::Maths::IVec2 defaultSize)
{
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	screenDim = defaultSize;
	ScreenSize = defaultSize;
}

int Core::App::Inputs::GetLastKey()
{
	return LastKey;
}

void Core::App::Inputs::ClearLastKey()
{
	LastKey = 0;
}

void Inputs::UpdateInputs(GLFWwindow* window)
{
	ScreenSize = screenDim;
	double newMouseX, newMouseY;
	glfwGetCursorPos(window, &newMouseX, &newMouseY);
	deltaMouse.x = (float)(newMouseX - MousePos.x);
	deltaMouse.y = (float)(newMouseY - MousePos.y);
	MousePos.x = (float)newMouseX;
	MousePos.y = (float)newMouseY;

	bool tmp = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
	if (tmp) leftMouse = leftMouse ? INPUT_HOLD : INPUT_PRESS;
	else leftMouse = 0;
	tmp = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
	if (tmp) rightMouse = rightMouse ? INPUT_HOLD : INPUT_PRESS;
	else rightMouse = 0;
	tmp = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	if (tmp) esc = esc ? INPUT_HOLD : INPUT_PRESS;
	else esc = 0;
	scroll = mouseScroll;
	mouseScroll = 0;

	if (esc & INPUT_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		mouseCaptured = !mouseCaptured;
	}
}