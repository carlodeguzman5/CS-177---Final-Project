void cross(const GLfloat *a, const GLfloat *b, GLfloat *output) {
	output[0] = a[1]*b[2] - a[2]*b[1];
	output[1] = a[2]*b[0] - a[0]*b[2];
	output[2] = a[0]*b[1] - a[1]*b[0];
}

void lookAt(GLfloat camX, GLfloat camY, GLfloat camZ, 
			GLfloat tX, GLfloat tY, GLfloat tZ,
			GLfloat uX, GLfloat uY, GLfloat uZ, GLfloat *output) {
	GLfloat z[] = { camX - tX, camY - tY, camZ - tZ };
	
	{
		const GLfloat dMag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
		if ( dMag < 0.00001 ) return;
		z[0] /= dMag, z[1] /= dMag, z[2] /= dMag;
	}
	
	const GLfloat u[] = {uX, uY, uZ};
	GLfloat x[3], y[3];
	//x = up % z
	cross(u, z, x);
	//y = z % x
	cross(z, x, y);
	
	output[0] = x[0],	output[4] = x[1],	output[8] = x[2];
	output[1] = y[0],	output[5] = y[1],	output[9] = y[2];
	output[2] = z[0],	output[6] = z[1],	output[10] = z[2];
	output[3] = 0,		output[7] = 0,		output[11] = 0;
	
	output[12] = -camX * x[0] - camY * x[1] - camZ * x[2];
	output[13] = -camX * y[0] - camY * y[1] - camZ * y[2];
	output[14] = -camX * z[0] - camY * z[1] - camZ * z[2];
	output[15] = 1;
	
}