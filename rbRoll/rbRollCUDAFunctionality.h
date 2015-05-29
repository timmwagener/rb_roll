
#ifndef RBROLLCUDAFUNCTIONALITY_H
#define RBROLLCUDAFUNCTIONALITY_H



//Include
//-----------------------------------------------



//Kernels
//-----------------------------------------------



//Definitions
//-----------------------------------------------

namespace cuda{

	//executeModifyArray
	void executeModifyArray(float* arrayToModify, int arrayLength);

	//executeVectorMatrixMultiplication
	void executeVectorMatrixMultiplication(float* vertexPositionXArray, float* vertexPositionYArray, float* vertexPositionZArray, float* rotationMatrixArray, int vertexPositionArrayLength);

	//executeAddTwoIntegers
	int executeAddTwoIntegers(int paramA, int paramB);

	//executeDeviceQuery
	int executeDeviceQuery(void);

}


#endif