#include"Vector.h"
#include<cmath>
Vector CameraPosition;


Vector TargetPosition;
Vector Vector:: cross(Vector& b){
	

		Vector tmp;
		tmp.x = y*b.z - z*b.y;
		tmp.y = z*b.x-x*b.z;
		tmp.z = x*b.y-y*b.x;
		return tmp;
}	

void Vector::setVector(float x,float y,float z){
		this->x = x;
		this->y = y;
		this->z = z;
}
Vector Vector::operator -(Vector& b){
	Vector tmp;
	tmp.x = this->x - b.x;
	tmp.y = this->y - b.y;
	tmp.z = this->z - b.z;
	return tmp;
	
}
Vector Vector::operator +(Vector& b){
	Vector tmp;
	tmp.x = this->x + b.x;
	tmp.y = this->y + b.y;
	tmp.z = this->z + b.z;
	return tmp;
	
}
void Vector::normalize(){
	float len = sqrt(x * x + y * y + z * z);
	if (len > 1e-5){
		x /= len;
		y /= len;
		z /= len;
	}	
	
}
float Vector::dot(Vector& b){
	

	return  	x * b.x+y * b.y+z * b.z;


}
