

#ifndef RBROLLCUDAOPTIMIZED_H
#define RBROLLCUDAOPTIMIZED_H

//rbRollCUDAOptimized
//-----------------------------------------------

//include
//-----------------------------------------------
//Maya
#include <maya/MObject.h>
#include <maya/MPxDeformerNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MItGeometry.h>
#include <maya/MPoint.h>
#include <maya/MDataHandle.h>
#include <maya/MAnimControl.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MVector.h>
#include <maya/MAngle.h>
#include <maya/MEulerRotation.h>
#include <maya/MPointArray.h>


//RbRollCUDAOptimized
class RbRollCUDAOptimized: public MPxDeformerNode
{
public:
	//Inherited Methods
	RbRollCUDAOptimized();
	virtual ~RbRollCUDAOptimized();
	static void* creator();
	static MStatus initialize();
	virtual MStatus deform(MDataBlock& data,
		MItGeometry& itGeo,
		const MMatrix& localToWorldMatrix,
		unsigned int geomIndex);

	//Custom Methods
	MVector getDifferenceVector(MVector vectorA, MVector vectorB);
	void uploadStartframeDataToGPU();



	//Attributes
	static MTypeId id;
	static MString typeName;

	static MObject aVerbose;

	static MObject aCurrentRotationMatrix;

	static MObject aStartFrame;
	
	static MObject aTranslateX;
	static MObject aPastTranslateX;
	
	static MObject aTranslateZ;
	static MObject aPastTranslateZ;

	static MObject aRadius;
	static MObject aRotationOffset;

	static MObject oRotateX;
	static MObject oRotateY;
	static MObject oRotateZ;

};

#endif