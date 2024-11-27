#include "game.h"

Game::Game()
{
}

void Game::Init()
{
	PhysicsSystem::Init();

    SceneManager::Init();

    SceneManager::ActualScene->AddObject("Point")->AddComponent<PointLight>(Vector3{ 0.4f, 0.4f, 0.4f }, Vector3{ 0.8f, 0.8f, 0.8f }, Vector3{ 0.5f, 0.5f, 0.5f }, 1.0f, 0.09f, 0.032f);

    Object* floor = SceneManager::ActualScene->AddObject("Floor");
    floor->GameTransform->Position = Vector3(0, -2, 0);
    floor->GameTransform->Rotation = Vector3(0.0f, 0.0f, 0.0f);
    floor->GameTransform->Scale = Vector3(100.0f, 1.0f, 100.0f);
    floor->AddComponent<BoxCollider>(floor->GameTransform->GetPosition(), floor->GameTransform->GetRotationQuat(), Vector3(100.0f, 2.0f, 100.0f), true);
    std::shared_ptr<Model> floorModel = floor->AddComponent<ModelRenderer>()->ModelObject = ResourceManager::Get<Model>("assets/ground.obj");
    floorModel->SetTexture(0, ResourceManager::Get<Texture>("assets/ground.png"));


    Object* object = SceneManager::ActualScene->AddObject("PikingRoom");
    object->GameTransform->Position = Vector3(0, -0.5f, 0);
    object->AddComponent<ModelRenderer>()->ModelObject = ResourceManager::Get<Model>("assets/viking_room.obj");

    Object* player = SceneManager::ActualScene->AddObject("Player");
    player->GameTransform->Position = Vector3(0.0f, 0.0f, 0.0f);
    //player->GameTransform->Rotation = Vector3(0.0f, 90.0f, 90.0f);
    player->GameTransform->Scale = Vector3(0.2f, 0.2f, 0.2f);
    std::shared_ptr<Model> mode = player->AddComponent<ModelRenderer>()->ModelObject = ResourceManager::Get<Model>("assets/dog.obj");
    mode->SetTexture(0, ResourceManager::Get<Texture>("assets/dog.jpg"));


    Object* sphere = SceneManager::ActualScene->AddObject("Sphere");
    sphere->GameTransform->Position = Vector3(0, 1.f, 0);
    sphere->AddComponent<ModelRenderer>()->ModelObject = ResourceManager::Get<Model>("assets/sphere.obj");
    CapsuleCollider* c = sphere->AddComponent<CapsuleCollider>(sphere->GameTransform->GetPosition(), sphere->GameTransform->GetRotationQuat(), 1, 1);

    //SOUND
    mSoundDevice = SoundDevice::Get();

    sound1 = SoundBuffer::Get()->AddSoundEffect(ResourceManager::Get<Audio>("audio/mario.wav"));
    sound2 = SoundBuffer::Get()->AddSoundEffect(ResourceManager::Get<Audio>("audio/desert.wav"));

    mSoundSource = new SoundSource;
    source1 = mSoundSource->CreateSource();
    source2 = mSoundSource->CreateSource();

    mKeyInput = ServiceLocator::Get<InputManager>()->GetKeyInput("editorCameraInput");
}

void Game::Update()
{
    SceneManager::GlobalUpdate();
    mSoundSource->SetPosition(source1, Vector3());

    if (mKeyInput->GetIsKeyDown(GLFW_KEY_X))
    {
        mSoundSource->Play(source1, sound1);
    }

    if (mKeyInput->GetIsKeyDown(GLFW_KEY_C))
    {
        mSoundSource->Play(source2, sound2);
    }

    if (mKeyInput->GetIsKeyDown(GLFW_KEY_V))
    {
        mSoundSource->Pause(source2, sound2);
    }

    if (mKeyInput->GetIsKeyDown(GLFW_KEY_B))
    {
        mSoundSource->Resume(source2, sound2);
    }

    mSoundDevice->SetPosition(Interface::EditorViewports[0]->ViewportCamera->CurrentCamera->Eye);
    mSoundDevice->SetOrientation(Interface::EditorViewports[0]->ViewportCamera->CurrentCamera->LookAt);
}

void Game::Terminate()
{
    SceneManager::Delete();
    PhysicsSystem::Terminate();
}
