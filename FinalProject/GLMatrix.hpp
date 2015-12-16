#ifndef GLMATRIX_HPP__
#define GLMATRIX_HPP__
/********************
 *
 * 4x4 OpenGL Matrix class
 *
 ********************/
struct GLMatrix4 {
	GLfloat mat[16];
	
	void setIdentity() {
		mat[0] = 1, mat[4] = 0, mat[8] = 0, mat[12] = 0;
		mat[1] = 0, mat[5] = 1, mat[9] = 0, mat[13] = 0;
		mat[2] = 0, mat[6] = 0, mat[10] = 1, mat[14] = 0;
		mat[3] = 0, mat[7] = 0, mat[11] = 0, mat[15] = 1;
	}
	
	void setRotation(GLfloat x, GLfloat y, GLfloat z, GLfloat theta) {
		create_rotation_matrix_4x4(x, y, z, theta, mat);
	}
	
	void setTranslation(GLfloat x, GLfloat y, GLfloat z) {
		mat[0] = 1, mat[4] = 0, mat[8] = 0, mat[12] = x;
		mat[1] = 0, mat[5] = 1, mat[9] = 0, mat[13] = y;
		mat[2] = 0, mat[6] = 0, mat[10] = 1, mat[14] = z;
		mat[3] = 0, mat[7] = 0, mat[11] = 0, mat[15] = 1;
	}
	
	void translate(GLfloat x, GLfloat y, GLfloat z) {
		mat[12] += x;
		mat[13] += y;
		mat[14] += z;
	}
	
	void scale(GLfloat sx, GLfloat sy, GLfloat sz) {
		mat[0] *= sx;
		mat[4] *= sx;
		mat[8] *= sx;
		mat[12] *= sx;
		
		mat[1] *= sy;
		mat[5] *= sy;
		mat[9] *= sy;
		mat[13] *= sy;
		
		mat[2] *= sz;
		mat[6] *= sz;
		mat[10] *= sz;
		mat[14] *= sz;
	}
	
	void mult(GLfloat &x, GLfloat &y, GLfloat &z, GLfloat &w) {
		GLfloat ox = x, oy = y, oz = z, ow = w;
		x = mat[0] * ox + mat[4] * oy + mat[8] * oz + mat[12] * ow;
		y = mat[1] * ox + mat[5] * oy + mat[9] * oz + mat[13] * ow;
		z = mat[2] * ox + mat[6] * oy + mat[10] * oz + mat[14] * ow;
		w = mat[3] * ox + mat[7] * oy + mat[11] * oz + mat[15] * ow;
	}
	
	void transpose() {
		swap(mat[4],mat[1]);
		swap(mat[8],mat[2]);
		swap(mat[12],mat[3]);
		swap(mat[9],mat[6]);
		swap(mat[13],mat[7]);
		swap(mat[14], mat[11]);
	}
	
	GLMatrix4& operator=(const GLMatrix4 &rhs) {
		memcpy(mat, rhs.mat, sizeof(mat));
		return *this;
	}
	
	GLMatrix4 operator*(const GLMatrix4 &rhs) const {
		GLMatrix4 ret;
		
		multiply_matrix_4x4(mat, rhs.mat, ret.mat);
		
		return ret;
	}
	
	GLMatrix4& operator*=(const GLMatrix4 &rhs) {
		GLfloat tmp[16];
		multiply_matrix_4x4(mat, rhs.mat, tmp);
		
		memcpy(mat, tmp, sizeof(mat));
		return *this;
	}
	
