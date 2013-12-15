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
#include "Well.h"

#define SPACEBAR 32
#define ESCAPE 27
#define ROTATE_LEFT 65 //capital A
#define ROTATE_RIGHT 83 //capital S

int InitProgram();
void InitCamera();
void GenerateBuffers(AbstractPiece& p, GLuint& vbo, GLuint& ibo);
void GenerateArrayBuffer(std::vector<float>& vertices, GLuint& vbo);
void GenerateElementBuffer(std::vector<unsigned short>& elements, GLuint& ibo);
void InitWell();
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
bool isCameraRotate = false;
glm::vec3 cameraPosition;
glm::vec3 cameraLookAt;
glm::vec3 cameraUp;
glm::vec3 cameraRight;
GLuint vs, fs;
GLuint vbo_cube, vbo_fixed, vbo_grid; //vertex buffer object
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
float yAngle;
float xAngle;
int prevxPos;
int prevyPos;

std::vector<Piece> pieces;
Well* well = 0;
Piece* cp = 0;

int init_resources(void) {

	InitProgram();
	InitCamera();

	InitWell();

	return 1;
}

void timerCallBack(int value) {

	int incX = 0;
	switch (specialKey) {
	case GLUT_KEY_LEFT:
		incX = -1;
		if (well->CanMove(*cp, incX, 0)) {
			cp->Move(incX, 0, true);
		}
		break;
	case GLUT_KEY_RIGHT:
		incX = 1;
		if (well->CanMove(*cp, incX, 0)) {
			cp->Move(incX, 0, true);
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

	if (cp->MustMove()) {
		if (well->CanMove(*cp)) {
			cp->Move(0, 1);
		} else {
			if (isDrop || moveDelay > 10) {
				moveDelay = 0;
				isDrop = false;
				wellEmpty = false;

				well->Add(*cp);

				GenerateBuffers(*well, vbo_fixed, ibo_fixed);

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
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(cp->getX(), cp->getY(), cp->getZ()));

		glm::mat4 view = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);

		glm::mat4 projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 100.0f);

		glUseProgram(program);
//	glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniform_v, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniform_p, 1, GL_FALSE, glm::value_ptr(projection));
		glutPostRedisplay();
		glutTimerFunc(100, timerCallBack, 0);
	}
}

void GenerateBuffers(AbstractPiece& p, GLuint& vbo, GLuint& ibo) {
	std::vector<float> cs;
	std::vector<unsigned short> el;
	p.ConvertToCubes(cs, el);

	GenerateArrayBuffer(cs, vbo);
	GenerateElementBuffer(el, ibo);
}

void GenerateArrayBuffer(std::vector<float>& vertices, GLuint& vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GenerateElementBuffer(std::vector<unsigned short>& elements, GLuint& ibo) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &ibo);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned short), &elements[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void onDisplay() {
	/* Clear the background as white */
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	translate_fixed = glm::translate(glm::mat4(1.0f), glm::vec3(well->getX(), well->getY(), well->getZ()));

	glEnableVertexAttribArray(attribute_coord3d);
	glEnableVertexAttribArray(attribute_normal);
	glEnableVertexAttribArray(attribute_colour);

	glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(translate_fixed));
	glBindBuffer(GL_ARRAY_BUFFER, vbo_grid);

	glVertexAttribPointer(attribute_coord3d, 3, //number of components per coordinate
	GL_FLOAT,
	GL_FALSE, sizeof(struct PC),  // stride
	0);  // offset

	glVertexAttribPointer(attribute_normal, 3, //number of components per normal
	GL_FLOAT,
	GL_FALSE, sizeof(struct PC),  // stride
	(GLvoid*) offsetof(struct PC, normal));

	glVertexAttribPointer(attribute_colour, 3, //number of components per colour
	GL_FLOAT,
	GL_FALSE, sizeof(struct PC),  // stride
	(GLvoid*) offsetof(struct PC, colour));

	int size = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawArrays(GL_LINES, 0, size / sizeof(PC));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	glDeleteBuffers(1, &vbo_grid);

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

void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		isCameraRotate = true;

		prevxPos = x;
		prevyPos = y;

	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		isCameraRotate = false;
	}
}

