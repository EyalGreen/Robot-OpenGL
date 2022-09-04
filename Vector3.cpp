
#include "Vector3.h";
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