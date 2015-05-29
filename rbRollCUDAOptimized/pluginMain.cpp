

//include
//-----------------------------------------------
#include "rbRollCUDAOptimized.h"


#include <maya/MFnPlugin.h>


//initialize
//-----------------------------------------------
MStatus initializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj, "Timm Wagener", "1.0", "Any");

	status = fnPlugin.registerNode(RbRollCUDAOptimized::typeName, RbRollCUDAOptimized::id, RbRollCUDAOptimized::creator, RbRollCUDAOptimized::initialize, MPxNode::kDeformerNode);

	return MStatus::kSuccess;
}


//uninitialize
//-----------------------------------------------
MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj);

	status = fnPlugin.deregisterNode(RbRollCUDAOptimized::id);
	

	return MStatus::kSuccess;
}