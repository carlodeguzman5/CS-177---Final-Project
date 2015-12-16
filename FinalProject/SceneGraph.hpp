#ifndef SCENEGRAPH_HPP__
#define SCENEGRAPH_HPP__

class SceneNode {
public:
	GLMatrix4 transform, normTransform;
	vector<SceneNode*> children;
	SceneNode() {
		transform.setIdentity();
		normTransform.setIdentity();
	}
	virtual void draw(const GLMatrix4 &parentTransform, const GLMatrix4 &parentNormTransform) {
		drawChildren(parentTransform * transform, parentNormTransform * normTransform);
	}
	
	virtual void update(double t) {
		for ( size_t i = 0; i < children.size(); ++i )
			children[i]->update(t);
	}
	
	void drawChildren(const GLMatrix4 &t, const GLMatrix4 &nt) {
		for ( size_t i = 0; i < children.size(); ++i )
			children[i]->draw(t, nt);
	}
	
	virtual ~SceneNode() {
	}
	
	static void setTransform(const GLMatrix4 &t, const GLMatrix4 &nt) {
		glUniformMatrix4fv(UNIFORM_modelMatrix, 1, false, t.mat);
		glUniformMatrix4fv(UNIFORM_normMatrix, 1, false, nt.mat);
	}
};

class RingNode : public SceneNode{
	vector<Vtx> vertices;
public:
	RingNode(GLfloat radius1, GLfloat radius2, GLfloat width, GLuint sides, GLuint color) : vertices(4 * (sides * 2 + 2)){
		int offset = 0;
		for (int i = 0; i < sides * 2 + 2; i+=2){
			vertices[i].color = color;
			vertices[i].nx = 1;
			vertices[i].ny = 1;
			vertices[i].nz = 1;
			
			vertices[i].x = radius2 * cos(i / 2 * (2.0 * MY_PI) / sides);
			vertices[i].y = radius2 * sin(i / 2 * (2.0 * MY_PI) / sides);
			vertices[i].z = -width / 2;
			
			vertices[i + 1].color = color;
			vertices[i + 1].nx = 1;
			vertices[i + 1].ny = 1;
			vertices[i + 1].nz = 1;

			vertices[i + 1].x = radius2 * cos(i / 2 * (2 * MY_PI) / sides);
			vertices[i + 1].y = radius2 * sin(i / 2 * (2 * MY_PI) / sides);
			vertices[i + 1].z = width / 2;

			offset = i;
		}	
		offset += 2;

		int offset2;
		
		for (int i = 0; i < sides * 2 + 2; i+=2){
			vertices[offset + i].color = color;
			vertices[offset + i].nx = 1;
			vertices[offset + i].ny = 1;
			vertices[offset + i].nz = 1;

			vertices[offset + i].x = radius1 * cos(i / 2 * (2 * MY_PI) / sides);
			vertices[offset + i].y = radius1 * sin(i / 2 * (2 * MY_PI) / sides);
			vertices[offset + i].z = -width / 2;

			vertices[offset + i + 1].color = color;
			vertices[offset + i + 1].nx = 1;
			vertices[offset + i + 1].ny = 1;
			vertices[offset + i + 1].nz = 1;
			
			vertices[offset + i + 1].x = radius2 * cos(i / 2 * (2 * MY_PI) / sides);
			vertices[offset + i + 1].y = radius2 * sin(i / 2 * (2 * MY_PI) / sides);
			vertices[offset + i + 1].z = -width / 2;
			
			offset2 = i + offset;
		}

		offset2 += 2;

		int offset3;

		for (int i = 0; i < sides * 2 + 2; i+=2){
			vertices[offset2 + i].color = color;
			vertices[offset2 + i].nx = 1;
			vertices[offset2 + i].ny = 1;
			vertices[offset2 + i].nz = 1;
		
			vertices[offset2 + i].x = radius1 * cos(i / 2 *(2 * MY_PI) / sides);
			vertices[offset2 + i].y = radius1 * sin(i / 2 *(2 * MY_PI) / sides);
			vertices[offset2 + i].z = width / 2;
		
			vertices[offset2 + i + 1].color = color;
			vertices[offset2 + i + 1].nx = 1;
			vertices[offset2 + i + 1].ny = 1;
			vertices[offset2 + i + 1].nz = 1;

			vertices[offset2 + i + 1].x = radius2 * cos(i / 2 *(2 * MY_PI) / sides);
			vertices[offset2 + i + 1].y = radius2 * sin(i / 2 *(2 * MY_PI) / sides);
			vertices[offset2 + i + 1].z = width / 2;
			
			offset3 = i + offset2;
		}
		
		offset3 += 2;

		for (int i = 0; i < sides * 2 + 2; i+=2){
			vertices[offset3 + i].color = color;
			vertices[offset3 + i].nx = 1;
			vertices[offset3 + i].ny = 1;
			vertices[offset3 + i].nz = 1;
		
			vertices[offset3 + i].x = radius1 * cos(i / 2 *(2 * MY_PI) / sides);
			vertices[offset3 + i].y = radius1 * sin(i / 2 *(2 * MY_PI) / sides);
			vertices[offset3 + i].z = -width / 2;
		
			vertices[offset3 + i + 1].color = color;
			vertices[offset3 + i + 1].nx = 1;
			vertices[offset3 + i + 1].ny = 1;
			vertices[offset3 + i + 1].nz = 1;


			vertices[offset3 + i + 1].x = radius1 * cos(i / 2 *(2 * MY_PI) / sides);
			vertices[offset3 + i + 1].y = radius1 * sin(i / 2 *(2 * MY_PI) / sides);
			vertices[offset3 + i + 1].z = width / 2;	
		}
	}
		virtual void draw(const GLMatrix4 &parentTransform, const GLMatrix4 &parentNormTransform) {
			const GLMatrix4 &t = parentTransform * transform,
				&nt = parentNormTransform * normTransform;
			bindVertexArray(&vertices[0]);
			setTransform(t, nt);


			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 4 );
			glDrawArrays(GL_TRIANGLE_STRIP, vertices.size() / 4 , vertices.size() / 4);
			glDrawArrays(GL_TRIANGLE_STRIP, 2 * vertices.size() / 4, vertices.size() / 4);
			glDrawArrays(GL_TRIANGLE_STRIP, 3 * vertices.size() / 4, vertices.size() / 4);

