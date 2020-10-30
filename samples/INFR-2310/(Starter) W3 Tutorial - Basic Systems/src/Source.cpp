/*
Week 3 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "Sprites/CSpriteRenderer.h"
#include "Sprites/CSpriteAnimator.h"
#include "CKnightFSM.h"
#include "CWarriorFSM.h"

#include "imgui.h"

#include <memory>

using namespace nou;

int main() 
{
	App::Init("Week 3 Tutorial - Sprites", 800, 800);
	App::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	App::InitImgui();

	//Load in some shaders.
	auto v_sprite = std::make_unique<Shader>("shaders/texturedunlit.vert", GL_VERTEX_SHADER);
	auto f_sprite = std::make_unique<Shader>("shaders/texturedunlit.frag", GL_FRAGMENT_SHADER);

	auto prog_sprite = ShaderProgram({ v_sprite.get(), f_sprite.get() });

	//Load in sprites.
	Texture2D boomTex = Texture2D("explosion.png", true);
	Texture2D knightTex = Texture2D("knight.png", true);
	Texture2D warriorTex = Texture2D("warrior_sheet.png", true);

	Material boomMat(prog_sprite);
	boomMat.AddTexture("albedo", boomTex);

	Material knightMat(prog_sprite);
	knightMat.AddTexture("albedo", knightTex);

	Material warriorMat(prog_sprite);
	warriorMat.AddTexture("albedo", warriorTex);

	
	//TODO: Load in explosion spritesheet.
	auto boomSheet = std::make_unique<Spritesheet>(boomTex, glm::vec2(222.0f, 222.0f));
	boomSheet->AddAnimation("boom", 0, 27, 30.0f);
	boomSheet->SetDefaultFrame(27);

	//Load in knight spritesheet, add animations.
	auto knightSheet = std::make_unique<Spritesheet>(knightTex, glm::vec2(64.0f, 64.0f));
	knightSheet->AddAnimation(CKnightFSM::idleClip, 0, 4, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::runClip, 5, 12, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::attackClip, 19, 21, 12.0f);

	//Loading warrior spritesheet and animations.
	auto warriorSheet = std::make_unique<Spritesheet>(warriorTex, glm::vec2(32.0f, 32.0f));
	warriorSheet->AddAnimation(CWarriorFSM::idleClip, 0, 19, 12.0f);
	warriorSheet->AddAnimation(CWarriorFSM::runClip, 20, 29, 12.0f);
	warriorSheet->AddAnimation(CWarriorFSM::attackClip, 30, 39, 12.0f);


	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Ortho(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 2.0f);

	//TODO: Create explosion entity.
	Entity okBoomer = Entity::Create();
	okBoomer.Add<CSpriteRenderer>(okBoomer, *boomSheet, boomMat);
	auto& boomAnim = okBoomer.Add<CSpriteAnimator>(okBoomer, *boomSheet);

	//Create the knight entity.
	Entity knightEntity = Entity::Create();
	knightEntity.transform.m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
	knightEntity.Add<CSpriteRenderer>(knightEntity, *knightSheet, knightMat);
	auto& knightAnim = knightEntity.Add<CSpriteAnimator>(knightEntity, *knightSheet);
	knightEntity.Add<CKnightFSM>(knightEntity);

	//Create the warrior entity.
	Entity warriorEntity = Entity::Create();
	warriorEntity.transform.m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
	warriorEntity.transform.m_pos = glm::vec3(44.0f, 44.0f, 0.0f);
	warriorEntity.Add<CSpriteRenderer>(warriorEntity, *warriorSheet, warriorMat);
	auto& warriorAnim = warriorEntity.Add<CSpriteAnimator>(warriorEntity, *warriorSheet);
	warriorEntity.Add<CWarriorFSM>(warriorEntity);

	App::Tick();

	//Disabling the depth buffer.
	//(The reason we do this is so we can naively draw sprites on top
	//of each other in the order we call Draw().)
	glDisable(GL_DEPTH_TEST);

	//Disable backface culling.
	//(The reason we disable culling is to make it simpler for us to 
	//flip sprites without making OpenGL think we're drawing polygons
	//that shouldn't be visible.)
	glDisable(GL_CULL_FACE);
	
	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: Control our knight.
		bool moving = Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_A);
		knightEntity.Get<CKnightFSM>().SetVariable("moving", moving);

		if (moving)
		{
			bool flip = Input::GetKey(GLFW_KEY_A);

			knightEntity.transform.m_scale.x = (flip) ? -2.0f : 2.0f;
			knightEntity.transform.m_pos.x += (flip) ? -500.0f * deltaTime : 500.0f * deltaTime;
		}

		//Control Warrior
		bool moving1 = Input::GetKey(GLFW_KEY_L) || Input::GetKey(GLFW_KEY_J);
		warriorEntity.Get<CWarriorFSM>().SetVariable("moving1", moving1);

		if (moving1 )
		{
			bool flip = Input::GetKey(GLFW_KEY_J);

			warriorEntity.transform.m_scale.x = (flip) ? -2.0f : 2.0f;
			warriorEntity.transform.m_pos.x += (flip) ? -500.0f * deltaTime : 500.0f * deltaTime;
		}


		//Updates all the entities.
		camEntity.Get<CCamera>().Update();
		warriorEntity.Get<CWarriorFSM>().Update();
		knightEntity.Get<CKnightFSM>().Update();
		//TODO: Update explosion entity.
		warriorEntity.Get<CSpriteAnimator>().Update(deltaTime);
		okBoomer.Get<CSpriteAnimator>().Update(deltaTime);
		knightEntity.Get<CSpriteAnimator>().Update(deltaTime);
		
		//Recomputes global matrices.
		//TODO: Update explosion entity.
		warriorEntity.transform.RecomputeGlobal();
		knightEntity.transform.RecomputeGlobal();
		okBoomer.transform.RecomputeGlobal();

		//Draws the sprites.
		//TODO: Draw explosion entity.
		okBoomer.Get<CSpriteRenderer>().Draw();
		knightEntity.Get<CSpriteRenderer>().Draw();	
		warriorEntity.Get<CSpriteRenderer>().Draw();

		//For Imgui stuff...
		App::StartImgui();

		
		static bool panelOpen = true;
		ImGui::Begin("Actions", &panelOpen, ImVec2(300, 100));

		if (ImGui::Button("Boom!"))
			okBoomer.Get<CSpriteAnimator>().PlayOnce("boom");

		if (ImGui::Button("Attack!"))
			knightEntity.Get<CKnightFSM>().SetTrigger("attack");

		if (ImGui::Button("Warrior Attack!"))
			warriorEntity.Get<CWarriorFSM>().SetTrigger("attack1");

		ImGui::End();
		App::EndImgui();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
