
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"

#include <iostream>

#include "ShaderLoader.h";
#include "Utils.h"
#include "Camera.h"
#include "Light.h"





class Camera;

#pragma once
	class GameModel
	{
	public:
		GameModel(ModelType modelType, Camera* _camera, std::string texFileName, Light * light, float ambientStrength, float specularStrength);
		~GameModel();
		
		 void update(GLfloat time);
		 void renderPass();

		 void screenSpaceRenderPass(GLuint m_texture, GLfloat rtType);

		 void screenSpaceLightingPass(GLuint m_PosTexture, GLuint m_NorTexture, GLuint m_AlbedoTexture, GLuint m_DepthTex, GLuint shadowMapTexture);
		 void screenSpaceReflectionPass(GLuint m_PosTexture, GLuint m_NorTexture, GLuint m_AlbedoTexture, GLuint m_LightingTexture);


		 void shadowMapPass(GLuint shadowMapProgram);

		void setPosition(glm::vec3 _position);
		void setScale(glm::vec3 _scale);
		void setRotation(glm::vec3 angle);
		void setRotationAxis(glm::vec3 rotationAxis);

		void setColor(glm::vec3 _color);
		void setCamera(Camera* camera);
		void setSpeed(float _speed);
		void setProgram(GLuint program);
		void setTexture(std::string texFileName);
		void setSpecularStrength(float strength);

		glm::vec3 getPosition();
		glm::vec3 getScale();
		glm::vec3 getRotation();
		glm::vec3 getRotationAxis();
		
		glm::vec3 getColor();
		
		void moveForward();
		void moveBackward();
		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		
		void rotate(glm::vec3 axis);

		std::vector<VertexFormat>vertices;
		std::vector<GLuint>indices;
		glm::mat4 modelMatrix;
		

		GLuint texture;
	private:

		Camera* camera;
		Light* light;

		
		bool bIsTextureSet = false;

		float speed;



		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 angle;
		glm::vec3 rotationAxis;
		glm::vec3 color;
		
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
	
		GLuint program;

		glm::mat4 lightSpaceMatrix;

		//glm::mat4 model;
	
		float specularStrength;
		float ambientStrength;

		
	
	};