			drawChildren(t, nt);
		}

		virtual void changeColor(GLuint color){
			for (int i = 0; i < vertices.size(); i++){
				vertices[i].color = color;
			}
		}

		virtual GLuint getColor(){
			return vertices[0].color;
		}

		virtual int getSegments(){
			return vertices.size() / 8 - 1;
		}

		virtual void setColorSegments(double seconds, double totalSeconds, GLuint color){
			int secondsPerSegment = totalSeconds / getSegments();

			int currentSegment = seconds / secondsPerSegment;
			for (int i = 0; i < currentSegment*2; i++){
				vertices[i].color = color;
				vertices[vertices.size() / 4 + i].color = color;
				vertices[vertices.size() / 2 + i].color = color;
				vertices[3 * vertices.size() / 4 + i].color = color;
			}
		}
};

class PyramidNode : public SceneNode {
	vector<Vtx> vertices;
	GLfloat height;
public:
	PyramidNode(GLfloat radius, GLfloat height, GLuint sides, GLuint tipColor, GLuint color) : vertices(2*(2 + max(sides,3u))), height(height) {
		sides = max(sides,3u);
		vertices[0].x = vertices[0].y = 0;
		vertices[0].z = height;
		vertices[0].color = tipColor;
		vertices[0].nx = 0;
		vertices[0].ny = 0;
		vertices[0].nz = 1;
		
		const GLuint skip = vertices.size()/2;
		
		vertices[skip] = vertices.front();
		vertices[skip].color = color;
		vertices[skip].z = 0;
		vertices[skip].nz = -1;
		//||perp(<radius, 0, 0> - <0, 0, height>)|| = ||perp(radius, 0, -height)|| = <height, 0, radius>/sqrt(height^2 + radius^2)
		const double normFactor = 1.0/sqrt(height*height + radius*radius);
		const double baseNormX = height * normFactor, baseNormZ = radius * normFactor;
		for ( size_t i = 0; i < sides; ++i ) {
			const double angle = 2.0 * i * MY_PI/(double)sides;
			const double c = cos(angle), s = sin(angle);
			vertices[i + 1].x = radius * c;
			vertices[i + 1].y = radius * s;
			vertices[i + 1].z = 0;
			vertices[i + 1].color = color;
			//Rz(||<height, 0, radius>||) 
			vertices[i + 1].nx = c * baseNormX;
			vertices[i + 1].ny = s * baseNormX;
			vertices[i + 1].nz = baseNormZ;
			
			vertices[skip+i+1] = vertices[i+1];
			vertices[skip+i+1].y *= -1;
			vertices[skip+i+1].nx = 0;
			vertices[skip+i+1].ny = 0;
			vertices[skip+i+1].nz = -1;
		}
		vertices[skip-1].x = radius;
		vertices[skip-1].y = 0;
		vertices[skip-1].z = 0;
		vertices[skip-1].nx = baseNormX;
		vertices[skip-1].ny = 0;
		vertices[skip-1].nz = baseNormZ;
		vertices[skip-1].color = color;
		
		vertices.back() = vertices[skip-1];
		vertices.back().nx = 0;
		vertices.back().ny = 0;
		vertices.back().nz = -1;
	}
	
