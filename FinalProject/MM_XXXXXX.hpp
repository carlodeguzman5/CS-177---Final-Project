#ifndef MM_XXXXXX_HPP
#define MM_XXXXXX_HPP

#include <cmath>

void multiply_matrix_4x4(const GLfloat *A, const GLfloat *B, GLfloat *C) {
	for ( size_t i = 0; i < 16; ++i ) {
		C[i] = 0;
		const size_t aBase = i % 4, bBase = (i/4) * 4;
		for ( size_t j = 0; j < 4; ++j ) {
			C[i] += A[aBase + j * 4] * B[bBase + j];
		}
	}
}

void create_rotation_matrix_4x4(GLfloat x, GLfloat y, GLfloat z, GLfloat theta, GLfloat *output) {
	
	using namespace std;
	
	for ( size_t i = 0; i < 16; ++i ) {
		if ( i % 5 == 0 )
			output[i] = 1;
		else
			output[i] = 0;
	}
	
	//normalize x, y, z
	GLfloat m = sqrt(x * x + y * y + z * z);
	if ( m < 0.00001 )
		return;
	x /= m; y /= m; z /= m;
	
	const GLfloat c = cos(theta), s = sin(theta);
	
	// First method:
	//
	// u3 = ||<x, y, z> % <1, 0, 0>|| = <0, z, -y>/sqrt(z*z + y*y);
	// u2 = u3 % u1 = <0, z, -y> % <x, y, z>/sqrt(z*z + y*y) = <z*z + y*y, -xy, -xz>/sqrt(z*z + y*y)
	// 
	// Second Method:
	//
	// u3 = ||<x, y, z> % <0, 0, 1>|| = <y, -x, 0>/sqrt(y*y + x*x);
	// u2 = u3 % u1 = <y, -x, 0> % <x, y, z>/sqrt(y*y + x*x) = <-xz, -yz, y*y + x*x>/sqrt(y*y + x*x)
	//
	GLfloat normalizationFactor = z*z + y*y;

	GLfloat alignMat[16];
	if ( normalizationFactor < 0.0001 ) {
	  normalizationFactor = y*y + x*x;
	  alignMat[0] = x,	alignMat[1] = -x*z,		alignMat[2] = y,	alignMat[3] = 0;
	  alignMat[4] = y,	alignMat[5] = -y*z,		alignMat[6] = -x,	alignMat[7] = 0;
	  alignMat[8] = z,	alignMat[9] = y*y+x*x,	alignMat[10] = 0,	alignMat[11] = 0;
	  alignMat[12] = 0,	alignMat[13] = 0,		alignMat[14] = 0,	alignMat[15] = 1;
	} else {
	  alignMat[0] = x,	alignMat[1] = z*z+y*y,	alignMat[2] = 0,	alignMat[3] = 0;
	  alignMat[4] = y,	alignMat[5] = -x*y,		alignMat[6] = z,	alignMat[7] = 0;
	  alignMat[8] = z,	alignMat[9] = -x*z,		alignMat[10] = -y,	alignMat[11] = 0;
	  alignMat[12] = 0,	alignMat[13] = 0,		alignMat[14] = 0,	alignMat[15] = 1;
	}
	GLfloat tmp[16];
	
	normalizationFactor = 1.0/sqrt(normalizationFactor);
	alignMat[1] *= normalizationFactor;
	alignMat[5] *= normalizationFactor;
	alignMat[9] *= normalizationFactor;
	
	alignMat[2] *= normalizationFactor;
	alignMat[6] *= normalizationFactor;
	alignMat[10] *= normalizationFactor;
	memcpy(output, alignMat, sizeof(tmp));
	
	GLfloat xRot[16] = {
		1, 0, 0, 0,
		0, c, s, 0,
		0, -s, c, 0,
		0, 0, 0, 1
	};
	
	multiply_matrix_4x4(xRot,output,tmp);
	memcpy(output, tmp, sizeof(tmp));

	//transpose alignMat
	for ( size_t k = 0; k < 4; ++k ) {
		for ( size_t j = k; j < 4; ++j ) {
			const size_t other = j * 4 + k;
			const size_t i = k * 4 + j;
			
			const GLfloat tmp = alignMat[i];
			alignMat[i] = alignMat[other];
			alignMat[other] = tmp;
		}		
	}
	
	multiply_matrix_4x4(alignMat, output, tmp);
	memcpy(output, tmp, sizeof(tmp));

}

#endif