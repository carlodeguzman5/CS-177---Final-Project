#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/GLU.h>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <iomanip>
#include "MM_XXXXXX.hpp"
#include "lookAt_XXXXXX.hpp"

using namespace std;
static const double MY_PI = 3.14159265358979323846264338327;
static const float sampleThreshold = 0.5f;
static const int scaleLimit = 30, limitHigh = 3;
int limitPointer = 0;

short *data;
long sampleCount;

long totalSeconds;
bool switches[10];
bool strobeSwitch = false;

struct Vtx {
	GLfloat x, y, z;
	GLfloat nx, ny, nz;
	//GLfloat s, t;
	GLuint color;
};

enum { ATTRIB_POS, ATTRIB_COLOR, ATTRIB_NORMAL, ATTRIB_TEXUNIT0 };

GLuint UNIFORM_viewProjMatrix, UNIFORM_modelMatrix, UNIFORM_normMatrix;

void bindVertexArray(Vtx *vtx) {
	glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, sizeof(Vtx), &(vtx->x));
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vtx), &(vtx->color));
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vtx), &(vtx->nx));
}

#include "GLMatrix.hpp"
#include "SceneGraph.hpp"

bool loadShaderSource(GLuint shader, const char *filePath) {
	FILE *f;
	fopen_s(&f, filePath, "r");
	if (!f) {
		cerr << "Cannot find file: " << filePath << '\n';
		return false;
	}
	fseek(f, 0, SEEK_END);
	const size_t sz = ftell(f) + 1;
	fseek(f, 0, SEEK_SET);

	GLchar *buffer = new GLchar[sz];
	fread(buffer, 1, sz, f);
	fclose(f);
	buffer[sz - 1] = 0;
	glShaderSource(shader, 1, (const GLchar**)&buffer, NULL);

	glCompileShader(shader);
	delete[] buffer;

	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar *log = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, &logLength, log);
		cout << "Shader Compile Log:\n" << log << endl;
		delete[] log;
	}

	return true;
}

SceneNode root;
SceneNode orbit;
PrismNodeStrip prism(0.5, 0.5, 30, 0xFFFF0000), prism2(0.5, 0.5, 30, 0xFFFF0000), prism3(0.5, 0.5, 30, 0xFFFF0000), prism4(0.5, 0.5, 30, 0xFFFF0000),
prism5(0.5, 0.5, 30, 0xFFFF0000), prism6(0.5, 0.5, 30, 0xFFFF0000), prism7(0.5, 0.5, 30, 0xFFFF0000), prism8(0.5, 0.5, 30, 0xFFFF0000), prism9(0.5, 0.5, 30, 0xFFFF0000);

CubeNode cube(1, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF),
cube2(1, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF),
cube3(1, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF),
cube4(1, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF);

GridNode plane(100, 100, 100, 100, 0xFFFFFFFF);

//PlaneNode starPlane(376, 376, 0xFFFFFFFF);

RingNode ring(5.5, 6, 1, 30, 0xFFFF0000),
spinRing1(1, 1.5, 0.5, 6, 0xFF0000FF),
spinRing2(1, 1.5, 0.5, 6, 0xFF00FF00), 
spinRing3(1, 1.5, 0.5, 6, 0xFF00FFFF),
spinRing4(1, 1.5, 0.5, 6, 0xFFFF0000),
outerRing(10, 11, .5, 200, 0xFF00FFFF);

