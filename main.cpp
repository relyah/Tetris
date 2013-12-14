/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>
#define GLM_MESSAGES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_utils.h"
#include "resTexture.cpp"
#include "Piece.h"

struct attributes {
	GLfloat coord3d[3];
	GLfloat texture[2];
	GLfloat normal[3];
};

//struct attributes cube[] = {
//// front
//		{ { -1.0, -1.0, 1.0 }, { 1.0, 0.0, 0.0 } }, { { 1.0, -1.0, 1.0 }, { 0.0,
//				1.0, 0.0 } }, { { 1.0, 1.0, 1.0 }, { 0.0, 0.0, 1.0 } }, { { -1.0, 1.0,
//				1.0 }, { 1.0, 1.0, 1.0 } },
//		// back
//		{ { -1.0, -1.0, -1.0 }, { 1.0, 0.0, 0.0 } }, { { 1.0, -1.0, -1.0 }, { 0.0,
//				1.0, 0.0 } }, { { 1.0, 1.0, -1.0 }, { 0.0, 0.0, 1.0 } }, { { -1.0, 1.0,
//				-1.0 }, { 1.0, 1.0, 1.0 } } };

//, , , ,
GLushort cube_elements[] = {
// front
		0, 1, 2 };
GLfloat cube[] = { -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0 };
//struct attributes cube[] = {
//  // front
//  {{-1.0, -1.0,  1.0},{0.0, 1.0},{0.0,0.0,1.0}},
//   {{1.0, -1.0,  1.0},{1.0, 1.0},{0.0,0.0,1.0}},
//   {{1.0,  1.0,  1.0},{1.0, 0.0},{0.0,0.0,1.0}},
//  {{-1.0,  1.0,  1.0},{0.0, 0.0},{0.0,0.0,1.0}},
//  // top
//  {{-1.0,  1.0,  1.0},{0.0, 1.0},{0.0,1.0,0.0}},
//   {{1.0,  1.0,  1.0},{1.0, 1.0},{0.0,1.0,0.0}},
//   {{1.0,  1.0, -1.0},{1.0, 0.0},{0.0,1.0,0.0}},
//  {{-1.0,  1.0, -1.0},{0.0, 0.0},{0.0,1.0,0.0}},
//  // back
//   {{1.0, -1.0, -1.0},{0.0, 1.0},{0.0,0.0,-1.0}},
//  {{-1.0, -1.0, -1.0},{1.0, 1.0},{0.0,0.0,-1.0}},
//  {{-1.0,  1.0, -1.0},{1.0, 0.0},{0.0,0.0,-1.0}},
//   {{1.0,  1.0, -1.0},{0.0, 0.0},{0.0,0.0,-1.0}},
//  // bottom
//  {{-1.0, -1.0, -1.0},{0.0, 1.0},{0.0,-1.0,0.0}},
//   {{1.0, -1.0, -1.0},{1.0, 1.0},{0.0,-1.0,0.0}},
//   {{1.0, -1.0,  1.0},{1.0, 0.0},{0.0,-1.0,0.0}},
//  {{-1.0, -1.0,  1.0},{0.0, 0.0},{0.0,-1.0,0.0}},
//  // left
//  {{-1.0, -1.0, -1.0},{0.0, 1.0},{-1.0,0.0,0.0}},
//  {{-1.0, -1.0,  1.0},{1.0, 1.0},{-1.0,0.0,0.0}},
//  {{-1.0,  1.0,  1.0},{1.0, 0.0},{-1.0,0.0,0.0}},
//  {{-1.0,  1.0, -1.0},{0.0, 0.0},{-1.0,0.0,0.0}},
//  // right
//   {{1.0, -1.0,  1.0},{0.0, 1.0},{1.0,0.0,0.0}},
//   {{1.0, -1.0, -1.0},{1.0, 1.0},{1.0,0.0,0.0}},
//   {{1.0,  1.0, -1.0},{1.0, 0.0},{1.0,0.0,0.0}},
//   {{1.0,  1.0,  1.0},{0.0, 0.0},{1.0,0.0,0.0}},
//};

