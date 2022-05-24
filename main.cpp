/****************************************************************************

Andrea Solla Alfonsin
andrea.solla.alfonsin@rai.usc.es
Proyecto final

-------------------------------INSTRUCCIONES-----------------------------------
1 -> vista en 1ª persona
2 -> vista de pájaro
3 -> cambio de disfraz
L -> iluinar escena

Flechas en vista de pájaro:
	Movimiento en el sentido de la flecha

 Flechas en 1ª persona
	flecha arriba -> ,archa alante
	flecha abajo -> marcha atrás
	flecha izquierda -> giro 90º hacia la izquierda
	flecha derecha -> giro 90º hacia la derecha

******************************************************************************/


#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include "esfera.h"
//Para el sonido
#include <irrklang/irrKlang.h>
using namespace irrklang;

//Para las transformaciones
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

#define PI 3.1416
#define RADIANES PI/180.0

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//Sonidos 
ISoundEngine* sonido = createIrrKlangDevice();
bool sonidoP=false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 800;

//Variables para la posición y el movimiento
float posInicialX = -1.7, posInicialY = 1.4;
int vertical=0, horizontal=1;

extern GLuint setShaders(const char *nVertx, const char *nFrag);
GLuint shaderProgram;

//Texturas
GLuint texturaSuelo, seto, perder, ganar, inicio;

unsigned int VAOCuadrado;
unsigned int VAOCubo;
unsigned int VAOEsfera;

//Variables para la vista y las escenas
int vista=0, escena=3;
GLfloat posicionCamaraX;
GLfloat posicionCamaraY;
GLfloat posicionCamaraZ;

//Estructuras para los elementos

typedef struct {
	float px, py, pz;
	float angulo_giro;
	float sx, sy, sz;
	unsigned int listaRender;
	int vertices;
	GLuint textura;
	GLuint trajes[];
}objeto; 

typedef struct {
	float px, py, pz;
	float angulo_giro;
	float sx, sy, sz;
	unsigned int listaRender;
	int vertices;
	float pasoX, pasoY;
	int ida;
	char eje;
	float r, g, b;
}fantasma;

typedef struct {
	float px, py, pz;
	float sx, sy, sz;
	unsigned int listaRender;
	int vertices;
	float r, g, b;
	GLuint textura;
	const char* imagen;
	int cogida;
	bool son;
	float angulo;
	bool sube;
}fruta;

objeto cuerpo = { -1.7, 1.4, 0.2, 0.0, 0.07, 0.07, 0.07, 0, 1080, 0 };

const char* trajes[] = {"traje1.jpg", "traje2.jpg", "traje3.jpg" };
int nTrajes = 3, n = 0;

fantasma fantasma1 = {0.0, -1.4, 0.1, 0, 0.08, 0.08, 0.08, 0, 1080, 0.002, 0, 1, 'x', 1.0, 0.0, 0.0};
fantasma fantasma2 = { 0.7, 0.75, 0.1, 0, 0.08, 0.08, 0.08, 0, 1080, 0.0, 0.002, 1, 'y', 0.0, 1.0, 0.0 };
fantasma fantasma3 = { -0.7, 0.7, 0.1, 0, 0.08, 0.08, 0.08, 0, 1080, 0.003, 0.0, 1, 'x', 0.0, 0.0, 1.0 };

fruta sandia = { 0.0, 0.0, 0.15, 0.04, 0.04, 0.06, 0, 1080, 1.0, 1.0, 1.0, 0, "sandia.jpg", 0, false, 0, true };
fruta limon = { 0.0, 0.0, 0.12, 0.04, 0.04, 0.06, 0, 1080, 1.0, 0.8, 0.0, 0, "limon.jpg", 0, false, 0, true };
fruta mango = { 0.0, 0.0, 0.12, 0.04, 0.04, 0.06, 0, 1080, 1.0, 0.6, 0.0, 0, "mango.jpg", 0, false, 0, true };
fruta manzana = { 0.0, 0.0, 0.12, 0.04, 0.04, 0.03, 0, 1080, 1.0, 0.8, 0.0, 0, "manzana.jpg", 0, false, 0, true };

int numFantasmas=3;
fantasma *todosF[] = { &fantasma1, &fantasma2, &fantasma3 };

int numFrutas = 4;
fruta* frutas[] = { &sandia, &limon, &mango, &manzana };