void scalePrism(int n, float scale){

	switch (n){
	case 1:
		if (scale > 1){
			if (prism.getScale() < scaleLimit-8){
				prism.transform.scale(1, 1, scale);
				prism.setScale(prism.getScale() + 1);
				prism.changeColor(prism.getColor() + 0x00000600 - 0x00060000);
			}
		}
		else if (scale < 1){
			if (prism.getScale() > 0){
				prism.transform.scale(1, 1, scale);
				prism.setScale(prism.getScale() - 1);
				prism.changeColor(prism.getColor() - 0x00000600 + 0x00060000);
			}
		}
		break;
	case 2:
		if (scale > 1){
			if (prism2.getScale() < scaleLimit-6){
				prism2.transform.scale(1, 1, scale);
				prism2.setScale(prism2.getScale() + 1);
				prism2.changeColor(prism2.getColor() + 0x00000600 - 0x00060000);
			}
		}
		else if (scale < 1){
			if (prism2.getScale() > 0){
				prism2.transform.scale(1, 1, scale);
				prism2.setScale(prism2.getScale() - 1);
				prism2.changeColor(prism2.getColor() - 0x00000600 + 0x00060000);
			}
		}
		break;
	case 3: if (scale > 1){
				if (prism3.getScale() < scaleLimit-4){
					prism3.transform.scale(1, 1, scale);
					prism3.setScale(prism3.getScale() + 1);
					prism3.changeColor(prism3.getColor() + 0x00000600 - 0x00060000);
				}
			}
			else if (scale < 1){
				if (prism3.getScale() > 0){
					prism3.transform.scale(1, 1, scale);
					prism3.setScale(prism3.getScale() - 1);
					prism3.changeColor(prism3.getColor() - 0x00000600 + 0x00060000);
				}
			}
			break;
	case 4: if (scale > 1){
		if (prism4.getScale() < scaleLimit-2){
			prism4.transform.scale(1, 1, scale);
			prism4.setScale(prism4.getScale() + 1);
			prism4.changeColor(prism4.getColor() + 0x00000600 - 0x00060000);
		}
	}
			else if (scale < 1){
				if (prism4.getScale() > 0){
					prism4.transform.scale(1, 1, scale);
					prism4.setScale(prism4.getScale() - 1);
					prism4.changeColor(prism4.getColor() - 0x00000600 + 0x00060000);
				}
			}
			break;
	case 5:if (scale > 1){
		if (prism5.getScale() < scaleLimit){
			prism5.transform.scale(1, 1, scale);
			prism5.setScale(prism5.getScale() + 1);
			prism5.changeColor(prism5.getColor() + 0x00000600 - 0x00060000);
		}
	}
		   else if (scale < 1){
			   if (prism5.getScale() > 0){
				   prism5.transform.scale(1, 1, scale);
				   prism5.setScale(prism5.getScale() - 1);
				   prism5.changeColor(prism5.getColor() - 0x00000600 + 0x00060000);
			   }
		   }
		   break;
	case 6: if (scale > 1){
		if (prism6.getScale() < scaleLimit-2){
			prism6.transform.scale(1, 1, scale);
			prism6.setScale(prism6.getScale() + 1);
			prism6.changeColor(prism6.getColor() + 0x00000600 - 0x00060000);
		}
	}
			else if (scale < 1){
				if (prism6.getScale() > 0){
					prism6.transform.scale(1, 1, scale);
					prism6.setScale(prism6.getScale() - 1);
					prism6.changeColor(prism6.getColor() - 0x00000600 + 0x00060000);
				}
			}
			break;
	case 7: if (scale > 1){
		if (prism7.getScale() < scaleLimit-4){
			prism7.transform.scale(1, 1, scale);
			prism7.setScale(prism7.getScale() + 1);
			prism7.changeColor(prism7.getColor() + 0x00000600 - 0x00060000);
		}
	}
			else if (scale < 1){
				if (prism7.getScale() > 0){
					prism7.transform.scale(1, 1, scale);
					prism7.setScale(prism7.getScale() - 1);
					prism7.changeColor(prism7.getColor() - 0x00000600 + 0x00060000);
				}
			}
			break;
	case 8: if (scale > 1){
		if (prism8.getScale() < scaleLimit-6){
			prism8.transform.scale(1, 1, scale);
			prism8.setScale(prism8.getScale() + 1);
			prism8.changeColor(prism8.getColor() + 0x00000600 - 0x00060000);
		}
	}
			else if (scale < 1){
				if (prism8.getScale() > 0){
					prism8.transform.scale(1, 1, scale);
					prism8.setScale(prism8.getScale() - 1);
					prism8.changeColor(prism8.getColor() - 0x00000600 + 0x00060000);
				}
			}
			break;
	case 9: if (scale > 1){
		if (prism9.getScale() < scaleLimit - 8){
			prism9.transform.scale(1, 1, scale);
			prism9.setScale(prism9.getScale() + 1);
			prism9.changeColor(prism9.getColor() + 0x00000600 - 0x00060000);
		}
	}
			else if (scale < 1){
				if (prism9.getScale() > 0){
					prism9.transform.scale(1, 1, scale);
					prism9.setScale(prism9.getScale() - 1);
					prism9.changeColor(prism9.getColor() - 0x00000600 + 0x00060000);
				}
			}
			break;
	}
}

float windowFunction(short n){
	return 0.5 * (1-cos(2 *MY_PI*n/1024));
}

