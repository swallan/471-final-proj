/*
CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
*/

#include <iostream>


#include <glad/glad.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

#include "WindowManager.h"
#include "Shape.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;
shared_ptr<Shape> shape, flr, wall;


double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime =glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}
class camera
{
public:
	glm::vec3 pos, rot;
	int w, a, s, d, yee, haw;
    bool godRays = true;
	camera()
	{
		w = a = s = d = 0;
		pos = rot = glm::vec3(0, 0, 0);
	}
	glm::mat4 process(double ftime)
	{
		float speed = 0;
		if (w == 1)
		{
			speed = 7*ftime;
		}
		else if (s == 1)
		{
			speed = -7*ftime;
		}
		float yangle=0;
		if (a == 1)
			yangle = -3*ftime;
		else if(d==1)
			yangle = 3*ftime;
        float zangle=0;
        if (yee == 1)
            zangle = -3*ftime;
        else if(haw==1)
            zangle = 3*ftime;
        
        
		rot.y += yangle;
        
        rot.z += zangle;
		glm::mat4 R = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
        glm::mat4 Rz = glm::rotate(glm::mat4(1), rot.z, glm::vec3(1, 0, 0));
		glm::vec4 dir = glm::vec4(0, 0, speed,1);
        
        
		dir = dir* Rz *R;
        
		pos += glm::vec3(dir.x, dir.y, dir.z);
		glm::mat4 T = glm::translate(glm::mat4(1), pos);
        
		return  Rz * R * T  ;
	}
};