void dibujaCubo() {
	unsigned int VBO;
	
	float  vertices[] = {

		//frente
		-.5f,.5f,.5f,	0.0f,0.0f,1.0f,  0.0, 1.0,
		-.5f,-.5f,.5f,	0.0f,0.0f,1.0f,  0.0, 0.0,
		.5f,-.5f,.5f,	0.0f,0.0f,1.0f,  1.0, 0.0,
		-.5f,.5f,.5f,	0.0f,0.0f,1.0f,  0.0, 1.0,
		.5f,-.5f,.5f,	0.0f,0.0f,1.0f,  1.0, 0.0,
		.5f, .5f, .5f,	0.0f,0.0f,1.0f,  1.0, 1.0,

		//derecha
		.5f, .5f, .5f,	1.0f,0.0f,0.0f,  0.0, 1.0,
		.5f,-.5f,.5f,	1.0f,0.0f,0.0f,  0.0, 0.0,
		.5f,-.5f,-.5f,	1.0f,0.0f,0.0f,  1.0, 0.0,
		.5f, .5f, .5f,	1.0f,0.0f,0.0f,  0.0, 1.0,
		.5f,-.5f,-.5f,	1.0f,0.0f,0.0f,  1.0, 0.0,
		.5f,.5f,-.5f,	1.0f,0.0f,0.0f,  1.0, 1.0,

		//izquierda
		-.5f,.5f,-.5f,	-1.0f,0.0f,0.0f,  0.0, 1.0,
		-.5f,-.5f,-.5f,	-1.0f,0.0f,0.0f,  0.0, 0.0,
		-.5f,-.5f,.5f,	-1.0f,0.0f,0.0f,  1.0, 0.0,
		-.5f,.5f,-.5f,	-1.0f,0.0f,0.0f,  0.0, 1.0,
		-.5f,-.5f,.5f,	-1.0f,0.0f,0.0f,  1.0, 0.0,
		-.5f,.5f,.5f,	-1.0f,0.0f,0.0f,  1.0, 1.0,

		//abajo
		-.5f,-.5f,.5f,	0.0f,-1.0f,0.0f,  0.0, 1.0,
		-.5f,-.5f,-.5f,	0.0f,-1.0f,0.0f,  0.0, 0.0,
		.5f,-.5f,-.5f,	0.0f,-1.0f,0.0f,  1.0, 0.0,
		-.5f,-.5f,.5f,	0.0f,-1.0f,0.0f,  0.0, 0.0,
		.5f,-.5f,-.5f,	0.0f,-1.0f,0.0f,  1.0, 0.0,
		.5f,-.5f,.5f,	0.0f,-1.0f,0.0f,  1.0, 1.0,

		//arriba
		-.5f,.5f,-.5f,	 0.0f,1.0f,0.0f,  0.0, 1.0,
		-.5f,.5f,.5f,	 0.0f,1.0f,0.0f,  0.0, 0.0,
		.5f,.5f,.5f,	 0.0f,1.0f,0.0f,  1.0, 0.0,
		-.5f,.5f,-.5f,	 0.0f,1.0f,0.0f,  0.0, 1.0,
		.5f, .5f, .5f,	 0.0f,1.0f,0.0f,  1.0, 0.0,
		.5f,.5f,-.5f,	 0.0f,1.0f,0.0f,  1.0, 1.0,

		//atras
		.5f,.5f,-.5f,	0.0f,0.0f,-1.0f,  0.0, 1.0,
		.5f,-.5f,-.5f,	0.0f,0.0f,-1.0f,  0.0, 0.0,
		-.5f,-.5f,-.5f,	0.0f,0.0f,-1.0f,  1.0, 0.0,
		.5f,.5f,-.5f,	0.0f,0.0f,-1.0f,  0.0, 1.0,
		-.5f,-.5f,-.5f,	0.0f,0.0f,-1.0f,  1.0, 0.0,
		-.5f,.5f,-.5f,	0.0f,0.0f,-1.0f,  1.0, 1.0,
	};


	glGenVertexArrays(1, &VAOCubo);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAOCubo);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
}

void dibujaCuadrado() {
	unsigned int VBO;
	
	float  vertices[] = {

		//Frontal vertices Normales texturas
		-.5f,.5f,.0f,	0.0f,0.0f,1.0f,  0.0, 1.0,
		-.5f,-.5f,.0f,	0.0f,0.0f,1.0f,  0.0, 0.0,
		.5f,-.5f,.0f,	0.0f,0.0f,1.0f,  1.0, 0.0,
		-.5f,.5f,.0f,	0.0f,0.0f,1.0f,  0.0, 1.0,
		.5f,-.5f,.0f,	0.0f,0.0f,1.0f,  1.0, 0.0,
		.5f, .5f, .0f,	0.0f,0.0f,1.0f,  1.0, 1.0,

	};

	glGenVertexArrays(1, &VAOCuadrado);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAOCuadrado);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void dibujaEsfera() {
	unsigned int VBO;

	glGenVertexArrays(1, &VAOEsfera);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAOEsfera);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);

}

