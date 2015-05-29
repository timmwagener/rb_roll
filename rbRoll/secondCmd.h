

#ifndef SECONDCMD_H
#define SECONDCMD_H



//include
//-----------------------------------------------
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MObject.h>



//RbRollCmd declaration
//-----------------------------------------------
class SecondCmd : public MPxCommand
{

public:
	
	//Constructor
	SecondCmd();
	
	/*doIt (Method that is overwritten from parent 
	and executed when the cmd is executed)*/
	virtual MStatus doIt(const MArgList& argList);

	//creator
	static void* creator();

};



#endif



