


#RbRoll Deformer
#-------------------------------------------------------------------------------------


#Last Modified: 08.01.13
#Author: Timm Wagener
#Description: Rotates Points in local space


#Imports
#-------------------------------------------------------------------------------------
import sys
import math
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaAnim as OpenMayaAnim










#RbRoll class
#-------------------------------------------------------------------------------------

class RbRoll(OpenMayaMPx.MPxDeformerNode):
	
	#Attributes
	aStartFrame = OpenMaya.MObject()
	
	aCurrentRotationMatrix = OpenMaya.MObject()
	
	
	aTranslateX = OpenMaya.MObject()
	aPastTranslateX = OpenMaya.MObject()
	
	aTranslateZ = OpenMaya.MObject()
	aPastTranslateZ = OpenMaya.MObject()
	
	aRadius = OpenMaya.MObject()
	aRotationOffset = OpenMaya.MObject()
	
	
	oRotateX = OpenMaya.MObject()
	oRotateY = OpenMaya.MObject()
	oRotateZ = OpenMaya.MObject()
	
	
	
	
	
	#Functions
	
	#Constructor
	def __init__(self):
		OpenMayaMPx.MPxDeformerNode.__init__(self)
		
	
	
	#deform
	def deform(self, data, itGeo, localToWorldMatrix, multiIndex):
		
		
		
		
		
		#Get Attributes
		#-------------------------------------------------------------------------------------
		
		#Get aXAxisDegrees Value
		startFrame = data.inputValue(self.aStartFrame).asInt()
		
		#Get aCurrentRotationMatrix Value
		currentRotationMatrix = OpenMaya.MMatrix()
		currentRotationMatrix = data.inputValue(self.aCurrentRotationMatrix).asMatrix()
		
		
		#Get aTranslateX Value
		translateX = data.inputValue(self.aTranslateX).asFloat()
		#Get aPastTranslateX Value
		pastTranslateX = data.inputValue(self.aPastTranslateX).asFloat()
		#Get aTranslateZ Value
		translateZ = data.inputValue(self.aTranslateZ).asFloat()
		#Get aPastTranslateZ Value
		pastTranslateZ = data.inputValue(self.aPastTranslateZ).asFloat()
		
		#Get aRadius Value
		radius = data.inputValue(self.aRadius).asFloat()
		
		#Get aRotationOffset Value
		rotationOffset = data.inputValue(self.aRotationOffset).asFloat()
		
		#Standard deformer envelope value
		aEnvelope = OpenMayaMPx.cvar.MPxDeformerNode_envelope
		envelope = data.inputValue(aEnvelope).asFloat()
		
		
		#Get Input geometry
		input = OpenMayaMPx.cvar.MPxDeformerNode_input
		inputGeom = OpenMayaMPx.cvar.MPxDeformerNode_inputGeom
		hInputGeomArrayAttr = data.outputArrayValue(input)
		hInputGeomArrayAttr.jumpToElement(multiIndex)
		
		hInputGeometry = hInputGeomArrayAttr.outputValue()
		inputGeometry = hInputGeometry.child(inputGeom).asMesh()
		
		
		
		#Get oRotateX Value
		hRotateX = data.outputValue(self.oRotateX)
		
		#Get oRotateY Value
		hRotateY = data.outputValue(self.oRotateY)
		
		#Get oRotateZ Value
		hRotateZ = data.outputValue(self.oRotateZ)
		
		#Set temp rotation values
		hRotateX.setFloat(0.0)
		hRotateY.setFloat(0.0)
		hRotateZ.setFloat(0.0)
		
		
		
		
		
		
		#If currentTime == startTime reset currentMatrix to identityMatrix 
		#-------------------------------------------------------------------------------------
		if(int(OpenMayaAnim.MAnimControl.currentTime().value()) == startFrame):
			hACurrentRotationMatrix = data.inputValue(self.aCurrentRotationMatrix)
			hACurrentRotationMatrix.setMMatrix(OpenMaya.MTransformationMatrix().asMatrix())
			OpenMaya.MGlobal.displayInfo('Reset currentMatrix')
		
		
		
		
		
		
		
		
		
		#Compute differenceVector for groundplane,  rotationDegrees and circumference
		#-------------------------------------------------------------------------------------
		
		#
		vectorX, vectorZ = self.getDifferenceVector([translateX,translateZ], [pastTranslateX,pastTranslateZ])
		distanceVectorXZ = self.getDistance([vectorX, vectorZ])
		
		#circumference
		circumference = 2.0 * math.pi * radius
		
		#If circumference is zero abort
		if(circumference == 0.0):
			OpenMaya.MGlobal.displayInfo('Circumference is zero')
			return OpenMaya.MStatus.kSuccess
		
		
		#Compute degrees
		rotationDegrees = (360.0 * (distanceVectorXZ/circumference)) + rotationOffset
		
		
		
		
		
		
		#Construct Rotation Matrix
		#-------------------------------------------------------------------------------------
		
		#frameRotationMatrix
		frameRotationMatrix = OpenMaya.MTransformationMatrix()
		vectorXYZ = OpenMaya.MVector(vectorX, 0.0, vectorZ)
		vectorXYZ = vectorXYZ * envelope
		surfaceNormalVector = OpenMaya.MVector(0.0, 1.0, 0.0)
		rotationAxis = vectorXYZ^surfaceNormalVector
		rotationAxis.normalize()
		frameRotationMatrix.setToRotationAxis(rotationAxis, math.radians(rotationDegrees))
		
		#multiply current with frame rotation matrix
		rotationMatrix = currentRotationMatrix * frameRotationMatrix.asMatrix()
		
		#set currentMatrix attr to new rotationMatrix
		hACurrentRotationMatrix = data.inputValue(self.aCurrentRotationMatrix)
		hACurrentRotationMatrix.setMMatrix(rotationMatrix)
		
		
		
		
		
		
		#Deform Loop
		#-------------------------------------------------------------------------------------
		while not(itGeo.isDone()):
			
			#Deform Point
			point = itGeo.position()
			
			point = point * rotationMatrix 
			
			itGeo.setPosition(point)
			
			itGeo.next()
		
		
		
		
		
		#Set Rotation Values for matrix transformation
		rotationExtractionMatrix = OpenMaya.MTransformationMatrix(rotationMatrix)
		hRotateX.setFloat(math.degrees(rotationExtractionMatrix.eulerRotation().x))
		hRotateY.setFloat(math.degrees(rotationExtractionMatrix.eulerRotation().y))
		hRotateZ.setFloat(math.degrees(rotationExtractionMatrix.eulerRotation().z))
		
		
		
		return OpenMaya.MStatus.kSuccess
		
		
		
		
	#getDifferenceVector
	def getDifferenceVector(self, vectorX, vectorZ):
		return [vectorZ[0] - vectorX[0], vectorZ[1] - vectorX[1]]
		
	
	#getDistance
	def getDistance(self, vector):
		return math.sqrt(math.pow(vector[0], 2) + math.pow(vector[1], 2))
		
		
	#normalize
	def normalize(self, vector):
		distance = math.sqrt(math.pow(vector[0], 2) + math.pow(vector[1], 2))
		return [vector[0] / distance, vector[1] / distance]
		
		




		
		
		
		
		
		
		