	virtual void draw(const GLMatrix4 &parentTransform, const GLMatrix4 &parentNormTransform) {
		const GLMatrix4 &t = parentTransform * transform,
		&nt = parentNormTransform * normTransform;
		bindVertexArray(&vertices[0]);
		setTransform(t, nt);
		
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size()/2);
		glDrawArrays(GL_TRIANGLE_FAN, vertices.size()/2, vertices.size()/2);		
		
		drawChildren(t, nt);
	}
};

class PrismNodeStrip : public SceneNode {
	vector<Vtx> vertices;
	int scale;
public:
	PrismNodeStrip(GLfloat radius, GLfloat height, GLuint sides, GLuint color) {
		scale = 0;

		sides += sides%2;
		
		vertices.resize(sides*2 + sides*2 + 6);
		
		for ( size_t i = 0; i <= sides; ++i ) {
			const double angle = 2.0 * i * MY_PI/(double)sides;
			const double c = radius * cos(angle), s = radius * sin(angle);
			vertices[2*i].x = c;
			vertices[2*i].y = s;
			vertices[2*i].z = height/2;
			vertices[2*i].color = color;
			vertices[2*i].nx = c;
			vertices[2*i].ny = s;
			vertices[2*i].nz = 0;
			vertices[2*i + 1].x = c;
			vertices[2*i + 1].y = s;
			vertices[2*i + 1].z = -height/2;
			vertices[2*i + 1].color = color;
			vertices[2*i + 1].nx = c;
			vertices[2*i + 1].ny = s;
			vertices[2*i + 1].nz = 0;
		}
		//cut the triangle strip using degenerate triangles
		vertices[2*sides+2] = vertices[2*sides+1];
		
		//Top lid
		
		//right point
		vertices[2*sides+4].x = radius;
		vertices[2*sides+4].y = 0;
		vertices[2*sides+4].z = height/2;
		vertices[2*sides+4].color = color;
		vertices[2*sides+4].nx = 0;
		vertices[2*sides+4].ny = 0;
		vertices[2*sides+4].nz = 1;

		vertices[2*sides+3] = vertices[2*sides+4];
		
		//left point
		vertices[3*sides+3].x = -radius;
		vertices[3*sides+3].y = 0;
		vertices[3*sides+3].z = height/2;
		vertices[3*sides+3].color = color;
		vertices[3*sides+3].nx = 0;
		vertices[3*sides+3].ny = 0;
		vertices[3*sides+3].nz = 1;
		
		vertices[3*sides+4] = vertices[3*sides+3];
		
		//everything in between
		{
			const GLuint offset = 2*sides+5;
			for ( size_t i = 0; i < (sides-2)/2; ++i ) {
				const double angle = (i + 1.0)/sides * 2.0 * MY_PI;
				const double c = cos(angle), s = sin(angle);
				vertices[offset+2*i].x = radius * c;
				vertices[offset+2*i].y = radius * s;
				vertices[offset+2*i].z = height/2;
				vertices[offset+2*i].color = color;
				vertices[offset+2*i].nx = 0;
				vertices[offset+2*i].ny = 0;
				vertices[offset+2*i].nz = 1;
				
				vertices[offset+2*i+1].x = radius * c;
				vertices[offset+2*i+1].y = -radius * s;
				vertices[offset+2*i+1].z = height/2;
				vertices[offset+2*i+1].color = color;
				vertices[offset+2*i+1].nx = 0;
				vertices[offset+2*i+1].ny = 0;
				vertices[offset+2*i+1].nz = 1;
			}
		}
		

		//bottom lid
		//right point
		vertices[3*sides+6].x = radius;
		vertices[3*sides+6].y = 0;
		vertices[3*sides+6].z = -height/2;
		vertices[3*sides+6].color = color;
		vertices[3*sides+6].nx = 0;
		vertices[3*sides+6].ny = 0;
		vertices[3*sides+6].nz = -1;
		
		vertices[3*sides+5] = vertices[3*sides+6];
		
		//left point
		vertices[4*sides+5].x = -radius;
		vertices[4*sides+5].y = 0;
		vertices[4*sides+5].z = -height/2;
		vertices[4*sides+5].color = color;
		vertices[4*sides+5].nx = 0;
		vertices[4*sides+5].ny = 0;
		vertices[4*sides+5].nz = -1;
		
		//everything in between
		{
			const GLuint offset = 3*sides+7;
			for ( size_t i = 0; i < (sides-2)/2; ++i ) {
				const double angle = (i + 1.0)/sides * 2.0 * MY_PI;
				const double c = cos(angle), s = sin(angle);
				vertices[offset+2*i].x = radius * c;
				vertices[offset+2*i].y = -radius * s;
				vertices[offset+2*i].z = -height/2;
				vertices[offset+2*i].color = color;
				vertices[offset+2*i].nx = 0;
				vertices[offset+2*i].ny = 0;
				vertices[offset+2*i].nz = -1;
				
				vertices[offset+2*i+1].x = radius * c;
				vertices[offset+2*i+1].y = radius * s;
				vertices[offset+2*i+1].z = -height/2;
				vertices[offset+2*i+1].color = color;
				vertices[offset+2*i+1].nx = 0;
				vertices[offset+2*i+1].ny = 0;
				vertices[offset+2*i+1].nz = -1;
			}
		}
	}
	