GLuint program;
GLuint textureId;
GLint uniformMytexture;
bool wellEmpty = true;
bool isGameOver = false;
GLuint vs, fs;
GLuint vbo_cube, vbo_fixed; //, vbo_cube_texcoords; //vertex buffer object
GLuint ibo_cube_elements, ibo_fixed; //index buffer object
GLint attribute_coord3d, attribute_colour, attribute_normal, attribute_texcoord;
GLint uniform_mvp, uniform_m, uniform_v, uniform_p, uniform_mytexture;
int screen_width;
int screen_height;
Piece cp = Piece(3, 3, 0.0, 0.0, 0.0);
Piece well = Piece(10, 14, 0.0, 0.0, 0.0);
glm::mat4 translate;
glm::mat4 translate_fixed;
int specialKey = -1;

int init_resources(void) {

	cp.Set(0, 0, true);
	std::vector<float> cs;
	std::vector<unsigned short> el;
	cp.ConvertToCubes(cs, el);

	glGenBuffers(1, &vbo_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, cs.size() * sizeof(float), &cs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort cube_elements[] = {
	// front
			0, 1, 2 };

//	 GLushort cube_elements[] = {
//	    // front
//	     0,  1,  2,
//	     2,  3,  0,
//	    // top
//	     4,  5,  6,
//	     6,  7,  4,
//	    // back
//	     8,  9, 10,
//	    10, 11,  8,
//	    // bottom
//	    12, 13, 14,
//	    14, 15, 12,
//	    // left
//	    16, 17, 18,
//	    18, 19, 16,
//	    // right
//	    20, 21, 22,
//	    22, 23, 20,
//	  };

	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements,	GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, el.size() * sizeof(unsigned short), &el[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if ((vs = create_shader("cube.v.glsl", GL_VERTEX_SHADER)) == 0)
		return 0;
	if ((fs = create_shader("cube.f.glsl", GL_FRAGMENT_SHADER)) == 0)
		return 0;

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		return 0;
	}

	const char* attribute_name = "v_coord";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

//	attribute_name = "v_normal";
//	attribute_normal = glGetAttribLocation(program, attribute_name);
//	if (attribute_colour == -1) {
//		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//		return 0;
//	}
//
//	attribute_name = "texcoord";
//	attribute_texcoord = glGetAttribLocation(program, attribute_name);
//	if (attribute_texcoord == -1) {
//		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
//		return 0;
//	}

	const char* uniform_name;
	uniform_name = "model";
	uniform_m = glGetUniformLocation(program, uniform_name);
	if (uniform_m == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	uniform_name = "view";
	uniform_v = glGetUniformLocation(program, uniform_name);
	if (uniform_v == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	uniform_name = "projection";
	uniform_p = glGetUniformLocation(program, uniform_name);
	if (uniform_p == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}
//
//	uniform_name = "mvp";
//	uniform_mvp = glGetUniformLocation(program, uniform_name);
//	if (uniform_mvp == -1) {
//		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
//		return 0;
//	}

//	glGenTextures(1, &textureId);
//	glBindTexture(GL_TEXTURE_2D, textureId);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexImage2D(GL_TEXTURE_2D, // target
//	0,  // level, 0 = base, no minimap,
//	GL_RGB, // internalformat
//	res_texture.width,  // width
//	res_texture.height,  // height
//	0,  // border, always 0 in OpenGL ES
//	GL_RGB,  // format
//	GL_UNSIGNED_BYTE, // type
//	res_texture.pixel_data);

//	uniform_name = "mytexture";
//	uniform_mytexture = glGetUniformLocation(program, "uniform_name");

	return 1;
}

void timerCallBack(int value) {

	switch (specialKey) {
	case GLUT_KEY_LEFT:
		if (well.CanMove(cp, 1, 0)) {
			cp.Move(1, 0, true);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (well.CanMove(cp, -1, 0)) {
			cp.Move(-1, 0, true);
		}
		break;
	}
	specialKey=-1;

	if (well.MustMove(cp)) {
		if (well.CanMove(cp)) {
			cp.Move(0, 1);
		} else {
			wellEmpty = false;

			well.Add(cp);

			std::vector<float> cs1;
			std::vector<unsigned short> el1;
			well.ConvertToCubes(cs1, el1);

			glGenBuffers(1, &vbo_fixed);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_fixed);
			glBufferData(GL_ARRAY_BUFFER, cs1.size() * sizeof(float), &cs1[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &ibo_fixed);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_fixed);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, el1.size() * sizeof(unsigned short), &el1[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			translate_fixed = glm::translate(glm::mat4(1.0f), glm::vec3(well.X(), well.Y() + 14.0, well.Z()));

			cp = Piece(3, 3, 0.0, 0.0, 0.0);
			cp.Set(0, 0, true);

			if (well.CanAdd(cp)) {
				std::vector<float> cs2;
				std::vector<unsigned short> el2;
				cp.ConvertToCubes(cs2, el2);

				glGenBuffers(1, &vbo_cube);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
				glBufferData(GL_ARRAY_BUFFER, cs2.size() * sizeof(float), &cs2[0], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glGenBuffers(1, &ibo_cube_elements);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, el2.size() * sizeof(unsigned short), &el2[0], GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			} else {
				isGameOver = true;
			}
		}
	} else {
		cp.Increment(false, true, false);
	}

	if (!isGameOver) {
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(cp.X(), cp.Y() + 14.0, cp.Z()));

		glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, -40.0),  // the position of your camera, in world space
		glm::vec3(0.0, 0.0, 0.0), // where you want to look at, in world space
		glm::vec3(0.0, 1.0, 0.0)); //up direction; probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too

		glm::mat4 projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 100.0f);
//glm::mat4 mvp = projection * view * model;// * anim;
		glUseProgram(program);
//glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
//	glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniform_v, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniform_p, 1, GL_FALSE, glm::value_ptr(projection));
		glutPostRedisplay();
		glutTimerFunc(100, timerCallBack, 0);
	}
}

void onDisplay() {
	/* Clear the background as white */
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glEnableVertexAttribArray(attribute_coord3d);
	glEnableVertexAttribArray(attribute_texcoord);
	glEnableVertexAttribArray(attribute_normal);

	if (!wellEmpty) {
		glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(translate_fixed));
		glBindBuffer(GL_ARRAY_BUFFER, vbo_fixed);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_fixed);

		glVertexAttribPointer(attribute_coord3d, 3,
		GL_FLOAT,
		GL_FALSE, sizeof(struct PC),  // stride
		0);  // offset
		int size = 0;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (!isGameOver) {
		glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(translate));

		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);

		glVertexAttribPointer(attribute_coord3d, 3,
		GL_FLOAT,
		GL_FALSE, sizeof(struct PC),  // stride
		0);  // offset

