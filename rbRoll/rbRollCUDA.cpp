

//C++ OpenMP Multithreaded Deformer implementation of rbRoll python deformer plugin blockout

//include
//-----------------------------------------------
#include "rbRollCUDA.h"
#include "rbRollCUDAFunctionality.h"




//RbRoll class definitions
//-----------------------------------------------




//Variables
//-----------------------------------------------
MTypeId RbRollCUDA::id(0x70003);
MString RbRollCUDA::typeName("rbRollCPPCUDA");

MObject RbRollCUDA::aVerbose;

MObject RbRollCUDA::aStartFrame;
	
MObject RbRollCUDA::aCurrentRotationMatrix;

MObject RbRollCUDA::aTranslateX;
MObject RbRollCUDA::aPastTranslateX;
	
MObject RbRollCUDA::aTranslateZ;
MObject RbRollCUDA::aPastTranslateZ;

MObject RbRollCUDA::aRadius;
MObject RbRollCUDA::aRotationOffset;

MObject RbRollCUDA::oRotateX;
MObject RbRollCUDA::oRotateY;
MObject RbRollCUDA::oRotateZ;

MObject RbRollCUDA::aComputeParallel;




//Custom methods
//-----------------------------------------------

//getDifferenceVector
MVector RbRollCUDA::getDifferenceVector(MVector vectorA, MVector vectorB)
{
	return vectorB-vectorA;
}

//executeModifyArray
void RbRollCUDA::executeModifyArray(float* arrayToModify, int arrayLength)
{
	cuda::executeModifyArray(arrayToModify, arrayLength);
}

//executeVectorMatrixMultiplication
void RbRollCUDA::executeVectorMatrixMultiplication(float* vertexPositionXArray, float* vertexPositionYArray, float* vertexPositionZArray, float* rotationMatrixArray, int vertexPositionArrayLength)
{
	cuda::executeVectorMatrixMultiplication(vertexPositionXArray, vertexPositionYArray, vertexPositionZArray, rotationMatrixArray, vertexPositionArrayLength);
}

//executeDeviceQuery
int RbRollCUDA::executeDeviceQuery(void)
{
	return cuda::executeDeviceQuery();
}

//executeAddTwoIntegers
int RbRollCUDA::executeAddTwoIntegers(int a, int b)
{
	return cuda::executeAddTwoIntegers(a, b);
}



//Standard methods
//-----------------------------------------------

//Constructor
RbRollCUDA::RbRollCUDA(){}

//Destructor
RbRollCUDA::~RbRollCUDA(){}

//creator
void* RbRollCUDA::creator()
{
	return new RbRollCUDA();
}

