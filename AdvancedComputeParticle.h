#pragma once


#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class ShaderLoader;



static inline float random_float() {
	float res;
	unsigned int tmp;
	static unsigned int seed = 0xFFFF0C59;

	seed *= 16807;
	tmp = seed ^ (seed >> 4) ^ (seed << 15);
	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;
	return (res - 1.0f);
}

static glm::vec3 random_vector(float minmag = 0.0f, float maxmag = 1.0f) {

	glm::vec3 randomvec(random_float() * 2.0f - 1.0f,
		random_float() * 2.0f - 1.0f,
		random_float() * 2.0f - 1.0f);


	randomvec = normalize(randomvec);
	randomvec *= (random_float() * (maxmag - minmag) + minmag);
	return randomvec;
}

enum {
	PARTICLE_GROUP_SIZE = 128,
	PARTICLE_GROUP_COUNT = 8000,
	PARTICLE_COUNT = (PARTICLE_GROUP_SIZE * PARTICLE_GROUP_COUNT),
	MAX_ATTRACTORS = 64
};

// Position and velocity buffers
// Union - use same memory region for storing different objects at different times
static union {
	struct {
		GLuint position_buffer;
		GLuint velocity_buffer;
	};
	GLuint buffers[2];
};

// TBOs texture buffer objects
static union {
	struct {
		GLuint position_tbo;
		GLuint velocity_tbo;
	};
	GLuint tbos[2];
};

class AdvancedComputeParticle
{
public:

	void init();
	void render();

	// Compute program
	GLuint  compute_prog;
	GLint   dt_location; // get location

	// Program, vao and vbo to render a full screen quad
	GLuint  render_prog, render_vao, render_vbo;

	// Attractor UBO
	GLuint  attractor_buffer;

	// Mass of the attractors
	float attractor_masses[MAX_ATTRACTORS];


};