#Node Initialization
#-------------------------------------------------------------------------------------

#Creator Function
def createRbRoll():
	return OpenMayaMPx.asMPxPtr(RbRoll())
	
#Initialize Function
def initializeRbRoll():
	
	#Create Attr Functionsets
	nAttr = OpenMaya.MFnNumericAttribute()
	mAttr = OpenMaya.MFnMatrixAttribute()
	
	
	#Initialize Attributes
	
	#outputGeom
	outputGeom = OpenMayaMPx.cvar.MPxDeformerNode_outputGeom
	
	
	#aCurrentRotationMatrix
	RbRoll.aCurrentRotationMatrix = mAttr.create('currentRotationMatrix','currentRotationMatrix')
	RbRoll.addAttribute(RbRoll.aCurrentRotationMatrix)
	
	
	#aStartFrame
	RbRoll.aStartFrame = nAttr.create('startFrame','startFrame', OpenMaya.MFnNumericData.kInt, 1)
	nAttr.setKeyable(True)
	RbRoll.addAttribute(RbRoll.aStartFrame)
	
	
	#aTranslateX
	RbRoll.aTranslateX = nAttr.create('translateX','translateX', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(True)
	RbRoll.addAttribute(RbRoll.aTranslateX)
	RbRoll.attributeAffects(RbRoll.aTranslateX, outputGeom)
	
	#aPastTranslateX
	RbRoll.aPastTranslateX = nAttr.create('pastTranslateX','pastTranslateX', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(True)
	RbRoll.addAttribute(RbRoll.aPastTranslateX)
	RbRoll.attributeAffects(RbRoll.aPastTranslateX, outputGeom)
	
	#aTranslateZ
	RbRoll.aTranslateZ = nAttr.create('translateZ','translateZ', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(True)
	RbRoll.addAttribute(RbRoll.aTranslateZ)
	RbRoll.attributeAffects(RbRoll.aTranslateZ, outputGeom)
	
	#aPastTranslateZ
	RbRoll.aPastTranslateZ = nAttr.create('pastTranslateZ','pastTranslateZ', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(True)
	RbRoll.addAttribute(RbRoll.aPastTranslateZ)
	RbRoll.attributeAffects(RbRoll.aPastTranslateZ, outputGeom)
	
	#aRadius
	RbRoll.aRadius = nAttr.create('radius','radius', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(True)
	RbRoll.addAttribute(RbRoll.aRadius)
	RbRoll.attributeAffects(RbRoll.aRadius, outputGeom)
	
	#aRotationOffset
	RbRoll.aRotationOffset = nAttr.create('rotationOffset','rotationOffset', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(True)
	RbRoll.addAttribute(RbRoll.aRotationOffset)
	RbRoll.attributeAffects(RbRoll.aRotationOffset, outputGeom)
	
	
	
	#oRotateX
	RbRoll.oRotateX = nAttr.create('rotateX','rotateX', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(False)
	nAttr.setWritable(False)
	nAttr.setStorable(False)
	RbRoll.addAttribute(RbRoll.oRotateX)
	RbRoll.attributeAffects(RbRoll.aTranslateX, RbRoll.oRotateX)
	RbRoll.attributeAffects(RbRoll.aTranslateZ, RbRoll.oRotateX)
	
	#oRotateY
	RbRoll.oRotateY = nAttr.create('rotateY','rotateY', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(False)
	nAttr.setWritable(False)
	nAttr.setStorable(False)
	RbRoll.addAttribute(RbRoll.oRotateY)
	RbRoll.attributeAffects(RbRoll.aTranslateX, RbRoll.oRotateY)
	RbRoll.attributeAffects(RbRoll.aTranslateZ, RbRoll.oRotateY)
	
	#oRotateZ
	RbRoll.oRotateZ = nAttr.create('rotateZ','rotateZ', OpenMaya.MFnNumericData.kFloat, 0.0)
	nAttr.setKeyable(False)
	nAttr.setWritable(False)
	nAttr.setStorable(False)
	RbRoll.addAttribute(RbRoll.oRotateZ)
	RbRoll.attributeAffects(RbRoll.aTranslateX, RbRoll.oRotateZ)
	RbRoll.attributeAffects(RbRoll.aTranslateZ, RbRoll.oRotateZ)
	
	
	
	
	
	

	
	
#Plugin Registration
#-------------------------------------------------------------------------------------

kPluginNodeName = "RbRoll"
kPluginNodeId = OpenMaya.MTypeId(0x8700B)


#Plugin Registration
def initializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.registerNode( kPluginNodeName, kPluginNodeId, createRbRoll, initializeRbRoll, OpenMayaMPx.MPxNode.kDeformerNode)
	except:
		sys.stderr.write( "Failed to register node: %s" % kPluginNodeName )
		raise

def uninitializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.deregisterNode( kPluginNodeId )
	except:
		sys.stderr.write( "Failed to deregister node: %s" % kPluginNodeName )
		raise


#-------------------------------------------------------------------------------------
	
	
