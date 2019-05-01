#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void playAudio(unsigned int Source, glm::vec3 pos);
	void blobEffect();

	void Skybox();

	GLuint skyboxVAO, skyboxVBO, cubemapTexture;
	vector<std::string> faces;

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;

	Texture texture;
	Texture texture1;
	Texture skybox;

	Overlay overlay;
	Shader shaderPass;
	Shader shaderSkybox;

	void setUniform(Shader currentShader, const char *name, float x, float y, float z);
	void setUniform(Shader currentShader, const char *name, const glm::vec3 & v);
	void setUniform(Shader currentShader, const char *name, const glm::mat4 & m);
	void setUniform(Shader currentShader, const char *name, const glm::mat3 & m);
	void setUniform(Shader currentShader, const char *name, float val);
	void setUniform(Shader currentShader, const char *name, int val);

	glm::mat4 modelView;

	Camera myCamera;
	Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
};