	void setOrtho(const GLfloat l, const GLfloat r,
				  const GLfloat b, const GLfloat t,
				  const GLfloat n, const GLfloat f) {
		
		mat[0] = 2/(r-l),	mat[4] = 0,			mat[8] = 0,				mat[12] = -(r+l)/(r-l);
		mat[1] = 0,			mat[5] = 2/(t-b),	mat[9] = 0,				mat[13] = -(t+b)/(t-b);
		mat[2] = 0,			mat[6] = 0,			mat[10] = 2/(f-n),		mat[14] = -(n+f)/(f-n);
		mat[3] = 0,			mat[7] = 0,			mat[11] = 0,				mat[15] = 1;
	}
	void setOblique(const GLfloat vx, const GLfloat vy, const GLfloat vz, const GLfloat n) {
		mat[0] = 1, mat[4] = 0, mat[8] = -vx/vz,	mat[12] = n*vx/vz;
		mat[1] = 0, mat[5] = 1, mat[9] = -vy/vz,	mat[13] = n*vy/vz;
		mat[2] = 0, mat[6] = 0, mat[10] = 1,		mat[14] = 0;
		mat[3] = 0, mat[7] = 0, mat[11] = 0,		mat[15] = 1;
	}
	
	void setPerspective(const GLfloat fovy, const GLfloat aspect, const GLfloat near, const GLfloat far) {
		const GLfloat cott = 1.0/tan(fovy/2);
		mat[0] = cott/aspect,	mat[4] = 0,		mat[8] = 0,								mat[12] = 0;
		mat[1] = 0,				mat[5] = cott,	mat[9] = 0,								mat[13] = 0;
		mat[2] = 0,				mat[6] = 0,		mat[10] = (near+far)/(near-far),		mat[14] = 2*far*near/(near-far);
		mat[3] = 0,				mat[7] = 0,		mat[11] = -1,							mat[15] = 1;
	}
};

/********************
 *
 * 3x3 OpenGL Matrix class
 *
 ********************/
struct GLMatrix3 {
	GLfloat mat[9];
	
	void setIdentity() {
		mat[0] = 1, mat[3] = 0, mat[6] = 0;
		mat[1] = 0, mat[4] = 1, mat[7] = 0;
		mat[2] = 0, mat[5] = 0, mat[8] = 1;
	}
	
	void setRotation(GLfloat x, GLfloat y, GLfloat theta) {
		const GLfloat c = cos(theta), s = sin(theta);
		mat[0] = c, mat[3] = -s, mat[6] = -c * x + s * y + x;
		mat[1] = s, mat[4] = c,  mat[7] = -s * x - c * y + y;
		mat[2] = 0, mat[5] = 0,  mat[8] = 1;
	}
	
	void setTranslation(GLfloat x, GLfloat y) {
		mat[0] = 1, mat[3] = 0, mat[6] = x;
		mat[1] = 0, mat[4] = 1, mat[7] = y;
		mat[2] = 0, mat[5] = 0, mat[8] = 1;
	}
	
	void translate(GLfloat x, GLfloat y) {
		mat[6] += x;
		mat[7] += y;
	}
	
	void scale(GLfloat sx, GLfloat sy) {
		mat[0] *= sx;
		mat[3] *= sx;
		mat[6] *= sx;
		
		mat[1] *= sy;
		mat[4] *= sy;
		mat[7] *= sy;
	}
	
	void transpose() {
		swap(mat[3],mat[1]);
		swap(mat[6],mat[2]);
		swap(mat[7],mat[5]);
	}
	
	GLMatrix3& operator=(const GLMatrix3 &rhs) {
		memcpy(mat, rhs.mat, sizeof(mat));
		return *this;
	}
	
	GLMatrix3 operator*(const GLMatrix3 &rhs) const {
		GLMatrix3 ret;
		for ( int i = 0; i < 9; ++i ) {
			const int a = i % 3, b = (i / 3) * 3;
			ret.mat[i] = mat[a]*rhs.mat[b] + mat[a+3]*rhs.mat[b+1] + mat[a+6]*rhs.mat[b+2];
		}
		return ret;
	}
	
	GLMatrix3& operator*=(const GLMatrix3 &rhs) {
		GLMatrix3 tmp;
		for ( int i = 0; i < 9; ++i ) {
			const int a = i % 3, b = (i / 3) * 3;
			tmp.mat[i] = mat[a]*rhs.mat[b] + mat[a+3]*rhs.mat[b+1] + mat[a+6]*rhs.mat[b+2];
		}
		memcpy(mat, tmp.mat, sizeof(mat));
		return *this;
	}
};

#endif