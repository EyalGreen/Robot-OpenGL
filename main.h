#include "lib.h"

using namespace std;

// Constants
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;
const double PI = 3.14159265358979323846;
const float WINDOW_RATIO = WINDOW_WIDTH / (float)WINDOW_HEIGHT;
string msg = "Nitay and Eyal have a nice smile ";
float aspect = WINDOW_RATIO;



// This is the number of frames per second to render.
static const int FPS = 60;

// This global variable keeps track of the current orientation of the polyhedron.
static GLfloat currentAngleOfRotation = 0.0;

static bool isCameraDirection = false;



// Function where the scene drawing occures
void display(void);

// Function for keyboard press
void keyboard(unsigned char key, int x, int y);

//Function for keyboard special
void SpecialInput(int key, int x, int y);

// Function to handle reshape of the screen
void reshape(GLint w, GLint h);

void init();


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

		xAngle = xAngle * (PI / 180);
		yAngle = yAngle* (PI/ 180);
		zAngle = zAngle * (PI / 180);


		float xNew = 0.0f, yNew = 0.0f, zNew = 0.0f;
		// xNew = x * (cos(yAngle) * cos(zAngle)) - y * (cos(yAngle) * sin(zAngle)) + z * sin(yAngle);
		// yNew = x * (sin(xAngle) * sin(yAngle) * cos(zAngle) + cos(xAngle) * sin(zAngle)) + y * (cos(xAngle) * cos(zAngle) - sin(xAngle) * sin(yAngle) * sin(zAngle)) - z * sin(xAngle) * cos(y);
		// zNew = x * (sin(xAngle) * sin(zAngle) - cos(xAngle) * sin(yAngle) * cos(zAngle)) + y * (cos(xAngle) * sin(yAngle) * sin(zAngle) + sin(xAngle) * cos(zAngle)) + z * (cos(yAngle) * cos(xAngle));

		//rotate by zAngle deg
		Vector3::x = x*cos(zAngle) - y*sin(zAngle);
		Vector3::y = x*sin(zAngle) - y*cos(zAngle);
		Vector3::z = z;


		//rotate by yAngle deg
		Vector3::x = x*cos(yAngle) + z*sin(yAngle);
		Vector3::y = y;
		Vector3::z = (-x * sin(yAngle)) + (z * cos(yAngle));

		//rotate by xAngle deg
		Vector3::x = x;
		Vector3::y = y*cos(xAngle) - z*sin(xAngle);
		Vector3::z = y*sin(xAngle) + z*cos(xAngle);

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


