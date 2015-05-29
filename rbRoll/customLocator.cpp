


//C++ custom locator implementation
//-----------------------------------------------




//include
//-----------------------------------------------
#include "customLocator.h"




//CustomLocator class definitions
//-----------------------------------------------





//Variables definition
//-----------------------------------------------
int CustomLocator::verbose = 1;

MGLFunctionTable *CustomLocator::pGlFT;

MTypeId CustomLocator::id(0x70001);
MString CustomLocator::typeName("CustomLocator");

//imageMatrix
cv::Mat CustomLocator::imageMatrix;

//capture object
CvCapture* CustomLocator::capture;



//Standard methods definition
//-----------------------------------------------

//Constructor
CustomLocator::CustomLocator(){}

//Destructor
CustomLocator::~CustomLocator(){}


//creator
void* CustomLocator::creator()
{
	return new CustomLocator();
}

//initialize
MStatus CustomLocator::initialize()
{
	
	//Pointer to GlFunctionTable
	//-----------------------------------------------

	//Assign gl function table object to pointer pGlFT
	//Pointer to hardware renderer
	MHardwareRenderer * hwRenderer = MHardwareRenderer::theRenderer();
	//get a pointer to the renderer openGl function table
	pGlFT = hwRenderer->glFunctionTable();

	//Success msg
	if(verbose){MGlobal::displayInfo("CustomLocator initialization successfull");};


	return MStatus::kSuccess;
}


//draw
void CustomLocator::draw(M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{

	
	//Status
	MStatus statusMsg;
	
	//bool to check for texture load
	bool textureLoadedSuccessfully = true;
	
	int width, height;

	//TMP OPENMP TEST
	#ifdef _OPENMP
	MGlobal::displayInfo("OpenMP defined!");
	#endif
	#ifndef _OPENMP
	MGlobal::displayInfo("OpenMP NOT defined!");
	#endif


	/*
	//getImage
	std::string filePath = "C:\\Users\\twagener\\Desktop\\audi_8.bmp";
	int size = 512;
	cv::Mat imageMatrix;
	imageMatrix = getImage(0, filePath, size);
	
	//check if imageMatrix empty
	if(imageMatrix.empty()){
		if(verbose){MGlobal::displayInfo("Error loading image, imageMatrix is empty.");};
		textureLoadedSuccessfully = false;
	}
	
	
	
	//imageMatrix Mat
	cv::Mat imageMatrix;

	//read image from file
	imageMatrix = cv::imread("C:\\Users\\twagener\\Desktop\\audi_8.bmp", CV_LOAD_IMAGE_COLOR);
	cv::flip(imageMatrix, imageMatrix, 0);

	width = imageMatrix.cols;
	height = imageMatrix.rows;
	

	*/
	
	
	//getImage
	std::string filePath = "C:\\Users\\twagener\\Desktop\\audi_8.bmp";
	int size = 512;
	getImage(0, filePath, size);
	

	
	
	

	//OpenGL
	//-----------------------------------------------
	view.beginGL();

	pGlFT->glPushAttrib(MGL_ALL_ATTRIB_BITS);

	//if textureloaded then create openGl texture
	if(textureLoadedSuccessfully)
	{
	GLuint textureID;

	
	pGlFT->glEnable (MGL_TEXTURE_2D);
	

	pGlFT->glGenTextures(1, &textureID);

	pGlFT->glBindTexture(MGL_TEXTURE_2D, textureID);
	
	pGlFT->glTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_MIN_FILTER, MGL_LINEAR);
	pGlFT->glTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_MAG_FILTER, MGL_LINEAR);

	pGlFT->glTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_WRAP_S, MGL_CLAMP);
	pGlFT->glTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_WRAP_T, MGL_CLAMP);

	pGlFT->glTexImage2D (MGL_TEXTURE_2D, 0, MGL_RGB, imageMatrix.cols, imageMatrix.rows, 0, MGL_RGB, MGL_UNSIGNED_BYTE, imageMatrix.ptr());
	
	
	pGlFT->glBindTexture(MGL_TEXTURE_2D, textureID);
	}
	
	//Else set color to red
	else
	{
		pGlFT->glColor3f( 1.0f, 0.0f, 0.0f );
	};
	

	//Draw quad
	pGlFT->glBegin(MGL_QUADS);
    
	pGlFT->glTexCoord2f (0.0, 0.0);
    pGlFT->glVertex3f( -1.0f, -1.0f, 0.0f );
	pGlFT->glTexCoord2f (0.0, 1.0);
    pGlFT->glVertex3f( -1.0f, 1.0f, 0.0f );
	pGlFT->glTexCoord2f (1.0, 1.0);
    pGlFT->glVertex3f( 1.0f, 1.0f, 0.0f );
	pGlFT->glTexCoord2f (1.0, 0.0);
    pGlFT->glVertex3f( 1.0f, -1.0f, 0.0f );
    
    pGlFT->glEnd();

	pGlFT->glPopAttrib();

	view.endGL();


}







//Custom methods definition
//-----------------------------------------------

//getImage
void CustomLocator::getImage(int type, std::string filePath, int size)
{

	
	//switch statement to retrieve Mat from different methods
	//-----------------------------------------------
	switch(type)
	{

		//File
		case 0:
			imageFromFile(imageMatrix, filePath);
			break;

		//Capture
		case 1:
			imageFromVideo(imageMatrix, filePath);
			break;
		
		//Default: Red imageMatrix of size 512²
		default:
			break;
			
	}
	
	
	
	//Scale and return imageMatrix
	//-----------------------------------------------

	//IF imageMatrix is empty print error msg
	if(!imageMatrix.empty()){
		//resize imageMatrix
		cv::resize(imageMatrix, imageMatrix, cv::Size(size,size) );
	};
	
}


//imageFromFile
void CustomLocator::imageFromFile(cv::Mat& imageMatrix, std::string filePath)
{

	//read image from file
	imageMatrix = cv::imread(filePath, CV_LOAD_IMAGE_COLOR);
	cv::flip(imageMatrix, imageMatrix, 0);

}



//imageFromVideo
void CustomLocator::imageFromVideo(cv::Mat& imageMatrix, std::string filePath)
{
	
	//Pointer to capture obj in memory
	capture = cvCreateFileCapture(filePath.c_str());
	
	//Assign image from capture
	imageMatrix = cvQueryFrame(capture);

	//Destroy pointer to capture obj in mem
	cvReleaseCapture(&capture);

	


}











//DEPRECATED
//-----------------------------------------------
	


	/*
	//MImage file retrieval
	//-----------------------------------------------
	//create MImage and read from file
	MImage fileImage;
	statusMsg = fileImage.readFromFile(MString("C:\\Users\\twagener\\Desktop\\audi_8.bmp"));

	//if error on texture read from file set bool to false
	if(!statusMsg){
		if(verbose){MGlobal::displayInfo("Error lading texture");};
		textureLoadedSuccessfully = false;
	};
	
	
	//assign width and height
	unsigned int width, height;
	fileImage.getSize(width, height);
	*/
