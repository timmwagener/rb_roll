


//Include
//-----------------------------------------------
#include "rbRollCUDAFunctionality.h"




//Kernels
//-----------------------------------------------
namespace kernels{

	__global__ void modifyArray(float* d_arrayToModify, float* d_resultArrayToModify)
	{
		int index = blockIdx.x * blockDim.x + threadIdx.x;
		d_resultArrayToModify[index] = d_arrayToModify[index] * 2.0;
	};


	__global__ void vectorMatrixMultiplication(float* vertexPositionXArray, float* vertexPositionYArray, float* vertexPositionZArray, float* rotationMatrixArray, float* resultVertexPositionXArray, float* resultVertexPositionYArray, float* resultVertexPositionZArray, int numberOperations)
	{

		int index = blockIdx.x * blockDim.x + threadIdx.x;
		
		if(index <= numberOperations)
		{
			resultVertexPositionXArray[index] = (vertexPositionXArray[index] * rotationMatrixArray[0]) + (vertexPositionYArray[index] * rotationMatrixArray[1]) + (vertexPositionZArray[index] * rotationMatrixArray[2]);
			resultVertexPositionYArray[index] = (vertexPositionXArray[index] * rotationMatrixArray[4]) + (vertexPositionYArray[index] * rotationMatrixArray[5]) + (vertexPositionZArray[index] * rotationMatrixArray[6]);
			resultVertexPositionZArray[index] = (vertexPositionXArray[index] * rotationMatrixArray[8]) + (vertexPositionYArray[index] * rotationMatrixArray[9]) + (vertexPositionZArray[index] * rotationMatrixArray[10]);
		}
	};
	
	
	__global__ void addTwoIntegers(int* a, int* b, int* result)
	{
		*result = *a+*b;
	};

}



//Definitions
//-----------------------------------------------
void cuda::executeModifyArray(float* arrayToModify, int arrayLength)
{

	//byteSize
	int byteSize = arrayLength * sizeof(float);

	//d_arrayToModify, d_resultArrayToModify
	float* d_arrayToModify, *d_resultArrayToModify;

	//allocate mem on GPU
	cudaMalloc((void**)&d_arrayToModify, byteSize);
	cudaMalloc((void**)&d_resultArrayToModify, byteSize);

	//copy host bytes to gpu
	cudaMemcpy(d_arrayToModify, arrayToModify, byteSize, cudaMemcpyHostToDevice);

	//launch kernel
	int blockDim = arrayLength;
	int numberOfBlocks = 1;
	kernels::modifyArray<<<numberOfBlocks, blockDim>>>(d_arrayToModify, d_resultArrayToModify);

	//copy results from device to host
	cudaMemcpy(arrayToModify, d_resultArrayToModify, byteSize, cudaMemcpyDeviceToHost);

	//free pointer
	cudaFree(d_arrayToModify); cudaFree(d_resultArrayToModify);

}

