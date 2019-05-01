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
	Audio* audioDevice();
	Texture* texture(); //load texture
	Texture* texture1(); //load texture
	Overlay* overlay(); //load texture
	Shader* shaderPass();
	//Shader* shaderBlur();
	//Shader* shaderToon();
	//Shader* shaderRim();
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
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	//texture.init("..\\res\\bricks.jpg"); //load texture
	//texture.init("..\\res\\water.jpg"); //load texture

	shaderSkybox.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag");

	mesh1.loadModel("..\\res\\Sword.obj");
	mesh2.loadModel("..\\res\\Bowl.obj");
	mesh3.loadModel("..\\res\\plate.obj");
	
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
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
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

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);

	if (AL_PLAYING != state)
	{
		//audioDevice.playSound(Source, pos);
	}
}

void MainGame::setADSLighting()
{
	modelView = transform.GetModel() * myCamera.GetView();
	
	shaderPass.setMat4("ModelViewMatrix", modelView);
	shaderPass.setMat4("ProjectionMatrix", myCamera.GetProjection()); 
	
	glm::mat4 normalMatrix = transpose(inverse(modelView));
	
	shaderPass.setMat4("NormalMatrix", normalMatrix);

	shaderPass.setVec4("Position", glm::vec4(10.0,10.0,10.0,1.0));
	shaderPass.setVec3("Intensity", glm::vec3(0.0, 0.0, 0.0));

	shaderPass.setVec3("ka", glm::vec3(0.5, 0.5, 0.5));
	shaderPass.setVec3("kd", glm::vec3(0.5, 0.5, 0.5));
	shaderPass.setVec3("ks", glm::vec3(0.5, 0.5, 0.5));

	shaderPass.setFloat("Shininess", 0.5);
}

void MainGame::setUniform(Shader currentShader, const char * name, const glm::mat4 & m)
{
	currentShader.setMat4(name, m);
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

void MainGame::blobEffect()
{
	GLuint blockIndex = glGetUniformBlockIndex(shaderBlur.getProgram(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(shaderBlur.getProgram(), blockIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize); //get information about blobsettings and save it in blockSize

	GLubyte * blockBuffer = (GLubyte *)malloc(blockSize); //allocates the requested memory and returns a pointer to it.

														  // Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	glGetUniformIndices(shaderBlur.getProgram(), 4, names, indices); // glGetUniformIndices retrieves the indices of a number of uniforms within program

	GLint offset[4];
	glGetActiveUniformsiv(shaderBlur.getProgram(), 4, indices, GL_UNIFORM_OFFSET, offset); //Returns information about several active uniform variables for the specified program object

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };

	GLfloat innerRadius = 0.0f, outerRadius = 3.0f;

	memcpy(blockBuffer + offset[0], innerColor,
		4 * sizeof(GLfloat)); //destination, source, no of bytes. 
	memcpy(blockBuffer + offset[1], outerColor,
		4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius,
		sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius,
		sizeof(GLfloat));

	GLuint uboHandle;

	glGenBuffers(1, &uboHandle);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
		GL_DYNAMIC_DRAW); //creates and initializes a buffer object's data store - targer, size, data, usage

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle); // bind a buffer object to an indexed buffer target - trager, index, buffer
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	Skybox();

	//Sets positon of Mesh 1
	transform.SetPos(glm::vec3(sinf(counter), 0.5, 0.0));
	transform.SetRot(glm::vec3(1.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

	shaderPass.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	shaderPass.Bind();

	setUniform(shaderPass, "model", transform.GetModel());
	setUniform(shaderPass, "view", myCamera.GetView());
	setUniform(shaderPass, "projection", myCamera.GetProjection());
	setUniform(shaderPass, "cameraPos", (myCamera.getPos()));

	mesh1.draw();

	transform.SetPos(glm::vec3(-sinf(counter), -1.0, -sinf(counter)*5));
	transform.SetRot(glm::vec3(1.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.01, 0.01, 0.01));

	texture.init("..\\res\\china.jpg"); //load texture
	texture.Bind(0);

	shaderPass.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");		
	shaderPass.Bind();
	
	setUniform(shaderPass, "u_vm", myCamera.GetView());
	setUniform(shaderPass, "u_pm", myCamera.GetProjection());

	shaderPass.Update(transform, myCamera);

	mesh2.draw();

	transform.SetPos(glm::vec3(-sinf(counter), -sinf(counter), -sinf(counter)));
	transform.SetRot(glm::vec3(1.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.01, 0.01, 0.01));

	texture.init("..\\res\\iron.jpg"); //load texture
	texture.Bind(1);	
	
	shaderPass.init("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.frag", "..\\res\\shaderGeoText.geom");
	shaderPass.Bind();

	setUniform(shaderPass, "time", 2.0f * counter);
	setUniform(shaderPass, "transform", transform.GetModel());

	shaderPass.Update(transform, myCamera);

	mesh3.draw();

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 