// #include "main.h";


#include "lib.h"

using namespace std;
#define DEFAULT_FOV 45.0f

// Constants
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;
const double PI = 3.14159265358979323846;
const float WINDOW_RATIO = WINDOW_WIDTH / (float)WINDOW_HEIGHT;
string msg = "Nitay and Eyal have a nice smile ";
float aspect = WINDOW_RATIO;

enum
{
	MOUSE_LEFT_BUTTON = 0,
	MOUSE_MIDDLE_BUTTON = 1,
	MOUSE_RIGHT_BUTTON = 2,
	MOUSE_SCROLL_UP = 3,
	MOUSE_SCROLL_DOWN = 4
};

//handleCamera directions
void cameraHandler(int x, int y);

//mouse scroll callback change fov
void scroll_callback(double velocity);

//mouse scroll callback
void mouseFunc(int button, int state, int x, int y);

//is mouse in center
bool inCenter(int posx, int posy);

// This is the number of frames per second to render.
static const int FPS = 60;

// This global variable keeps track of the current orientation of the polyhedron.
static GLfloat currentAngleOfRotation = 0.0;




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
	//in deg
	void rotate(float yaw, float pitch){
		pitch = pitch * (PI / 180);
		yaw = yaw * (PI / 180);
		Vector3::x = cos(yaw)*cos(pitch);
		Vector3::y = sin(pitch);
		Vector3::z = sin(yaw) * cos(pitch);
	}

	void rotate(float xAngle, float yAngle, float zAngle) {

		xAngle = xAngle * (PI / 180);
		yAngle = yAngle* (PI/ 180);
		zAngle = zAngle * (PI / 180);
		
			

		// float xNew = 0.0f, yNew = 0.0f, zNew = 0.0f;
		// // xNew = x * (cos(yAngle) * cos(zAngle)) - y * (cos(yAngle) * sin(zAngle)) + z * sin(yAngle);
		// // yNew = x * (sin(xAngle) * sin(yAngle) * cos(zAngle) + cos(xAngle) * sin(zAngle)) + y * (cos(xAngle) * cos(zAngle) - sin(xAngle) * sin(yAngle) * sin(zAngle)) - z * sin(xAngle) * cos(y);
		// // zNew = x * (sin(xAngle) * sin(zAngle) - cos(xAngle) * sin(yAngle) * cos(zAngle)) + y * (cos(xAngle) * sin(yAngle) * sin(zAngle) + sin(xAngle) * cos(zAngle)) + z * (cos(yAngle) * cos(xAngle));

		// //rotate by zAngle deg
		// Vector3::x = x*cos(zAngle) - y*sin(zAngle);
		// Vector3::y = x*sin(zAngle) - y*cos(zAngle);
		// Vector3::z = z;


		// //rotate by yAngle deg
		// Vector3::x = x*cos(yAngle) + z*sin(yAngle);
		// Vector3::y = y;
		// Vector3::z = (-x * sin(yAngle)) + (z * cos(yAngle));

		// //rotate by xAngle deg
		// Vector3::x = x;
		// Vector3::y = y*cos(xAngle) - z*sin(xAngle);
		// Vector3::z = y*sin(xAngle) + z*cos(xAngle);

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

float deltaTime = 0.0f; //time between current frame and last frame
float lastFrame = 0.0f;

// mouse state
bool firstMouse = true;
int lastX = WINDOW_WIDTH / 2;
int lastY = WINDOW_HEIGHT / 2;
static bool isCameraDirection = false;

struct Camera{
	float MOUSE_FOV_VELOCITY = 0.5f;

	struct Vector3 pos;
	struct Vector3 front;
	struct Vector3 up; 
	float yaw, pitch;
	float fov;

	Camera(){
		pos = Vector3(0.0f, 5.0f, 20.0f);
		front = Vector3(0.0f, 0.0f, -1.0f);
		up =  Vector3(0.0f, 1.0f, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		fov = DEFAULT_FOV;
	}

	public:
		void reset(){
			yaw = -90.0f;
			pitch = 0.0f;
			firstMouse = true;
		}
};


Camera camera = Camera();
// Vector3 cameraPos = Vector3(20.0f, 5.0f, 20.0f);
// float yAngle = 0.0f, zAngle = 0.0f; //rotate over Y axis and Z axis

void init(){
	 GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat position[] = { 20.0, 5.0, 20.0, 0.0 };

    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat local_view[] = { 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
}


// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(400, 150);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("3D Robot - Nitay and Eyal");
	glutPassiveMotionFunc(cameraHandler);
	glutMouseFunc(mouseFunc);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialInput); 
	// glutSetCursor(GLUT_CURSOR_NONE); //hide mouse cursor 
 
	

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
void shiftCursorToCenter();


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

	// Vector3 rotatedVec = Vector3(1 - cameraPos.x, 1 - cameraPos.y, 1 - cameraPos.z);
	// rotatedVec.rotate(0, yAngle, zAngle);

	// Vector3 dir = rotatedVec + cameraPos;
	float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;


	// gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
	// 	dir.x, dir.y, dir.z,
	// 	0.0f, 1.0f, 0.0f);

	
	gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z, 
		camera.front.x + camera.pos.x, camera.front.y + camera.pos.y, camera.front.z + camera.pos.z,
		camera.up.x, camera.up.y, camera.up.z);

	
	glScalef(1, 1, 1);
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
	// float velocity = 1.0f;
	// switch (key)
	// {
	// case GLUT_KEY_LEFT:
	// 	yAngle += velocity;
	// 	break;
	// case GLUT_KEY_RIGHT:
	// 	yAngle -= velocity;
	// 	break;
	// case GLUT_KEY_UP:
	// 	zAngle -= velocity;
	// 	break;
	// case GLUT_KEY_DOWN:
	// 	zAngle += velocity;
	// 	break;

	// default:
	// 	break;
	// }
}

