#include <iostream>
#include <time.h>
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Other includes
#include "Shader.h"
#include "World.h"
#include <SOIL/SOIL.h>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos); 
void sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}
//game world global 
World gameWorld;

void move_object();
void do_movement();
int score;
bool keys[1024];
bool ffkeys[2014];//testing flip flop mechanism
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
GLfloat lastX = 400, lastY = 300;
GLfloat fallDeltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFallFrame = 0.0f;  	// Time of last frame
GLfloat fallRate = 0.5f; // how fast objects fall, inverse unit
int gameTicks = 0;
//input 
GLfloat inputDeltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastInputTime = 0.0f;  	// Time of last frame
GLfloat inputRate = 0.1f; // how fast objects fall, inverse unit
bool inputFlag = true;
GLfloat yaw    = -90.0f;
GLfloat pitch  =  0.0f;
bool firstMouse= true;
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
glm::vec3 cameraPos   = glm::vec3(0.0f, 5.0f,  20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
// The MAIN function, from here we start the application and run the game loop
int main()
{
	//Setup game world
	gameWorld = World();

	glm::vec3 color;
	color.x = 0.0f;
	color.y = 0.0f;
	color.z = 0.0f;
	score = 0;



	glm::mat4 model;
	model = glm::rotate(model, -85.0f, glm::vec3(1.0f, 0.0f, 0.0f)); 

	glm::mat4 view;
	// Note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f)); 

	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3DTetris", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);


	// Build and compile our shader program
	Shader ourShader("shader.vs", "shader.frag");
	Shader texShader("shader.vs", "texShader.frag");
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.0f, 0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		0.0f,0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		0.0f,-0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		0.0f,-0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};

	GLfloat board[] = {
		// Positions            // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
		2.0f,  -0.5f,  4.5f,  0.0f, 1.0f,
		-5.0f, -0.5f,  4.5f,  0.0f, 0.0f,
		-5.0f, -0.5f, -0.5f,  1.0f, 0.0f,
		2.0f,  -0.5f,  4.5f,  0.0f, 1.0f,
		-5.0f, -0.5f, -0.5f,  1.0f, 0.0f,
		2.0f,  -0.5f, -0.5f,  1.0f, 1.0f								
	};
	GLfloat cubevertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	GLuint VBO, VAO, EBO, VAOBoard, VBOBoard;


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubevertices), cubevertices, GL_DYNAMIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Tex Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);  
	glBindVertexArray(0); // Unbind VAO

	glGenVertexArrays(1, &VAOBoard);
	glGenBuffers(1, &VBOBoard);
	glBindVertexArray(VAOBoard);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBoard);
	glBufferData(GL_ARRAY_BUFFER, sizeof(board), &board, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);


	int width, height;
	unsigned char* image = SOIL_load_image("steelbox.jpg", &width, &height, 0, SOIL_LOAD_RGB); 
	GLuint texture;
	glGenTextures(1, &texture);  

	glBindTexture(GL_TEXTURE_2D, texture);  
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Set texture filtering parametersD
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//texture 2
	unsigned char* image2 = SOIL_load_image("red.jpg", &width, &height, 0, SOIL_LOAD_RGB); 
	GLuint redTex;
	glGenTextures(1, &redTex);  
	glBindTexture(GL_TEXTURE_2D, redTex);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_MIRRORED_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_MIRRORED_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);


	//blue texture
	unsigned char* image4 = SOIL_load_image("blue.jpg", &width, &height, 0, SOIL_LOAD_RGB); 
	GLuint blueTex;
	glGenTextures(1, &blueTex);  
	glBindTexture(GL_TEXTURE_2D, blueTex);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_MIRRORED_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_MIRRORED_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image4);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image4);
	glBindTexture(GL_TEXTURE_2D, 0);

	//green texture
	unsigned char* image5 = SOIL_load_image("green.jpg", &width, &height, 0, SOIL_LOAD_RGB); 
	GLuint greenTex;
	glGenTextures(1, &greenTex);  
	glBindTexture(GL_TEXTURE_2D, greenTex);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_MIRRORED_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_MIRRORED_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image5);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image5);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint boardTex;
	glGenTextures(1, &boardTex);  
	unsigned char* image3 = SOIL_load_image("ground.jpg", &width, &height, 0, SOIL_LOAD_RGB); 
	glBindTexture(GL_TEXTURE_2D, boardTex);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image3);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image3);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Game loop
	glEnable(GL_DEPTH_TEST);  //Z-buffer
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  //control mouse cursor

	//set fall rate

	GLfloat initFallTime = glfwGetTime();
	lastFallFrame = initFallTime;
	lastInputTime = initFallTime;




	int xT = 0;
	int yT = 0;
	int zT = 0;

	view = glm::lookAt(cameraPos, 
		cameraPos + cameraFront, 
		cameraUp);
	bool action = false;
	while (!glfwWindowShouldClose(window))
	{
		int shapeNum = (int)(rand() %7);
		int randX = (int)(rand() % 1);
		int randY = 15;
		int randZ = (int)(rand() % 1);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();  

		glfwSetCursorPosCallback(window, mouse_callback);  
		// Render
		// Clear the colorbuffer
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ourShader.Use();
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;  




		fallDeltaTime = currentFrame - lastFallFrame;
		inputDeltaTime = glfwGetTime() - lastInputTime;
		if(gameWorld.checkGameOver()){
			std::cout << "game over. Pless Any key to restart " << std::endl;
			cin.get();
			gameWorld = World();
		}
		else{
			if(inputDeltaTime > inputRate){
				lastInputTime = glfwGetTime();
				move_object();
			}
			if(fallDeltaTime > fallRate){
				gameWorld.moveFallObjNatural();
				lastFallFrame = glfwGetTime();
				gameTicks++;
				//int levels = gameWorld.clearLevel();
				//score +=levels;
				std::cout << "current score: " << std::endl;
				std::cout << score << std::endl;
			}
			if(gameTicks > 7 && !action){
				gameTicks = 0;
				gameWorld.setFallObj(2, 12, 3, shapeNum);
				/*
				zT++;
				if(zT == 5){
				zT = 0;
				xT++;
				}
				if(xT == 7){
				xT = 0;
				yT++;
				}*/

			}

		}

			// Draw the triangle
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, greenTex);
			glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

			ourShader.Use();
			GLfloat radius = 20.0f;
			GLfloat camX = sin(2*glfwGetTime()) * radius;
			GLfloat camZ = cos(2*glfwGetTime()) * radius;
			glm::mat4 view;
			view = glm::lookAt(cameraPos, 
				cameraPos + cameraFront, 
				cameraUp);
			//model = glm::rotate(model, 0.005, glm::vec3(0.5f, 1.0f, 0.0f));  
			GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(VAO);

			//get game world boundaries 
			int xLen = gameWorld.xLength;
			int yLen = 10;
			int zLen = gameWorld.zLength;
			for(int x = 0; x < xLen; x++){
				for(int y = 0; y < yLen; y++){
					for(int z = 0; z < zLen; z++){
						if(gameWorld.occupied[x][y][z] == 1){
							glm::mat4 model;
							glm::vec3 loc;
							//int color = gameWorld.blockColors[x][y][z];
							//set the current location of the blocks
							loc.x = x;
							loc.y = y;
							loc.z = z;
							model = glm::translate(model, loc);
							//get the color of the block at that location
							ourShader.Use();
							GLfloat scale = 1.00;
							glBindVertexArray(VAO);
							model = glm::scale(model, glm::vec3(scale, scale, scale));
							//draw the original boxes
							GLint vertexColorLocation = glGetUniformLocation(ourShader.Program, "ourColor");
							glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f); //load the uniform color
							glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); //assign world coordinate to cube
							glBindVertexArray(VAO);
							glDrawArrays(GL_TRIANGLES, 0, 36);
							glBindVertexArray(0);
						}
					}
				}
			}
		

		/*
		for(int x = 0; x < xLen; x++){
		for(int y = 0; y < yLen; y++){
		for(int z = 0; z < zLen; z++){


		if(gameWorld.occupied[x][y][z] == 1){
		}
		}
		}
		}

		for(GLuint i = 0; i < 10; i++)
		{
		glm::mat4 model;

		glm::vec3 loc;
		loc.x = 0.0f;
		loc.y = 0.0f;
		loc.z = 0.0f;
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i; 
		//model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		*/

		texShader.Use();
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boardTex);
		glUniform1i(glGetUniformLocation(texShader.Program, "ourTexture1"), 0);
		//setup the darn board
		modelLoc = glGetUniformLocation(texShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		viewLoc = glGetUniformLocation(texShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		projLoc = glGetUniformLocation(texShader.Program, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAOBoard);
		model = glm::mat4();
		glm::vec3 trans;
		trans.x = 4.5f;
		trans.y = 0.0f;
		trans.z = 0.0f;
		model = glm::translate(model, trans);
		glUniformMatrix4fv(glGetUniformLocation(texShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);				

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(action == GLFW_PRESS)
		keys[key] = true;
	else if(action == GLFW_RELEASE)
		keys[key] = false;  

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(firstMouse) // this bool variable is initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw   +=  xoffset;
	pitch += -yoffset;  
	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}
void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 20.0f * deltaTime;
	if(keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if(keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if(keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if(keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
void move_object()
{
	// Camera controls
	if(keys[GLFW_KEY_UP]){ //object rotations
		gameWorld.moveBlock(0, 0, -1);
	}
	else if(keys[GLFW_KEY_DOWN]){
		gameWorld.moveBlock(0, 0, 1);
	}
	else if(keys[GLFW_KEY_LEFT]){
		gameWorld.moveBlock(-1, 0, 0);
	}
	else if(keys[GLFW_KEY_RIGHT]){
		gameWorld.moveBlock(1, 0, 0);
	}
	else if(keys[GLFW_KEY_SPACE]){
		gameWorld.moveBlock(0, -1, 0);
	}
	else if(keys[GLFW_KEY_J]){
		gameWorld.rotateBlock(0, 0, 1);
	}
	else if(keys[GLFW_KEY_K]){
		gameWorld.rotateBlock(0, 1, 0);
	}
	else if(keys[GLFW_KEY_L]){
		gameWorld.rotateBlock(1, 0, 0);
	}
	else if(keys[GLFW_KEY_C]){
		gameWorld = World();
	}
	else if(keys[GLFW_KEY_R]){
		//gameWorld.replace();
	}

}
