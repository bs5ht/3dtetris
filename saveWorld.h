#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>          // std::queue
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shape.h>
#include <vector>
class World
{
public:

	static const int xLength = 7;
	static const int yLength = 20;
	static const int zLength = 5;
	int occupied[xLength][yLength][zLength]; //tells whether or not that space location is occupied, is kept track at all times
	int blockColors[xLength][yLength][zLength]; //keeps track of colors at every block
	glm::vec3 controlPos[20]; //falling object's position
	int controlLen;
	int staticObj[xLength][yLength][zLength]; //keeps track of all static objects
	std::vector<Shape > fallObjQueue;
	int fallObjSize;
	// Constructor generates the shader on the fly
	World()
	{
		for(int x = 0; x < xLength; x++){
			for(int y = 0; y < yLength; y++){
				for(int z = 0; z < zLength; z++){
					occupied[x][y][z] = 0;
					staticObj[x][y][z] = 0;
					blockColors[x][y][z] = 0;
				}
			}
		}
		controlLen = 0;
		fallObjSize = 0;
	}
	// Uses the current shader
	void Use() 
	{ 

	}
	void setBlock(int x, int y, int z, int color) 
	{ 
		occupied[x][y][z] = 1;
		blockColors[x][y][z] = color;
	}
	void setFallObj(int x, int y, int z, int color) 
	{ 
		glm::vec3 temp = glm::vec3(x, y, z);
		Shape s = Shape(x, y, z, 1);
		std::vector<Shape>::iterator it;

		fallObjQueue.push_back(s);
		//controlPos[controlLen] = temp;
		//controlLen++;
		occupied[x][y][z] = 1;
		blockColors[x][y][z] = color;
	}

	void addFallObj(int x, int y, int z, int color){
		glm::vec3 temp = glm::vec3(x, y, z);
		occupied[x][y][z] = 1;
		blockColors[x][y][z] = color;
	}


	void freeFallBlock(int x, int y, int z){

		if(staticObj[x][y][z] == 0){
			occupied[x][y][z] = 0;
			blockColors[x][y][z] = 0;
		}
	}
	void assignFallBlock(int x, int y, int z, int color){

		//if the block is not static, then deallocate it
		if(staticObj[x][y][z] == 0){
			occupied[x][y][z] = 1;
			blockColors[x][y][z] = 0;
		}
	}



	void moveBlock(int dx, int dy, int dz){
		//glm::vec3 temp = fallObjPos[0];
		std::queue<int> touchGroundQueue;
		//	int tX = (int)temp.x;
		//int tY = (int)temp.y;
		//int tZ = (int)temp.z;
		//freeFallBlock(tX, tY, tZ);
		//assignFallBlock(tX, tY+1, tZ, 0);
		//std::cout << tX << std::endl;
		//std::cout << controlLen<< std::endl;
		//need to abstract out falling blocks AND the focused block

		if(fallObjQueue.size() > 0){ // make sure that there are falling objects you can take control of 

			Shape curObj = fallObjQueue[0];
			int curLen = curObj.length;
			glm::vec3* blocks = curObj.blocks;
			std::cout << "hi" <<std::endl;
			for(int i = 0; i < curLen; i++){
				glm::vec3 temp = blocks[i];

				int tX = (int)temp.x;
				int tY = (int)temp.y;
				int tZ = (int)temp.z;
				//new variables
				int nX = tX + dx;
				int nY = tY + dy;
				int nZ = tZ + dz;
				std::cout << "info" << std::endl;
				std::cout << tX << std::endl;
				std::cout << tY << std::endl;
				std::cout << tZ << std::endl;
			
			}
		}

	}
	//checks if the object has touched ground
	bool touchedGround(int x, int y, int z){

		//if the block is not static, then deallocate it
		if(y == 0){
			return true;
		}
		return false;
	}

	void setStaticBlock(int x, int y, int z){
		occupied[x][y][z] = 1;
		staticObj[x][y][z] = 1;
	}

