


#ifndef CUSTOMLOCATOR_H
#define CUSTOMLOCATOR_H



//C++ custom locator implementation
//-----------------------------------------------



//Test script
//-----------------------------------------------

/*
import pymel.core as pm


#reload plugin
#----------------------------------
try:
    #load new scene without saving
    pm.mel.eval('file -f -new;')
    #if plugin is loaded unload and reload it, else reload
    if(pm.pluginInfo( 'rbRollCPP.mll', query=True, loaded=True )):
        pm.unloadPlugin( 'rbRollCPP.mll' )
        pm.loadPlugin( 'rbRollCPP.mll' )
        print('Successfully reloaded plugin')
    else:
        pm.loadPlugin( 'rbRollCPP.mll' )
        print('Successfully loaded plugin')
except:
	print('Error Reloading Plugin')




#create plugin
#----------------------------------
pm.select(cl = True)
customLocatorShape = pm.createNode('CustomLocator')
customLocatorTransform = customLocatorShape.getParent()
pm.select(cl = True)
*/




//include
//-----------------------------------------------
#include <maya/MObject.h>
#include <maya/MPxLocatorNode.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MGLFunctionTable.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MGlobal.h>
#include <maya/MImage.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnNumericAttribute.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



//CustomLocator class declaration
//-----------------------------------------------
class CustomLocator : public MPxLocatorNode
{
public:

	//Variables declaration
	//-----------------------------------------------

	//verbose
	static int verbose;

	//pointer to openGl function table
	static MGLFunctionTable *pGlFT;

	//MTypeId
	static MTypeId id;

	//MTypeName
	static MString typeName;

	//imageMatrix
	static cv::Mat imageMatrix;

	//capturePointer
	static CvCapture* capture;
	
	
	
	//Standard methods declaration
	//-----------------------------------------------

	//Constructor
	CustomLocator();
	//Destructor
	virtual ~CustomLocator();

	//creator function (delivers pointer to new instance)
	static void* creator();

	//initialize (assignments to class variables. Called only once)
	static MStatus initialize();

	//draw
	virtual void draw(
		M3dView & view,
		const MDagPath & path,
		M3dView::DisplayStyle style,
		M3dView::DisplayStatus status
		);
	
	
	

	
	//Custom methods declaration
	//-----------------------------------------------

	//getImage
	static void getImage(int type, std::string filePath, int size);

	//imageFromMImage
	static void imageFromFile(cv::Mat& imageMatrix, std::string filePath);

	//imageFromVideo
	static void imageFromVideo(cv::Mat& imageMatrix, std::string filePath);


};
#endif