#include "Core/App/App.hpp"

#include <iostream>
#include <filesystem>

#include "Core/Debug/Gizmo.hpp"
#include "Resources/Shader.hpp"
#include "Core/Maths/Maths.hpp"
#include "Resources/ResourceLoader.hpp"
#include "Resources/TextureAtlas.hpp"

#include "World/World.hpp"

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <ImGUI/imgui_stdlib.hpp>

#define MAX_LAG_DELTA 1/15.0

const char* const defaultModels[] =
{
	"DebugSphere",
	"DebugCube",
	"DebugCapsuleA",
	"DebugCapsuleB",
	"DebugCapsuleC",
	"DebugPlane",
};

namespace Core::App
{
	void error_callback(int error, const char* description)
	{
		LOG("! Error ! %s", description);
	}

	int App::InitApp(const AppInit& data)
	{
		glfwInit();
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, data.majorVersion);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, data.minorVersion);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		glfwSetErrorCallback(error_callback);
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

		// glfw window creation
		// --------------------
		window = glfwCreateWindow(data.width, data.height, data.name, NULL, NULL);
		if (window == NULL)
		{
			LOG("Failed to create GLFW window");
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		windowIcon = Resources::Texture::ReadIcon("DefaultResources/Icon/Icon_48.png");
		if (windowIcon) glfwSetWindowIcon(window,1,windowIcon);
		//glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
		inputs.InitInputs(window, Core::Maths::IVec2(data.width, data.height));
		return InitOpenGL(data);
	}

	int App::InitOpenGL(const AppInit& data)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG("Failed to initialize GLAD; Error code : %d", glad_glGetError());
			return -1;
		}

		GLint flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(data.glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		glfwSwapInterval(1);

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		InitDatas();
		return 0;
	}

	void App::InitDatas()
	{
#ifndef _DEBUG
		DebugMode = false;
#endif
		InputBindings[static_cast<unsigned int>(InputType::Front)] = ImGuiKey_W;
		InputBindings[static_cast<unsigned int>(InputType::Back)] = ImGuiKey_S;
		InputBindings[static_cast<unsigned int>(InputType::Left)] = ImGuiKey_A;
		InputBindings[static_cast<unsigned int>(InputType::Right)] = ImGuiKey_D;
		InputBindings[static_cast<unsigned int>(InputType::Jump)] = ImGuiKey_Space;
		InputBindings[static_cast<unsigned int>(InputType::Crouch)] = ImGuiKey_LeftCtrl;
		InputBindings[static_cast<unsigned int>(InputType::Run)] = ImGuiKey_LeftShift;
		InputBindings[static_cast<unsigned int>(InputType::Drop)] = ImGuiKey_Q;
		InputBindings[static_cast<unsigned int>(InputType::Swap)] = ImGuiKey_F;
		InputBindings[static_cast<unsigned int>(InputType::Inventory)] = ImGuiKey_E;
		InputBindings[static_cast<unsigned int>(InputType::View)] = ImGuiKey_F5;
		Resources::Texture::SetFilterType(GL_NEAREST);
		res.SetPathAutoAppend(true);
		textures.CreateTexture(&res, "DefaultResources/Textures/debug.png", GL_NEAREST);
		textures.CreateTexture(&res, "DefaultResources/Textures/Blank.png");
		size_t index = textures.CreateTexture(&res, "DefaultResources/Textures/ScreenBuffer.png");
		ScreenBuffer = textures.GetTextures()[index];
		textures.CreateTexture(&res, "DefaultResources/Textures/graph.png");
		frameGraph = Core::Debug::FrameGraph(res.Get<Resources::Texture>("DefaultResources/Textures/graph.png"), Core::Maths::IVec2(240, 160));
		textures.CreateTexture(&res, "DefaultResources/Textures/normal.png");

		textures.CreateFont(&res, "DefaultResources/Font/default_font.png", GL_NEAREST);

		LowRenderer::Model* obj;
		for (size_t i = 0; i < sizeof(defaultModels) / sizeof(const char*); i++)
		{
			obj = new LowRenderer::Model();
			obj->LoadMesh(defaultModels[i], &res, &materials, &textures, &meshes);
			meshes.AddModel(obj, defaultModels[i]);
		}

		lights.Init(&res);
		//Resources::ResourceLoader::LoadResources(&res, &meshes, &materials, &textures, &shaders);

		res.SetPathAutoAppend(true);
		Resources::FragmentShader* frag = res.Create<Resources::FragmentShader>("DefaultResources/Shaders/default_fragment.frag");
		shaders.AddShader(frag);
		Resources::VertexShader* vert = res.Create<Resources::VertexShader>("DefaultResources/Shaders/default_vertex.vert");
		shaders.AddShader(vert);
		shaders.CreateShaderProgram(vert, frag, "Default Shader");

		frag = res.Create<Resources::FragmentShader>("DefaultResources/Shaders/lit_fragment.frag");
		shaders.AddShader(frag);
		vert = res.Create<Resources::VertexShader>("DefaultResources/Shaders/lit_vertex.vert");
		shaders.AddShader(vert);
		shaders.CreateShaderProgram(vert, frag, "Lit Shader");

		frag = res.Create<Resources::FragmentShader>("DefaultResources/Shaders/outline_fragment.frag");
		shaders.AddShader(frag);
		vert = res.Create<Resources::VertexShader>("DefaultResources/Shaders/outline_vertex.vert");
		shaders.AddShader(vert);
		shaders.CreateShaderProgram(vert, frag, "Outline Shader");

		frag = res.Create<Resources::FragmentShader>("DefaultResources/Shaders/ui_fragment.frag");
		shaders.AddShader(frag);
		vert = res.Create<Resources::VertexShader>("DefaultResources/Shaders/ui_vertex.vert");
		shaders.AddShader(vert);
		shaders.CreateShaderProgram(vert, frag, "UI Shader");

		// TODO load world

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// TODO remove
	unsigned int shadowMapID = 0;
	void App::Update()
	{
		Entities::Entity::SetResourceManager(&res);
		Resources::ShaderProgram* MainShader = shaders.GetShaderProgram("default shader");
		Resources::ShaderProgram* LitShader = shaders.GetShaderProgram("lit shader");
		Resources::TextureAtlas atlas = Resources::TextureAtlas(Core::Maths::IVec2(1024, 1024));
		textures.LoadAtlas(atlas);
		textures.LoadTextures(&res);
		Blocks::BlockRegister::RegisterBlocks(&atlas);
		//Resources::Texture::SaveImage("atlas", (unsigned char*)atlas.GetData(), 1024, 1024);
		atlas.Load();
		world = new World::World(glfwGetTime(), &meshes, MainShader, LitShader, atlas.GetID());
		seed = world->GetSeed();
		LowRenderer::Model debug;
		debug.CreateFrom(meshes.GetModels("DebugCube").at(0)->model);
		debug.shaderProgram = LitShader;
		glUseProgram(MainShader->GetProgramID());
		glUniform1i(MainShader->GetLocation(Resources::ShaderData::Texture), atlas.GetID());
		glUniform3f(MainShader->GetLocation(Resources::ShaderData::MatAmbient), 1.0f, 1.0f, 1.0f);
		shaderProgram = MainShader;
		shadowMapID = world->shadowMap.GetTextureID();
		while (!glfwWindowShouldClose(window))
		{
			res.Update();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			glfwPollEvents();
			ProcessInput();
			deltaTime = (float)glfwGetTime() - globalTime;
			globalTime = (float)glfwGetTime();

			fps = 1 / deltaTime;
			if ((int)(globalTime / 5) != (int)((globalTime + deltaTime) / 5))
			{
				minFps = fps;
				maxFps = fps;
			}
			else
			{
				if (minFps > fps)
					minFps = fps;
				if (maxFps < fps)
					maxFps = fps;
			}
			frameGraph.Update(deltaTime);
			//if (playMode != PlayType::Game)
				DrawGUI();
			if (deltaTime > MAX_LAG_DELTA)
			{
				LOG("Warning : Lagspike of %.3f ms, %.3f ms skipped", deltaTime * 1000.0, (deltaTime - MAX_LAG_DELTA) * 1000.0);
				deltaTime = (float)(MAX_LAG_DELTA);
			}

			world->UpdateWorld(glfwGetTime(), deltaTime);

			glClearColor(0.0f, 0.125f, 0.443f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			Core::Maths::Mat4 vp;
			if (!ortho)
			{
				vp = MainCamera.GetProjectionMatrix() * MainCamera.GetViewMatrix();
			}
			else
			{
				vp = MainCamera.GetOrthoMatrix() * MainCamera.GetViewMatrix();
			}
			world->RenderWorld(VAO, &shaderProgram, vp);
			bool wf = false;
			for (size_t i = 0; i < Core::Debug::Gizmo::GetSize(); i++)
			{
				const Core::Debug::ColoredBox& element = Core::Debug::Gizmo::GetItem(i);
				debug.modelMat = Core::Maths::Mat4::CreateTransformMatrix(element.box.center, Core::Maths::Vec3(), element.box.size);
				debug.SetColor(element.color);
				if (element.wireframe != wf) SwitchWireFrame(wf);
				debug.Render(VAO, &shaderProgram, vp, nullptr);
			}
			glClear(GL_DEPTH_BUFFER_BIT);
			for (size_t i = 0; i < Core::Debug::OverlayGizmo::GetSize(); i++)
			{
				const Core::Debug::ColoredBox& element = Core::Debug::OverlayGizmo::GetItem(i);
				debug.modelMat = Core::Maths::Mat4::CreateTransformMatrix(element.box.center, Core::Maths::Vec3(), element.box.size);
				debug.SetColor(element.color);
				if (element.wireframe != wf) SwitchWireFrame(wf);
				debug.Render(VAO, &shaderProgram, vp, nullptr);
			}
			if (wf != wireframe) SwitchWireFrame(wf);
			Core::Debug::OverlayGizmo::ClearElements();
			Core::Debug::Gizmo::ClearElements();
			if (ShouldClose) glfwSetWindowShouldClose(window, true);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}
		world->Exit();
		atlas.UnLoad();
		Blocks::BlockRegister::FreeBlocks();
	}

	void App::SwitchWireFrame(bool& newVal)
	{
		newVal = !newVal;
		if (newVal)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
	}

	void App::ClearApp()
	{
		// TODO destroy world
		meshes.ClearModels();
		materials.ClearMaterials();
		frameGraph.Destroy();
		shaders.DestroyShaderPrograms();
		res.Terminate();
		res.ClearResources();

		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
		delete[] windowIcon->pixels;
		delete windowIcon;
		delete world;
	}

	App::App()
	{
		window = nullptr;
		windowIcon = nullptr;
		VAO = 0xffffffff;
		shaderProgram = nullptr;
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void App::ProcessInput()
	{
		if (SelectedComponent == 0 && SelectedComponentAlt == 0) SelectedComponentAlt = 1;
		if (NextComponent != 0)
		{
			SelectedComponent = NextComponent;
			NextComponent = 0;
			SelectedComponentAlt = 0;
		}
		unsigned int MInputs = 0;
		for (unsigned int i = 0; i < static_cast<unsigned int>(InputType::Jump); i++)
		{
			MInputs |= ImGui::IsKeyDown(GetInputBindings()[i]) << i;
		}
		if (MInputs && SelectedComponent == 0)
		{
			SelectedComponent = SelectedComponentAlt;
			SelectedComponentAlt = 0;
		}
		if (inputs.deltaMouse.getLength() > 5 && SelectedComponentAlt == 0)
		{
			SelectedComponentAlt = SelectedComponent;
			SelectedComponent = 0;
		}
		static Core::Maths::IVec2 savedSize;
		static Core::Maths::IVec2 savedPos;
		/*
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			if (!requestEsc)
			{
				requestEsc = true;
				switch (State)
				{
				case Core::App::GameState::MainMenu:
					glfwSetWindowShouldClose(window, true);
					break;
				case Core::App::GameState::Option:
					// TODO
					State = GameState::MainMenu;
					break;
				case Core::App::GameState::Pause:
					break;
				case Core::App::GameState::Level:
					
					State = GameState::MainMenu;
					break;
				default:
					break;
				}
			}
		}
		else
		{
			requestEsc = false;
		}
		*/
		if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		{
			if (!requestShaderReload)
			{
				shaders.ReloadShaders();
				shaderProgram = nullptr;
				requestShaderReload = true;
			}
		}
		else
		{
			requestShaderReload = false;
		}
		if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
		{
			if (!requestFullscreen)
			{
				requestFullscreen = true;
				if (fullscreen)
				{
					glfwSetWindowMonitor(window, NULL, savedPos.x, savedPos.y, savedSize.x, savedSize.y, GLFW_DONT_CARE);
				}
				else
				{
					glfwGetWindowPos(window, &savedPos.x, &savedPos.y);
						glfwGetWindowSize(window, &savedSize.x, &savedSize.y);
					GLFWmonitor* monitor = glfwGetPrimaryMonitor();
					const GLFWvidmode* mode = glfwGetVideoMode(monitor);
					glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
					glfwSwapInterval(1);
				}
				fullscreen = !fullscreen;
			}
		}
		else
		{
			requestFullscreen = false;
		}
		inputs.UpdateInputs(window);
		if (writeScreen && inputs.ScreenSize.x*inputs.ScreenSize.y > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, ScreenBuffer->GetTextureID());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, inputs.ScreenSize.x, inputs.ScreenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, inputs.ScreenSize.x, inputs.ScreenSize.y);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		{
			if (!requestScreenShot)
			{
				requestScreenShot = true;
				PrintScreen();
			}
		}
		else
		{
			requestScreenShot = false;
		}
	}

	void App::DrawGUI()
	{
		if (ImGui::Begin("Config"))
		{
			//Camera Overlay
			if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
			{
				MainCamera.RenderGUI();
				bool newVal = wireframe;
				ImGui::Checkbox("Wireframe render", &newVal);
				if (newVal != wireframe)
				{
					SwitchWireFrame(wireframe);
				}
				newVal = vsync;
				ImGui::Checkbox("VSync", &newVal);
				if (newVal != vsync)
				{
					vsync = newVal;
					if (vsync)
					{
						glfwSwapInterval(1);
					}
					else
					{
						glfwSwapInterval(0);
					}
				}
				ImGui::Checkbox("Debug View", &DebugMode);
				ImGui::Checkbox("Orthographic Projection", &ortho);
				ImGui::Checkbox("Write Screen Buffer to Texture", &writeScreen);
			}
			if (ImGui::CollapsingHeader("Game", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Current Block : %s", Blocks::BlockRegister::GetBlock(world->player->GetSelectedBlock())->GetName());
				ImGui::Text("Seed: %lu", seed);
			}
			//Draw FPS
			ImGui::Text("FPS: %.1f  Min: %.1f  Max: %.1f", fps, minFps, maxFps);
			frameGraph.Print();

			// Extra debug for fun
			ImGui::Image((ImTextureID)(size_t)shadowMapID, { 240.0f, 240.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
		}
		ImGui::End();
	}

	void App::PrintScreen()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		unsigned char* buf = new unsigned char[(size_t)(4llu * inputs.ScreenSize.x * inputs.ScreenSize.y)];
		glReadPixels(0, 0, inputs.ScreenSize.x, inputs.ScreenSize.y, GL_RGBA, GL_UNSIGNED_BYTE, buf);
		ScreenBuffer->Overwrite(buf, inputs.ScreenSize.x, inputs.ScreenSize.y);
		Resources::Texture::SaveImage("Screenshots/Screenshot", buf, inputs.ScreenSize.x, inputs.ScreenSize.y);
		delete[] buf;
	}

	LowRenderer::Rendering::EditorCamera* Core::App::App::GetMainCamera()
	{
		return &MainCamera;
	}
	/*
	* Leave this just for future self
	* 
	* bool App::DoesSceneExist(const char* path)
	{
		std::string fullPath = "Scenes/";
		fullPath.append(path);
		fullPath.append(".sdata");
		return std::filesystem::exists(fullPath);
	}
	*/
	
}