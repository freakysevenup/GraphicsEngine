#pragma once
#include "ViewPort.h"
#include "Mesh.h"
#include "Texture.h"
#include "ShaderNova.h"
#include "Camera3D.h"
#include "Shape.h"
#include "InputManager.h"
#include <GL/glew.h>
#include "AssImpModelLoader.h"
#include "Lighting.h"


enum GameState
{
	PLAY, PAUSE, EXIT
};

class MainGame
{

public:
	MainGame();
	~MainGame();

	void run();
	void init();

	void draw();
	void gameLoop();

	void processInput();

	void drawSphere(
		glm::vec3 pos, 
		glm::vec3 rot, 
		glm::vec3 scale);

	void drawShape(
		Shape * shape, 
		glm::vec3 pos, 
		glm::vec3 rot, 
		glm::vec3 scale, 
		bool glows = false, 
		bool isSkyBox = false,
		bool affectedByNewShader = false);

	void drawCube(
		Shape * shape, 
		Texture & texture, 
		glm::vec3 pos, 
		glm::vec3 rot, 
		glm::vec3 scale);

	void createWall(
		Shape * shape, 
		glm::vec3 startPosition, 
		int wallHeight, 
		int wallLength, 
		bool alongZ, 
		float scaleX = 1.0f, 
		float scaleY = 1.0f, 
		float scaleZ = 1.0f);

private:
	
	float getDeltaTime(float totalDeltaTime);
	void updateInputManager(float deltaTime);
	inline glm::vec2 getGLWindowCoords(float x, float y)
	{
		float X_Coord = x;
		float Y_Coord = y;

		X_Coord -= m_viewPortWidth / 2.0f;
		Y_Coord -= m_viewPortHeight / 2.0f;

		Y_Coord *= -1;

		X_Coord /= 1000.0f;
		Y_Coord /= 1000.0f;

		return glm::vec2(X_Coord, Y_Coord);
	}

	void setCamera3D(Camera3D & camera) { m_cam3D = camera; }
	void setViewPort(ViewPort & viewPort) { m_viewPort = viewPort; }

	void setBasicShaderProgram(ShaderNova & shader) { m_shaderProgram = shader; }
	void setSpotLightShaderProgram(ShaderNova & shader) { m_spotLightShader = shader; }
	void setGlowShaderProgram(ShaderNova & shader) { m_glowShader = shader; }
	void setPointLightShaderProgram(ShaderNova & shader) { m_pointLightShader = shader; }
	void setCubeShaderProgram(ShaderNova & shader) { m_cubeShader = shader; }
	void setNewShaderProgram(ShaderNova & shader) { m_newShader = shader; }

	ShaderNova m_newShader, m_shaderProgram, m_spotLightShader, m_glowShader, m_pointLightShader, m_cubeShader;

	ViewPort m_viewPort;
	int m_viewPortWidth;
	int m_viewPortHeight;
	GameState m_gameState;

	InputManager m_inputManager;

	float m_counter = 0.0f;
	float m_deltaTime = 0.0f;
	Uint32 m_previousTicks = 0;

	Camera3D m_cam3D;
	float m_camFov = 0.0f;
	glm::vec2 m_offset;

	Texture 
		m_moonTex, 
		m_earthTex, 
		m_sunTex, 
		m_mercTex, 
		m_venTex, 
		m_marsTex, 
		m_groundTex, 
		m_crystalTex, 
		m_cellTex, 
		m_bricksTex,
		m_gamma;

	Shape m_moon, m_earth, m_sun, m_mercury, m_venus, m_mars;

	Texture m_cubeTex, m_cubeSkyTex; // TEXTURE MAP
	Texture m_skyDomeTex;
	Shape m_box, m_pyramid, m_cube, m_skyDome, m_ground, m_ground2;

	int m_numBoxes = 0;
	std::vector<Shape> m_boxes;

	bool twoPressed = false;
	bool drawWalls = false;
	bool drawPyramid = false;
	bool drawBoxPyramid = false;
	bool drawGround = false;
	bool drawCubeShape = false;
	bool m_updateSun = false;

	Light light;
	glm::vec3 m_intensity = glm::vec3(15.0f);

	float m_pointLightIntensity = 1.0f;

	glm::vec2 m_pixels = glm::vec2(100.0f, 100.0f);

	glm::vec3 m_sunPos = glm::vec3(0.5f);

	ModelMesh m_mario_assimp, m_luigi_assimp;
	Lighting m_effect[4];
	DirectionalLight m_directionalLight;
	PointLight m_pointLights[2];
	SpotLight m_spotLights[2];

	Transform m_world;

};
