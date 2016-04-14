#pragma once

#include <stdlib.h>
#include <time.h>

class Water
{
public:

	/**
	* @brief: default constructor
	* @param: void
	* @return: none
	*/
	Water(void);
	
	/**
	* @brief: constructor to be used
	* @param: size of x axis to be used as xDimensions(int),  size of z axis to be used as zDimensions(int), base height of flat plane
	* @return: none
	*/
	Water(int xDimensions, int zDimensions, float yBase);
	
	/**
	* @brief: defsault deconstructor
	* @param: void
	* @return: none
	*/
	~Water(void);

	/**
	* @brief: fractal-forming method
	* @param: lowest x as x1 (uint), lowest z as z1 (uint), highest x as x2 (uint), highest z as z2(uint), scale amount as range (float), level in array as level (uint)
	* @return: none
	*/
	void DiamondSquare(unsigned x1, unsigned z1, unsigned x2, unsigned z2, float range, unsigned level);

	/**
	* @brief: returns size of x axis in array (x * z size)
	* @param: none
	* @return: value as integer
	*/
	int GetSizeX();

	/**
	* @brief: returns size of z axis in array (x * z size)
	* @param: none
	* @return: value as integer
	*/
	int GetSizeZ();

	/**
	* @brief: return value stored at index [x][z].
	* @param: x-index as x (int), z-index as z (int)
	* @return: value at array[x][z] as float
	*/
	float GetValAt(int x, int z);

private:

	float m_plane[1000][1000]; ///1000*1000 sized plane. Holds y values of plane. x/z co-ordinates implicit in drawing
	int m_iSizeX;///size of plane-array used along x-axis
	int m_iSizeZ;///size of plane-array used along z-axis
};
	/**
	* @brief: global randomising function provided by Water.h
	* @param: none
	* @return: random float -1 to 1
	*/
	float frand();