void cuda::executeVectorMatrixMultiplication(float* vertexPositionXArray, float* vertexPositionYArray, float* vertexPositionZArray, float* rotationMatrixArray, int vertexPositionArrayLength)
{

	
	//byteSize
	int byteSizeVertexPositionArray = vertexPositionArrayLength * sizeof(float);
	int byteSizeRotationMatrixArray = 16 * sizeof(float);

	//device pointers for vertexPositionArrays
	float* d_vertexPositionXArray, *d_vertexPositionYArray, *d_vertexPositionZArray;
	//device pointers for result arrays
	float* d_resultVertexPositionXArray, *d_resultVertexPositionYArray, *d_resultVertexPositionZArray;
	//device pointer rotationMatrixArray
	float* d_rotationMatrixArray;

	//allocate memory on device
	cudaMalloc((void**)&d_vertexPositionXArray, byteSizeVertexPositionArray);
	cudaMalloc((void**)&d_vertexPositionYArray, byteSizeVertexPositionArray);
	cudaMalloc((void**)&d_vertexPositionZArray, byteSizeVertexPositionArray);

	cudaMalloc((void**)&d_resultVertexPositionXArray, byteSizeVertexPositionArray);
	cudaMalloc((void**)&d_resultVertexPositionYArray, byteSizeVertexPositionArray);
	cudaMalloc((void**)&d_resultVertexPositionZArray, byteSizeVertexPositionArray);
	
	cudaMalloc((void**)&d_rotationMatrixArray, byteSizeRotationMatrixArray);

	//copy values from host to device
	cudaMemcpy(d_vertexPositionXArray, vertexPositionXArray, byteSizeVertexPositionArray, cudaMemcpyHostToDevice);
	cudaMemcpy(d_vertexPositionYArray, vertexPositionYArray, byteSizeVertexPositionArray, cudaMemcpyHostToDevice);
	cudaMemcpy(d_vertexPositionZArray, vertexPositionZArray, byteSizeVertexPositionArray, cudaMemcpyHostToDevice);

	cudaMemcpy(d_rotationMatrixArray, rotationMatrixArray, byteSizeRotationMatrixArray, cudaMemcpyHostToDevice);

	//execute kernel
	int blockSize = 512;
	int numberOfBlocks =  (vertexPositionArrayLength / blockSize) + 1;

	kernels::vectorMatrixMultiplication<<<numberOfBlocks, blockSize>>>(d_vertexPositionXArray, d_vertexPositionYArray, d_vertexPositionZArray, d_rotationMatrixArray, d_resultVertexPositionXArray, d_resultVertexPositionYArray, d_resultVertexPositionZArray, vertexPositionArrayLength);

	//Copy values back to host
	cudaMemcpy(vertexPositionXArray, d_resultVertexPositionXArray, byteSizeVertexPositionArray, cudaMemcpyDeviceToHost);
	cudaMemcpy(vertexPositionYArray, d_resultVertexPositionYArray, byteSizeVertexPositionArray, cudaMemcpyDeviceToHost);
	cudaMemcpy(vertexPositionZArray, d_resultVertexPositionZArray, byteSizeVertexPositionArray, cudaMemcpyDeviceToHost);

	//free device pointer
	cudaFree(d_vertexPositionXArray); cudaFree(d_vertexPositionYArray); cudaFree(d_vertexPositionZArray);
	cudaFree(d_resultVertexPositionXArray);cudaFree(d_resultVertexPositionYArray);cudaFree(d_resultVertexPositionZArray);
	cudaFree(d_rotationMatrixArray);
	
}


int cuda::executeAddTwoIntegers(int paramA, int paramB)
{
	
	//byteSize
	int byteSize = sizeof(int);

	//initialze integers
	int a = paramA, b = paramB, result;

	//device pointers for integers
	int* d_a, *d_b, *d_result;
	
	//allocate memory on the device
	cudaMalloc((void**)&d_a, byteSize);
	cudaMalloc((void**)&d_b, byteSize);
	cudaMalloc((void**)&d_result, byteSize);

	//copy values from host to device
	cudaMemcpy(d_a, &a, byteSize, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, &b, byteSize, cudaMemcpyHostToDevice);
	
	//execute kernel
	kernels::addTwoIntegers<<<1,1>>>(d_a, d_b, d_result);

	//copy results back from device to host
	cudaMemcpy(&result, d_result, byteSize, cudaMemcpyDeviceToHost);

	//CleanUp dynamically allocated memory
	cudaFree(d_a);cudaFree(d_b);cudaFree(d_result);

	//printf("Result: %d\n" , result);
	//printf("Cuda Error Msg: %s\n", cudaGetErrorString(cudaGetLastError()));
	
	return result;
}


int cuda::executeDeviceQuery(void)
{
	//deviceCount
	int deviceCount = 0;
	cudaError_t cudaErrorId;

	cudaErrorId = cudaGetDeviceCount(&deviceCount);
	//printf("DeviceCount: %d\n", deviceCount);
	//printf("Cuda ErrorId: %d\nErrorMsg: %s\n", int(cudaErrorId), cudaGetErrorString(cudaErrorId));

	return deviceCount;
}