	bool moveFallObjNatural()// moves all the objects down one unit naturally
	{ 
		bool grounded = false;
		bool objected = false;
		std::queue<int> touchGroundQueue;
		for(int k = 0; k < fallObjQueue.size(); k++ ){ //for each object that is falling 
			Shape curObj = fallObjQueue[k];
			int curLen = curObj.length;
			glm::vec3* blocks = curObj.blocks;
			std::cout << "hi" <<std::endl;
			for(int i = 0; i < curLen; i++){ //for each block, first check if there's going to be a grounding or on top of action 
				glm::vec3 temp = blocks[i];
				//deallocate occupied space
				int tX = (int)temp.x;
				int tY = (int)temp.y;
				int tZ = (int)temp.z;
				//		int color = blockColors[tX][tY][tZ];
				int color = 0;
				//freeFallBlock((int)temp.x, (int)temp.y, (int)temp.z);
				temp.y = temp.y -1;
				tY = (int)temp.y;
				std::cout << tY << std::endl;
				if(staticObj[tX][tY][tZ] == 1){// if the current block touched ground 
					std::cout << "static touch!" << std::endl;
					fallObjQueue[k].grounded = true; //set the entire shape as grounded
					objected = true;
					break;
				}	
				else if(touchedGround(tX, tY, tZ) == 1){ //either the objects touched the ground or on top of another object
					fallObjQueue[k].grounded = true; //set the entire shape as grounded
					grounded = true;
					std::cout << "opbject touch!" << std::endl;
					//loop through all of the other block positions and set them as static, make sure to remove this object from the fallObjQueue **** 
					break; //break out of the loop and stop updating
				}

				if(grounded){
					break;
				}
				/
				else{
				assignFallBlock((int)temp.x, (int)temp.y, (int)temp.z, color);
				}*/
				//if the block reaches the ground or touches the static blocks, then set the controlLen to zero
			}

			//if the objects have touched either the ground or on top of other objects
			if(grounded){
				for(int i = 0; i < curLen; i++){ //for each block, first check if there's going to be a grounding or on top of action 
					glm::vec3 temp = blocks[i];
					//deallocate occupied space
					int tX = (int)temp.x;
					int tY = (int)temp.y;
					int tZ = (int)temp.z;
					std::cout << (int)temp.y << std::endl;
					std::cout << "info" << std::endl;
					std::cout << tX << std::endl;
					std::cout << tY << std::endl;
					std::cout << tZ << std::endl;
					//		int color = blockColors[tX][tY][tZ];
					int color = 0;
					freeFallBlock((int)temp.x, (int)temp.y, (int)temp.z);
					temp.y = temp.y -1;
					tY = (int)temp.y;
					std::cout << tY << std::endl;
					//first check if the next surface we want to move to is already occupied
					//need to check if the object has touched the ground or if it has touched another grounded object
					fallObjQueue[k].blocks[i] = temp; //reassign
					assignFallBlock((int)temp.x, (int)temp.y, (int)temp.z, color);
					staticObj[tX][tY][tZ] = 1; //set previous coordinates to being stuck
					occupied[tX][tY][tZ] = 1;
					
					//if the block reaches the ground or touches the static blocks, then set the controlLen to zero
				}
			//	touchGroundQueue.push(k);
			}
			else{ //none of the objects touched the ground, so we can safely move all the blocks down
				
					for(int i = 0; i < curLen; i++){ //for each block, first check if there's going to be a grounding or on top of action 
					glm::vec3 temp = blocks[i];
					//deallocate occupied space
					int tX = (int)temp.x;
					int tY = (int)temp.y;
					int tZ = (int)temp.z;
					std::cout << (int)temp.y << std::endl;
					std::cout << "info" << std::endl;
					std::cout << tX << std::endl;
					std::cout << tY << std::endl;
					std::cout << tZ << std::endl;
					//		int color = blockColors[tX][tY][tZ];
					int color = 0;
					freeFallBlock((int)temp.x, (int)temp.y, (int)temp.z);
					temp.y = temp.y -1;
					tY = (int)temp.y;
					std::cout << tY << std::endl;
					//first check if the next surface we want to move to is already occupied
					//need to check if the object has touched the ground or if it has touched another grounded object
					fallObjQueue[k].blocks[i] = temp; //reassign
					assignFallBlock((int)temp.x, (int)temp.y, (int)temp.z, color);
				

					//if the block reaches the ground or touches the static blocks, then set the controlLen to zero
				}

			}

			//need to update all of the coordinates so that it reflects the new value since we excited out of the loop

		}
		while(!touchGroundQueue.empty()){
			
			int pos = touchGroundQueue.front();
			touchGroundQueue.pop();
			std::cout << "ground touch!" << std::endl;
			Shape curObj = fallObjQueue[pos];
			int curLen = curObj.length;
			glm::vec3* blocks = curObj.blocks;
			for(int k = 0; k < curLen; k++){
				std::cout << "yolo touch!" << std::endl;
				int tX = (int)fallObjQueue[pos].blocks[k].x;
				int tY = (int)fallObjQueue[pos].blocks[k].y;
				int tZ = (int)fallObjQueue[pos].blocks[k].z;
				staticObj[tX][tY][tZ] = 1;
				occupied[tX][tY][tZ] = 1;
			}
		}

		return grounded;
	}
	glm::vec3* getFallingObj() 
	{ 
		return controlPos;
	}
};

#endif