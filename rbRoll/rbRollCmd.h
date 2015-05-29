

#ifndef RBROLLCMD_H
#define RBROLLCMD_H



//include
//-----------------------------------------------
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MObject.h>



//RbRollCmd declaration
//-----------------------------------------------
class RbRollCmd : public MPxCommand
{

public:
	
	//Constructor
	RbRollCmd();
	
	/*doIt (Method that is overwritten from parent 
	and executed when the cmd is executed)*/
	virtual MStatus doIt(const MArgList& argList);

	//creator
	static void* creator();

};



#endif



