#include "MainGame.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <GLM/gtc/type_ptr.hpp>

const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime
const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
const float MS_PER_SECOND = 1000.0f; // Number of milliseconds in a second
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame

#define LEFT_ROTATION_PANE m_inputManager.getMouseCoords().x < 700
#define RIGHT_ROTATION_PANE m_inputManager.getMouseCoords().x > m_viewPortWidth - 700

#define TOP_ROTATION_PANE m_inputManager.getMouseCoords().y < 200
#define BOTTOM_ROTATION_PANE m_inputManager.getMouseCoords().y > m_viewPortHeight - 200

MainGame::MainGame() :
m_viewPortWidth(1600),
m_viewPortHeight(900),
m_camFov(70.0f), 
m_numBoxes(15)
{ 
	/* Empty */
}

// TODO : Shadow Mapping
// TODO : Deferred Shading
// TODO : Multipass Shadow Mapping
// TODO : Vertex Array Objects
// TODO : Animations
// TODO : Object Motion Blur
// TODO : Billboarding and the Geometry Shader
// TODO : Tessalation
// TODO : Particle System
// TODO : Instanced Rendering
// TODO : Scene Graph
// TODO : GLFX ???
// TODO : Keep Doing Tutorials here http://ogldev.atspace.co.uk/index.html

MainGame::~MainGame()
{
	delete[] m_effect;
	delete[] m_pointLights;
	delete[] m_spotLights;
}

void MainGame::run()
{
	std::string title = "First Window Test";

	ViewPort display(title, m_viewPortWidth, m_viewPortHeight, 0);

	setViewPort(display);

	m_previousTicks = SDL_GetTicks();

	init();
}

