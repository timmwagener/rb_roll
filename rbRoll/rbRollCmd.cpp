


//include
//-----------------------------------------------
#include "rbRollCmd.h"



//RbRollCmd definitions
//-----------------------------------------------

//Constructor
RbRollCmd::RbRollCmd()
{
}

//creator
void* RbRollCmd::creator()
{
	return new RbRollCmd;
}

//doIt
MStatus RbRollCmd::doIt(const MArgList& argList)
{
	MGlobal::displayInfo("I just made a sick cmd plugin");
	return MStatus::kSuccess;
}