void openGlInit() {
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glDepthFunc(GL_ALWAYS);
	glCullFace(GL_BACK);
}

//Funcion que comprueba si se choca con un fantasma dado
void comprobarChoque(fantasma fant) {
	float choqueX, choqueY, dX, dY;
	choqueX = abs(fant.px - cuerpo.px);
	choqueY = abs(fant.py - cuerpo.py);
	dX = fant.sx + cuerpo.sx;
	dY = fant.sy + cuerpo.sy;
	if (choqueX < dX && choqueY < dY && !sonidoP) {
		escena = 1;
		sonido->play2D("sonidoPerder.wav", false);
		sonidoP = true;
	}
}

//FUncion que comprueba si se coge alguna fruta
void cogerFruta(fruta *fr) {
	float choqueX, choqueY, dX, dY;
	choqueX = abs(fr->px - cuerpo.px);
	choqueY = abs(fr->py - cuerpo.py);
	dX = fr->sx + cuerpo.sx;
	dY = fr->sy + cuerpo.sy;
	if (choqueX < dX && choqueY < dY) {
		fr->cogida = 1;
		if (!fr->son) {
			sonido->play2D("sonidoGanar.wav", false);
			fr->son = true;
		}
	}
}

//Funcion que hace las comprobaciones necesarias durante la partida
void comprobarPartida() {
	int i;
	bool ganar = true;
	for (i = 0; i < numFantasmas; i++) {
		comprobarChoque(*todosF[i]);
	}
	for (i = 0; i < numFrutas; i++) {
		cogerFruta(frutas[i]);
		if (frutas[i]->cogida == 0) {
			ganar = false;
		}
	}
	if (ganar) {
		escena = 2;
	}
}