	virtual void draw(const GLMatrix4 &parentTransform, const GLMatrix4 &parentNormTransform) {
		const GLMatrix4 &t = parentTransform * transform,
		&nt = parentNormTransform * normTransform;
		bindVertexArray(&vertices[0]);
		setTransform(t, nt);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
		
		drawChildren(t, nt);
	}

	virtual void changeColor(GLuint color){
		for (int i = 0; i < vertices.size(); i++){
			vertices[i].color = color;
		}
	}

	virtual GLuint getColor(){
		return vertices[0].color;
	}

	virtual GLuint getScale(){
		return scale;
	}
	virtual void setScale(GLuint s){
		scale = s;
	}
};

class CubeNode : public SceneNode {
	Vtx vtx[6*6];
public:
	CubeNode(GLfloat size, GLuint cx, GLuint cy, GLuint cz, GLuint ncx, GLuint ncy, GLuint ncz) {
		static const GLfloat permutation[][4][3] = {
			{
				{-1,1,1},{-1,-1,1},{1,-1,1},{1,1,1}
			},
			{
				{1,1,-1},{1,-1,-1},{-1,-1,-1},{-1,1,-1}
			},
			{
				{1,1,1},{1,-1,1},{1,-1,-1},{1,1,-1}
			},
			{
				{-1,1,-1},{-1,-1,-1},{-1,-1,1},{-1,1,1}
			},
			{
				{-1,1,-1},{-1,1,1},{1,1,1},{1,1,-1}
			},
			{
				{1,-1,-1},{1,-1,1},{-1,-1,1},{-1,-1,-1}
			}
		};
		static const GLfloat norms[][3] = {
			{0, 0, 1},
			{0, 0, -1},
			{1, 0, 0},
			{-1, 0, 0},
			{0, 1, 0},
			{0, -1, 0}
		};
		const GLuint colors[] = {
			cz, ncz, cx, ncx, cy, ncy
		};
		
		int index = 0;
		size /= 2;
		for ( size_t i = 0; i < 6; ++i ) {
			vtx[index].x = size * permutation[i][0][0];
			vtx[index].y = size * permutation[i][0][1];
			vtx[index].z = size * permutation[i][0][2];
			vtx[index].color = colors[i];
			vtx[index].nx = norms[i][0];
			vtx[index].ny = norms[i][1];
			vtx[index].nz = norms[i][2];
			++index;
			vtx[index].x = size * permutation[i][1][0];
			vtx[index].y = size * permutation[i][1][1];
			vtx[index].z = size * permutation[i][1][2];
			vtx[index].color = colors[i];
			vtx[index].nx = norms[i][0];
			vtx[index].ny = norms[i][1];
			vtx[index].nz = norms[i][2];
			++index;
			vtx[index].x = size * permutation[i][3][0];
			vtx[index].y = size * permutation[i][3][1];
			vtx[index].z = size * permutation[i][3][2];
			vtx[index].color = colors[i];
			vtx[index].nx = norms[i][0];
			vtx[index].ny = norms[i][1];
			vtx[index].nz = norms[i][2];
			++index;
			vtx[index].x = size * permutation[i][3][0];
			vtx[index].y = size * permutation[i][3][1];
			vtx[index].z = size * permutation[i][3][2];
			vtx[index].color = colors[i];
			vtx[index].nx = norms[i][0];
			vtx[index].ny = norms[i][1];
			vtx[index].nz = norms[i][2];
			++index;
			vtx[index].x = size * permutation[i][1][0];
			vtx[index].y = size * permutation[i][1][1];
			vtx[index].z = size * permutation[i][1][2];
			vtx[index].color = colors[i];
			vtx[index].nx = norms[i][0];
			vtx[index].ny = norms[i][1];
			vtx[index].nz = norms[i][2];
			++index;
			vtx[index].x = size * permutation[i][2][0];
			vtx[index].y = size * permutation[i][2][1];
			vtx[index].z = size * permutation[i][2][2];
			vtx[index].color = colors[i];
			vtx[index].nx = norms[i][0];
			vtx[index].ny = norms[i][1];
			vtx[index].nz = norms[i][2];
			++index;
		}
		
		
	}
	
