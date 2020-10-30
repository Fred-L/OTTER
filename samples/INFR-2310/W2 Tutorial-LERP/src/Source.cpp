/*
Tutorial 1 Sample - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "NOU/GLTFLoader.h"

#include "Logging.h"

#include <memory>

using namespace nou;

//Templated LERP function.
//(This will work for any type that supports addition and scalar multiplication.)
template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}

int main() 
{
	App::Init("Week 1 Tutorial - LERP", 800, 800);
	App::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	auto prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	//Load in the ducky model.
	Mesh duckMesh;
	GLTF::LoadMesh("duck/Duck.gltf", duckMesh);
	Texture2D duckTex = Texture2D("duck/DuckCM.png");

	Material duckMat(prog_texLit);
	duckMat.AddTexture("albedo", duckTex);

	//Load in Bird model.
	Mesh birdMesh;
	GLTF::LoadMesh("Bird/Bird.gltf", birdMesh);
	Texture2D birdTex = Texture2D("Bird/Bird.png");

	Material birdMat(prog_texLit);
	birdMat.AddTexture("albedo", birdTex);


	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//Creating the duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, duckMesh, duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Creating the bird entity.
	Entity birdEntity = Entity::Create();
	birdEntity.Add<CMeshRenderer>(birdEntity, birdMesh, birdMat);
	birdEntity.transform.m_scale = glm::vec3(0.25f, 0.25f, 0.25f);
	birdEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 1.0f);
	birdEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Endpoints for lerping our position.
	glm::vec3 origPos = glm::vec3(0.0f, -1.0f, 0.0f), newPos = glm::vec3(0.0f, 0.0f, 0.0f);
	//Desired time for position lerp and timer.
	float posTimeLimit = 2.0f;
	float posTimer = 0.0f;
	//Whether we'll be going forward or backward.
	bool duckyGoPlaces = true;

	//Endpoints for lerping our colour.
	glm::vec3 origColor = glm::vec3(1.0f, 1.0f, 1.0f), newColor = glm::vec3(0.0f, 1.0f, 1.0f);
	//Desired time for colour lerp and timer.
	float colTimeLimit = 4.0f;
	float colTimer = 0.0f;
	//Whether we'll be going white or green.
	bool duckyGoGreen = true;

	//Endpoints for lerping our scale.
	glm::vec3 origScale = glm::vec3(0.004f, 0.004f, 0.004f), newScale = glm::vec3(0.005f, 0.015f, 0.015f);
	//Desired time for scale lerp and timer.
	float scaleTimeLimit = 5.0f;
	float scaleTimer = 0.0f;
	//Whether we'll be going 0.0005 or 0.01.
	bool duckyGoBig = true;

	//Endpoints for lerping our position.
	glm::vec3 origPos2 = glm::vec3(0.0f, -1.0f, 0.0f), newPos2 = glm::vec3(0.0f, 0.0f, 0.0f);
	//Whether we'll be going forward or backward.
	bool birdGoPlaces = true;

	//Endpoints for lerping our colour.
	glm::vec3 origColor2 = glm::vec3(1.0f, 1.0f, 1.0f), newColor2 = glm::vec3(0.0f, 1.0f, 1.0f);
	//Whether we'll be going white or green.
	bool birdGoGreen = true;

	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//Increment timers.
	    posTimer += deltaTime;
		colTimer += deltaTime;
		scaleTimer += deltaTime;

		//Resetting timers and adjusting back/forth booleans.
		if (posTimer >= posTimeLimit)
		{
			posTimer = 0.f;
			duckyGoPlaces = !duckyGoPlaces;
			birdGoPlaces = !birdGoPlaces;
		}

		if (colTimer >= colTimeLimit)
		{
			colTimer = 0.f;
			duckyGoGreen = !duckyGoGreen;
			birdGoGreen = !birdGoGreen;
		}

		if (scaleTimer >= scaleTimeLimit)
		{
			scaleTimer = 0.f;
			duckyGoBig = !duckyGoBig;
		}
		
		//Calculate t.
		float tPos = posTimer / posTimeLimit;
		float tColor = colTimer / colTimeLimit;
		float tScale = scaleTimer / scaleTimeLimit;

		//If true the duck lerps to a new pos--otherwise it returns to the original pos.
		if (duckyGoPlaces)
		{
			duckEntity.transform.m_pos = LERP(origPos, newPos, tPos);
		}
		else
		{
			duckEntity.transform.m_pos = LERP(newPos, origPos, tPos);
		}

		//If true the duck turns green, otherwise it lerps to its original colour (white).
		//(Material colour is multiplied with texture colour.)
		if (duckyGoGreen)
		{
			duckMat.m_color = LERP(origColor, newColor, tColor);
		}
		else
		{
			duckMat.m_color = LERP(newColor, origColor, tColor);
		}

		//If true the duck lerps to a new size--otherwise it returns to the original size.
		if (duckyGoBig)
		{
			duckEntity.transform.m_scale = LERP(origScale, newPos, tScale);
		}
		else
		{
			duckEntity.transform.m_scale = LERP(newScale, origPos, tScale);
		}
		
		//If true the duck lerps to a new pos--otherwise it returns to the original pos.
		if (birdGoPlaces)
		{
			birdEntity.transform.m_pos = LERP(origPos2, newPos2, tPos);
		}
		else
		{
			birdEntity.transform.m_pos = LERP(newPos2, origPos2, tPos);
		}

		//If true the duck turns green, otherwise it lerps to its original colour (white).
		//(Material colour is multiplied with texture colour.)

		if (birdGoPlaces == true)
		{
			birdEntity.transform.m_pos = LERP(origPos2, newPos2, tPos);
		}
		else if (birdGoPlaces == false)
		{
			birdEntity.transform.m_pos = LERP(newPos2, origPos2, tPos);
		}

		if (birdGoGreen == true)
		{
			birdMat.m_color = LERP(origColor2, newColor2, tColor);
		}
		else if (birdGoGreen == false)
		{
			birdMat.m_color = LERP(newColor2, origColor2, tColor);
		}
		//Updates the camera.
		camEntity.Get<CCamera>().Update();

		//The duck's transformation matrix gets recomputed to make sure it's accurate.
		duckEntity.transform.RecomputeGlobal();

		//The duck is drawn.
		duckEntity.Get<CMeshRenderer>().Draw();

		//The bird's transformation matrix gets recomputed to make sure it's accurate.
		birdEntity.transform.RecomputeGlobal();

		//The bird is drawn.
		birdEntity.Get<CMeshRenderer>().Draw();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