void onMotion(int x, int y) {
	if (isCameraRotate) {  // if left button is pressed

		int xPos = x;
		int yPos = y;

		//yaw
		int delta = xPos - prevxPos;
		yAngle += (float) delta * 0.1f;
//		D3DXMATRIX rotation;
//		D3DXMatrixRotationAxis(&rotation, &cameraUp, D3DXToRadian((float) delta));
//		D3DXVec3TransformNormal(&right, &right, &rotation);
//		D3DXVec3TransformNormal(&cameraPosition, &cameraPosition, &rotation);

		glm::mat4 rotate = glm::mat4(1.0f);
		rotate = glm::rotate(rotate, yAngle, cameraUp);
		cameraRight = glm::vec3(glm::normalize(rotate * glm::vec4(cameraRight, 0.0)));
		cameraPosition = glm::vec3(rotate * glm::vec4(cameraPosition, 0.0));

		//pitch
		delta = prevyPos - yPos;
		xAngle += (float) delta * 0.1f;
//
//    D3DXMatrixRotationAxis(&rotation, &right, D3DXToRadian((float)delta));
//              D3DXVec3TransformNormal(&cameraUp, &cameraUp, &rotation);
//              D3DXVec3TransformNormal(&cameraPosition, &cameraPosition, &rotation);
		rotate = glm::mat4(1.0f);
		rotate = glm::rotate(rotate, xAngle, cameraRight);
		cameraUp = glm::vec3(glm::normalize(rotate * glm::vec4(cameraUp, 0.0)));
		cameraPosition = glm::vec3(rotate * glm::vec4(cameraPosition, 0.0));

		prevxPos = xPos;
		prevyPos = yPos;

		if (yAngle >= 360.0f)
			yAngle -= 360.0f;
		if (yAngle <= 0.0f)
			yAngle += 360.0f;

	}
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
		glutMouseFunc(onMouse);
		glutMotionFunc(onMotion);

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

void InitWell() {
	MakePieces();
	PickPiece();

	std::vector<float> grid;
	well->MakeGrid(grid);
	GenerateArrayBuffer(grid, vbo_grid);
	GenerateBuffers(*cp, vbo_cube, ibo_cube_elements);
}

void MakePieces() {
	Piece t = Piece(3, 0.0, 0.0, 0.0);
	t.Set(0, 0, true);
	t.Set(1, 0, true);
	t.Set(2, 0, true);
	t.Set(1, 1, true);
	t.Set(1, 2, true);

	Piece i = Piece(3, 0.0, 0.0, 0.0);
	i.Set(0, 0, true);
	i.Set(0, 1, true);
	i.Set(0, 2, true);

	Piece l = Piece(3, 0.0, 0.0, 0.0);
	l.Set(0, 0, true);
	l.Set(0, 1, true);
	l.Set(0, 2, true);
	l.Set(1, 2, true);

	Piece sqr = Piece(2, 0.0, 0.0, 0.0);
	sqr.Set(0, 0, true);
	sqr.Set(0, 1, true);
	sqr.Set(1, 0, true);
	sqr.Set(1, 1, true);

	Piece zl = Piece(3, 0.0, 0.0, 0.0);
	zl.Set(0, 0, true);
	zl.Set(0, 1, true);
	zl.Set(1, 1, true);
	zl.Set(2, 1, true);

	Piece zr = Piece(3, 0.0, 0.0, 0.0);
	zr.Set(2, 0, true);
	zr.Set(1, 0, true);
	zr.Set(1, 1, true);
	zr.Set(0, 1, true);

	pieces.push_back(sqr);
//	pieces.push_back(t);
//	pieces.push_back(i);
//	pieces.push_back(l);
//	pieces.push_back(zl);
//	pieces.push_back(zr);

	well = new Well(10, 14, 0.0, 0.0, 0.0);
}

void PickPiece() {

	int piece = rand() % pieces.size();
	cp = &(pieces[piece]);
	cp->Reset();
	cp->Move(0, 0, true);

}

int InitProgram() {
	program = create_program("cube.v.glsl", "cube.f.glsl");
	if (program == 0)
		return 0;
	attribute_coord3d = get_attrib(program, "vertex_position");
	attribute_normal = get_attrib(program, "vertex_normal");
	attribute_colour = get_attrib(program, "vertex_colour");

	uniform_m = get_uniform(program, "model");
	uniform_v = get_uniform(program, "view");
	uniform_p = get_uniform(program, "projection");
	return 1;
}

void InitCamera() {
	cameraPosition = glm::vec3(-5.0, -14.0, -40.0);  // the position of your camera, in world space
	cameraLookAt = glm::vec3(-5.0, -14.0, 0.0);  // where you want to look at, in world space
	cameraUp = glm::vec3(0.0, 1.0, 0.0); //up direction; probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
	cameraRight = glm::vec3(1.0, 0.0, 0.0);

	yAngle = 0.0f;
	xAngle = 0.0f;
	prevxPos=0;
	prevyPos=0;

}