	virtual void draw(const GLMatrix4 &parentTransform, const GLMatrix4 &parentNormTransform) {
		const GLMatrix4 &t = parentTransform * transform,
		&nt = parentNormTransform * normTransform;
		bindVertexArray(vtx);
		setTransform(t, nt);
		
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vtx)/sizeof(Vtx));
		
		drawChildren(t,nt);
	}

	virtual void changeColor(GLuint color){
		for (int i = 0; i < 36; i++){
			vtx[i].color = color;
		}
	}
};

class GridNode : public SceneNode {
	vector<Vtx> vertices;
public:
	GridNode(GLsizei xSize, GLsizei ySize, GLsizei xLines, GLsizei yLines, GLuint color) :
	vertices((xLines+1) * 2 + (yLines+1) * 2) {
		size_t vtx = 0;
		for ( size_t i = 0; i <= xLines; ++i ) {
			vertices[vtx].x = -xSize/2;
			vertices[vtx].y = (double)ySize * (double)i / xLines - ySize/2.0;
			vertices[vtx].z = 0;
			vertices[vtx].color = color;
			vertices[vtx].nx = 0;
			vertices[vtx].ny = 0;
			vertices[vtx].nz = 1;
			++vtx;
			
			vertices[vtx].x = xSize/2;
			vertices[vtx].y = (double)ySize * (double)i / xLines - ySize/2.0;
			vertices[vtx].z = 0;
			vertices[vtx].color = color;
			vertices[vtx].nx = 0;
			vertices[vtx].ny = 0;
			vertices[vtx].nz = 1;
			++vtx;
		}
		
		for ( size_t j = 0; j <= yLines; ++j ) {
			vertices[vtx].x = (double)xSize * (double)j / yLines - xSize/2.0;
			vertices[vtx].y = -ySize/2;
			vertices[vtx].z = 0;
			vertices[vtx].color = color;
			vertices[vtx].nx = 0;
			vertices[vtx].ny = 0;
			vertices[vtx].nz = 1;
			++vtx;
			
			vertices[vtx].x = (double)xSize * (double)j / yLines - xSize/2.0;
			vertices[vtx].y = ySize/2;
			vertices[vtx].z = 0;
			vertices[vtx].color = color;
			vertices[vtx].nx = 0;
			vertices[vtx].ny = 0;
			vertices[vtx].nz = 1;
			++vtx;
		}
	}
	