//	glVertexAttribPointer(attribute_texcoord, 2,
//	GL_FLOAT,
//	GL_FALSE, sizeof(struct attributes),  // stride
//	(GLvoid*) offsetof(struct attributes, texture));
//
//	glVertexAttribPointer(attribute_normal, 3,
//	GL_FLOAT,
//	GL_FALSE, sizeof(struct attributes),  // stride
//	(GLvoid*) offsetof(struct attributes, normal));

//	glEnableVertexAttribArray(attribute_texcoord);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
//	glVertexAttribPointer(attribute_texcoord, // attribute
//	2,                  // number of elements per vertex, here (x,y)
//	GL_FLOAT,           // the type of each element
//	GL_FALSE,           // take our values as-is
//	0,                  // no extra data between each position
//	0                   // offset of first element
//	);

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textureId);
//	glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);

		/* Push each element in buffer_vertices to the vertex shader */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		int size = 0;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	/* Display the result */
	glutSwapBuffers();

	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_colour);
}

void free_resources() {
	glUseProgram(0);

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	glDeleteShader(fs);
	glDeleteShader(vs);

	glDeleteProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo_cube);
	glDeleteBuffers(1, &ibo_cube_elements);

	if (vbo_fixed != 0) {
		glDeleteBuffers(1, &vbo_fixed);
		glDeleteBuffers(1, &ibo_fixed);
	}

	glDeleteTextures(1, &textureId);
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}

void keyPressed(unsigned char key, int x, int y) {

}

void specialKeyPressed(int key, int x, int y) {
	specialKey = key;
}

int main(int argc, char* argv[]) {
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
//glutInitContextVersion(4, 0);
//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
	GLUT_ACTION_ON_WINDOW_CLOSE,
	GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(640, 480);
	glutCreateWindow("Tetris");

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}

	/* When all init functions run without errors,
	 the program can initialise the resources */
	if (1 == init_resources()) {
		/* We can display it if everything goes OK */
		glutDisplayFunc(onDisplay);
		glutReshapeFunc(onReshape);
		glutKeyboardFunc(keyPressed);
		glutSpecialFunc(specialKeyPressed);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glutTimerFunc(0, timerCallBack, 0);
		//glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutMainLoop();
	}

	/* If the program exits in the usual way,
	 free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