//Funcion para cargar cada textura
unsigned int cargaTexturas(const char *imagen) {
	GLuint textura;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(imagen, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return textura;
}

//Funcion que carga todas las texturas necesarias
void texturas() {
	int i;
	texturaSuelo = cargaTexturas("suelo3.jpg");
	perder = cargaTexturas("perder3.jpg");
	ganar = cargaTexturas("ganar.jpg");
	inicio = cargaTexturas("inicio.jpg");
	seto = cargaTexturas("seto.jpg");
	for (i = 0; i < numFrutas; i++) {
		frutas[i]->textura = cargaTexturas(frutas[i]->imagen);
	}
	for (i = 0; i < nTrajes; i++) {
		cuerpo.trajes[i] = cargaTexturas(trajes[i]);
	}
}

void vistaPrimeraPersona() {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	float mirarX = 10*cos(cuerpo.angulo_giro * RADIANES);
	float mirarY = 10*sin(cuerpo.angulo_giro * RADIANES);

	view = glm::mat4();
	posicionCamaraX = cuerpo.px;
	posicionCamaraY = cuerpo.py;
	posicionCamaraZ = cuerpo.pz;
	view = glm::lookAt(glm::vec3(posicionCamaraX, posicionCamaraY, posicionCamaraZ), glm::vec3(mirarX+cuerpo.px, mirarY+cuerpo.py, -0.04), glm::vec3(0.0, 0.0, 1.0));
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	projection = glm::mat4();
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void vistaNormal() {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	view = glm::mat4();
	posicionCamaraX = 0;
	posicionCamaraY = 0;
	posicionCamaraZ = 7;
	view = glm::lookAt(glm::vec3(posicionCamaraX, posicionCamaraY, posicionCamaraZ), glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0));
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	projection = glm::mat4();
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void actualizarVista() {
	switch (vista)
	{
	case 0:
		vistaNormal();
		break;
	case 1:
		vistaPrimeraPersona();
		break;
	default:
		break;
	}
}

bool moverVertical() {
	float pos = cuerpo.px;
	if ((pos > -1.46 && pos < -1.34) || (pos > -0.76 && pos < -0.64) || (pos > -0.06 && pos < 0.06) || (pos > 0.64 && pos < 0.76) || (pos > 1.34 && pos < 1.46)) {
		//Si llega al extremo sale por el otro lado
		if (cuerpo.py > 1.9) {
			cuerpo.py = -1.9;
		}
		else if (cuerpo.py < -1.9) {
			cuerpo.py = 1.9;
		}
		return true;
	}
	return false;
}

bool moverHorizontal() {
	float pos = cuerpo.py;
	if ((pos > -1.46 && pos < -1.34) || (pos > -0.76 && pos < -0.64) || (pos > -0.06 && pos < 0.06) || (pos > 0.64 && pos < 0.76) || (pos > 1.34 && pos < 1.46)) {
		//Si llega al extremo sale por el otro lado
		if (cuerpo.px > 1.9) {
			cuerpo.px = -1.9;
		}
		else if (cuerpo.px < -1.9) {
			cuerpo.px = 1.9;
		}
		return true;
	}
	return false;
}

void moverFantasma(fantasma *fant) {
	float pos;
	if (fant->eje == 'x') {
		pos = fant->px;
	}
	else {
		pos = fant->py;
	}
	if (fant->ida == 1 && pos < 2) {
		fant->px += fant->pasoX;
		fant->py += fant->pasoY;
	}
	else if (fant->ida == 1 && pos >= 2) {
		fant->ida = 0;
		fant->px -= fant->pasoX;
		fant->py -= fant->pasoY;
	}
	else if (fant->ida == 0 && pos > -2) {
		fant->px -= fant->pasoX;
		fant->py -= fant->pasoY;
	}
	else {
		fant->ida = 1;
		fant->px += fant->pasoX;
		fant->py += fant->pasoY;
	}
}

void moverFruta(fruta *fruta) {
	if (fruta->pz < 0.2 && fruta->sube) {
		fruta->pz += 0.0001;
	}
	else if(fruta->pz > 0.1 && !fruta->sube) {
		fruta->pz -= 0.0001;
	}
	else {
		fruta->sube = !fruta->sube;
	}
	fruta->angulo += 0.25;
	if (fruta->angulo > 360) {
		fruta->angulo = 0.0;
	}
}

void movimiento() {
	//actualizarVista();
	int i;
	for (i = 0; i < numFantasmas; i++) {
		moverFantasma(todosF[i]);
	}
	for (i = 0; i < numFrutas; i++) {
		moverFruta(frutas[i]);
	}
}

void linterna(glm::mat4 transform) {
	glUseProgram(shaderProgram);
	glUniform1f(glGetUniformLocation(shaderProgram, "angulo"), 25.0);
	unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

	// la posicion de la luz va con el objeto que se mueve
	unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glm::vec3 posLinterna = glm::vec3(transform * glm::vec4(-5.0f, 0.0f, 0.0f, 1.0f));
	glUniform3f(lightPosLoc, posLinterna[0], posLinterna[1], posLinterna[2]);
	//dirección del foco
	unsigned int lightDirLoc = glGetUniformLocation(shaderProgram, "dirSuelo");
	glm::vec3 dirSuelo = glm::vec3(transform * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	glUniform3f(lightDirLoc, dirSuelo[0], dirSuelo[1], dirSuelo[2]);

	//la posicion de la camara
	unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, posicionCamaraX, posicionCamaraY, posicionCamaraZ);
}

void dibujaElemento() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glm::mat4 transform;
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniform1i(glGetUniformLocation(shaderProgram, "usaTextura"), 1);

	cuerpo.textura = cuerpo.trajes[n];
	glUniform3f(colorLoc, 1.0, 1.0, 1.0);

	transform = glm::mat4();
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, cuerpo.textura);
	transform = glm::translate(transform, glm::vec3(cuerpo.px, cuerpo.py, cuerpo.pz));
	transform = glm::rotate(transform, (float)(cuerpo.angulo_giro * RADIANES), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(cuerpo.sx, cuerpo.sy, cuerpo.sz));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(cuerpo.listaRender);
	glDrawArrays(GL_TRIANGLES, 0, cuerpo.vertices);

	linterna(transform);
}