void ringEnlarge(){
	if (limitPointer <= limitHigh){
		limitPointer++;
		ring.transform.scale(1.05, 1.05, 1.05);
		ring.changeColor(ring.getColor() + 0x0000030 - 0x00300000);
		//outerRing.changeColor(ring.getColor() + 0x00030000 - 0x0000003);	
	}
}

void ringShrink(){
	if (limitPointer >= 0){
		limitPointer--;
		ring.transform.scale(1 / 1.05, 1 / 1.05, 1 / 1.05);
		if (limitPointer <= 0){
			ring.changeColor(0xFFFF0000);
			//outerRing.changeColor(0xFF00FFFF);
		}
		else{
			ring.changeColor(ring.getColor() - 0x00000030 + 0x00300000);
			//outerRing.changeColor(ring.getColor() - 0x00030000 + 0x0000003);
		}
	}
}

void generateMovementPerSecond(long second){
	long bitrate = 88200;
	long pointer = second * bitrate;

	long samplesPerCylinder = bitrate / 9;

	float average = 0;
	long temp = second * bitrate;
	while(pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;
	if (1 - average >= sampleThreshold){
		switches[9] = true;
		switches[0] = true;
	}
	else{
		switches[9] = false;
		switches[0] = false;
	}

	temp += samplesPerCylinder;
	average = 0;
	while (pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;
	if (1 - average >= .50){
		switches[1] = true;
	}
	else{
		switches[1] = false;
	}

	temp += samplesPerCylinder;
	average = 0;
	while (pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;
	if (1 - average >= .50){
		switches[2] = true;
	}
	else{
		switches[2] = false;
	}

	temp += samplesPerCylinder;
	average = 0;
	while (pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;
	if (1 - average >= .50){
		switches[3] = true;
	}
	else{
		switches[3] = false;
	}

	temp += samplesPerCylinder;
	average = 0;
	while (pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;
	if (1 - average >= .50){
		switches[4] = true;
	}
	else{
		switches[4] = false;
	}

	temp += samplesPerCylinder;
	average = 0;
	while (pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;
	if (1 - average >= .50){
		switches[5] = true;
	}
	else{
		switches[5] = false;
	}

	temp += samplesPerCylinder;
	average = 0;
	while (pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;
	if (1 - average >= .50){
		switches[6] = true;
	}
	else{
		switches[6] = false;
	}
	temp += samplesPerCylinder;
	average = 0;
	while (pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;

	if (1 - average >= .50){
		switches[7] = true;
	}
	else{
		switches[7] = false;
	}



	temp += samplesPerCylinder;
	average = 0;
	while (pointer < temp + samplesPerCylinder){
		average += windowFunction(data[pointer]);
		pointer++;
	}
	average = average / samplesPerCylinder;
	if (1 - average >= .50){
		switches[8] = true;
	}
	else{
		switches[8] = false;
	}




}

void generateSpectrum(){
	static const long bitrate = 88200; //44100 bitrate, 2 channels Left & Right

	totalSeconds = sampleCount / bitrate;
	long minutes = totalSeconds / 60;
	long seconds = totalSeconds % 60;
	cout << "Length: " << minutes << ":" << seconds << endl;

	float min = 21700000000, max = 0;
	for (int i = 0; i < sampleCount; i++){
		if (data[i] > max)
			max = data[i];
		else if (data[i] < min)
			min = data[i];
	}
	cout << "Min Sample: " << windowFunction(min) << endl;
	cout << "Max Sample: " << windowFunction(max) << endl;
}

//
//GLuint loadGLTexture(const char *fileName) {
//	GLFWimage img;
//	if (glfwReadImage(fileName, &img, 0) == GL_FALSE) {
//		cerr << "Unable to open image: " << fileName << '\n';
//		return 0;
//	}
//	GLuint ret = 0;
//	glGenTextures(1, &ret);
//
//	glBindTexture(GL_TEXTURE_2D, ret);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.Width, img.Height, 0, img.Format, GL_UNSIGNED_BYTE, img.Data);
//
//	glfwFreeImage(&img);
//
//	return ret;
//}

class MyStream : public sf::SoundStream
{
public:

	void load(const sf::SoundBuffer& buffer)
	{
		// extract the audio samples from the sound buffer to our own container
		m_samples.resize(1024);
		m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

		// reset the current playing position 
		m_currentSample = 0;

		// initialize the base class
		initialize(buffer.getChannelCount(), buffer.getSampleRate());
	}

private:

	virtual bool onGetData(Chunk& data){
		// number of samples to stream every time the function is called;
		// in a more robust implementation, it should be a fixed
		// amount of time rather than an arbitrary number of samples
		const int samplesToStream = 1024; //50000

		// set the pointer to the next audio samples to be played
		data.samples = &m_samples[m_currentSample];

		// have we reached the end of the sound?
		if (m_currentSample + samplesToStream <= m_samples.size())
		{
			// end not reached: stream the samples and continue
			data.sampleCount = samplesToStream;
			m_currentSample += samplesToStream;
			return true;
		}
		else
		{
			// end of stream reached: stream the remaining samples and stop playback
			data.sampleCount = m_samples.size() - m_currentSample;
			m_currentSample = m_samples.size();
			return false;
		}
	}

	virtual void onSeek(sf::Time timeOffset)
	{
		// compute the corresponding sample index according to the sample rate and channel count
		m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
	}

	std::vector<sf::Int16> m_samples;
	std::size_t m_currentSample;
};

int main() {

	// load an audio buffer from a sound file
	sf::SoundBuffer buffer;
	buffer.loadFromFile("carly.wav");

	sampleCount = buffer.getSampleCount();
	data = (short*)(buffer.getSamples());

	generateSpectrum();
	
	// initialize and play our custom stream
	MyStream stream;
	stream.load(buffer);
	stream.play();

	if (!glfwInit()) {
		cerr << "Unable to initialize OpenGL!\n";
		return -1;
	}
	if (!glfwOpenWindow(700, 700,
		8, 8, 8, 8,
		24, 0,
		GLFW_WINDOW)) {
		cerr << "Unable to create OpenGL window.\n";
		glfwTerminate();
		return -1;
	}

	if (glewInit() != GLEW_OK) {
		cerr << "Unable to hook OpenGL extensions!\n";
		return -1;
	}
	glfwSetWindowTitle("CS 177 Final Project");

	glfwEnable(GLFW_STICKY_KEYS);
	glfwSwapInterval(1);

	GLuint vtxShader = glCreateShader(GL_VERTEX_SHADER),
		fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!loadShaderSource(vtxShader, "diffuse_pf.vsh"))  {
		cerr << "Unable to load vertex shader.";
		return -1;
	}
	if (!loadShaderSource(fragShader, "diffuse_pf.fsh")) {
		cerr << "Unable to load fragment shader.";
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);

	GLuint program = glCreateProgram();
	glAttachShader(program, vtxShader);
	glAttachShader(program, fragShader);

	glBindAttribLocation(program, ATTRIB_POS, "pos");
	glBindAttribLocation(program, ATTRIB_COLOR, "color");
	glBindAttribLocation(program, ATTRIB_NORMAL, "normal");
	//glBindAttribLocation(program, ATTRIB_TEXUNIT0, "texCoord0");


	glLinkProgram(program);


	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			GLchar *log = new GLchar[logLength];
			glGetProgramInfoLog(program, logLength, &logLength, log);
			cout << "Program Compile Log:\n" << log << endl;
			delete[] log;
		}
	}
	glDeleteShader(fragShader);
	glDeleteShader(vtxShader);
	fragShader = 0;
	vtxShader = 0;

	UNIFORM_viewProjMatrix = glGetUniformLocation(program, "viewProjMatrix");
	UNIFORM_modelMatrix = glGetUniformLocation(program, "modelMatrix");
	UNIFORM_normMatrix = glGetUniformLocation(program, "normMatrix");

	GLuint UNIFORM_lightPos0 = glGetUniformLocation(program, "lightPos0"),
		UNIFORM_lightPos1 = glGetUniformLocation(program, "lightPos1"),
		UNIFORM_lightPos2 = glGetUniformLocation(program, "lightPos2"),
		UNIFORM_lightIntensity0 = glGetUniformLocation(program, "lightIntensity0"),
		UNIFORM_lightIntensity1 = glGetUniformLocation(program, "lightIntensity1"),
		UNIFORM_lightIntensity2 = glGetUniformLocation(program, "lightIntensity2"),
		UNIFORM_lightAmbient = glGetUniformLocation(program, "lightAmbient");
		//UNIFORM_texUnit0 = glGetUniformLocation(program, "texUnit0");

	glUseProgram(program);


	glEnableVertexAttribArray(ATTRIB_POS);
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	//glEnableVertexAttribArray(ATTRIB_TEXUNIT0);


	/*GLuint tex = loadGLTexture("image.tga");

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	GLuint t0 = glGetUniformLocation(program, "texUnit0");
	glUniform1i(t0, 0);

	glBindTexture(GL_TEXTURE_2D, tex);*/


	double t = 0;

	GLMatrix4 projection, view;
	GLfloat dist = 7;
	projection.setPerspective(MY_PI / 3, 1, .1, 300);
	lookAt(dist, dist, dist, 0, 0, 0, 0, 0, 1, view.mat);

	GLMatrix4 identity;
	identity.setIdentity();

	glUniform3f(UNIFORM_lightIntensity0, 1, 0, 0);
	glUniform3f(UNIFORM_lightIntensity1, 0, 0, 1);
	glUniform3f(UNIFORM_lightIntensity2, 1, 1, 1);
	glUniform3f(UNIFORM_lightAmbient, 0, .5, 0);


	GLfloat lx = 10, ly = 10, lz = 3, lw = 1;
	GLfloat lx1 = -10, ly1 = -10, lz1 = 3, lw1 = 1;
	
	GLMatrix4 lightAnimation;
	lightAnimation.setRotation(0, 0, 1, 0.05);

	GLMatrix4 lightAnimation1;
	lightAnimation1.setRotation(0, 0, -1, 0.05);


	int xpos, ypos;
	double xpos2, ypos2;


	prism.transform.translate(-3, -3, 0);
	prism2.transform.translate(-2.25, -2.25, 0);
	prism3.transform.translate(-1.5, -1.5, 0);
	prism4.transform.translate(-0.75, -0.75, 0);
	prism5.transform.translate(0, 0, 0);
	prism6.transform.translate(0.75, 0.75, 0);
	prism7.transform.translate(1.5, 1.5, 0);
	prism8.transform.translate(2.25, 2.25, 0);
	prism9.transform.translate(3, 3, 0);
	
	cube.transform.setRotation(1, 1, 0, MY_PI/4);
	cube.transform.translate(8, 0, 2);
	cube2.transform.setRotation(1, 1, 0, MY_PI / 4);
	cube2.transform.translate(-8, 0, 2);
	cube3.transform.setRotation(1, 1, 0, MY_PI / 4);
	cube3.transform.translate(0, 8, 2);
	cube4.transform.setRotation(1, 1, 0, MY_PI / 4);
	cube4.transform.translate(0, -8, 2);
	plane.transform.translate(0, 0, -5);
	
	spinRing1.transform.translate(8, 0, 2);
	spinRing2.transform.translate(-8, 0, 2);
	spinRing3.transform.translate(0, 8, 2);
	spinRing4.transform.translate(0, -8, 2);
	
	//starPlane.transform.translate(0, 0, -7);

	{
		root.children.push_back(&prism);
		root.children.push_back(&prism2);
		root.children.push_back(&prism3);
		root.children.push_back(&prism4);
		root.children.push_back(&prism5);
		root.children.push_back(&prism6);
		root.children.push_back(&prism7);
		root.children.push_back(&prism8);
		root.children.push_back(&prism9);
		
		orbit.children.push_back(&cube);
		orbit.children.push_back(&cube2);
		orbit.children.push_back(&cube3);
		orbit.children.push_back(&cube4);
		orbit.children.push_back(&spinRing1);
		orbit.children.push_back(&spinRing2);
		orbit.children.push_back(&spinRing3);
		orbit.children.push_back(&spinRing4);

		ring.children.push_back(&orbit);
		ring.children.push_back(&outerRing);

		root.children.push_back(&ring);
		root.children.push_back(&plane);
		//root.children.push_back(&starPlane);
	}

	GLMatrix4 r1, r2, r3, r4, o, g, gErraticMovement, sr1, sr2, sr3, sr4, outerRingMovement, mousePosition;
	r1.setRotation(1, 1, 0, 0.05);
	r2.setRotation(-1, -1, 0, 0.05);
	r3.setRotation(1, -1, 0, 0.05);
	r4.setRotation(-1, 1, 0, 0.05);

	o.setRotation(0, 0, 1, 0.025);
	g.setRotation(0, 0, 1, 0.01);

	sr1.setRotation(-1, -1, 0, 0.05);
	sr2.setRotation(1, 1, 0, 0.05);
	sr3.setRotation(-1, 1, 0, 0.05);
	sr4.setRotation(1, -1, 0, 0.05);

	outerRingMovement.setRotation(1, 0, 0, 0.01);


	float timeInSeconds = 0;
	long double start = time(0);

	float decay = 10000;
	float decayFactor = 0.95;

	GLfloat moveGridX = 0.1, 
		moveGridY = 0.1;

	do {


		glfwGetMousePos(&xpos, &ypos);
		xpos2 = (xpos * 30 / 700.0) - 15;
		ypos2 = (ypos * 30 / 700.0) - 15;
		

		decay *= decayFactor;
		if (decay <= rand() % 100){
			moveGridX = moveGridX * -1;
			moveGridY = moveGridY * -1;
			decay = 1000;
		}

		gErraticMovement.setTranslation(moveGridX, moveGridY, 0);
		plane.transform.operator*=(gErraticMovement);

		if (switches[9]){
			ringEnlarge();
		}
		else{
			ringShrink();
		}

		for (int i = 0; i < 9; i++){
			if (switches[i]){
				scalePrism(i + 1, 1.1);
			}
			else{
				scalePrism(i + 1, 1 / 1.1);
			}
		}

		if (time(0) >= start + 1){
			generateMovementPerSecond(timeInSeconds);
			outerRing.setColorSegments(timeInSeconds, totalSeconds, 0xFF0000FF);
			start = time(0);
			timeInSeconds++;
		}
		if (strobeSwitch){
			glUniform3f(UNIFORM_lightIntensity0, rand() % 2, rand() % 2, rand() % 2);
			glUniform3f(UNIFORM_lightIntensity1, rand() % 2, rand() % 2, rand() % 2);
			glUniform3f(UNIFORM_lightIntensity2, rand() % 2, rand() % 2, rand() % 2);
		}

		orbit.transform.operator*=(o);
		cube.transform.operator*=(r1);
		cube2.transform.operator*=(r2);
		cube3.transform.operator*=(r3);
		cube4.transform.operator*=(r4);
		spinRing1.transform.operator*=(sr1);
		spinRing2.transform.operator*=(sr2);
		spinRing3.transform.operator*=(sr3);
		spinRing4.transform.operator*=(sr4);
		plane.transform.operator*=(g);
		outerRing.transform.operator*=(outerRingMovement);
		ring.transform.operator*=(g);
		

		if (t >= .5){
			cube.changeColor(0xFF000000 + rand() % 0x00FFFFFF);
			cube2.changeColor(0xFF000000 + rand() % 0x00FFFFFF);
			cube3.changeColor(0xFF000000 + rand() % 0x00FFFFFF);
			cube4.changeColor(0xFF000000 + rand() % 0x00FFFFFF);
			t = 0;
		}
	

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
			GLMatrix4 r;
			r.setRotation(1, 0, 0, 0.1);
			view = view * r;
		}
	    if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
			GLMatrix4 r;
			r.setRotation(1, 0, 0, -0.1);
			view = view * r;
		}
		if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
			GLMatrix4 r;
			r.setRotation(0, 0, 1, 0.1);
			view = view * r;
		}
		if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
			GLMatrix4 r;
			r.setRotation(0, 0, 1, -0.1);
			view = view * r;
		}
		if (glfwGetKey(GLFW_KEY_RSHIFT) == GLFW_PRESS) {
			GLMatrix4 r;
			view.scale(1/1.1, 1/1.1, 1/1.1);
		}
		if (glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS) {
			GLMatrix4 r;
			view.scale(1.1, 1.1, 1.1);
		}

		if (glfwGetKey(GLFW_KEY_TAB)){
			strobeSwitch = !strobeSwitch;
		}

		if (glfwGetKey(GLFW_KEY_F1)){
			GLMatrix4 r;
			r.setRotation(1, 0, 0, 0.05);
			orbit.transform.operator*=(r);
		}
		if (glfwGetKey(GLFW_KEY_F2)){
			GLMatrix4 r;
			r.setRotation(0, 1, 0, 0.05);
			orbit.transform.operator*=(r);
		}

		GLMatrix4 vp = projection * view;

		glUniformMatrix4fv(UNIFORM_viewProjMatrix, 1, false, vp.mat);

		lightAnimation.mult(lx, ly, lz, lw);
		glUniform3f(UNIFORM_lightPos0, lx, ly, lz);

		lightAnimation1.mult(lx1, ly1, lz1, lw1);
		glUniform3f(UNIFORM_lightPos1, lx1, ly1, lz1);

		glUniform3f(UNIFORM_lightPos2, xpos2, ypos2, 4);

		//glUniform3f(UNIFORM_lightAmbient, 0, 0, 1);

		root.draw(identity, identity);
		//glUniform3f(UNIFORM_lightAmbient, 1, 0, 0);
		
		glfwSwapBuffers();
		t += 0.02f;
	} while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
	glfwTerminate();

	return 0;
}
