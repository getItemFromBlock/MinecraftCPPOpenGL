#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "LowRenderer/Rendering/EditorCamera.hpp"
#include "Resources/ResourceManager.hpp"
#include "Core/App/Input.hpp"
#include "Resources/ShaderProgram.hpp"
#include "Resources/ShaderManager.hpp"
#include "Resources/TextureManager.hpp"
#include "Resources/MaterialManager.hpp"
#include "Resources/MeshManager.hpp"
#include "Core/Debug/FrameGraph.hpp"
#include "LowRenderer/Lightning/LightManager.hpp"
#include "Physics/PhysicsHandler.hpp"
#include "Core/Maths/PRNG.hpp"
#include "Blocks/Blocks.hpp"

namespace Core::App
{
	enum class MouseInput : unsigned int
	{
		Left_Down = 0,
		Right_Down,
		Middle_Down,
		Forward_Down,
		Back_Down,
		Left_Press,
		Right_Press,
		Middle_Press,
		Forward_Press,
		Back_Press,
		COUNT,
		ALL_DOWN = 0b0000000000011111,
		ALL_PRESS = 0b0000001111100000,
		ALL = 0b0000001111111111,
	};

	enum class InputType : unsigned int
	{
		Front = 0,
		Back,
		Left,
		Right,
		Jump,
		Crouch,
		Run,
		Drop,
		Swap,
		Inventory,
		View,
		All,
	};

	enum class GameState : unsigned int
	{
		MainMenu = 0,
		Option,
		Pause,
		Level,
	};

	struct AppInit
	{
		const unsigned int width;
		const unsigned int height;
		const unsigned int majorVersion;
		const unsigned int minorVersion;
		const char* name;
		void (*glDebugOutput)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};

	enum DefaultTextures
	{
		Debug = 1,
		Blank,
		Screen_Buffer,
		Graph,
		Blank_Normal,
		Default_Font,
	};

	class App
	{
	private:
		GLFWwindow* window;
		GLFWimage* windowIcon;
		static inline Inputs inputs;
		static inline LowRenderer::Rendering::EditorCamera MainCamera = LowRenderer::Rendering::EditorCamera();
		static inline bool DebugMode = true;
		static inline bool wireframe = false;
		static inline int InputBindings[static_cast<unsigned int>(InputType::All)] = { 0 };
		static inline unsigned int SelectedComponent = 0;
		static inline unsigned int SelectedComponentAlt = 0;
		static inline unsigned int NextComponent = 0;
		static inline bool ShouldClose = false;
		static inline GameState State = GameState::MainMenu;
		static inline Core::Maths::PRNG rng;
		LowRenderer::Lightning::LightManager lights;
		Resources::ShaderManager shaders;
		Resources::TextureManager textures;
		Resources::MaterialManager materials;
		Resources::MeshManager meshes;
		Core::Debug::FrameGraph frameGraph;
		Resources::ResourceManager res;
		Physics::PhysicsHandler physics;
		
		bool ortho = false;
		bool vsync = true;
		bool writeScreen = false;
		bool fullscreen = false;
		bool requestFullscreen = false;
		bool requestScreenShot = false;
		bool requestShaderReload = false;
		bool requestEsc = false;
		bool requestSave = false;
		Resources::Texture* ScreenBuffer = nullptr;
		World::World* world = nullptr;
		unsigned int seed = 0;

		unsigned int VAO;
		Resources::ShaderProgram* shaderProgram;
		float globalTime = 0;
		float deltaTime = 0;
		float fps = 60;
		float minFps = 60;
		float maxFps = 60;

		int InitOpenGL(const AppInit& app);

		void InitDatas();

		void ProcessInput();

		void DrawGUI();

		void SwitchWireFrame(bool& newVal);
		
	public:
		App();

		int InitApp(const AppInit& data);

		void ClearApp();

		void Update();

		void PrintScreen();

		static LowRenderer::Rendering::EditorCamera* GetMainCamera();

		static bool IsDebugMode() { return DebugMode; }

		static bool IsWireframe() { return wireframe; }

		static inline int* GetInputBindings() { return InputBindings; }

		static inline unsigned int GetSelectedComponent() { return SelectedComponent; }

		static inline void SetSelectedComponent(unsigned int in) { NextComponent = in; }

		static inline int GetLastKeyPressed() { return inputs.GetLastKey(); }

		static inline void ClearLastKeyPressed() { inputs.ClearLastKey(); }

		static inline Inputs* GetInputs() { return &inputs; }

		static inline void RequestExit() { ShouldClose = true; }

		static inline void SetGameState(GameState in) { State = in; }

		static inline Core::Maths::PRNG& GetRNG() { return rng; }

	};

}