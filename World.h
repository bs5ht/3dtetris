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
#include <glm/ext.hpp>
using namespace::std;
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

	void setBlock(int x, int y, int z, int color) 
	{ 
		occupied[x][y][z] = 1;
		blockColors[x][y][z] = color;
	}
	void clearBoard() 
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
	}
	bool checkBounds(int x, int y, int z){

		if( x < 0){
			return false;
		}
		if( x >= xLength){
			return false;
		}

		if( y < 0){
			return false;
		}
		if( y >= yLength){
			return false;
		}
		if( z < 0){
			return false;
		}
		if( z >= zLength){
			return false;
		}
		return true;
	}

	bool checkGameOver(){
		for(int y = 10; y < yLength; y++){
			for(int x = 0; x < xLength; x++){
				for(int z = 0; z < zLength; z++){
					if(staticObj[x][y][z] == 1){
						return true;
					}
				}
			}
		}
		return false;
	}



	void resetOccupied(){
		for(int x = 0; x < xLength; x++){
			for(int y = 0; y < yLength; y++){
				for(int z = 0; z < zLength; z++){
					occupied[x][y][z] = staticObj[x][y][z];
				}
			}
		}
	}
	void setFallObj(int x, int y, int z, int type) 
	{ 
		//glm::vec3 temp = glm::vec3(x, y, z);
		Shape s = Shape(x, y, z, type);
		std::vector<Shape>::iterator it;
		fallObjQueue.push_back(s);
		for(int i = 0; i < s.length; i++){
			int nX = s.blocks[i].x ;
			int nY = s.blocks[i].y;
			int nZ = s.blocks[i].z;
			occupied[nX][nY][nZ] = 1;
		}
		//blockColors[x][y][z] = color;
	}

	void addFallObj(int x, int y, int z, int color){
		glm::vec3 temp = glm::vec3(x, y, z);
		occupied[x][y][z] = 1;
		blockColors[x][y][z] = color;
	}


	void freeFallBlock(int x, int y, int z){
		//resetOccupied();

		occupied[x][y][z] = 0;
		blockColors[x][y][z] = 0;
	}
	void assignFallBlock(int x, int y, int z, int color){

		//if the block is not static, then deallocate it
		if(staticObj[x][y][z] == 0){
			occupied[x][y][z] = 1;
			blockColors[x][y][z] = 0;
		}
	}
	bool checkOnTop(Shape s, int dx, int dy, int dz){
		int length = s.length;
		for(int x = 0; x < length; x++){
			int tX = (int)s.blocks[x].x + dx;
			int tY = (int)s.blocks[x].y + dy;
			int tZ = (int)s.blocks[x].z + dz;
			if(staticObj[tX][tY][tZ]){
				return true;
			}
		}
		return false;
	}

	int clearLevel(){
		int tempOccupied[xLength][yLength][zLength]; 
		int tempStaticObj[xLength][yLength][zLength];
		int levelsCleared = 0;
		bool clearLev = false;
		for(int x = 0; x < xLength; x++){
			for(int y = 0; y < yLength; y++){
				for(int z = 0; z < zLength; z++){
					tempOccupied[x][y][z] = occupied[x][y][z];
					tempStaticObj[x][y][z] = staticObj[x][y][z];
				}
			}
		}
		bool allFilled = true;
		int filledFloor = 0;
		int bottomFloor = 0; 
		for(int y = 0; y < yLength; y++){
			bool curLevelFull = true;
			for(int x = 0; x < xLength; x++){ //check the state of the current 3d board
				for(int z = 0; z < zLength; z++){
					if(occupied[x][y][z] == 0){
						curLevelFull= false;
					}
				}
			}
			if(curLevelFull){
				filledFloor = y;
				clearLev = true;
				levelsCleared++;
				for(int dy = filledFloor; dy < yLength-2; dy++){ //start from the current floor and go to n -1 floors
					for(int dx = 0; dx < xLength; dx++){ // loop x and y coordinates
						for(int dz = 0; dz < zLength; dz++){
							tempOccupied[dx][dy][dz] = occupied[dx][dy+1][dz]; //set current temporary board to 
							tempStaticObj[dx][dy][dz] = staticObj[dx][dy+1][dz]; //set current temporary board to 
						}
					}
				}
			}
		}

		if(clearLev){
			for(int x = 0; x < xLength; x++){
				for(int y = 0; y < yLength; y++){
					for(int z = 0; z < zLength; z++){
						occupied[x][y][z] = tempOccupied[x][y][z];
						staticObj[x][y][z] = tempStaticObj[x][y][z];
					}
				}
			}
		}
		return levelsCleared;
	}

	glm::vec3 clampValues(int x, int y, int z){

		if( x < 0){
			x = 0;
		}
		if( x > xLength){
			x = xLength -1;
		}

		if( y < 0){
			y = 0;
		}
		if( y > yLength){
			y = yLength -1;
		}
		if( z < 0){
			z = 0;
		}
		if( z > zLength){
			z = zLength -1;
		}
		return glm::vec3(x, y, z);



	}


	void moveBlock(int dx, int dy, int dz){
		//glm::vec3 temp = fallObjPos[0];
		
		for(int i = 0; i < 1; i++){
			//check if object is grounded
			//check if object is on top of other objects 
			Shape s = fallObjQueue[i];
			int aX = s.blocks[1].x;
			int aY = s.blocks[1].y;
			int aZ = s.blocks[1].z;
			for(int j = 0; j < s.length; j++){ //check if ANY of the BLOCKS touches the ground 
				int tX = s.blocks[j].x;
				int tY = s.blocks[j].y;
				int tZ = s.blocks[j].z;

				int nX = tX + dx;
				int nY = tY + dy;
				int nZ = tZ + dz;
				if(!checkBounds(nX, nY, nZ)){
					return;
				}
			}
		}
		std::queue<int> touchGroundQueue;
		for(int i = 0; i < 1; i++){
			//check if object is grounded
			//check if object is on top of other objects 
			Shape s = fallObjQueue[i];
			int aX = s.blocks[1].x;
			int aY = s.blocks[1].y;
			int aZ = s.blocks[1].z;
			for(int j = 0; j < s.length; j++){ //check if ANY of the BLOCKS touches the ground 
				int tX = s.blocks[j].x;
				int tY = s.blocks[j].y;
				int tZ = s.blocks[j].z;

				int nX = tX + dx;
				int nY = tY + dy;
				int nZ = tZ + dz;
				if(checkBounds(nX, nY, nZ)){
					freeFallBlock(tX, tY, tZ);
					fallObjQueue[i].blocks[j] = clampValues(nX, nY, nZ);
					assignFallBlock(nX, nY, nZ, 1);
				}
			}
		}
	}
	void rotateBlock(int dx, int dy, int dz){
		int lastIndex = 0;
		queue<int> eraseList;
		bool erase = false;
			for(int i = 0; i < 1; i++){
			//check if object is grounded
			//check if object is on top of other objects 
			lastIndex = i;

			bool ground = false;
			bool otherShapes = false;
			Shape s = fallObjQueue[i];
			int aX = s.blocks[1].x;
			int aY = s.blocks[1].y;
			int aZ = s.blocks[1].z;
			for(int j = 0; j < s.length; j++){ //check if ANY of the BLOCKS touches the ground 
				int tX = s.blocks[j].x;
				int tY = s.blocks[j].y;
				int tZ = s.blocks[j].z;
				freeFallBlock(tX, tY, tZ);
				glm::mat4 rotMatrix = glm::mat4();
				rotMatrix = glm::translate(rotMatrix, glm::vec3(aX, aY, aZ)); //translate to origin
				glm::vec4 test;
				rotMatrix = glm::rotate(rotMatrix, 90.0f, glm::vec3(dx, dy, dz )); //roate around an axis
				//std::cout<<glm::to_string(rotMatrix)<<std::endl;
				rotMatrix = glm::translate(rotMatrix, glm::vec3(-1*aX, -1*aY, -1*aZ)); //translate this back
				//std::cout<<glm::to_string(rotMatrix)<<std::endl;
				glm::vec4 preMult = glm::vec4(tX, tY, tZ, 1.0f);

				glm::vec4 result = rotMatrix * preMult;
				int nX = abs(result.x);
				int nY = abs(result.y);
				int nZ = abs(result.z);
				//freeFallBlock(tX, tY, tZ);
				//assignFallBlock(tX, newY, tZ, 0);
				if(!checkBounds(nX, nY, nZ)){
					return;
				}

			}
			//cin.get();
		}
		

		for(int i = 0; i < 1; i++){
			//check if object is grounded
			//check if object is on top of other objects 
			lastIndex = i;

			bool ground = false;
			bool otherShapes = false;
			Shape s = fallObjQueue[i];
			int aX = s.blocks[1].x;
			int aY = s.blocks[1].y;
			int aZ = s.blocks[1].z;
			for(int j = 0; j < s.length; j++){ //check if ANY of the BLOCKS touches the ground 
				int tX = s.blocks[j].x;
				int tY = s.blocks[j].y;
				int tZ = s.blocks[j].z;
				freeFallBlock(tX, tY, tZ);
				glm::mat4 rotMatrix = glm::mat4();
				rotMatrix = glm::translate(rotMatrix, glm::vec3(aX, aY, aZ)); //translate to origin
				glm::vec4 test;
				rotMatrix = glm::rotate(rotMatrix, 90.0f, glm::vec3(dx, dy, dz )); //roate around an axis
				//std::cout<<glm::to_string(rotMatrix)<<std::endl;
				rotMatrix = glm::translate(rotMatrix, glm::vec3(-1*aX, -1*aY, -1*aZ)); //translate this back
				//std::cout<<glm::to_string(rotMatrix)<<std::endl;
				glm::vec4 preMult = glm::vec4(tX, tY, tZ, 1.0f);

				glm::vec4 result = rotMatrix * preMult;
				int nX = abs(result.x) ;
				int nY = abs(result.y);
				int nZ = abs(result.z);
				//freeFallBlock(tX, tY, tZ);
				//assignFallBlock(tX, newY, tZ, 0);
				fallObjQueue[i].blocks[j].x = nX;
				fallObjQueue[i].blocks[j].y = nY;
				fallObjQueue[i].blocks[j].z = nZ;
				assignFallBlock(nX, nY, nZ, 1);

			}
			//cin.get();
		}


	}
	//checks if the object has touched ground
	bool touchedGround(int x, int y, int z){

		//if the block is not static, then deallocate it
		if(y <= -1){
			return true;
		}
		return false;
	}

	void setStaticBlock(int x, int y, int z){
		//occupied[x][y][z] = 1;
		//staticObj[x][y][z] = 1;
	}

	bool moveFallObjNatural()// moves all the objects down one unit naturally
	{
		int lastIndex = 0;
		queue<int> eraseList;
		bool erase = false;
		for(int i = 0; i < fallObjQueue.size(); i++){
			//check if object is grounded
			//check if object is on top of other objects 
			lastIndex = i;

			bool ground = false;
			bool otherShapes = false;
			Shape s = fallObjQueue[i];
			for(int j = 0; j < s.length; j++){ //check if ANY of the BLOCKS touches the ground 
				int tX = s.blocks[j].x;
				int tY = s.blocks[j].y;
				int tZ = s.blocks[j].z;
				//freeFallBlock(tX, tY, tZ);
				int newY = s.blocks[j].y -1;
				//assignFallBlock(tX, newY, tZ, 0);
				ground = touchedGround(tX, newY, tZ);
				otherShapes = staticObj[tX][newY][tZ];
				if(otherShapes){
					break;
				}
				if(ground){
					break;
				}
			}
			if(otherShapes){
				for(int j = 0; j < s.length; j++){ //check if ANY of the BLOCKS touches the ground 
					int tX = s.blocks[j].x;
					int tY = s.blocks[j].y;
					int tZ = s.blocks[j].z;
					freeFallBlock(tX, tY, tZ);
					int newY = s.blocks[j].y -1;
					//assignFallBlock(tX, newY, tZ, 0);
					occupied[tX][tY][tZ] = 1;
					staticObj[tX][tY][tZ] = 1;
				}
				erase = true;
			}
			else if(ground){
				for(int j = 0; j < s.length; j++){ //check if ANY of the BLOCKS touches the ground 
					int tX = s.blocks[j].x;
					int tY = s.blocks[j].y;
					int tZ = s.blocks[j].z;
					//freeFallBlock(tX, tY, tZ);
					int newY = s.blocks[j].y -1;
					//assignFallBlock(tX, newY, tZ, 0);
					occupied[tX][tY][tZ] = 1;
					staticObj[tX][tY][tZ] = 1;
				}
				erase = true;
			}
			else{
				for(int j = 0; j < s.length; j++){ //this is the case when there are no intersections
					int tX = s.blocks[j].x;
					int tY = s.blocks[j].y;
					int tZ = s.blocks[j].z;
					freeFallBlock(tX, tY, tZ);
					int newY = s.blocks[j].y -1;
					assignFallBlock(tX, newY, tZ, 0);
					fallObjQueue[i].blocks[j].y = newY;
				}
			}
		}
		if(erase){
			fallObjQueue.erase(fallObjQueue.begin());
		}




		return false;
	}
	glm::vec3* getFallingObj() 
	{ 
		return controlPos;
	}
};

#endif