void dibujaSuelo() {
	glm::mat4 transform, transform2;
	float escalaCubo = 0.5;
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniform1i(glGetUniformLocation(shaderProgram, "usaTextura"), 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	float i = -1.75, j = -1.75, escala_suelo = 5;

	//Suelo
	transform = glm::mat4();
	glActiveTexture(GL_TEXTURE);
	glUniform3f(colorLoc, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texturaSuelo);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(4.0, 4.0, 4.0));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(VAOCuadrado);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	for (i = -1.75; i <= 2; i += (0.2 + escalaCubo)) {
		for (j = -1.75; j <= 2; j += (0.2 + escalaCubo)) {
			transform2 = glm::mat4();
			glActiveTexture(GL_TEXTURE);
			glUniform3f(colorLoc, 0.3, 0.6, 1.0);
			glBindTexture(GL_TEXTURE_2D, seto);
			transform2 = glm::translate(transform2, glm::vec3(i, j, 0.2f));
			transform2 = glm::scale(transform2, glm::vec3(escalaCubo, escalaCubo, escalaCubo));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform2));
			
			glBindVertexArray(VAOCubo);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

void dibujaFantasma(fantasma fant) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glm::mat4 transform;
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniform1i(glGetUniformLocation(shaderProgram, "usaTextura"), 0);
	
	glUniform3f(colorLoc, fant.r, fant.g, fant.b);

	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(fant.px, fant.py, fant.pz));
	transform = glm::rotate(transform, (float)(fant.angulo_giro * RADIANES), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(fant.sx, fant.sy, fant.sz));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(fant.listaRender);
	glDrawArrays(GL_TRIANGLES, 0, fant.vertices);
}

float iniciarPosicionFruta() {
	float a;
	
	a = ((rand() % 400) - 200) / 100.0;
	if (a < -1) {
		a = -1.4;
	}
	else if (a < -0.85) {
		a = -0.7;
	}
	else if (a < 0.35) {
		a = 0;
	}
	else if (a < 1.05) {
		a = 0.7;
	}
	else {
		a = 1.4;
	}
	return a;
}

//Funcion para colocar las frutas en una posicion aleatoria
void colocaFrutas() {
	int i;
	for (i = 0; i < numFrutas; i++) {
		frutas[i]->px = iniciarPosicionFruta();
		frutas[i]->py = ((rand() % 400) - 200) / 100.0;
		while (frutas[i]->px == -1.4 && frutas[i]->py == 1.4) {
			frutas[i]->py = ((rand() % 400) - 200) / 100.0;
		}
		frutas[i]->cogida = 0;
		frutas[i]->son = false;
	}
}

void dibujaFruta(fruta fruta) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glm::mat4 transform;
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniform1i(glGetUniformLocation(shaderProgram, "usaTextura"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "fruta"), 1);
	glUniform3f(colorLoc, fruta.r, fruta.g, fruta.b);

	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, fruta.textura);

	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(fruta.px, fruta.py, fruta.pz));
	transform = glm::rotate(transform, (float)(fruta.angulo * RADIANES), glm::vec3(0.0f, 0.0f, 1.0f));

	transform = glm::scale(transform, glm::vec3(fruta.sx, fruta.sy, fruta.sz));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(fruta.listaRender);
	glDrawArrays(GL_TRIANGLES, 0, fruta.vertices);
}

//Funcion para dibujar la partida 
void dibujaEscena() {
	int i=0;
	glm::mat4 transform, transformtemp, aux;
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");	

	//SUELO
	dibujaSuelo();

	//COMECOCOS
	dibujaElemento();

	//FANTASMAS
	for (i = 0; i < numFantasmas; i++) {
		dibujaFantasma(*todosF[i]);
	}

	//FRUTAS
	for (i = 0; i < numFrutas; i++) {
		if (frutas[i]->cogida == 0) {
			dibujaFruta(*frutas[i]); //Si aun no la ha cogido se dibuja
		}
	}
}

//Funcion para dibujar las pantallas de inicio y fin
void dibujaPantalla(GLuint dibujo) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glm::mat4 transform;
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniform1f(glGetUniformLocation(shaderProgram, "angulo"), 90.0);
	glUniform1i(glGetUniformLocation(shaderProgram, "usaTextura"), 1);
	glUniform3f(colorLoc, 1.0, 1.0, 1.0);

	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, dibujo);
	
	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(0.0, 0.0, 0.0));

	transform = glm::scale(transform, glm::vec3(4.0, 4.0, 4.0));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(VAOCuadrado);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Iluminación
	unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

	// la posicion de la luz va con el objeto que se mueve
	unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0.0, 0.0, 7.0);

	//dirección del foco
	unsigned int lightDirLoc = glGetUniformLocation(shaderProgram, "dirSuelo");
	glUniform3f(lightDirLoc, 0.0, 0.0, 0.0);

	//la posicion de la camara
	unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, posicionCamaraX, posicionCamaraY, posicionCamaraZ);
}

