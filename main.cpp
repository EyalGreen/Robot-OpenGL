

#include <iostream>
#include <stdlib.h>

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <math.h>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <iostream>



using namespace std;

// Constants
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;
const double PI = 3.14159265358979323846;
const float WINDOW_RATIO = WINDOW_WIDTH / (float)WINDOW_HEIGHT;
string msg = "Press any key to change projection";
float aspect = WINDOW_RATIO;



// This is the number of frames per second to render.
static const int FPS = 60;

// This global variable keeps track of the current orientation of the polyhedron.
static GLfloat currentAngleOfRotation = 0.0;

// Set this to true to change projection.
static bool orthoProjection = false;



// Function where the scene drawing occures
void display(void);

// Function for keyboard press
void keyboard(unsigned char key, int x, int y);

//Function for keyboard special
void SpecialInput(int key, int x, int y);

// Function to handle reshape of the screen
void reshape(GLint w, GLint h);


// Function to handle the timer
void timer(int v);

// Function to update the projection
void updateProjection();

struct Color {
	float r, g, b;
public:
	Color(float r, float g, float b) {
		Color::r = r;
		Color::g = g;
		Color::b = b;
	}
	Color() {
		r = 0.2f;

	}
};

// dependency: Vector3 and Matrix4
struct Vector3
{
	float x;
	float y;
	float z;

public:
	Vector3()
	{
		Vector3::x = 0;
		Vector3::y = 0;
		Vector3::z = 0;
	}
	Vector3(float x, float y, float z)
	{
		Vector3::x = x;
		Vector3::y = y;
		Vector3::z = z;
	}

	void normalize()
	{
		double sum = sqrt(x * x + y * y + z * z);
		x /= sum;
		y /= sum;
		z /= sum;
	}
	void rotate(float xAngle, float yAngle, float zAngle) {
		float xNew = 0.0f, yNew = 0.0f, zNew = 0.0f;
		xNew = x * cos(yAngle) * cos(zAngle) - y * cos(yAngle) * sin(zAngle) + z * sin(yAngle);
		yNew = x * (sin(xAngle) * sin(yAngle) * cos(zAngle) + cos(xAngle) * sin(zAngle)) + y * (cos(xAngle) * cos(zAngle) - sin(xAngle) * sin(yAngle) * sin(zAngle)) - z * sin(xAngle) * cos(y);
		zNew = x * (sin(xAngle) * sin(zAngle) - cos(xAngle) * sin(yAngle) * cos(zAngle)) + y * (cos(xAngle) * sin(yAngle) * sin(zAngle) + sin(xAngle) * cos(zAngle)) + z * cos(yAngle) * cos(xAngle);


		Vector3::x = xNew;
		Vector3::y = yNew;
		Vector3::z = zNew;
	}

	// computes and returns the cross product of 2 vectors
	Vector3 cross(Vector3 vector)
	{
		Vector3 resultVector;
		resultVector.x = Vector3::y * vector.z - Vector3::z * vector.y;
		resultVector.y = -(Vector3::x * vector.z - Vector3::z * vector.x);
		resultVector.z = Vector3::x * vector.y - Vector3::y * vector.x;
		return resultVector;
	}

	Vector3 operator-(Vector3 const& obj)
	{
		Vector3 res;
		res.x = x - obj.x;
		res.y = y - obj.y;
		res.z = z - obj.z;
		return res;
	}
	Vector3 operator+(Vector3 const& obj)
	{
		Vector3 res;
		res.x = x + obj.x;
		res.y = y + obj.y;
		res.z = z + obj.z;
		return res;
	}

	void factor(float scalar)
	{
		Vector3::x *= scalar;
		Vector3::y *= scalar;
		Vector3::z *= scalar;
	}
	Vector3 getFactoredVector(float scalar) {
		return Vector3(Vector3::x * scalar, Vector3::y * scalar, Vector3::z * scalar);
	}

};
void draw_rect(Vector3 origin, Vector3 v1, Vector3 v2, float width, float height, Color color);

Vector3 cameraPos = Vector3(20.0f, 5.0f, 20.0f);
float yAngle = 0.0f, zAngle = 0.0f; //rotate over Y axis and Z axis

// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(400, 150);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("3D Robot - Nitay and Eyal");
	Vector3 eye = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 target = Vector3(-1.0f, 0.0f, -1.0f);
	Vector3 dir = Vector3(0.0f, 1.0f, 0.0f);
	// lookAt(eye, target, dir);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialInput);

	glutMainLoop();
}

// Handles the window reshape event
void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	aspect = w / h;
	updateProjection();
}

void draw_square(Vector3 origin, Vector3 v1, Vector3 v2, float size, Color color)
{
	draw_rect(origin, v1, v2, size, size, color);
}
void draw_rect(Vector3 origin, Vector3 v1, Vector3 v2, float width, float height, Color color) {
	v1.factor(width);
	v2.factor(height);
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_QUADS);
	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(origin.x + v1.x, origin.y + v1.y, origin.z + v1.z);
	glVertex3f(origin.x + v1.x + v2.x, origin.y + v1.y + v2.y, origin.z + v1.z + v2.z);
	glVertex3f(origin.x + v2.x, origin.y + v2.y, origin.z + v2.z);
	glEnd();

	glColor3f(0.8, 0.0, 0.0);

}


void draw_shape(Vector3 origin, Vector3 dir, Vector3 up, float width, float length, float height, Color color) {
	Vector3 dirPerpendicular = up.cross(dir);
	draw_rect(origin, dir, dirPerpendicular, width, length, color);
	draw_rect(origin + up.getFactoredVector(height), dir, dirPerpendicular, width, length, color);

	//draw walls
	draw_rect(origin, up, dirPerpendicular, height, length, color);

	draw_rect(origin + dir.getFactoredVector(width), up, dirPerpendicular, height, length, color);

	draw_rect(origin, up, dir, height, width, color);

	draw_rect(origin + dirPerpendicular.getFactoredVector(length), up, dir, height, width, color);

}

void draw_box(Vector3 origin, Vector3 dir, Vector3 up, float size, float height, Color color) {

	draw_shape(origin, dir, up, size, size, height, color);
}


void draw_table(Vector3 origin) {
	float height = 3.0f;
	float size = 0.3f;
	float distanceWidth = 5;
	float distanceLength = 1;
	draw_box(origin + Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), size, height, Color(0.8f, 0.7f, 0.1f));

	draw_box(origin + Vector3(2, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), size, height, Color(0.8f, 0.7f, 0.1f));

	draw_box(origin + Vector3(0, 0, 2), Vector3(1, 0, 0), Vector3(0, 1, 0), size, height, Color(0.8f, 0.7f, 0.1f));

	draw_box(origin + Vector3(2, 0, 2), Vector3(1, 0, 0), Vector3(0, 1, 0), size, height, Color(0.8f, 0.7f, 0.1f));

	draw_box(origin + Vector3(-size, height, -size), Vector3(0, 0, 1), Vector3(0, 1, 0), height, size, Color(0.7, 0.4, 0.1));
}


void draw_tv(Vector3 origin) {
	float height = 0.3f;
	float size = 1.5f;
	float tv_length = 0.2f;
	float tv_width = 3.0f;
	float tv_height = 2.0f;
	// draw stand
	draw_box(origin, Vector3(1, 0, 0), Vector3(0, 1, 0), size, height, Color(0.2f, 0.2f, 0.2f));
	draw_box(origin + Vector3((size / 2) - 0.05f, height, (-size / 2) - 0.05f), Vector3(0, 0, 1), Vector3(0, 1, 0), size / 5, height * 3, Color(0.4f, 0.4f, 0.4f));
	draw_shape(origin + Vector3(-0.75f, 3 * height, -(size / 2 + 0.1f)), Vector3(1, 0, 0), Vector3(0, 1, 0), tv_width, tv_length, tv_height, Color(0.3f, 0.3f, 0.3f));
}