void MainGame::init()
{
	m_world.setScale(glm::vec3(20000.0f));
	m_world.setRotation(glm::vec3(0.0f, 0.5f, 0.0f));
	m_world.setPosition(glm::vec3(0.5f, 0.5f, 0.5f));

	m_effect[0] = Lighting();
	m_effect[1] = Lighting();
	m_effect[2] = Lighting();
	m_effect[3] = Lighting();

	if (!m_effect[0].init())
	{
		std::cout << "Ambient Lighting effect 1 initialized" << std::endl;
	}
	if (!m_effect[1].init())
	{
		std::cout << "Ambient Lighting effect 2 initialized" << std::endl;
	}
	if (!m_effect[2].init())
	{
		std::cout << "Ambient Lighting effect 3 initialized" << std::endl;
	}
	if (!m_effect[3].init())
	{
		std::cout << "Ambient Lighting effect 4 initialized" << std::endl;
	}

	m_directionalLight.ambientIntensity = 0.1f;
	m_directionalLight.colour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.diffuseIntensity = 0.1f;
	m_directionalLight.direction = glm::vec3(1.0f, -1.0f, 0.0);

	m_pointLights[0].diffuseIntensity = 1.5f;
	m_pointLights[0].colour = glm::vec3(1.0f, 0.0f, 0.0f);
	m_pointLights[0].attenuation.constant = 0.1f;
	m_pointLights[1].diffuseIntensity = 1.5f;
	m_pointLights[1].colour = glm::vec3(1.0f, 1.0f, 0.0f);
	m_pointLights[1].attenuation.constant = 0.1f;

	m_spotLights[0].diffuseIntensity = 0.9f;
	m_spotLights[0].colour = glm::vec3(0.0f, 0.0f, 1.0f);
	m_spotLights[0].attenuation.constant = 0.1f;
	m_spotLights[0].cutoff = 10.0f;
	
	m_spotLights[1].diffuseIntensity = 0.9f;
	m_spotLights[1].colour = glm::vec3(0.0f, 1.0f, 0.0f);
	m_spotLights[1].attenuation.constant = 0.1f;
	m_spotLights[1].cutoff = 20.0f;

	if (m_mario_assimp.LoadMesh(
		"./Assets/Models/Alien2/alien2.obj",
		"./Assets/Models/Alien2/"))
	{
		std::cout << "The Assimp Model Loader loaded a file." << std::endl;
	}

	if (m_luigi_assimp.LoadMesh(
		"./Assets/Models/Alien2/alien2.obj",
		"./Assets/Models/Alien2/"))
	{
		std::cout << "The Assimp Model Loader loaded a file." << std::endl;
	}

	std::vector <std::string> cubeTextures;
	cubeTextures.push_back("./Assets/SkyBoxes/helloKitty/front.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/helloKitty/back.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/helloKitty/top.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/helloKitty/bottom.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/helloKitty/left.jpg");
	cubeTextures.push_back("./Assets/SkyBoxes/helloKitty/right.jpg");

	Texture cubeTexture;
	cubeTexture.createCubeMap(
		cubeTextures[0].c_str(),
		cubeTextures[1].c_str(),
		cubeTextures[2].c_str(),
		cubeTextures[3].c_str(),
		cubeTextures[4].c_str(),
		cubeTextures[5].c_str()
		);

	m_cubeTex = cubeTexture;

	std::vector <std::string> cubeSkyTextures;
	cubeSkyTextures.push_back("./Assets/SkyBoxes/black/front.jpg");
	cubeSkyTextures.push_back("./Assets/SkyBoxes/black/back.jpg");
	cubeSkyTextures.push_back("./Assets/SkyBoxes/black/top.jpg");
	cubeSkyTextures.push_back("./Assets/SkyBoxes/black/bottom.jpg");
	cubeSkyTextures.push_back("./Assets/SkyBoxes/black/left.jpg");
	cubeSkyTextures.push_back("./Assets/SkyBoxes/black/right.jpg");

	Texture cubeSkyTexture;
	cubeSkyTexture.createCubeMap(
		cubeSkyTextures[0].c_str(),
		cubeSkyTextures[1].c_str(),
		cubeSkyTextures[2].c_str(),
		cubeSkyTextures[3].c_str(),
		cubeSkyTextures[4].c_str(),
		cubeSkyTextures[5].c_str()
		);

	m_cubeSkyTex = cubeSkyTexture;


	////SHAPE INITIALIZATION///////////////////////////////////////////////////

	///// SKY DOME ////////////////////////////////////////////////////////////

	m_skyDome.createShape(SPHERE, 15000.0f, 15000.0f, 512);
	Mesh skyDomeMesh(m_skyDome.getVertices(), m_skyDome.getVertices().size());

	///////////////////////////////////////////////////////////////////////////

	//// PLANETS //////////////////////////////////////////////////////////////
	m_ground.createShape(CUBOID, 1.0f, 10.0f, 16, 50.0f, 1.0f, 50.0f);
	Mesh groundMesh(m_ground.getVertices(), m_ground.getVertices().size());

	m_ground2.createShape(CUBOID, 1.0f, 10.0f, 16, 5.0f, 1.0f, 5.0f);
	Mesh ground2Mesh(m_ground2.getVertices(), m_ground2.getVertices().size());

	m_moon.createShape(SPHERE, 0.075f, 512);
	Mesh moonMesh(m_moon.getVertices(), m_moon.getVertices().size());

	m_sun.createShape(SPHERE, 0.5f, 512);
	Mesh sunMesh(m_sun.getVertices(), m_sun.getVertices().size());

	m_earth.createShape(SPHERE, 0.15f, 512);
	Mesh earthMesh(m_earth.getVertices(), m_earth.getVertices().size());

	m_mercury.createShape(SPHERE, 0.1f, 512);
	Mesh mercuryMesh(m_mercury.getVertices(), m_mercury.getVertices().size());

	m_venus.createShape(SPHERE, 0.15f, 512);
	Mesh venusMesh(m_venus.getVertices(), m_venus.getVertices().size());

	m_mars.createShape(SPHERE, 0.11f, 512);
	Mesh marsMesh(m_mars.getVertices(), m_mars.getVertices().size());

	//// BOXES ///////////////////////////////////////////////////////////////////

	m_boxes.resize(m_numBoxes);
	std::vector<Mesh> meshes;
	for (int i = 0; i < m_numBoxes; i++)
	{
		m_boxes[i].createShape(CUBE, 1.0f);
		meshes.emplace_back(m_boxes[i].getVertices(), m_boxes[i].getVertices().size());

	}

	///// PYRAMIDS ///////////////////////////////////////////////////////////////

	m_pyramid.createShape(PYRAMID, 10.0f);
	Mesh pyramid2Mesh(m_pyramid.getVertices(), m_pyramid.getVertices().size());

	m_cube.createShape(TEXTURED_CUBE, 10.0f, 0.0f);
	Mesh cubeMesh(m_cube.getVertices(), m_cube.getVertices().size());

	//////////////////////////////////////////////////////////////////////////////

	////TEXTURE INITIALIZATION////////////////////////////////////////////////////

	Texture earthTexture("./Assets/Textures/earth2.png");
	Texture moonTexture("./Assets/Textures/moon2.jpg");
	Texture sunTexture("./Assets/Textures/sun.jpg");
	Texture ground("./Assets/Textures/seamless_grass.jpg");
	Texture mercuryTex("./Assets/Textures/mercury.jpg");
	Texture venusTex("./Assets/Textures/venus.png");
	Texture marsTex("./Assets/Textures/mars.jpg");
	Texture crystal("./Assets/Textures/crystal.jpg");
	Texture cells("./Assets/Textures/seamless_lava.jpg");
	Texture whiteBricks("./Assets/Textures/white_bricks.jpg");
	Texture radioactive("./Assets/Textures/plane.jpg");
	Texture nightSkyDome("./Assets/Textures/night_sky.png");
	Texture ballSpongeBob("./Assets/Textures/seamless_lava.jpg");


	m_earthTex = earthTexture;
	m_moonTex = moonTexture;
	m_sunTex = sunTexture;
	m_groundTex = ground;
	m_mercTex = mercuryTex;
	m_venTex = venusTex;
	m_marsTex = marsTex;
	m_crystalTex = crystal;
	m_cellTex = cells;
	m_bricksTex = whiteBricks;
	m_gamma = radioactive;
	m_skyDomeTex = nightSkyDome;

	/////////////////////////////////////////////////////////////////////////////

	////SHADER INITIALIZATION////////////////////////////////////////////////////

	ShaderNova spotLightShader;

	std::vector<std::string> spotLightShaders;

	spotLightShaders.push_back("./Assets/Shaders/spotLightVert.glsl");
	spotLightShaders.push_back("./Assets/Shaders/spotLightFrag.glsl");

	spotLightShader.compileShadersFromFile(spotLightShaders);

	spotLightShader.addAttributes("vert");
	spotLightShader.addAttributes("vertTexCoord");
	spotLightShader.addAttributes("vertNormal");
	spotLightShader.linkShaders();

	setSpotLightShaderProgram(spotLightShader);

	ShaderNova glowShader;

	std::vector<std::string> glowShaders;

	glowShaders.push_back("./Assets/Shaders/glowShaderVert.glsl");
	glowShaders.push_back("./Assets/Shaders/glowShaderFrag.glsl");

	glowShader.compileShadersFromFile(glowShaders);
	glowShader.addAttributes("position");
	glowShader.addAttributes("textureCoords");
	glowShader.addAttributes("normal");
	glowShader.linkShaders();

	setGlowShaderProgram(glowShader);

	ShaderNova mainShader;

	std::vector<std::string> shaders;
	shaders.push_back("./Assets/Shaders/basicShaderVert.glsl");
	shaders.push_back("./Assets/Shaders/basicShaderFrag.glsl");

	mainShader.compileShadersFromFile(shaders);
	mainShader.addAttributes("position");
	mainShader.addAttributes("textureCoords");
	mainShader.addAttributes("normal");
	mainShader.linkShaders();

	setBasicShaderProgram(mainShader);

	ShaderNova pointLightShader;

	std::vector<std::string> pointLightShaders;
	pointLightShaders.push_back("./Assets/Shaders/pointLightVert.glsl");
	pointLightShaders.push_back("./Assets/Shaders/pointLightFrag.glsl");

	pointLightShader.compileShadersFromFile(pointLightShaders);
	pointLightShader.addAttributes("position");
	pointLightShader.addAttributes("textureCoords");
	pointLightShader.addAttributes("normal");
	pointLightShader.linkShaders();

	setPointLightShaderProgram(pointLightShader);

	ShaderNova cubeShader;

	std::vector<std::string> cubeShaders;

	cubeShaders.push_back("./Assets/Shaders/cubeShaderVertex.glsl");
	cubeShaders.push_back("./Assets/Shaders/cubeShaderFragment.glsl");

	cubeShader.compileShadersFromFile(cubeShaders);

	cubeShader.addAttributes("position");
	cubeShader.addAttributes("textureCoords");
	cubeShader.addAttributes("normal");
	cubeShader.linkShaders();

	setCubeShaderProgram(cubeShader);

	ShaderNova multiplePLShader;

	std::vector<std::string> multiplePLShaders;

	multiplePLShaders.push_back("./Assets/Shaders/dualConeSpotLightVertex.glsl");
	multiplePLShaders.push_back("./Assets/Shaders/dualConeSpotLightFrag.glsl");

	multiplePLShader.compileShadersFromFile(multiplePLShaders);

	multiplePLShader.addAttributes("position");
	multiplePLShader.addAttributes("texCoords");
	multiplePLShader.addAttributes("normal");
	multiplePLShader.linkShaders();

	setNewShaderProgram(multiplePLShader);


	//////////////////////////////////////////////////////////////////////////////////

	////CAMERA INITIALIZATION/////////////////////////////////////////////////////////

	m_cam3D.setPosition(glm::vec3(0.0f, 50.0f, 400.0f));
	m_cam3D.setFieldOfView(m_camFov);
	m_cam3D.setViewportAspectRatio((float)m_viewPortWidth / (float)m_viewPortHeight);
	m_cam3D.setNearAndFarPlanes(1.0f, 20000.0f);  
	/*
	
	   Near plane needs to be a number greater than zero,
	   if it is a floating point number less than 1.0 it must round up
	   to 1.0. Otherwise some machines will treat it as if it is 0, if it is zero
	   the depth testing will stop working and shapes, models and shading will all 
	   be wonky.
	
	*/


	gameLoop();

}