//Funcion para asignar a cada elemento su lista correspondiente
void asignarListas() {
	cuerpo.listaRender = VAOEsfera;
	int i;
	for (i = 0; i < numFantasmas; i++) {
		todosF[i]->listaRender = VAOEsfera;
	}
	for (i = 0; i < numFrutas; i++) {
		frutas[i]->listaRender = VAOEsfera;
	}
}

//Funcion para preparar el comienzo de la partida
void Empezar() {
	cuerpo.px = posInicialX;
	cuerpo.py = posInicialY;
	colocaFrutas();
	escena = 0;
	sonidoP = false;
	cuerpo.angulo_giro = 0.0;
	vertical = 0;
	horizontal = 1;
	glUniform1f(glGetUniformLocation(shaderProgram, "ambientI"), 0.0);
}

void incrementarX(float in) {
	if (moverHorizontal()) {
		cuerpo.px += in;
	}
}

void incrementarY(float in) {
	if (moverVertical()) {
		cuerpo.py += in;
	}
}

//Funcion para cambiar el traje
void siguienteTraje() {
	if (n < (nTrajes - 1)) {
		n++;
	}
	else {
		n = 0;
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << key << std::endl;
	float paso = 0.02;
	int aux;

	//MOVIMIENTO
	if (key == 265) { //arriba
		if (vista == 0) {
			cuerpo.angulo_giro = 90.0;
			vertical = 1;
			horizontal = 0;
			incrementarY(paso);
		}
		else {
			incrementarX(paso * horizontal);
			incrementarY(paso * vertical);
		}
	}
	if (key == 264) { //abajo
		if (vista == 0) {
			cuerpo.angulo_giro = 270.0;
			vertical = -1;
			horizontal = 0;
			incrementarY(-paso);
		}
		else {
			incrementarX(-paso * horizontal);
			incrementarY(-paso * vertical);
		}
	}
	if (glfwGetKey (window, GLFW_KEY_RIGHT) == GLFW_PRESS) {  //derecha
		if (vista == 0) {
			cuerpo.angulo_giro = 0.0;
			vertical = 0;
			horizontal = 1;
			incrementarX(paso);
		}
		else {
			cuerpo.angulo_giro = fmod(cuerpo.angulo_giro - 90.0, 360);
			aux = horizontal;
			horizontal = vertical;
			vertical = -aux;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { //izquierda
		if (vista == 0) {
			cuerpo.angulo_giro = 180.0;
			vertical = 0;
			horizontal = -1;
			incrementarX(-paso);
		}
		else {
			cuerpo.angulo_giro = fmod(cuerpo.angulo_giro + 90.0, 360);
			aux = horizontal;
			horizontal = -vertical;
			vertical = aux;
		}
	}

	//VISTAS
	if (key == 49) { //1
		vista = 1;  //vista Primera Persona
	}
	if (key == 50) { //2
		vista = 0; //vista normal
	}

	//Luz
	if (key == 76) {
		glUniform1f(glGetUniformLocation(shaderProgram, "ambientI"), 0.2);
	}

	//Empezar
	if (key == 257 && escena != 0) {
		Empezar();
	}

	//Cambio de traje
	if (glfwGetKey(window, 51) == GLFW_PRESS) {
		siguienteTraje();
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT) {
		std::cout << "Space key pressed" << std::endl;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//Creo la ventana						
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pacman", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo Crear Ventana" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetKeyCallback(window, keyCallback);
	openGlInit();
	
	dibujaCubo();
	dibujaCuadrado();
	dibujaEsfera();
	asignarListas();

	srand(time(NULL));
	colocaFrutas();

	shaderProgram = setShaders("shaderPres.vert", "shaderPres.frag");
	texturas();
	glUseProgram(shaderProgram); 

	vistaNormal();
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);


		actualizarVista();
		//tiempoAgua();
		switch (escena)
		{
		case 0: //Durante la partida
			movimiento();
			dibujaEscena();
			comprobarPartida();
			break;
		case 1:  //Si pierdes
			vista = 0;
			dibujaPantalla(perder);
			break;
		case 2:  //Si ganas
			vista = 0;
			dibujaPantalla(ganar);
			break;
		case 3:  //Para empezar
			vista = 0;
			dibujaPantalla(inicio);
			break;
		default:
			break;
		}	
		
		glBindVertexArray(0); 
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAOCubo);
	glDeleteVertexArrays(1, &VAOCuadrado);
	glDeleteVertexArrays(1, &VAOEsfera);
	sonido->drop();

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}
