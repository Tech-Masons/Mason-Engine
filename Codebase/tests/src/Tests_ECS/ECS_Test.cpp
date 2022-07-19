#include "ECS_Test.h"

#include "ecs/Scene.h"

#include "input/Keyboard.h"
#include <random>
#include <graphics/DebugRenderer.h>

ECS_Test::ECS_Test(Win32Window* pWindow, MasterRenderer* pRenderer, std::map<InputType, iInputSystem*> inputSystems)
: iTest(L"ECS_Test", pWindow, pRenderer, inputSystems)
{

}

void ECS_Test::Init()
{
	scene = LoadScene();

	EntityID editor_camera = scene->CreateEntity(TEST_SUITE_CAMERA_NAME);
	Transform* cam_trans = scene->GetComponent<Transform>(editor_camera);

	PerspectiveCamera* cam = scene->AddComponent<PerspectiveCamera>(editor_camera);

	cam_trans->SetPosition({ 0, 5, -10 });
	cam_trans->SetRotation({ TO_RADIAN(30.0f),0.0f, 0.0f });

	// test entity 

	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();

	std::default_random_engine prng_engine(seed);
	std::uniform_real_distribution<float> prng(-10, 10);

	int entity_test_draw_count = 8196;

	for (int i = 0; i < entity_test_draw_count; i++)
	{
		EntityID test_entity = scene->CreateEntity();
		Transform* test_entity_trans = scene->AddComponent<Transform>(test_entity);
		test_entity_trans->SetPosition({ prng(prng_engine),prng(prng_engine), prng(prng_engine) });

	}

}

void ECS_Test::Input()
{
}

void ECS_Test::Update(float deltaTime)
{
	HandleEditorCameraControl(deltaTime);

	///SUBMIT ALL RENDER COMMANDS HERE -------------------------------------------------

	//Aqure the Debug Renderer
	auto dbg = TypeCast<DebugRenderer>(renderer->GetRenderer(RenderType::Debug));

	dbg->DrawGridXZ(8, 1.0f, { 1.0, 1.0, 1.0, 1.0 });

	for (EntityID entity : ComponentView<Transform>(scene))
	{
		if (!scene->GetComponent<PerspectiveCamera>(entity)) {
			Transform* t = scene->GetComponent<Transform>(entity);
			dbg->DrawMatrix(t->GetMatrix());
		}
	}

}

void ECS_Test::LateUpdate(float fixedDeltaTime)
{
}

void ECS_Test::Destroy()
{
}

void ECS_Test::HandleEditorCameraControl(double deltaTime)
{
	Transform* cam_trans = nullptr;

	for (EntityID entity : ComponentView<PerspectiveCamera>(scene)) {

		if (scene->GetComponent<Tag>(entity)->value == TEST_SUITE_CAMERA_NAME) {

			cam_trans = scene->GetComponent<Transform>(entity);

			float3 pos = cam_trans->GetPosition();
			float last_y = pos.y;

			Keyboard* kbd = TypeCast<Keyboard>(input[InputType::Keyboard]);

			struct {//camera
				float MoveSpeed = 10.0f;
			} camera;

			float3 right = cam_trans->GetMatrix().row0.xyz();
			float3 up = cam_trans->GetMatrix().row1.xyz();
			float3 forward = cam_trans->GetMatrix().row2.xyz();

			if (kbd->IsKeyPressed(KeyCode::W)) {
				pos += forward * camera.MoveSpeed * deltaTime;
				pos.y = last_y;
			}
			if (kbd->IsKeyPressed(KeyCode::S)) {
				pos -= forward * camera.MoveSpeed * deltaTime;
				pos.y = last_y;
			}
			if (kbd->IsKeyPressed(KeyCode::A)) {
				pos -= right * camera.MoveSpeed * deltaTime;
			}
			if (kbd->IsKeyPressed(KeyCode::D)) {
				pos += right * camera.MoveSpeed * deltaTime;
			}

			cam_trans->SetPosition(pos);

		}

	}

	renderer->UpdateViewMatrix(Matrix::Inverse(cam_trans->GetMatrix()));

}

Scene* ECS_Test::LoadScene()
{
	return new Scene();
}