vec3 sunLOC = vec3(200.0f, 35.0f, 0.0f);
vec3 sunSCALE = vec3(10.0f, 10.0f, 10.0f);
camera mycam;
bool compare_fct(vec3& i, vec3& j)
{
vec3 diff_i = i - (-mycam.pos);
vec3 diff_j = j - (-mycam.pos);
return (glm::length(diff_i) > glm::length(diff_j)); // > or < not sure
}
class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog, psky, heightshader, house_floor, house_wall, lightBulb, prog_framebuffer, cloud;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID, MeshPosID, MeshTexID;

	// Data necessary to give our box to OpenGL
	GLuint VertexBufferID, VertexNormDBox, VertexTexBox, IndexBufferIDBox;
    
    
    // for cloud
    GLuint VertexArrayIDcloud, MeshPosIDcloud, MeshTexIDcloud, IndexBufferIDBoxCloud;
    
    // frame buffer
    GLuint FBOtexColor, FrameBufferObj, depth_rb, VertexArrayIDRect, VertexBufferIDRect, VertexBufferTexRect;
    GLuint FBOtexBW, FrameBufferObjBW, depth_rbBW, VertexArrayIDRectBW, VertexBufferIDRectBW, VertexBufferTexRectBW;
    
    GLuint FBObloom;


	//texture data
	GLuint Texture, TextureN, HeightTex;
	GLuint Texture2, floorTexture, wallTexture, cloudTex;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			mycam.w = 1;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			mycam.w = 0;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			mycam.s = 1;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			mycam.s = 0;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			mycam.a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			mycam.a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			mycam.d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			mycam.d = 0;
		}
        
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            mycam.yee = 1;
        }
        if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
        {
            mycam.yee = 0;
        }
        
        if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
            mycam.haw = 1;
        }
        if (key == GLFW_KEY_R && action == GLFW_RELEASE)
        {
            mycam.haw = 0;
        }
        
        if (key == GLFW_KEY_G && action == GLFW_PRESS)
        {
            if (mycam.godRays == 1.0){
                mycam.godRays = 0;
            }else{
                mycam.godRays = 1.0;
            }
        }
        
        
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;
		float newPt[2];
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "Pos X " << posX <<  " Pos Y " << posY << std::endl;

			//change this to be the points converted to WORLD
			//THIS IS BROKEN< YOU GET TO FIX IT - yay!
			newPt[0] = 0;
			newPt[1] = 0;

			std::cout << "converted:" << newPt[0] << " " << newPt[1] << std::endl;
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
			//update the vertex array with the updated points
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*6, sizeof(float)*2, newPt);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}
    
    #define MESHSIZE 100
    void init_mesh(){
        // generate VAO
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        // generate vertex buffer to give to OGL
        glGenBuffers(1, &MeshPosID);
        glBindBuffer(GL_ARRAY_BUFFER, MeshPosID);
        
        vec3 vertices[MESHSIZE * MESHSIZE * 4];
        
        for (int x = 0; x < MESHSIZE; x++){
            for (int z = 0; z < MESHSIZE; z++){
                vertices[x * 4 + z*MESHSIZE * 4 + 0] = vec3(0.0, 0.0, 0.0) + vec3(x, 0.0, z);
                vertices[x * 4 + z*MESHSIZE * 4 + 1] = vec3(1.0, 0.0, 0.0) + vec3(x, 0.0, z);
                vertices[x * 4 + z*MESHSIZE * 4 + 2] = vec3(1.0, 0.0, 1.0) + vec3(x, 0.0, z);
                vertices[x * 4 + z*MESHSIZE * 4 + 3] = vec3(0.0, 0.0, 1.0) + vec3(x, 0.0, z);
            }
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * MESHSIZE * MESHSIZE * 4, vertices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void * )0);
        
        // texture coordinates
        float t = 1.0 / 100;
                 
         vec2 tex[MESHSIZE*MESHSIZE*4];
         for(int x = 0; x < MESHSIZE; x++){
            for(int  z = 0;  z < MESHSIZE;  z++){
                tex[x * 4 +   z*MESHSIZE * 4 +   0] = vec2(0.0, 0.0) + vec2(x,  z) * t;
                  tex[x * 4 +  z*MESHSIZE * 4 + 1] = vec2(t,   0.0) + vec2(x,  z) * t;
                  tex[x * 4 +  z*MESHSIZE * 4 + 2] = vec2(t,     t) + vec2(x,  z) * t;
                  tex[x * 4 +  z*MESHSIZE * 4 + 3] = vec2(0.0,   t) + vec2(x,  z) * t;
            }
         }
        glGenBuffers(1, &MeshTexID);
        // set the current state to focus on our vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, MeshTexID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * MESHSIZE * MESHSIZE * 4, tex, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
         
         
        glGenBuffers(1, &IndexBufferIDBox);
        glBindBuffer(GL_ARRAY_BUFFER, IndexBufferIDBox);
        
        GLushort elements[MESHSIZE * MESHSIZE * 6];
        int ind = 0;
        for(int i = 0; i < MESHSIZE * MESHSIZE * 6; i+=6, ind+=4){
            elements[i + 0] = ind + 0;
            elements[i + 1] = ind + 1;
            elements[i + 2] = ind + 2;
            elements[i + 3] = ind + 0;
            elements[i + 4] = ind + 2;
            elements[i + 5] = ind + 3;
        }
         
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * MESHSIZE * MESHSIZE * 6, elements, GL_STATIC_DRAW);
        glBindVertexArray(0);
}

	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
        init_mesh();
        glGenVertexArrays(1, &VertexArrayIDRect);
        glBindVertexArray(VertexArrayIDRect);
        
        // generate vertex buffer and hand off ot OGL
        glGenBuffers(1, &VertexBufferIDRect);
        // set the current state to focus on our vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIDRect);
        
        GLfloat *ver = new GLfloat[18];
        // front
        int verc = 0;
        ver[verc++] = -1.0, ver[verc++] = -1.0, ver[verc++] = 0;
        ver[verc++] = 1.0, ver[verc++] = -1.0, ver[verc++] = 0;
        ver[verc++] = -1.0, ver[verc++] = 1.0, ver[verc++] = 0;
        ver[verc++] = 1.0, ver[verc++] = -1.0, ver[verc++] = 0;
        ver[verc++] = 1.0, ver[verc++] = 1.0, ver[verc++] = 0;
        ver[verc++] = -1.0, ver[verc++] = 1.0, ver[verc++] = 0;
        
        // actually memcopy the data - only do this once
        glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), ver, GL_STATIC_DRAW);
        // we need to set up the vertex array
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        // gererate vertex buffer to hand off to ogl
        glGenBuffers(1, &VertexBufferTexRect);
        // set the current state to focus on our vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferTexRect);
        
        float t = .1 / 100.;
        GLfloat *cube_tex = new GLfloat[ 12];
        int texc = 0;
        
        cube_tex[texc++] = 0, cube_tex[texc++] = 0;
        cube_tex[texc++] = 1, cube_tex[texc++] = 0;
        cube_tex[texc++] = 0, cube_tex[texc++] = 1;
        cube_tex[texc++] = 1, cube_tex[texc++] = 0;
        cube_tex[texc++] = 1, cube_tex[texc++] = 1;
        cube_tex[texc++] = 0, cube_tex[texc++] = 1;
        
        // actually memocyr the data only do this once
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), cube_tex, GL_STATIC_DRAW);
        // we need to set up the vertex array
        glEnableVertexAttribArray(1);
        // key function to get up how many elements to pull out at a time
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        
		string resourceDirectory = "../../resources" ;
        string imageDirectory = resourceDirectory + "/images";
        string objectDirectory = resourceDirectory + "/objects";

		// Initialize mesh.
		shape = make_shared<Shape>();
		//shape->loadMesh(resourceDirectory + "/t800.obj");
		shape->loadMesh(objectDirectory + "/sphere.obj");
		shape->resize();
		shape->init();
        
        // Initialize mesh.
        flr = make_shared<Shape>();
        flr->loadMesh(objectDirectory + "/wall.obj");
        flr->resize();
        flr->init();
        
        // Initialize mesh.
        wall = make_shared<Shape>();
        wall->loadMesh(objectDirectory + "/wall.obj");
        wall->resize();
        wall->init();

		int width, height, channels;
		char filepath[1000];

		//texture 1
        string str = imageDirectory + "/grass.jpg";
        strcpy(filepath, str.c_str());
        unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
        glGenTextures(1, &Texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        //texture height
        str = imageDirectory + "/grass.jpg";
        strcpy(filepath, str.c_str());
        data = stbi_load(filepath, &width, &height, &channels, 4);
        glGenTextures(1, &HeightTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, HeightTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);



		//texture 2
		str = imageDirectory + "/sky.jpg";
		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &Texture2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

        
        //texture floor
        str = imageDirectory + "/floor.jpg";
        strcpy(filepath, str.c_str());
        data = stbi_load(filepath, &width, &height, &channels, 4);
        glGenTextures(1, &floorTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        //texture wall
        str = imageDirectory + "/wall_texture.jpg";
        strcpy(filepath, str.c_str());
        data = stbi_load(filepath, &width, &height, &channels, 4);
        glGenTextures(1, &wallTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        
		//[TWOTEXTURES]
		//set the 2 textures to the correct samplers in the fragment shader:
		GLuint Tex1Location = glGetUniformLocation(prog->pid, "tex");//tex, tex2... sampler in the fragment shader
		GLuint Tex2Location = glGetUniformLocation(prog->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(prog->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);

		Tex1Location = glGetUniformLocation(psky->pid, "tex");//tex, tex2... sampler in the fragment shader
		Tex2Location = glGetUniformLocation(psky->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(psky->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);

        Tex1Location = glGetUniformLocation(heightshader->pid, "tex");//tex, tex2... sampler in the fragment shader
        Tex2Location = glGetUniformLocation(heightshader->pid, "tex2");
        // Then bind the uniform samplers to texture units:
        glUseProgram(heightshader->pid);
        glUniform1i(Tex1Location, 0);
        glUniform1i(Tex2Location, 1);
//
//        Tex1Location = glGetUniformLocation(heightshader->pid, "tex");//tex, tex2... sampler in the fragment shader
//        Tex2Location = glGetUniformLocation(heightshader->pid, "tex2");
//        // Then bind the uniform samplers to texture units:
//        glUseProgram(cloud->pid);
//        glUniform1i(Tex1Location, 0);
//        glUniform1i(Tex2Location, 1);
        
//
        Tex1Location = glGetUniformLocation(house_floor->pid, "tex");
        glUseProgram(house_floor->pid);
        glUniform1i(Tex1Location, 0);
        
        Tex1Location = glGetUniformLocation(house_wall->pid, "tex");
        glUseProgram(house_wall->pid);
        glUniform1i(Tex1Location, 0);
//
        
        
        glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
        glGenTextures(1, &FBOtexColor);
        glBindTexture(GL_TEXTURE_2D, FBOtexColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glGenTextures(1, &FBOtexBW);
        glBindTexture(GL_TEXTURE_2D, FBOtexBW);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glGenTextures(1, &FBObloom);
        glBindTexture(GL_TEXTURE_2D, FBObloom);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        

        
        glGenFramebuffers(1, &FrameBufferObj);
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObj);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtexColor, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FBOtexBW, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, FBObloom, 0);
        
        glGenRenderbuffers(1, &depth_rb);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);
        
        GLenum status;
        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch (status) {
            case GL_FRAMEBUFFER_COMPLETE:
                cout <<"status framebuffer BW good\n";
                break;
                
            default:
                cout << "status framebuffer color bad!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        }
        
        
        Tex1Location = glGetUniformLocation(prog_framebuffer->pid, "colorTexure");
        Tex2Location = glGetUniformLocation(prog_framebuffer->pid, "BWTexure");
        GLuint Tex3Location = glGetUniformLocation(prog_framebuffer->pid, "bloom");
        glUseProgram(prog_framebuffer->pid);
        glUniform1i(Tex1Location, 0);
        glUniform1i(Tex2Location, 1);
        glUniform1i(Tex3Location, 2);
//

	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();
        string shaderDirectory = resourceDirectory + "/shaders";
		// Set background color.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_DEPTH_TEST);
		// Initialize the GLSL program.
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(shaderDirectory + "/shader_vertex.glsl", resourceDirectory + "/shaders/shader_fragment.glsl");
		if (!prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("campos");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");
        
        
        
        
        heightshader = std::make_shared<Program>();
        heightshader->setVerbose(true);
        heightshader->setShaderNames(shaderDirectory + "/height_vertex.glsl", resourceDirectory + "/shaders/height_frag.glsl");
        if (!heightshader->init())
        {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1);
        }
        heightshader->addUniform("P");
        heightshader->addUniform("V");
        heightshader->addUniform("M");
        heightshader->addUniform("campos");
        heightshader->addUniform("camoff");
        heightshader->addAttribute("vertPos");
        heightshader->addAttribute("vertTex");
        heightshader->addAttribute("vertNor");
        heightshader->addUniform("isBW");
        
//        cloud = std::make_shared<Program>();
//        cloud->setVerbose(true);
//        cloud->setShaderNames(shaderDirectory + "/cloud_frag.glsl", resourceDirectory + "/shaders/cloud_frag.glsl");
//        if (!cloud->init())
//        {
//            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
//            exit(1);
//        }
//        cloud->addUniform("P");
//        cloud->addUniform("V");
//        cloud->addUniform("M");
//        cloud->addUniform("campos");
//        cloud->addUniform("camoff");
//        cloud->addAttribute("vertPos");
//        cloud->addAttribute("vertTex");
//        cloud->addAttribute("vertNor");
//        cloud->addUniform("isBW");


		psky = std::make_shared<Program>();
		psky->setVerbose(true);
		psky->setShaderNames(shaderDirectory + "/skyvertex.glsl", shaderDirectory + "/skyfrag.glsl");
		if (!psky->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		psky->addUniform("dn");
		psky->addUniform("P");
		psky->addUniform("V");
		psky->addUniform("M");
		psky->addUniform("campos");
		psky->addAttribute("vertPos");
		psky->addAttribute("vertNor");
		psky->addAttribute("vertTex");
        
        
        house_floor = std::make_shared<Program>();
        house_floor->setVerbose(true);
        house_floor->setShaderNames(shaderDirectory + "/floor_vert.glsl", shaderDirectory + "/floor_frag.glsl");
        if (!house_floor->init())
        {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1);
        }
        house_floor->addUniform("P");
        house_floor->addUniform("V");
        house_floor->addUniform("M");
//
        house_floor->addUniform("x");
        house_floor->addUniform("y");
        house_floor->addUniform("z");
        
        house_floor->addUniform("campos");
        house_floor->addAttribute("vertPos");
        house_floor->addAttribute("vertNor");
        house_floor->addAttribute("vertTex");
        house_floor->addUniform("isBW");
        
        
        house_wall = std::make_shared<Program>();
        house_wall->setVerbose(true);
        house_wall->setShaderNames(shaderDirectory + "/wall_vert.glsl", shaderDirectory + "/wall_frag.glsl");
        if (!house_wall->init())
        {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1);
        }
        house_wall->addUniform("P");
        house_wall->addUniform("V");
        house_wall->addUniform("M");
        house_wall->addUniform("x");
        house_wall->addUniform("y");
        house_wall->addUniform("z");
        house_wall->addUniform("campos");
        house_wall->addAttribute("vertPos");
        house_wall->addAttribute("vertNor");
        house_wall->addAttribute("vertTex");
        house_wall->addUniform("isBW");
        
        lightBulb = std::make_shared<Program>();
        lightBulb->setVerbose(true);
        lightBulb->setShaderNames(shaderDirectory + "/lightBulb_vert.glsl", shaderDirectory + "/lightBulb_frag.glsl");
        if (!lightBulb->init())
        {
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1);
        }
        lightBulb->addUniform("P");
        lightBulb->addUniform("V");
        lightBulb->addUniform("M");
        lightBulb->addUniform("isSUN");
        lightBulb->addUniform("isBW");
        lightBulb->addUniform("campos");
        lightBulb->addAttribute("vertPos");
        lightBulb->addAttribute("vertNor");
        lightBulb->addAttribute("vertTex");
        
        prog_framebuffer = make_shared<Program>();
        prog_framebuffer-> setVerbose(true);
        prog_framebuffer->setShaderNames(shaderDirectory + "/vertFB.glsl", shaderDirectory + "/fragFB.glsl");
        if (!prog_framebuffer->init()){
            std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
            exit(1);
        }
        
//        prog_framebuffer->init();
        prog_framebuffer->addAttribute("vertPos");
        prog_framebuffer->addAttribute("vertTex");
//        prog_framebuffer->addUniform("lightPositionOnScreen1");
//        prog_framebuffer->addUniform("lightPositionOnScreen2");
        prog_framebuffer->addUniform("P");
        prog_framebuffer->addUniform("V");
        prog_framebuffer->addUniform("M");
        prog_framebuffer->addUniform("P1");
        prog_framebuffer->addUniform("V1");
        prog_framebuffer->addUniform("M1");
        prog_framebuffer->addUniform("raysOn");
        
	}


	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
    
    glm::vec3 Vec3s[100];
    void init_Vec3s(){
        int num = 100;
        int ind = 0;
//        srand(10);
        for (int x = 0; x < sqrt(num); x ++){
            for (int y = 0; y < sqrt(num); y ++){
                float r1 = ((float)(rand() % 30));
                float r2 = ((float)(rand() % 30)) ;
                float r3 = ((float)(rand() % 30)) ;
                cout << r1 << " " << r2 << endl;
                Vec3s[ind] = glm::vec3(r1, r3, r2);
                ind++;
            }
        }

    }
    
    
    void render(){
        int width, height;
        glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
        float aspect = width / (float)height;
        
        float frametime = (float)get_last_elapsed_time();
        glm::mat4 V, M, P; //View, Model and Perspective matrix
        V = mycam.process((float)frametime);
        P = glm::perspective((float)(3.14159f / 4.f), (float)((float)width/ (float)height), 0.1f, 1000.0f);
        mat4 S = scale(mat4(1.0f), sunSCALE);
        mat4 T = translate(mat4(1.0f), sunLOC);
        M = T * S;
        
        prog_framebuffer->bind();
        glUniformMatrix4fv(prog_framebuffer->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(prog_framebuffer->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(prog_framebuffer->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        
        S = scale(mat4(1.0f), vec3(.1, .1, .1));
        T = translate(mat4(1.0f), vec3(0.0f, .80f, 0.0f));
        mat4 M1 = T * S;
        
        glUniformMatrix4fv(prog_framebuffer->getUniform("P1"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(prog_framebuffer->getUniform("V1"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(prog_framebuffer->getUniform("M1"), 1, GL_FALSE, &M1[0][0]);

//        float raysOn = 0.0f;
//        if (mycam.godRays == 1) raysOn = 1.0f;
        glUniform1f(prog_framebuffer->getUniform("raysOn"), mycam.godRays);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // full color texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FBOtexColor);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FBOtexBW);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, FBObloom);
        
        glBindVertexArray(VertexArrayIDRect);
        glDrawArrays(GL_TRIANGLES, 0,6);
  
        prog_framebuffer->unbind();
        
        
        
    }
    
    
	void render_to_framebuffer()
	{
        
        // render the color one
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObj);
        GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, buffers);
        
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		double frametime = get_last_elapsed_time();

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width/(float)height;
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create the matrix stacks - please leave these alone for now
		
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = mycam.process(frametime);
                
		M = glm::mat4(1);
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones
		
		float sangle = 3.1415926 / 2.;
		glm::mat4 RotateXSky = glm::rotate(glm::mat4(1.0f), sangle, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 camp = -mycam.pos;
		glm::mat4 TransSky = glm::translate(glm::mat4(1.0f), camp);
		glm::mat4 SSky = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));

		M = TransSky * RotateXSky * SSky;


		psky->bind();
		glUniformMatrix4fv(psky->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(psky->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(psky->getUniform("M"), 1, GL_FALSE, &M[0][0]);
//        glUniformMatrix4fv(psky->getUniform("P1"), 1, GL_FALSE, &P[0][0]);
//        glUniformMatrix4fv(psky->getUniform("V1"), 1, GL_FALSE, &V[0][0]);
//        glUniformMatrix4fv(psky->getUniform("M1"), 1, GL_FALSE, &M[0][0]);
		glUniform3fv(psky->getUniform("campos"), 1, &mycam.pos[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureN);
		static float ttime = 0;
		ttime += frametime;
		float dn = sin(ttime)*0.5 +0.5;
		glUniform1f(psky->getUniform("dn"), dn);
		glDisable(GL_DEPTH_TEST);
		shape->draw(psky, false);
        glEnable(GL_DEPTH_TEST);
		psky->unbind();
        
        int num = 100;
        

//        glDisable(GL_DEPTH_TEST);
        sort(begin(Vec3s),end(Vec3s), compare_fct);
        glm::mat4 RotateX;
        glm::mat4 TransZ;
        glm::mat4 S;

        
        heightshader->bind();


        glm::mat4 transBill;
//        for (int i = 0; i < num; i++){
        glm::vec3 v3 = vec3(0.0, 0.0, 0.0);
        glm::mat4 iVR = V;

        iVR[3][0] = 0;
        iVR[3][1] = 0;
        iVR[3][2] = 0;

        iVR = glm::inverse(iVR);
//        cout << v3[0] << endl;
//        transBill = glm::translate(glm::mat4(1.0f), v3);


        glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -.580f, -50.0f));
//        glm::mat4 R = glm::rotate(glm::mat4(1.0f),(float)( 3.14/2.0), glm::vec3(0.0f, 0.0f, 1.0f));
//        S = glm::scale(glm::mat4(1.0f), glm::vec3(.2, .2,.2));
        M =  T  ;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        heightshader->bind();
        glUniformMatrix4fv(heightshader->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(heightshader->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(heightshader->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glUniform3fv(heightshader->getUniform("campos"), 1, &mycam.pos[0]);
        
        vec3 offset = mycam.pos;
        offset.y = 0;
        offset.x = (int)offset.x;
        offset.z = (int)offset.z;
        glUniform3fv(heightshader->getUniform("camoff"), 1, &offset[0]);
        glBindVertexArray(VertexArrayID);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, HeightTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Texture);
        
//        glPatchParameteri(GL_PATCH_VERTICES, 3);

        glDrawElements(GL_TRIANGLES, MESHSIZE * MESHSIZE * 6 , GL_UNSIGNED_SHORT, (void*)0);
        heightshader->unbind();
        
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        house_floor->bind();
        T = translate(mat4(1.0f), vec3(0.0f, -.55f, 0.0f));
        M = T;
        glUniformMatrix4fv(house_floor->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(house_floor->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(house_floor->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glUniform3fv(house_floor->getUniform("campos"), 1, &mycam.pos[0]);
        
        glUniform1f(house_floor->getUniform("x"), 1.0);
        glUniform1f(house_floor->getUniform("y"), 2.0);
        glUniform1f(house_floor->getUniform("z"),5.0);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        flr->draw(house_floor, false);
        house_floor->unbind();
        
        // back wall
        house_wall->bind();
        mat4 R = rotate(mat4(1.0f), 3.14f / 2.0f, vec3(0.0f, 0.0f, 1.0f));
        T = glm::translate(mat4(1.0f), vec3(.55f, 0.0f, 0.0f));
        M = T * R;
        glUniformMatrix4fv(house_wall->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(house_wall->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(house_wall->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glUniform3fv(house_wall->getUniform("campos"), 1, &mycam.pos[0]);
 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        wall->draw(house_wall, false);
       
        
        // side wall 1
     
        S = scale(mat4(1.0f), vec3(1.0f, .55f, 1.0f));
        R = rotate(mat4(1.0f), 3.14f / 2.0f, vec3(1.0f, 0.0f, 0.0f));
        T = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 1.0f));
        M = S * T * R;
        glUniformMatrix4fv(house_wall->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(house_wall->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(house_wall->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glUniform3fv(house_wall->getUniform("campos"), 1, &mycam.pos[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        wall->draw(house_wall, false);
        
        // side wall 2
        
        S = scale(mat4(1.0f), vec3(1.0f, .55f, 1.0f));
        R = rotate(mat4(1.0f), 3.14f / 2.0f, vec3(1.0f, 0.0f, 0.0f));
        T = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f));
        M = S * T * R;
        glUniformMatrix4fv(house_wall->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(house_wall->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(house_wall->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glUniform3fv(house_wall->getUniform("campos"), 1, &mycam.pos[0]);
        GLSL::checkError("GET_FILE_LINE");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        wall->draw(house_wall, false);
        house_wall->unbind();
        
        // lightbulb
        lightBulb->bind();
        S = scale(mat4(1.0f), sunSCALE);
        T = translate(mat4(1.0f), sunLOC);
        M = T * S;
        glUniformMatrix4fv(lightBulb->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(lightBulb->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(lightBulb->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glUniform1f(lightBulb->getUniform("isSUN"),1.0f);
        glUniform3fv(lightBulb->getUniform("campos"), 1, &mycam.pos[0]);
        glUniform1f(lightBulb->getUniform("isBW"),0.0f);
        shape->draw(lightBulb, false);

        S = scale(mat4(1.0f), vec3(.1, .1, .1));
        T = translate(mat4(1.0f), vec3(0.0f, .50f, 0.0f));
        M = T * S;
        glUniformMatrix4fv(lightBulb->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(lightBulb->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(lightBulb->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glUniform1f(lightBulb->getUniform("isSUN"),0.0f);
        glUniform3fv(lightBulb->getUniform("campos"), 1, &mycam.pos[0]);
        glUniform1f(lightBulb->getUniform("isBW"),0.0f);
        shape->draw(lightBulb, false);
        
        lightBulb->unbind();
        
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};
//******************************************************************************************
int main(int argc, char **argv)
{
    
	std::string resourceDir = "../../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
    
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;
    application->init_Vec3s();

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
        application->render_to_framebuffer();
		
        
        application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
