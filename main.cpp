/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>
#define GLM_MESSAGES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctype.h>

#include "shader_utils.h"
#include "resTexture.cpp"
#include "Piece.h"

#define SPACEBAR 32
#define ESCAPE 27
#define ROTATE_LEFT 65 //capital A
#define ROTATE_RIGHT 83 //capital S

void GenerateBuffers(Piece& p, GLuint& vbo, GLuint& ibo);
void MakePieces();
void PickPiece();

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
GLuint vbo_cube, vbo_fixed; //vertex buffer object
GLuint ibo_cube_elements, ibo_fixed; //index buffer object
GLint attribute_coord3d, attribute_colour, attribute_normal;
GLint uniform_m, uniform_v, uniform_p;
int screen_width;
int screen_height;
glm::mat4 translate;
glm::mat4 translate_fixed;
int specialKey = -1;
unsigned char key;
int moveDelay = 0;

std::vector<Piece> pieces;
Piece* well = 0;
Piece* cp = 0;

int init_resources(void) {

	MakePieces();
	PickPiece();

	GenerateBuffers(*cp, vbo_cube, ibo_cube_elements);

//	GLushort cube_elements[] = {
//	// front
//			0, 1, 2 };

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


	program = create_program("cube.v.glsl", "cube.f.glsl");
	if (program == 0)
		return 0;
	attribute_coord3d = get_attrib(program, "vertex_position");
	attribute_normal = get_attrib(program, "vertex_normal");
	attribute_colour = get_attrib(program, "vertex_colour");

	uniform_m = get_uniform(program, "model");
	uniform_v = get_uniform(program, "view");
	uniform_p = get_uniform(program, "projection");

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
		if (well->CanMove(*cp, 1, 0)) {
			cp->Move(1, 0, true);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (well->CanMove(*cp, -1, 0)) {
			cp->Move(-1, 0, true);
		}
		break;
	}
	specialKey = -1;

	bool isDrop = false;
	switch (key) {
	case SPACEBAR:
		well->Drop(*cp);
		isDrop = true;
		break;
	case ROTATE_LEFT:
		if (well->CanRotateLeft(*cp)) {
			cp->RotateLeft();
			GenerateBuffers(*cp, vbo_cube, ibo_cube_elements);
		}
		break;
	case ROTATE_RIGHT:
		if (well->CanRotateRight(*cp)) {
			cp->RotateRight();
			GenerateBuffers(*cp, vbo_cube, ibo_cube_elements);
		}
		break;
	}
	key = -1;

	if (well->MustMove(*cp)) {
		if (well->CanMove(*cp)) {
			cp->Move(0, 1);
		} else {
			if (isDrop || moveDelay > 10) {
				moveDelay = 0;
				isDrop = false;
				wellEmpty = false;

				well->Add(*cp);

				GenerateBuffers(*well, vbo_fixed, ibo_fixed);

				translate_fixed = glm::translate(glm::mat4(1.0f), glm::vec3(well->X(), well->Y() + 14.0, well->Z()));

				PickPiece();

				if (well->CanAdd(*cp)) {
					GenerateBuffers(*cp, vbo_cube, ibo_cube_elements);
				} else {
					isGameOver = true;
				}
			} else {
				moveDelay++;
			}

		}
	} else {
		cp->Increment(false, true, false);
	}

	if (!isGameOver) {
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(cp->X(), cp->Y() + 14.0, cp->Z()));

		glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, -40.0),  // the position of your camera, in world space
		glm::vec3(0.0, 0.0, 0.0),  // where you want to look at, in world space
		glm::vec3(0.0, 1.0, 0.0)); //up direction; probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too

		glm::mat4 projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 100.0f);

		glUseProgram(program);
//	glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniform_v, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniform_p, 1, GL_FALSE, glm::value_ptr(projection));
		glutPostRedisplay();
		glutTimerFunc(100, timerCallBack, 0);
	}
}

