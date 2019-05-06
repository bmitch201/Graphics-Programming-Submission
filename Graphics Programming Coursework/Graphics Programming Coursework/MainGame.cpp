#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>

Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
	Texture* texture(); //load texture
	Shader* shaderPass();
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 

	shaderSkybox.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag");

	mesh1.loadModel("..\\res\\Hexagon.obj");
	mesh2.loadModel("..\\res\\pokeball.obj");
	mesh3.loadModel("..\\res\\cylinder.obj");
	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};
	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
				case SDLK_a://If key 'a' down
					myCamera.MoveRight(1);
					break;

				case SDLK_d://If key 'd' down
					myCamera.MoveRight(-1);
					break;

				case SDLK_w://If key 'w' down
					myCamera.MoveForward(1);
					break;

				case SDLK_s://If key 's' down
					myCamera.MoveForward(-1);
					break;

				case SDLK_e://If key 'e' down
					myCamera.RotateY(-0.1f);
					break;

				case SDLK_q://If key 'q' down
					myCamera.RotateY(0.1f);
					break;
				}
		}
	}
}

void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void MainGame::setUniform(Shader currentShader, const char * name, const glm::mat4 & m)
{
	currentShader.setMat4(name, m);
}
void MainGame::setUniform(Shader currentShader, const char * name, const glm::mat3 & m)
{
	currentShader.setMat3(name, m);
}

void MainGame::setUniform(Shader currentShader, const char * name, float val)
{
	currentShader.setFloat(name, val);
}

void MainGame::setUniform(Shader currentShader, const char *name, const glm::vec3 & v)
{
	currentShader.setVec3(name, v);
}

void MainGame::setUniform(Shader currentShader, const char *name, int val)
{
	currentShader.setInt(name, val);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	//Sets up the skybox 
	Skybox();

	//Sets the Positon, Rotation and Scale of Mesh 1
	transform.SetPos(glm::vec3(sinf(counter), 0.5, 0.0));
	transform.SetRot(glm::vec3(1.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.05, 0.05, 0.05)); //Hexagon Model
	//transform.SetScale(glm::vec3(0.005, 0.005, 0.005)); //Pokeball model
	//transform.SetScale(glm::vec3(0.25, 0.25, 0.25)); //Cylinder model

	shaderPass.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag"); //Initalises the Reflection shader
	shaderPass.Bind(); //Binds the initalised shader

	setUniform(shaderPass, "model", transform.GetModel()); //Passes in the transform matrix
	setUniform(shaderPass, "view", myCamera.GetView()); //Passes in the view matrix
	setUniform(shaderPass, "projection", myCamera.GetProjection()); //Passes in the projection matrix
	setUniform(shaderPass, "cameraPos", (myCamera.getPos())); //Passes in the camera postion

	//Draws mesh 1
	mesh1.draw();

	//Sets the Positon, Rotation and Scale of Mesh 2
	transform.SetPos(glm::vec3(-sinf(counter), -2.0, 0.5));
	transform.SetRot(glm::vec3(counter, counter * 5, counter * 2));
	//transform.SetScale(glm::vec3(0.05, 0.05, 0.05)); //Hexagon Model
	transform.SetScale(glm::vec3(0.005, 0.005, 0.005)); //Pokeball model
	//transform.SetScale(glm::vec3(0.25, 0.25, 0.25)); //Cylinder model

	shaderPass.init("..\\res\\shaderGooch.vert", "..\\res\\shaderGooch.frag"); //Initalises the Gooch shader	
	shaderPass.Bind(); //Binds the initalised shader
	
	setUniform(shaderPass, "Projection", myCamera.GetProjection()); //Passes in the projection matrix
	setUniform(shaderPass, "ModelViewMatrix", myCamera.GetView() * transform.GetModel()); //Passes in the model view matrix
	setUniform(shaderPass, "NormalMatrix", glm::mat3(glm::transpose(glm::inverse(transform.GetModel())))); //Passes in the Nonral matrix
	setUniform(shaderPass, "lightPos", glm::vec3(-7.0, 0.5, 0.0)); //Passes in a position for the light

	//Draws mesh 2
	mesh2.draw();

	//Sets the Positon, Rotation and Scale of Mesh 3
	transform.SetPos(glm::vec3(2.0, -sinf(counter), -sinf(counter)));
	transform.SetRot(glm::vec3(1.0, counter * 5, 0.0));
	//transform.SetScale(glm::vec3(0.05, 0.05, 0.05)); //Hexagon Model
	//transform.SetScale(glm::vec3(0.005, 0.005, 0.005)); //Pokeball model
	transform.SetScale(glm::vec3(0.25, 0.25, 0.25)); //Cylinder model

	texture.init("..\\res\\iron.jpg"); //load texture
	texture.Bind(1); //bind the texture
	
	shaderPass.init("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.frag", "..\\res\\shaderGeoText.geom"); //Initalises the Geometry shader
	shaderPass.Bind(); //Binds the initalised shader
	 
	setUniform(shaderPass, "time", 5.0f * counter); //Passes in a time affected by the counter
	setUniform(shaderPass, "transform", transform.GetModel()); //Passes in the transform matrix

	shaderPass.Update(transform, myCamera); //Makes use of the Update function within the shader to pass throught the ModelViewProjection Matrix to the shader program

	//Draws mesh 3
	mesh3.draw();

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 