// Handles the display callback to show what we have drawn.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3 rotatedVec = Vector3(1 - cameraPos.x, 1 - cameraPos.y, 1 - cameraPos.z);
	rotatedVec.rotate(0, yAngle, zAngle);

	Vector3 dir = rotatedVec + cameraPos;


	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
		dir.x, dir.y, dir.z,
		0.0f, 1.0f, 0.0f);

	glScalef(1, 1, 1);
	// glRotatef(15, 1.0, 1.0, 1.0);
	glLineWidth(4.0);

	//Draw floor
	Vector3 X = Vector3(0, 0, 0);
	Vector3 v1 = Vector3(1, 0, 0);
	Vector3 v2 = Vector3(0, 0, 1);
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++)
		{
			X = Vector3(i, 0, j);
			if ((i + j) % 2 == 0)
			{
				draw_square(X, v1, v2, 1, Color(0.0f, 0.0f, 0.0f));
			}
			else {
				draw_square(X, v1, v2, 1, Color(255.0f, 255.0f, 255.0f));
			}

		}
	}




	X = Vector3(0, 0, 0);
	v2 = Vector3(0, 1, 0);
	draw_square(X, v1, v2, 20, Color(0.2f, 0.2f, 0.2f));
	v1 = Vector3(0, 0, 1);
	draw_square(X, v1, v2, 20, Color(0.2f, 0.2f, 0.2f));

	glPushMatrix();

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1000.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1000.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1000.0);
	glEnd();

	glPopMatrix();
	draw_table(Vector3(5, 0, 5));
	draw_tv(Vector3(5, 3.1f, 6.5f));

	glRasterPos3f(0.0, 3.0, 0.0);
	for (char& c : msg)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
	glFlush();
	glutSwapBuffers();
}

// Handles the timer by incrementing the angle of rotation and requesting the
// window to display again.
// Since the timer function is only called once, it sets the same function to
// be called again.
void timer(int v)
{
	currentAngleOfRotation += 1.0;
	if (currentAngleOfRotation > 360.0)
	{
		currentAngleOfRotation -= 360.0;
	}
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, v);
}

void SpecialInput(int key, int x, int y) {
	float velocity = 0.05f;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		yAngle += velocity;
		break;
	case GLUT_KEY_RIGHT:
		yAngle -= velocity;
		break;
	case GLUT_KEY_UP:
		zAngle -= velocity;
		break;
	case GLUT_KEY_DOWN:
		zAngle += velocity;
		break;

	default:
		break;
	}
}

// Handles keyboard press
void keyboard(unsigned char key, int x, int y)
{

	Vector3 rotatedVec = Vector3(1 - cameraPos.x, 1 - cameraPos.y, 1 - cameraPos.z);
	rotatedVec.rotate(0, yAngle, zAngle);


	

	// cameraPos + reotatedVec*t
	
	float t = 0.05f;
	Vector3 dir = cameraPos - rotatedVec.getFactoredVector(t) ;
	float velocity = 0.5f;
	switch (key)
	{
	case 'w':
		// cameraPos.z -= velocity;
		t = -0.05f;
		 dir = cameraPos - rotatedVec.getFactoredVector(t) ;
		cameraPos.x = dir.x;
		cameraPos.y = dir.y;
		cameraPos.z = dir.z;
		// cameraPos.x -= velocity*dir.x;
		// cameraPos.y -= velocity*dir.y;
		// cameraPos.z -= velocity*dir.z;
		break;
	case 's':
		t = 0.05f;
		 dir = cameraPos - rotatedVec.getFactoredVector(t) ;
		cameraPos.x = dir.x;
		cameraPos.y = dir.y;
		cameraPos.z = dir.z;
		// cameraPos.z += velocity;
		break;
	case 'a':
		rotatedVec.rotate(0, 90, 0);
		t = -0.05f;
		 dir = cameraPos - rotatedVec.getFactoredVector(t) ;
		cameraPos.x = dir.x;
		cameraPos.y = dir.y;
		cameraPos.z = dir.z;

		// cameraPos.x -= velocity;
		break;
	case 'd':
		cameraPos.x += velocity;
		break;
	case ' ':
		cameraPos.y += velocity;
		break;
	case 'z':
		cameraPos.y -= velocity;
		break;

	default:
		break;
	}


	updateProjection();
}

void updateProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (orthoProjection)
	{
		glOrtho(-5.0, 5.0, -5.0, 5.0, 1.0, 100.0);
		glColor3f(0.2, 1.0, 0.2);
	}
	else
	{
		gluPerspective(60.0, aspect, 1, 100.0);
		glColor3f(0.0, 0.0, 0.8);
	}
	glutPostRedisplay();
}