void GenerateBuffers(Piece& p, GLuint& vbo, GLuint& ibo) {
	std::vector<float> cs;
	std::vector<unsigned short> el;
	p.ConvertToCubes(cs, el);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, cs.size() * sizeof(float), &cs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, el.size() * sizeof(unsigned short), &el[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void onDisplay() {
	/* Clear the background as white */
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glEnableVertexAttribArray(attribute_coord3d);
	glEnableVertexAttribArray(attribute_normal);
	glEnableVertexAttribArray(attribute_colour);

	if (!wellEmpty) {
		glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(translate_fixed));
		glBindBuffer(GL_ARRAY_BUFFER, vbo_fixed);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_fixed);

		glVertexAttribPointer(attribute_coord3d, 3,
		GL_FLOAT,
		GL_FALSE, sizeof(struct PC),  // stride
		0);  // offset

		glVertexAttribPointer(attribute_normal, 3,
		GL_FLOAT,
		GL_FALSE, sizeof(struct PC),  // stride
		(GLvoid*) offsetof(struct PC, normal));

		glVertexAttribPointer(attribute_colour, 3,
		GL_FLOAT,
		GL_FALSE, sizeof(struct PC),  // stride
		(GLvoid*) offsetof(struct PC, colour));

		int size = 0;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glVertexAttribPointer(attribute_normal, 3,
	GL_FLOAT,
	GL_FALSE, sizeof(struct PC),  // stride
	(GLvoid*) offsetof(struct PC, normal));

	glVertexAttribPointer(attribute_colour, 3,
	GL_FLOAT,
	GL_FALSE, sizeof(struct PC),  // stride
	(GLvoid*) offsetof(struct PC, colour));

	if (!isGameOver) {
		glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(translate));

		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);

		glVertexAttribPointer(attribute_coord3d, 3,
		GL_FLOAT,
		GL_FALSE, sizeof(struct PC),  // stride
		0);  // offset

		glVertexAttribPointer(attribute_normal, 3,
		GL_FLOAT,
		GL_FALSE, sizeof(struct PC),  // stride
		(GLvoid*) offsetof(struct PC, normal));

		glVertexAttribPointer(attribute_colour, 3,
		GL_FLOAT,
		GL_FALSE, sizeof(struct PC),  // stride
		(GLvoid*) offsetof(struct PC, colour));

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
	::key = toupper(key);

	if (key == ESCAPE)
		glutLeaveMainLoop();
}

void specialKeyPressed(int key, int x, int y) {
	specialKey = key;
}

int main(int argc, char* argv[]) {
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
//glutInitContextVersion(4, 0);
//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION); //alternative GLUT_ACTION_GLUTMAINLOOP_RETURNS

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

void MakePieces() {
	Piece t = Piece(3,3, 0.0, 0.0, 0.0);
	t.Set(0, 0, true);
	t.Set(1, 0, true);
	t.Set(2, 0, true);
	t.Set(1, 1, true);
	t.Set(1, 2, true);

	Piece i = Piece(3,3, 0.0, 0.0, 0.0);
	i.Set(0, 0, true);
	i.Set(0, 1, true);
	i.Set(0, 2, true);

	Piece l = Piece(3,3, 0.0, 0.0, 0.0);
	l.Set(0, 0, true);
	l.Set(0, 1, true);
	l.Set(0, 2, true);
	l.Set(1, 2, true);

	Piece sqr = Piece(3,3, 0.0, 0.0, 0.0);
	sqr.Set(0, 0, true);
	sqr.Set(0, 1, true);
	sqr.Set(1, 0, true);
	sqr.Set(1, 1, true);

	Piece zl = Piece(3,3, 0.0, 0.0, 0.0);
	zl.Set(0, 0, true);
	zl.Set(0, 1, true);
	zl.Set(1, 1, true);
	zl.Set(2, 1, true);

	Piece zr = Piece(3,3, 0.0, 0.0, 0.0);
	zr.Set(2, 0, true);
	zr.Set(1, 0, true);
	zr.Set(1, 1, true);
	zr.Set(0, 1, true);

//	pieces.push_back(t);
//	pieces.push_back(i);
//	pieces.push_back(l);
	pieces.push_back(sqr);
//	pieces.push_back(zl);
//	pieces.push_back(zr);

	well = new Piece(10, 14, 0.0, 0.0, 0.0);
}

void PickPiece(){

	int piece = rand()%pieces.size();
	cp = &(pieces[piece]);

}