// Handles keyboard press
void keyboard(unsigned char key, int x, int y)
{

	float cameraSpeed = 0.1 * deltaTime;
	Vector3 dir;
	switch (key)	{
		case 'w':
			camera.pos = camera.pos + camera.front.getFactoredVector(cameraSpeed);
			break;
		case 's':
			camera.pos = camera.pos - camera.front.getFactoredVector(cameraSpeed);
			break;
		case 'a':
			// cameraPos.x -= velocity;
			dir = camera.front.cross(camera.up);
			dir.normalize();
			camera.pos = camera.pos - dir.getFactoredVector(cameraSpeed);
			break;
		case 'd':
			dir = camera.front.cross(camera.up);
			dir.normalize();
			camera.pos = camera.pos + dir.getFactoredVector(cameraSpeed);
			break;
		case ' ':
			camera.pos = camera.pos + camera.up.getFactoredVector(cameraSpeed);
			break;
		case 'z':
			camera.pos = camera.pos - camera.up.getFactoredVector(cameraSpeed);
			break;
		case 'v':
			isCameraDirection = !isCameraDirection;
			if(!isCameraDirection){
				camera.reset();
			}
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
	gluPerspective(camera.fov, aspect, 1, 100.0);
	glColor3f(0.0, 0.0, 0.8);
	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y){
	// cout << "T";
	if (button == MOUSE_LEFT_BUTTON)
	{
		cout << "test";
		scroll_callback(camera.MOUSE_FOV_VELOCITY);
	}
	else if(button == MOUSE_SCROLL_DOWN){
			scroll_callback(-camera.MOUSE_FOV_VELOCITY);
	}
}
void scroll_callback(double velocity){
	if (camera.fov >= 1.0f && camera.fov <= DEFAULT_FOV)
		camera.fov -= velocity;;
	if (camera.fov <= 1.0f)
		camera.fov = 1.0f;
	if (camera.fov >= DEFAULT_FOV)
		camera.fov = DEFAULT_FOV;
		
		
}

void cameraHandler(int x, int y){
	if(!isCameraDirection){
		return;
	}

	

	// if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT){
	// 	shiftCursorToCenter(&x, &y);
	// 	return;
	// }
	

	if(firstMouse){
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float sensetivity = 0.1f;

	float xOffset = (x - lastX) * sensetivity;
	float yOffset = (lastY - y) * sensetivity;

	lastX = x;
	lastY = y;

	camera.pitch += yOffset;
	camera.yaw += xOffset;

	if(camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	else if (camera.pitch < -89.0f)
		camera.pitch = -89.0f;
	
	camera.front.rotate(camera.yaw, camera.pitch); 	
	camera.front.normalize();
	if(!inCenter(x, y)){
		shiftCursorToCenter();
		firstMouse = true; 
		return;
	}
	// std::cout << x << ", " <<  y << std::endl;
}

bool inCenter(int posx, int posy){
	int length = 350;
	return ((WINDOW_WIDTH / 2) - length  <= posx  && posx <= (WINDOW_WIDTH/2) + length )&&(
		(WINDOW_HEIGHT / 2) - length <= posy && posy <= (WINDOW_HEIGHT / 2) + length);	
}
void shiftCursorToCenter(){
	glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);	
}