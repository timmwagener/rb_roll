

//include
//-----------------------------------------------
#include "rbRollCmd.h"
#include "secondCmd.h"
#include "rbRoll.h"
#include "customLocator.h"
#include "rbRollOpenMP.h"
#include "rbRollCUDA.h"

#include <maya/MFnPlugin.h>


//initialize
//-----------------------------------------------
MStatus initializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj, "Timm Wagener", "1.0", "Any");

	status = fnPlugin.registerCommand("RbRollCmd", RbRollCmd::creator);
	status = fnPlugin.registerCommand("SecondCmd", SecondCmd::creator);
	status = fnPlugin.registerNode(RbRoll::typeName, RbRoll::id, RbRoll::creator, RbRoll::initialize, MPxNode::kDeformerNode);
	status = fnPlugin.registerNode(RbRollOpenMP::typeName, RbRollOpenMP::id, RbRollOpenMP::creator, RbRollOpenMP::initialize, MPxNode::kDeformerNode);
	status = fnPlugin.registerNode(RbRollCUDA::typeName, RbRollCUDA::id, RbRollCUDA::creator, RbRollCUDA::initialize, MPxNode::kDeformerNode);
	status = fnPlugin.registerNode(CustomLocator::typeName, CustomLocator::id, CustomLocator::creator, CustomLocator::initialize, MPxNode::kLocatorNode);

	return MStatus::kSuccess;
}


//uninitialize
//-----------------------------------------------
MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj);

	status = fnPlugin.deregisterCommand("RbRollCmd");
	status = fnPlugin.deregisterCommand("SecondCmd");
	status = fnPlugin.deregisterNode(RbRoll::id);
	status = fnPlugin.deregisterNode(RbRollOpenMP::id);
	status = fnPlugin.deregisterNode(RbRollCUDA::id);
	status = fnPlugin.deregisterNode(CustomLocator::id);

	return MStatus::kSuccess;
}