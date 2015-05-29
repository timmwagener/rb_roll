


//include
//-----------------------------------------------
#include "secondCmd.h"



//RbRollCmd definitions
//-----------------------------------------------

//Constructor
SecondCmd::SecondCmd()
{
}

//creator
void* SecondCmd::creator()
{
	return new SecondCmd;
}

//doIt
MStatus SecondCmd::doIt(const MArgList& argList)
{
	MGlobal::displayInfo("I just made my second cmd plugin");
	return MStatus::kSuccess;
}
