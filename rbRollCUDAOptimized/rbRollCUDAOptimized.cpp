

//include
//-----------------------------------------------
#include "rbRollCUDAOptimized.h"
#include "rbRollCUDAOptimizedFunctionality.h"



//RbRollCUDAOptimized class definitions
//-----------------------------------------------





//Variables
//-----------------------------------------------
MObject RbRollCUDAOptimized::aVerbose;

MTypeId RbRollCUDAOptimized::id(0x70006);
MString RbRollCUDAOptimized::typeName("rbRollCUDAOptimized");

MObject RbRollCUDAOptimized::aStartFrame;
	
MObject RbRollCUDAOptimized::aCurrentRotationMatrix;
	
MObject RbRollCUDAOptimized::aTranslateX;
MObject RbRollCUDAOptimized::aPastTranslateX;
	
MObject RbRollCUDAOptimized::aTranslateZ;
MObject RbRollCUDAOptimized::aPastTranslateZ;

MObject RbRollCUDAOptimized::aRadius;
MObject RbRollCUDAOptimized::aRotationOffset;

MObject RbRollCUDAOptimized::oRotateX;
MObject RbRollCUDAOptimized::oRotateY;
MObject RbRollCUDAOptimized::oRotateZ;




//Custom methods
//-----------------------------------------------

//getDifferenceVector
MVector RbRollCUDAOptimized::getDifferenceVector(MVector vectorA, MVector vectorB)
{
	return vectorB-vectorA;
}

//uploadStartframeDataToGPU
void RbRollCUDAOptimized::uploadStartframeDataToGPU()
{
	cuda::uploadStartframeDataToGPU();
}




//Standard methods
//-----------------------------------------------

//Constructor
RbRollCUDAOptimized::RbRollCUDAOptimized(){}

//Destructor
RbRollCUDAOptimized::~RbRollCUDAOptimized(){}

//creator
void* RbRollCUDAOptimized::creator()
{
	return new RbRollCUDAOptimized();
}

//initialize
MStatus RbRollCUDAOptimized::initialize()
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



	return status;

}



//deform
MStatus RbRollCUDAOptimized::deform(MDataBlock& data,
		MItGeometry& itGeo,
		const MMatrix& localToWorldMatrix,
		unsigned int geomIndex)
{

	//MStatus var to pass to functions
	MStatus status;

	//verbose
	int verbose = data.inputValue(aVerbose).asInt();
	
	
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
		//Get handle to matrix attr and set it to identityMatrix, also set currentRotationMatrix to identityMatrix
		MDataHandle hACurrentRotationMatrix = data.inputValue(aCurrentRotationMatrix);
		hACurrentRotationMatrix.setMMatrix(MTransformationMatrix().asMatrix());
		currentRotationMatrix = MTransformationMatrix().asMatrix();

		
		//Upload vertexArrays and rotationMatrix to GPU memory
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
		currentRotationMatrix.get(rotationMatrixArray2D);

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

		//Upload to GPU and retrieve pointer
		

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
	







	
	//Deform loop
	//-----------------------------------------------
	while (!itGeo.isDone()) 
	{
		MPoint pointPos = itGeo.position();
		
		pointPos = pointPos * rotationMatrix;
		
		itGeo.setPosition(pointPos);

		itGeo.next();
	}






	//set rotation values for matrix transformation
	//-----------------------------------------------
	MTransformationMatrix rotationExtractionMatrix(rotationMatrix);
	hRotateX.setFloat(MAngle(rotationExtractionMatrix.eulerRotation().x, MAngle::kRadians).asDegrees());
	hRotateY.setFloat(MAngle(rotationExtractionMatrix.eulerRotation().y, MAngle::kRadians).asDegrees());
	hRotateZ.setFloat(MAngle(rotationExtractionMatrix.eulerRotation().z, MAngle::kRadians).asDegrees());

	
	return MStatus::kSuccess;

}