#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>          // std::queue
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shape
{
public:
	int color;
	int shapeType;
	int length;
	bool grounded;
	glm:: vec3 center;
	glm::vec3 blocks[20]; //falling object's group of blocks
    // Constructor generates the shader on the fly
    Shape(int x, int y, int z, int s)//s is the shapetype
    {
		color = 0;
		if( s == 0){
			glm::vec3 block = glm::vec3(x, y, z);
			blocks[0] = block;
			length = 1;
		}
		if( s == 1) {
			glm::vec3 block = glm::vec3(x+1, y, z);
			glm::vec3 block2 = glm::vec3(x, y, z);
			glm::vec3 block3 = glm::vec3(x-1, y, z);
			glm::vec3 block4 = glm::vec3(x-2, y, z);
			blocks[0] = block;
			blocks[1] = block2;
			blocks[2] = block3;
			blocks[3] = block4;
			center = block2;
			length = 4;
		}
		if( s == 2) { //T shape
			glm::vec3 block = glm::vec3(x+1, y, z);
			glm::vec3 block2 = glm::vec3(x-1, y, z);
			glm::vec3 block3 = glm::vec3(x, y, z);
			glm::vec3 block4 = glm::vec3(x, y, z+1);
			blocks[0] = block;
			blocks[1] = block2;
			blocks[2] = block3;
			blocks[3] = block4;
			center = block3;
			length = 4;
		}
		if( s == 3) { //O shape
			glm::vec3 block = glm::vec3(x+1, y, z);
			glm::vec3 block2 = glm::vec3(x, y, z);
			glm::vec3 block3 = glm::vec3(x+1, y, z+1);
			glm::vec3 block4 = glm::vec3(x, y, z+1);
			blocks[0] = block;
			blocks[1] = block2;
			blocks[2] = block3;
			blocks[3] = block4;
			center = block2;
			length = 4;
		}
		if( s == 4) { //Z shape
			glm::vec3 block = glm::vec3(x-1, y, z);
			glm::vec3 block2 = glm::vec3(x, y, z);
			glm::vec3 block3 = glm::vec3(x, y, z+1);
			glm::vec3 block4 = glm::vec3(x+1, y, z+1);
			blocks[0] = block;
			blocks[1] = block2;
			blocks[2] = block3;
			blocks[3] = block4;
			center = block3;
			length = 4;
		}
		if( s == 5) { //Z shape
			glm::vec3 block = glm::vec3(x+1, y, z);
			glm::vec3 block2 = glm::vec3(x, y, z);
			glm::vec3 block3 = glm::vec3(x, y, z+1);
			glm::vec3 block4 = glm::vec3(x-1, y, z-1);
			blocks[0] = block;
			blocks[1] = block2;
			blocks[2] = block3;
			blocks[3] = block4;
			center = block3;
			length = 4;
		}
		if( s == 6) { //J shape
			glm::vec3 block = glm::vec3(x+1, y, z);
			glm::vec3 block2 = glm::vec3(x, y, z);
			glm::vec3 block3 = glm::vec3(x, y, z-1);
			glm::vec3 block4 = glm::vec3(x, y, z-2);
			blocks[0] = block;
			blocks[1] = block2;
			blocks[2] = block3;
			blocks[3] = block4;
			center = block3;
			length = 4;
		}
		if( s == 7) { //L shape
			glm::vec3 block = glm::vec3(x-1, y, z);
			glm::vec3 block2 = glm::vec3(x, y, z);
			glm::vec3 block3 = glm::vec3(x, y, z-1);
			glm::vec3 block4 = glm::vec3(x, y, z-2);
			blocks[0] = block;
			blocks[1] = block2;
			blocks[2] = block3;
			blocks[3] = block4;
			center = block3;
			length = 4;
		}
			if( s == 8) { //Simple shape
			glm::vec3 block = glm::vec3(x-1, y, z);
			glm::vec3 block2 = glm::vec3(x, y, z);
			blocks[0] = block;
			blocks[1] = block2;
			center = block2;
			length = 2;
		}
	
	
	
	
		grounded = false;
    }
   
};

#endif