	virtual void draw(const GLMatrix4 &parentTransform, const GLMatrix4 &parentNormalTransform) {
		const GLMatrix4 &t = parentTransform * transform,
		&nt = parentNormalTransform * normTransform;
		bindVertexArray(&vertices[0]);
		setTransform(t, nt);
		
		glDrawArrays(GL_LINES, 0, vertices.size());
		
		drawChildren(t, nt);
	}
};


class PlaneNode : public SceneNode {
	Vtx vertices[4];
public:
	PlaneNode(GLsizei xSize, GLsizei ySize, GLuint color) {
		vertices[0].x = -xSize/2;
		vertices[0].y = ySize/2;
		vertices[0].z = 0;
		vertices[0].color = color;
		vertices[0].nx = 0;
		vertices[0].ny = 0;
		vertices[0].nz = 1;
		
		vertices[1].x = -xSize/2;
		vertices[1].y = -ySize/2;
		vertices[1].z = 0;
		vertices[1].color = color;
		vertices[1].nx = 0;
		vertices[1].ny = 0;
		vertices[1].nz = 1;
		
		vertices[2].x = xSize/2;
		vertices[2].y = ySize/2;
		vertices[2].z = 0;
		vertices[2].color = color;
		vertices[2].nx = 0;
		vertices[2].ny = 0;
		vertices[2].nz = 1;
		
		vertices[3].x = xSize/2;
		vertices[3].y = -ySize/2;
		vertices[3].z = 0;
		vertices[3].color = color;
		vertices[3].nx = 0;
		vertices[3].ny = 0;
		vertices[3].nz = 1;
	}
	
	virtual void draw(const GLMatrix4 &parentTransform, const GLMatrix4 &parentNormalTransform) {
		const GLMatrix4 &t = parentTransform * transform,
		&nt = parentNormalTransform * normTransform;
		bindVertexArray(vertices);
		setTransform(t, nt);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		drawChildren(t, nt);
	}
};
#endif