//initialize
MStatus RbRollCUDA::initialize()
{

	//MStatus var as return value
	MStatus status;

	//Functionsets for attrs
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute mAttr;
	MFnEnumAttribute eAttr;


	//aVerbose
	aVerbose = eAttr.create("verbose", "verbose");
	eAttr.addField("Off", 0);
	eAttr.addField("On", 1);
	addAttribute(aVerbose);


	//aCurrentRotationMatrix
	aCurrentRotationMatrix = mAttr.create("currentRotationMatrix", "currentRotationMatrix");
	addAttribute(aCurrentRotationMatrix);

	//aStartFrame
	aStartFrame = nAttr.create("startFrame", "startFrame", MFnNumericData::kInt, 1);
	addAttribute(aStartFrame);
	attributeAffects(aStartFrame, outputGeom);

	//aTranslateX
	aTranslateX = nAttr.create("translateX", "translateX", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(1);
	addAttribute(aTranslateX);
	attributeAffects(aTranslateX, outputGeom);

	//aPastTranslateX
	aPastTranslateX = nAttr.create("pastTranslateX", "pastTranslateX", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(1);
	addAttribute(aPastTranslateX);
	attributeAffects(aPastTranslateX, outputGeom);

	//aTranslateZ
	aTranslateZ = nAttr.create("translateZ", "translateZ", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(1);
	addAttribute(aTranslateZ);
	attributeAffects(aTranslateZ, outputGeom);

	//aPastTranslateZ
	aPastTranslateZ = nAttr.create("pastTranslateZ", "pastTranslateZ", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(1);
	addAttribute(aPastTranslateZ);
	attributeAffects(aPastTranslateZ, outputGeom);

	//aRadius
	aRadius = nAttr.create("radius", "radius", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(1);
	addAttribute(aRadius);
	attributeAffects(aRadius, outputGeom);

	//aRotationOffset
	aRotationOffset = nAttr.create("rotationOffset", "rotationOffset", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(1);
	addAttribute(aRotationOffset);
	attributeAffects(aRotationOffset, outputGeom);


	//oRotateX
	oRotateX = nAttr.create("rotateX", "rotateX", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(0);
	nAttr.setWritable(0);
	nAttr.setStorable(0);
	addAttribute(oRotateX);
	attributeAffects(aTranslateX, oRotateX);
	attributeAffects(aTranslateZ, oRotateX);
	attributeAffects(aPastTranslateX, oRotateX);
	attributeAffects(aPastTranslateZ, oRotateX);

	//oRotateY
	oRotateY = nAttr.create("rotateY", "rotateY", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(0);
	nAttr.setWritable(0);
	nAttr.setStorable(0);
	addAttribute(oRotateY);
	attributeAffects(aTranslateX, oRotateY);
	attributeAffects(aTranslateZ, oRotateY);
	attributeAffects(aPastTranslateX, oRotateY);
	attributeAffects(aPastTranslateZ, oRotateY);

	//oRotateZ
	oRotateZ = nAttr.create("rotateZ", "rotateZ", MFnNumericData::kFloat, 0.0);
	nAttr.setKeyable(0);
	nAttr.setWritable(0);
	nAttr.setStorable(0);
	addAttribute(oRotateZ);
	attributeAffects(aTranslateX, oRotateZ);
	attributeAffects(aTranslateZ, oRotateZ);
	attributeAffects(aPastTranslateX, oRotateZ);
	attributeAffects(aPastTranslateZ, oRotateZ);


	//aComputeParallel
	aComputeParallel = eAttr.create("computeParallel", "computeParallel");
	eAttr.addField("Off", 0);
	eAttr.addField("OpenMP", 1);
	eAttr.addField("CUDA", 2);
	addAttribute(aComputeParallel);


	return status;

}



//deform
MStatus RbRollCUDA::deform(MDataBlock& data,
		MItGeometry& itGeo,
		const MMatrix& localToWorldMatrix,
		unsigned int geomIndex)
{	

	//verbose
	int verbose = data.inputValue(aVerbose).asInt();

	//Set computeParallel
	//-----------------------------------------------
	//computeParallel
	int computeParallel = data.inputValue(aComputeParallel).asInt();

	//check if CUDA available
	if(computeParallel == 2){
		if(executeDeviceQuery() == 0)
		{
			if(verbose){MGlobal::displayInfo("No CUDA capable GPU found. Defaulting to OpenMP.");};
			computeParallel = 1;
		}
	}

	//if computeParallel == OMP then syncNumOpenMPThreads
	if(computeParallel == 1)
	{	
		//OpenMP Sync Threads
		MThreadUtils::syncNumOpenMPThreads();
	}

	//MStatus var to pass to functions
	MStatus status;

	//Get all attributes
	//-----------------------------------------------


	//Input attrs

	//startFrame
	int startFrame = data.inputValue(aStartFrame).asInt();

	//currentRotationMatrix
	MMatrix currentRotationMatrix = data.inputValue(aCurrentRotationMatrix).asMatrix();

	//translateX
	float translateX = data.inputValue(aTranslateX).asFloat();
	//pastTranslateX
	float pastTranslateX = data.inputValue(aPastTranslateX).asFloat();

	//translateZ
	float translateZ = data.inputValue(aTranslateZ).asFloat();
	//pastTranslateZ
	float pastTranslateZ = data.inputValue(aPastTranslateZ).asFloat();

	//radius
	float radius = data.inputValue(aRadius).asFloat();

	//rotationOffset
	float rotationOffset = data.inputValue(aRotationOffset).asFloat();

	//envelope
	float env = data.inputValue(envelope).asFloat();


	//Output attrs

	//oRotateX
	MDataHandle hRotateX = data.outputValue(oRotateX);
	//oRotateY
	MDataHandle hRotateY = data.outputValue(oRotateY);
	//oRotateZ
	MDataHandle hRotateZ = data.outputValue(oRotateZ);

	//Set temp rotation values
	hRotateX.setFloat(0.0);
	hRotateY.setFloat(0.0);
	hRotateZ.setFloat(0.0);




	
	
	
	
	//If currentTime == startFrame reset currentMatrix to identityMatrix
	//-----------------------------------------------
	if(MAnimControl::currentTime().value() == startFrame)
	{
		MDataHandle hACurrentRotationMatrix = data.inputValue(aCurrentRotationMatrix);
		hACurrentRotationMatrix.setMMatrix(MTransformationMatrix().asMatrix());
		currentRotationMatrix = MTransformationMatrix().asMatrix();
		//print msg if verbose
		if(verbose){MGlobal::displayInfo("Current rotation matrix has been reset");};
	}




	
	
	
	//Compute differenceVector for groundplane,  rotationDegrees and circumference
	//-----------------------------------------------
	
	//differenceVector and distanceDifferenceVector
	MVector differenceVector = getDifferenceVector(MVector(translateX, 0.0,  translateZ), MVector(pastTranslateX, 0.0, pastTranslateZ));
	float distanceDifferenceVector = differenceVector.length();

	//circumference
	const float  PI_F=3.14159265358979f;
	float circumference = 2.0 * PI_F * radius;

	//if circumference == 0 then leave deform
	if(circumference == 0.0)
	{
		if(verbose){MGlobal::displayInfo("Circumference is zero");};
		return MStatus::kSuccess;
	};

	//compute degrees
	float rotationDegrees = (360.0 *(distanceDifferenceVector/circumference)) + rotationOffset;






	//Construct rotation matrix
	//-----------------------------------------------
	MTransformationMatrix frameRotationMatrix;
	differenceVector *= env;
	MVector surfaceNormalVector(0.0,1.0,0.0);
	MVector rotationAxis = differenceVector^surfaceNormalVector;
	rotationAxis.normalize();
	frameRotationMatrix.setToRotationAxis(rotationAxis, MAngle(rotationDegrees, MAngle::kDegrees).asRadians());

	//Multiply current with frameRotationMatrix
	MMatrix rotationMatrix = currentRotationMatrix * frameRotationMatrix.asMatrix();


	//Set currentMatrix attr to new rotationMatrix
	MDataHandle hACurrentRotationMatrix = data.inputValue(aCurrentRotationMatrix);
	hACurrentRotationMatrix.setMMatrix(rotationMatrix);
	

	//Compute rotation either serial, multithreaded with OMP or CUDA

	//Serial
	if(computeParallel == 0)
	{
		//Non Threaded Deform loop
		//-----------------------------------------------
		while (!itGeo.isDone()) 
		{
			MPoint pointPos = itGeo.position();
		
			pointPos = pointPos * rotationMatrix;
		
			itGeo.setPosition(pointPos);

			itGeo.next();
		}
		//Status Msg
		if(verbose){MGlobal::displayInfo("Deform method executed serial");};
	}
	
	//OpenMP
	else if(computeParallel == 1)
	{
		//Threaded deform loop vars
		//-----------------------------------------------
		MPointArray vertexPositionList;
		itGeo.allPositions(vertexPositionList);
		int vertexPositionListLength = vertexPositionList.length();

		MPoint vertexPosition;

		//Threaded deform loop
		//-----------------------------------------------
	
	
		#ifdef _OPENMP
		#pragma omp parallel for private(vertexPosition)
		#endif
		for(int index = 0; index < vertexPositionListLength; index++)
		{
			vertexPosition = vertexPositionList[index];
			vertexPosition *= rotationMatrix;
			vertexPositionList[index] = vertexPosition;
		}
		itGeo.setAllPositions(vertexPositionList);

		//Status Msg
		if(verbose){MGlobal::displayInfo("Deform method executed using OpenMP");};
	}

	//CUDA
	else
	{

		//For loop  for deformation vars
		//-----------------------------------------------
	
		//vertexPositionArrays
		MPointArray vertexPositionList;
		itGeo.allPositions(vertexPositionList);
		int vertexPositionListLength = vertexPositionList.length();

		float* vertexPositionXArray = (float*)malloc(vertexPositionListLength * sizeof(float));
		float* vertexPositionYArray = (float*)malloc(vertexPositionListLength * sizeof(float));
		float* vertexPositionZArray = (float*)malloc(vertexPositionListLength * sizeof(float));

		//iterate and assign x,y,z values to arrays
		for(int index = 0; index < vertexPositionListLength; index++)
		{
			vertexPositionXArray[index] = float(vertexPositionList[index].x);
			vertexPositionYArray[index] = float(vertexPositionList[index].y);
			vertexPositionZArray[index] = float(vertexPositionList[index].z);
		}


		//create matrix array
		double rotationMatrixArray2D [4][4];
		rotationMatrix.inverse().get(rotationMatrixArray2D);

		float rotationMatrixArray[16];
		for(int indexRow = 0; indexRow < 4; indexRow++)
		{
			for(int indexColumn = 0; indexColumn < 4; indexColumn++)
			{	
				int rowDimension = 4;
				int currentIndex = indexRow * rowDimension + indexColumn;
				rotationMatrixArray[currentIndex] = float(rotationMatrixArray2D[indexRow][indexColumn]);
			}
		}

		//execute kernel
		executeVectorMatrixMultiplication(vertexPositionXArray, vertexPositionYArray, vertexPositionZArray,  rotationMatrixArray, vertexPositionListLength);
	
		MPoint vertexPosition;

		//For loop  for deformation
		//-----------------------------------------------
		for(int index = 0; index < vertexPositionListLength; index++)
		{
		
			vertexPosition = MPoint(vertexPositionXArray[index], vertexPositionYArray[index], vertexPositionZArray[index]);
			vertexPositionList[index] = vertexPosition;
		}
		itGeo.setAllPositions(vertexPositionList);

		//Clear dynamic memory
		free(vertexPositionXArray);free(vertexPositionYArray);free(vertexPositionZArray);

		//Status Msg
		if(verbose){MGlobal::displayInfo("Deform method executed with CUDA");};
	}


	
	//set rotation values for matrix transformation
	//-----------------------------------------------
	MTransformationMatrix rotationExtractionMatrix(rotationMatrix);
	hRotateX.setFloat(MAngle(rotationExtractionMatrix.eulerRotation().x, MAngle::kRadians).asDegrees());
	hRotateY.setFloat(MAngle(rotationExtractionMatrix.eulerRotation().y, MAngle::kRadians).asDegrees());
	hRotateZ.setFloat(MAngle(rotationExtractionMatrix.eulerRotation().z, MAngle::kRadians).asDegrees());
	
	return MStatus::kSuccess;

}