#include "Core/App/Input.hpp"

using namespace Core::App;

float mouseScroll = 0;
int LastKey = 0;
std::string LastKeys = "";
bool trackTyping = false;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouseScroll += (float)yoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		LastKey = key;
	}
}

void character_callback(GLFWwindow* window, unsigned int key)
{
	if (!trackTyping || key < 32) return;
	if (key > 126)
	{
		switch (key)
		{
		case 163:
			LastKeys.push_back((char)156);
			break;
		case 164:
			LastKeys.push_back((char)249);
			break;
		case 167:
			LastKeys.push_back((char)250);
			break;
		case 168:
			LastKeys.push_back((char)173);
			break;
		case 176:
			LastKeys.push_back((char)248);
			break;
		case 178:
			LastKeys.push_back((char)253);
			break;
		case 181:
			LastKeys.push_back((char)230);
			break;
		case 224:
			LastKeys.push_back((char)133);
			break;
		case 226:
			LastKeys.push_back((char)131);
			break;
		case 227:
			LastKeys.push_back((char)134);
			break;
		case 228:
			LastKeys.push_back((char)132);
			break;
		case 231:
			LastKeys.push_back((char)135);
			break;
		case 232:
			LastKeys.push_back((char)138);
			break;
		case 233:
			LastKeys.push_back((char)130);
			break;
		case 234:
			LastKeys.push_back((char)136);
			break;
		case 235:
			LastKeys.push_back((char)137);
			break;
		case 238:
			LastKeys.push_back((char)140);
			break;
		case 239:
			LastKeys.push_back((char)139);
			break;
		case 241:
			LastKeys.push_back((char)164);
			break;
		case 244:
			LastKeys.push_back((char)147);
			break;
		case 245:
			LastKeys.push_back((char)127);
			break;
		case 246:
			LastKeys.push_back((char)148);
			break;
		case 249:
			LastKeys.push_back((char)151);
			break;
		case 251:
			LastKeys.push_back((char)150);
			break;
		case 252:
			LastKeys.push_back((char)129);
			break;
		case 8364:
			LastKeys.push_back((char)155);
			break;
		case 9786:
			LastKeys.push_back((char)254);
			break;
		default:
			LastKeys.push_back((char)255);
			break;
		}
	}
	else
	{
		LastKeys.push_back(key);
	}
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
	glfwSetCharCallback(window, character_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	screenDim = defaultSize;
	ScreenSize = defaultSize;
}

int Core::App::Inputs::GetLastKey()
{
	return LastKey;
}

std::string Core::App::Inputs::GetLastKeys()
{
	return LastKeys;
}

void Core::App::Inputs::ClearLastKey()
{
	LastKey = 0;
}

void Core::App::Inputs::ClearLastKeys()
{
	LastKeys.clear();
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

	if (esc & INPUT_PRESS || myBalls)
	{
		glfwSetInputMode(window, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		mouseCaptured = !mouseCaptured;
		myBalls = false;
	}
}

void Core::App::Inputs::SetTypingEnabled(bool newVal)
{
	if (trackTyping != newVal) myBalls = true;
	trackTyping = newVal;
}
