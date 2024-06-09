class Vector{

//	float z = eye.x*(y1 * z2 - z1 * y2) + eye.y*(z1 * x2 - x1 * z2) + eye.z*(x1 * y2 - y1 * x2);
	public:
	float x = 0,y = 0,z = 0;
	
	Vector cross(Vector& b);
	void setVector(float x,float y,float z);
	Vector operator -(Vector & b);
	void normalize();
	Vector operator +(Vector & b);
	float dot(Vector& b);
	Vector(int x = 0,int y = 0,int z = 0){
		this->x = x;
		this->y = y;
		this->z = z;
	}
};