void MainGame::drawShape(
	Shape * shape, 
	glm::vec3 pos, 
	glm::vec3 rot, 
	glm::vec3 scale, 
	bool glows /* = false */, 
	bool isSkyDome /* = false */,
	bool affectedByNewShader /* = false */)
{

	if (isSkyDome)
	{
		m_shaderProgram.startUse();

		shape->setPosition(pos);
		shape->setRotation(rot);
		shape->setScale(scale);

		GLint camUniform = m_shaderProgram.getUniformLocation("camera");
		glm::mat4 cameraMatrix =
			m_cam3D.matrix() * shape->getModel();
		glUniformMatrix4fv(camUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

		shape->draw();

		m_shaderProgram.stopUse();

		
	}
	else if (glows)
	{
		m_glowShader.startUse();

		shape->setPosition(pos);
		shape->setRotation(rot);
		shape->setScale(scale);

		GLint camUniform = m_glowShader.getUniformLocation("camera");
		glm::mat4 cameraMatrix =
			m_cam3D.matrix() * shape->getModel();
		glUniformMatrix4fv(camUniform, 1, GL_FALSE, &cameraMatrix[0][0]);
		GLint deezPixelDims = m_glowShader.getUniformLocation("pixelDims");
		glUniform2fv(deezPixelDims, 1, (GLfloat*)&(glm::vec2(0.01f, 0.01f)));

		shape->draw();


		m_glowShader.stopUse();
	}
	else if (affectedByNewShader)
	{

		//m_newShader.startUse();

		/*shape->setPosition(pos);
		shape->setRotation(rot);
		shape->setScale(scale);

		GLint camUniform = m_newShader.getUniformLocation("camera");
		glm::mat4 cameraMatrix = m_cam3D.matrix() * shape->getModel();
		glUniformMatrix4fv(camUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

		GLint lightUniform = m_newShader.getUniformLocation("light_position");
		glm::vec3 lightPosition = glm::vec3(glm::vec3(
			m_sun.getPosition().x,
			m_sun.getPosition().y,
			m_sun.getPosition().z));
		glUniform3fv(lightUniform, 1, &lightPosition[0]);

		GLint lightPower = m_newShader.getUniformLocation("lightPower");
		float lightPow = 1.25f;
		glUniform1fv(lightPower, 1, (GLfloat*)&lightPow);

		shape->draw();*/

		//m_newShader.stopUse();

	}
	else
	{
		m_pointLightShader.startUse();

		shape->setPosition(pos);
		shape->setRotation(rot);
		shape->setScale(scale);

		light.position = glm::vec3(m_cam3D.position());
		light.intensities = m_intensity;

		 GLint camUniform = m_pointLightShader.getUniformLocation("camera");

		 glm::mat4 cameraMatrix = m_cam3D.matrix() * shape->getModel();

		 glUniformMatrix4fv(camUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

		GLint modelUniform = m_pointLightShader.getUniformLocation("model");

		glm::mat4 modelMatrix = shape->getModel();

		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &modelMatrix[0][0]);

		GLint lightUniform = m_pointLightShader.getUniformLocation("lightSource");

		glm::vec3 lightPosition = m_sun.getPosition();

		glUniform3fv(lightUniform, 1, (GLfloat*)&lightPosition);

		GLint lightIntense = m_pointLightShader.getUniformLocation("lightIntensity");

		glUniform1fv(lightIntense, 1, &m_pointLightIntensity);

		shape->draw();

		m_pointLightShader.stopUse();

		/*m_spotLightShader.startUse();

		light.position = glm::vec3(m_sun.getPosition());
		light.intensities = m_intensity;

		GLint camUniform = m_spotLightShader.getUniformLocation("camera");

		glm::mat4 cameraMatrix =
			m_cam3D.matrix() * shape->getModel();

		glUniformMatrix4fv(camUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

		GLint modelUniform = m_spotLightShader.getUniformLocation("model");

		glm::mat4 modelMatrix = shape->getModel();

		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &modelMatrix[0][0]);

		GLint lightIntense = m_spotLightShader.getUniformLocation("light.intensities");

		glUniform3fv(lightIntense, 1, glm::value_ptr(light.intensities));

		GLint lightPosition = m_spotLightShader.getUniformLocation("light.position");

		glUniform3fv(lightPosition, 1, glm::value_ptr(light.position));

		shape->setPosition(pos);
		shape->setRotation(rot);
		shape->setScale(scale);

		shape->draw();

		m_spotLightShader.stopUse();*/
	}
	
}

void MainGame::drawCube(
	Shape * shape, 
	Texture & texture, 
	glm::vec3 pos, 
	glm::vec3 rot, 
	glm::vec3 scale)
{
	m_cubeShader.startUse();

	shape->setPosition(pos);
	shape->setRotation(rot);
	shape->setScale(scale);

	GLint camUniform = m_cubeShader.getUniformLocation("camera");
	glm::mat4 cameraMatrix = m_cam3D.matrix() * shape->getModel();
	glUniformMatrix4fv(camUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	texture.bindCube(0);

	shape->draw();

	m_cubeShader.stopUse();
}

void MainGame::createWall(
	Shape * shape,
	glm::vec3 startPosition, 
	int wallHeight, 
	int wallLength, 
	bool alongZ, 
	float scaleX /* = 1.0f */, 
	float scaleY /* = 1.0f */,
	float scaleZ /* = 1.0f */)
{

	float spread = (float)wallLength;
	float height = 0.0f;

	if (alongZ)
	{
		for (int i = 0; i < wallHeight; i++)
		{
			for (int j = 0; j < wallLength; j++)
			{
				drawShape(
					shape,
					startPosition + glm::vec3(0.0f, height * scaleY, spread * scaleZ),
					glm::vec3(sinf(m_counter)),
					glm::vec3(scaleX, scaleY, scaleZ));
				spread -= 2.0f;
			}
			spread = (float)wallLength;
			height += 2.0f;
		}
	}
	else
	{
		for (int i = 0; i < wallHeight; i++)
		{
			for (int j = 0; j < wallLength; j++)
			{
				drawShape(
					shape,
					startPosition + glm::vec3(spread * scaleX, height * scaleY, 0.0f),
					glm::vec3(0.0f, cosf(m_counter * 2.0f), m_counter),
					glm::vec3(scaleX, scaleY, scaleZ));
				spread -= 2.0f;
			}
			spread = (float)wallLength;
			height += 2.0f;
		}
	}
	
}

void MainGame::processInput()
{
	// Calculate the frameTime in milliseconds
	Uint32 newTicks = SDL_GetTicks();
	float frameTime = (float)newTicks - (float)m_previousTicks;

	// Store newTicks in previousTicks so we can use it next frame
	m_previousTicks = newTicks;

	// Get the total delta time
	float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
	int i = 0;

	while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
	{
		m_deltaTime = getDeltaTime(totalDeltaTime);
		i++;
	}
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords((float)e.motion.x, (float)e.motion.y);
			m_offset = getGLWindowCoords((float)e.motion.x, (float)e.motion.y);
			break;
		case SDL_MOUSEWHEEL:
			if ((m_cam3D.fieldOfView() - e.wheel.y) > m_camFov - 20.0f &&
				(m_cam3D.fieldOfView() - e.wheel.y) < m_camFov + 20.0f)
				m_cam3D.setFieldOfView(m_cam3D.fieldOfView() - e.wheel.y * 2);
			break;
		case SDL_KEYDOWN:
			m_inputManager.keyPressed(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.keyReleased(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.keyPressed(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.keyReleased(e.button.button);
			break;
		default:
			break;
		}


		if (e.type == SDL_WINDOWEVENT) {
			switch (e.window.event) {
			case SDL_WINDOWEVENT_SHOWN:
				SDL_Log("Window %d shown", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				SDL_Log("Window %d hidden", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_Log("Window %d exposed", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_MOVED:
				SDL_Log("Window %d moved to %d,%d",
					e.window.windowID, e.window.data1,
					e.window.data2);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				SDL_Log("Window %d resized to %dx%d",
					e.window.windowID, e.window.data1,
					e.window.data2);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				SDL_Log("Window %d size changed to %dx%d",
					e.window.windowID, e.window.data1,
					e.window.data2);
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				SDL_Log("Window %d minimized", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				SDL_Log("Window %d maximized", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				SDL_Log("Window %d restored", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_ENTER:
				SDL_Log("Mouse entered window %d",
					e.window.windowID);
				break;
			case SDL_WINDOWEVENT_LEAVE:
				SDL_Log("Mouse left window %d", e.window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				SDL_Log("Window %d gained keyboard focus",
					e.window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				SDL_Log("Window %d lost keyboard focus",
					e.window.windowID);
				break;
			case SDL_WINDOWEVENT_CLOSE:
				SDL_Log("Window %d closed", e.window.windowID);
				break;
			default:
				SDL_Log("Window %d got unknown event %d",
					e.window.windowID, e.window.event);
				break;
			}
		}
	}
}

void MainGame::draw()
{
	glm::vec3 startLightPos = glm::vec3(-50.0f, 0.5f, -20.0f);

	m_pointLights[0].position = m_earth.getPosition();
	m_pointLights[1].position = m_venus.getPosition();

	m_spotLights[0].position = m_mercury.getPosition();
	m_spotLights[0].direction = glm::normalize(m_ground2.getPosition() - m_mercury.getPosition());
	
	m_spotLights[1].position = m_mars.getPosition();
	m_spotLights[1].direction = glm::normalize(m_ground2.getPosition() - m_mars.getPosition());

	///// AMBIENT - DIFFUSE - SPECULAR LIGHTING EXAMPLE //////////////////////////////////////////

	m_mario_assimp.setPosition(m_world.getPosition() + glm::vec3(-700.0f, 10.0f, 0.0f));
	m_mario_assimp.setRotation(glm::vec3(m_counter));
	m_mario_assimp.setScale(glm::vec3(1.0f));

	m_effect[0].enable();
	m_effect[0].setTextureID(0);
	m_effect[0].setWorldViewProjection(m_cam3D.matrix() * (m_mario_assimp.getModel()));
	m_effect[0].setWorldMatrix(glm::mat4(1.0f) * m_mario_assimp.getModel());

	m_directionalLight.direction = glm::normalize(m_mario_assimp.getPosition() - m_earth.getPosition());

	m_effect[0].setDirectionalLight(m_directionalLight);

	m_mario_assimp.Render();

	m_effect[0].disable();

	//////////  ////////////  /////////////  /////////////  ///////////////  /////////  //////////

	m_luigi_assimp.setPosition(glm::vec3(700.0f, 10.0f, 0.0f));
	m_luigi_assimp.setRotation(glm::vec3(m_counter));
	m_luigi_assimp.setScale(glm::vec3(1.0f));

	m_effect[1].enable();
	m_effect[1].setTextureID(0);
	m_effect[1].setWorldViewProjection(m_cam3D.matrix() * (m_luigi_assimp.getModel()));
	m_effect[1].setWorldMatrix(glm::mat4(1.0f) * m_luigi_assimp.getModel());

	m_directionalLight.direction = glm::normalize(m_luigi_assimp.getPosition() - m_earth.getPosition());

	m_effect[1].setDirectionalLight(m_directionalLight);

	m_luigi_assimp.Render();

	m_effect[1].disable();

	///// EXAMPLE ENDS /////

	///// POINTLIGHTS EXAMPLE//////////////////////////////////////////

	m_bricksTex.bind2D(0);
	m_ground.setPosition(glm::vec3(0.5f, -100.0f, 0.5f));
	m_ground.setRotation(glm::vec3(0.0f));
	m_ground.setScale(glm::vec3(20.0f));

	m_effect[2].enable();
	m_effect[2].setTextureID(0);
	m_effect[2].setWorldViewProjection(m_cam3D.matrix() * (m_ground.getModel()));
	m_effect[2].setWorldMatrix(glm::mat4(1.0f) * m_ground.getModel());

	m_directionalLight.direction = glm::normalize(m_ground.getPosition() - m_sun.getPosition());

	m_effect[2].setEyeWorldPos(m_cam3D.position());
	m_effect[2].setMatSpecularIntensity(5.0f);
	m_effect[2].setMatSpecularPower(10.0f);

	m_effect[2].setDirectionalLight(m_directionalLight);
	m_effect[2].SetPointLights(2, m_pointLights);

	m_ground.draw();

	m_effect[2].disable();

	///// EXAMPLE ENDS /////

	///// SPOTLIGHTS EXAMPLE //////////////////////////////////////////////////////////////////////////////

	m_bricksTex.bind2D(0);
	m_ground2.setPosition(glm::vec3(50.0f, -90.0f, 50.0f));
	m_ground2.setRotation(glm::vec3(0.0f));
	m_ground2.setScale(glm::vec3(20.0f));

	m_effect[3].enable();
	m_effect[3].setTextureID(0);
	m_effect[3].setWorldViewProjection(m_cam3D.matrix() * (m_ground2.getModel()));
	m_effect[3].setWorldMatrix(glm::mat4(1.0f) * m_ground2.getModel());

	m_effect[3].setEyeWorldPos(m_cam3D.position());
	m_effect[3].setMatSpecularIntensity(5.0f);
	m_effect[3].setMatSpecularPower(10.0f);

	m_effect[3].SetSpotLights(2, m_spotLights);

	m_ground2.draw();

	m_effect[3].disable();

	///// EXAMPLE ENDS /////

	/*
	
	m_bricksTex.bind2D(0);
	m_ground.setPosition(glm::vec3(-0.5f, -10.0f, 0.5f));
	m_ground.setRotation(glm::vec3(0.0f));
	m_ground.setScale(glm::vec3(20.0f));

	m_effect[2].enable();
	m_effect[2].setTextureID(0);
	m_effect[2].setWorldViewProjection(m_cam3D.matrix() * (m_ground.getModel()));
	m_effect[2].setWorldMatrix(glm::mat4(1.0f) * m_ground.getModel());

	m_directionalLight.direction = glm::normalize(m_ground.getPosition() - m_sun.getPosition());

	m_effect[2].setEyeWorldPos(m_cam3D.position());
	m_effect[2].setMatSpecularIntensity(5.0f);
	m_effect[2].setMatSpecularPower(10.0f);

	m_effect[2].setDirectionalLight(m_directionalLight);

	m_ground.draw();

	m_effect[2].disable();
	
	*/


	//if (drawCubeShape)
	//{
	//	float spread;
	//	for (int i = 0; i < 10; i++)
	//	{
	//		spread = i + 100.0f;

	//		drawCube(
	//			&m_cube,
	//			m_cubeTex,
	//			glm::vec3(
	//			m_sun.getSize() + m_sun.getPosition().x + (sinf(m_counter * 2) * 2.0f * m_sun.getScale().x) * spread,
	//			m_sun.getSize() + m_sun.getPosition().y - 1.0f + spread * 2.0f,
	//			m_sun.getSize() + m_sun.getPosition().z + (cosf(m_counter * 2)) * 2.0f * m_sun.getScale().z) / spread,
	//			glm::vec3(sinf(m_counter), cosf(m_counter), sinf(m_counter)),
	//			glm::vec3(1.0f));

	//		spread++;
	//	}

	//	
	//}

	/*if (drawBoxPyramid)
	{
		{
			float spread = -10.0f;
			float height = 0.0f;

			m_gamma.bind2D(0);

			for (int i = 0; i < 6; i++)
			{
				drawShape(
					&m_boxes[0],
					glm::vec3(spread, height, 10.0f),
					glm::vec3(0.0f),
					glm::vec3(1.0f));

				spread += 2.5f;
			}

			spread = -8.75f;
			height += 2.0f;

			for (int i = 0; i < 5; i++)
			{
				drawShape(
					&m_boxes[0],
					glm::vec3(spread, height, 10.0f),
					glm::vec3(0.0f),
					glm::vec3(1.0f));

				spread += 2.5f;
			}

			spread = -7.5f;
			height += 2.0f;

			for (int i = 0; i < 4; i++)
			{
				drawShape(
					&m_boxes[0],
					glm::vec3(spread, height, 10.0f),
					glm::vec3(0.0f),
					glm::vec3(1.0f));

				spread += 2.5f;
			}

			spread = -6.25f;
			height += 2.0f;

			for (int i = 0; i < 3; i++)
			{
				drawShape(
					&m_boxes[0],
					glm::vec3(spread, height, 10.0f),
					glm::vec3(0.0f),
					glm::vec3(1.0f));

				spread += 2.5f;
			}

			spread = -5.0f;
			height += 2.0f;

			for (int i = 0; i < 2; i++)
			{
				drawShape(
					&m_boxes[0],
					glm::vec3(spread, height, 10.0f),
					glm::vec3(0.0f),
					glm::vec3(1.0f));

				spread += 2.5f;
			}

			spread = -3.75f;
			height += 2.0f;

			drawShape(
				&m_boxes[0],
				glm::vec3(spread, height, 10.0f),
				glm::vec3(0.0f),
				glm::vec3(1.0f));

		}
	}*/

	{
		if (drawWalls)
		{
			m_crystalTex.bind2D(0);
			createWall(&m_boxes[1], glm::vec3(-50.0f, -4.75f, 50.0f), 10, 10, true);

			m_bricksTex.bind2D(0);
			createWall(&m_boxes[2], glm::vec3(50.0f, -4.75f, -50.0f), 20, 20, false);

			m_cellTex.bind2D(0);
			createWall(&m_boxes[3], glm::vec3(50.0f, -4.75f, 50.0f), 30, 30, true);
		}
	}

	{
		if (drawPyramid)
		{
			m_gamma.bind2D(0);
			drawShape(
				&m_pyramid,
				glm::vec3(0.0f, 0.0f, 50.0),
				glm::vec3(sinf(m_counter), cosf(m_counter), sinf(m_counter)),
				glm::vec3(1.0f));
		}

	}

	{

		/////PLANETS DRAW CODE////////////////////////////////////////////////////////

		float solarSystemScale = 20.0f;

		////MOON SHAPE DRAW CODE///////////////////////////////////////////////

		m_moonTex.bind2D(0);
		drawShape(
			&m_moon,
			glm::vec3(
			m_earth.getPosition().x + (sinf(m_counter * 2) * m_earth.getScale().x),
			m_earth.getSize() + m_earth.getPosition().y,
			m_earth.getPosition().z + (cosf(m_counter * 2)) * m_earth.getScale().z),
			glm::vec3(0.0f, m_counter * 2.0f, 0.0f),
			glm::vec3(solarSystemScale));

		///////////////////////////////////////////////////////////////////////////

		////SUN SHAPE DRAW CODE///////////////////////////////////////////////

		if (m_updateSun)
		{
			m_sunPos = glm::vec3(
				0.0f + sinf(m_counter * 2.0f) * 150.0f,
				0.0f + cosf(m_counter * 15.0f) * 100.0f,
				0.0f + cosf(m_counter * 3.1415f) * 200.0f);
		}

		m_sunTex.bind2D(0);
		drawShape(
			&m_sun,
			m_sunPos,
			glm::vec3(0.0f, m_counter, 0.0f),
			glm::vec3(solarSystemScale),
			true);


		///////////////////////////////////////////////////////////////////////////

		////MERCURY SHAPE DRAW CODE///////////////////////////////////////////////

		m_mercTex.bind2D(0);
		drawShape(
			&m_mercury,
			glm::vec3(
			m_sun.getSize() + m_sun.getPosition().x + (sinf(m_counter * 2) * 8.0f * m_sun.getScale().x),
			m_sun.getSize() + m_sun.getPosition().y - 1.0f,
			m_sun.getSize() + m_sun.getPosition().z + (cosf(m_counter * 2)) * 8.0f * m_sun.getScale().z),
			glm::vec3(0.0f, m_counter * 4, 0.0f),
			glm::vec3(solarSystemScale));

		///////////////////////////////////////////////////////////////////////////

		////VENUS SHAPE DRAW CODE///////////////////////////////////////////////

		m_venTex.bind2D(0);
		drawShape(
			&m_venus,
			glm::vec3(
			m_sun.getSize() + m_sun.getPosition().x + (sinf(m_counter) * 18.0f * m_sun.getScale().x),
			m_sun.getSize() + m_sun.getPosition().y - 1.0f,
			m_sun.getSize() + m_sun.getPosition().z + (cosf(m_counter)) * 18.0f * m_sun.getScale().z),
			glm::vec3(0.0f, m_counter * 3, 0.0f),
			glm::vec3(solarSystemScale));

		///////////////////////////////////////////////////////////////////////////

		////EARTH SHAPE DRAW CODE///////////////////////////////////////////////

		m_earthTex.bind2D(0);
		drawShape(
			&m_earth,
			glm::vec3(
			m_sun.getSize() + m_sun.getPosition().x + (sinf(m_counter / 3) * 25.0f * m_sun.getScale().x),
			m_sun.getSize() + m_sun.getPosition().y - 1.0f,
			m_sun.getSize() + m_sun.getPosition().z + (cosf(m_counter / 3)) * 25.0f * m_sun.getScale().z),
			glm::vec3(0.0f, m_counter, 0.0f),
			glm::vec3(solarSystemScale));

		///////////////////////////////////////////////////////////////////////////

		////MARS SHAPE DRAW CODE///////////////////////////////////////////////

		m_marsTex.bind2D(0);
		drawShape(
			&m_mars,
			glm::vec3(
			m_sun.getSize() + m_sun.getPosition().x + (sinf(m_counter / 5) * 35.0f * m_sun.getScale().x),
			m_sun.getSize() + m_sun.getPosition().y - 1.0f,
			m_sun.getSize() + m_sun.getPosition().z + (cosf(m_counter / 5)) * 35.0f * m_sun.getScale().z),
			glm::vec3(0.0f, m_counter * 2, 0.0f),
			glm::vec3(solarSystemScale));

		///////////////////////////////////////////////////////////////////////////

		//GROUND PLANE DRAW CODE/////////////////////////////////////////////////

		//if (drawGround)
		//{
		//	m_groundTex.bind2D(0);
		//	drawShape(
		//		&m_ground,
		//		glm::vec3(0.0f, -250.0f, -250.0f),
		//		glm::vec3(0.0f, 0.0f, 0.0f),
		//		glm::vec3(1.0f),
		//		false, false, true);
		//}
		///////////////////////////////////////////////////////////////////////////
	}

	{

		m_skyDomeTex.bind2D(0);
		drawShape(
			&m_skyDome,
			glm::vec3(m_cam3D.position().x, m_cam3D.position().y, m_cam3D.position().z),
			glm::vec3(0.0f),
			glm::vec3(1.0f),
			false, true);
	}

	m_counter += 0.01f;


}

void MainGame::updateInputManager(float deltaTime)
{
	if (m_inputManager.isKeyPressed(SDLK_ESCAPE))
	{
		m_gameState = GameState::EXIT;
	}

	if (m_inputManager.isKeyDown(SDLK_w))
	{
		m_cam3D.setPosition((m_cam3D.position() + m_cam3D.forward() * deltaTime));
	}
	if (m_inputManager.isKeyDown(SDLK_s))
	{
		m_cam3D.setPosition((m_cam3D.position() - m_cam3D.forward() * deltaTime));
	}
	if (m_inputManager.isKeyDown(SDLK_a))
	{
		m_cam3D.setPosition((m_cam3D.position() - m_cam3D.right()* deltaTime));
	}
	if (m_inputManager.isKeyDown(SDLK_d))
	{
		m_cam3D.setPosition((m_cam3D.position() + m_cam3D.right() * deltaTime));
	}
	if (m_inputManager.isKeyDown(SDLK_r))
	{
		m_cam3D.setPosition((m_cam3D.position() + m_cam3D.up() * deltaTime));
	}
	if (m_inputManager.isKeyDown(SDLK_f))
	{
		m_cam3D.setPosition((m_cam3D.position() - m_cam3D.up() * deltaTime));
	}

	if (m_inputManager.isKeyDown(SDLK_1))
	{
		m_cam3D.setPosition(glm::vec3(0.0f, 800.0f, 0.0f));
	}

	if (m_inputManager.isKeyPressed(SDLK_2))
	{
		m_cam3D.setPosition(glm::vec3(0.0f, 40.0f, 70.0f));
	}

	if (m_inputManager.isKeyPressed(SDLK_3))
	{
		m_cam3D.setPosition(glm::vec3(0.0f, 50.0f, 400.0f));
	}

	if (m_inputManager.isKeyPressed(SDLK_4))
	{
		drawWalls = !drawWalls;
	}
	if (m_inputManager.isKeyPressed(SDLK_5))
	{
		drawPyramid = !drawPyramid;
	}
	if (m_inputManager.isKeyPressed(SDLK_6))
	{
		drawBoxPyramid = !drawBoxPyramid;
	}
	if (m_inputManager.isKeyPressed(SDLK_7))
	{
		drawGround = !drawGround;
	}
	if (m_inputManager.isKeyPressed(SDLK_8))
	{
		drawCubeShape = !drawCubeShape;
	}
	if (m_inputManager.isKeyPressed(SDLK_9))
	{
		m_updateSun = !m_updateSun;
	}
	if (m_inputManager.isKeyPressed(SDLK_0))
	{
		m_sunPos = glm::vec3(0.5f);
	}
	if (m_inputManager.isKeyPressed(SDLK_LEFTBRACKET))
	{
		m_directionalLight.ambientIntensity += 0.05f;
	}
	if (m_inputManager.isKeyPressed(SDLK_RIGHTBRACKET))
	{
		m_directionalLight.ambientIntensity -= 0.05f;
	}
	if (m_inputManager.isKeyPressed(SDLK_p))
	{
		m_directionalLight.diffuseIntensity += 0.05f;
	}
	if (m_inputManager.isKeyPressed(SDLK_o))
	{
		m_directionalLight.diffuseIntensity -= 0.05f;
	}

	if (LEFT_ROTATION_PANE || RIGHT_ROTATION_PANE)
	{
		m_cam3D.offsetOrientation(0.0f, m_offset.x * (deltaTime));
	}

	if (TOP_ROTATION_PANE || BOTTOM_ROTATION_PANE)
	{
		m_cam3D.offsetOrientation(-m_offset.y * (deltaTime), 0.0f);
	}
}

void MainGame::gameLoop()
{
	while (m_gameState == GameState::PLAY)
	{
		m_viewPort.Update();

		m_viewPort.swapBuffer(0.0f, 0.15f, 0.3f, 1.0f);

		updateInputManager(m_deltaTime);

		m_inputManager.update();

		draw();

		processInput();

	}

	if (m_gameState == GameState::EXIT)
	{
		m_shaderProgram.dispose();
		SDL_Quit();
		exit(0);
	}

}

float MainGame::getDeltaTime(float totalDeltaTime)
{
	return std::min(totalDeltaTime, MAX_DELTA_TIME);
}