#include "application.h"

#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <toolbox/calc.h>

#include "service_locator.h"

#include "wrapper/time.h"

#include "resources/texture.h"
#include "resources/model.h"
#include "resources/model_renderer.h"
#include "resources/resource_manager.h"

#include "world/dir_light.h"
#include "world/point_light.h"
#include "world/spot_light.h"
#include "world/skybox.h"

#include "memory_leak.h"

#include "world/scene_manager.h"
#include "world/box_collider.h"
#include "world/capsule_collider.h"

#include "interface/interface.h"

#include "reflection/runtime_classes.h"

#include "resources/audio.h"
#include "audio/sound_buffer.h"

#include "world/player_test.h"

Application::Application()
{
    ServiceLocator::Setup();

    mWindowManager = ServiceLocator::Get<Window>();
    mRenderer = ServiceLocator::Get<Renderer>();
}

void Application::Init()
{
    mWindowManager->Init();
    mRenderer->Init();

    RuntimeClasses::AddAllClasses();

    // Callback
    ServiceLocator::SetupCallbacks();

    mEditor.Init();

    BaseShader = ResourceManager::Get<Shader>("base_shader");

    mGame.Init();

    Skybox::Setup();
}

void Application::Update()
{
    Time::SetTimeVariables();

    mRenderer->SetClearColor(0,0,0);

    Camera::ProcessInput();

    mGame.Update();

    mEditor.Update();

    // Draw loop for all editors
    for (int i = 0; i < Interface::EditorViewports.size(); i++)
    {
        Interface::EditorViewports[i]->RescaleViewport();
        Interface::EditorViewports[i]->ViewportCamera->Update();
        Skybox::Update(Interface::EditorViewports[i]->ViewportCamera);

        mRenderer->BindFramebuffer(GL_FRAMEBUFFER, Interface::EditorViewports[i]->GetFBO_ID());

        mRenderer->EnableTest(GL_DEPTH_TEST);
        
        mRenderer->SetClearColor(0,0,0);
        mRenderer->ClearBuffer();
        
        mRenderer->UseShader(BaseShader->ID);

        mRenderer->SetUniform(BaseShader->ID ,"vp", Interface::EditorViewports[i]->ViewportCamera->GetVP());
        mRenderer->SetUniform(BaseShader->ID ,"viewPos", Interface::EditorViewports[i]->ViewportCamera->Eye);

        SceneManager::Draw();

        mRenderer->UnUseShader();

        mRenderer->BindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    EditorViewport::SetIsGizmoUpdated(false);

    Interface::Render();

    mWindowManager->SwapBuffers();
    mRenderer->ClearBuffer();
    Logger::CheckForExit();
}

void Application::Clear()
{
    mRenderer->UnUseShader();
    mEditor.Terminate();
    mGame.Terminate();
    ServiceLocator::CleanServiceLocator();
    Logger::Stop();
}
