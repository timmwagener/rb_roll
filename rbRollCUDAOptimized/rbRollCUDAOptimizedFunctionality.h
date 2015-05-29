
#ifndef RBROLLCUDAOPTIMIZEDFUNCTIONALITY_H
#define RBROLLCUDAOPTIMIZEDFUNCTIONALITY_H



//Include
//-----------------------------------------------



//Kernels
//-----------------------------------------------



//Definitions
//-----------------------------------------------

namespace cuda{

	//uploadStartframeDataToGPU()
	void uploadStartframeDataToGPU();

	//executeModifyArray
	void executeModifyArray(float* arrayToModify, int arrayLength);

	//executeVectorMatrixMultiplication
	void executeVectorMatrixMultiplication(float* vertexPositionXArray, float* vertexPositionYArray, float* vertexPositionZArray, float* rotationMatrixArray, int vertexPositionArrayLength);

	//executeDeviceQuery
	int executeDeviceQuery(void);

}


#endif