


#RbRoll Deformer
#-------------------------------------------------------------------------------------


#Last Modified: 08.01.13
#Author: Timm Wagener
#Description: Rotates Points in local space






#Setup Script
#-------------------------------------------------------------------------------------

'''
import pymel.core as pm


try:

	#load new scene without saving
	pm.mel.eval('file -f -new;')
	
	#if plugin is loaded unload and reload it, else reload
	if(pm.pluginInfo( 'rbRoll.py', query=True, loaded=True )):
		pm.unloadPlugin( 'rbRoll.py' )
		pm.loadPlugin( 'rbRoll.py' )
		
	else: pm.loadPlugin( 'rbRoll.py' )
except:
	print('Error Reloading Plugin')

try:
	#Create Sphere
	sphereTrans = pm.polySphere(ch = False, r = 1)[0]
	pm.select(cl = True)
	sphereShape = sphereTrans.getShape()
	pm.select(cl = True)
	
	#Create root_j and root_j_grp
	root_j = pm.joint(a = True, p = (0.0,0.0,0.0), n = 'root_j')
	pm.select(cl = True)
	root_j_grp = pm.group(root_j, n = 'root_j_grp')
	pm.select(cl = True)
	
	#create root_j_manip
	root_j_manip = pm.circle(r = 1.3, nr = (0.0,1.0,0.0), n = 'root_j_manip', ch = False)[0]
	pm.select(cl = True)
	root_j_manip_grp = pm.group(root_j_manip, n = 'root_j_manip_grp')
	pm.select(cl = True)
	
	#constrain j to manip
	pm.parentConstraint(root_j_manip, root_j_grp , mo = True)
	pm.scaleConstraint(root_j_manip, root_j_grp , mo = True)
	pm.select(cl = True)
	

	#Create Deformer
	pm.select(sphereTrans, r = True)
	rbRollNode = pm.deformer(type = 'RbRoll')[0]
	pm.setAttr(rbRollNode.radius, 1.0)
	pm.select(cl = True)
	
	#create skincluster
	skincluster = pm.skinCluster( root_j, sphereTrans, foc = False)
	pm.select(cl = True)
	
	#create frameCache nodes
	frameCacheX = pm.createNode('frameCache')
	pm.select(cl = True)
	pm.rename(frameCacheX, 'frameCacheX')
	pm.select(cl = True)
	
	frameCacheZ = pm.createNode('frameCache')
	pm.select(cl = True)
	pm.rename(frameCacheZ, 'frameCacheZ')
	pm.select(cl = True)
	
	#Connect Attrs
	pm.connectAttr(root_j_manip.translateX, frameCacheX.stream, f = True)
	pm.connectAttr(root_j_manip.translateZ, frameCacheZ.stream, f = True)
	
	pm.connectAttr(root_j_manip.translateX, rbRollNode.translateX, f = True)
	pm.connectAttr(root_j_manip.translateZ, rbRollNode.translateZ, f = True)
	
	pm.connectAttr(frameCacheX.past[1], rbRollNode.pastTranslateX, f = True)
	pm.connectAttr(frameCacheZ.past[1], rbRollNode.pastTranslateZ, f = True)
	pm.select(cl = True)
	
	
	#create and apply checker to sphere
	checkerMt = pm.shadingNode('lambert', name='checkerMt',asShader=1)
	pm.select(cl = True)
	
	checkerTexture = pm.shadingNode('checker', name='checkerTex', asTexture=1)
	pm.select(cl = True)
	
	place2dTexture = pm.createNode('place2dTexture')
	pm.select(cl = True)
	pm.setAttr(place2dTexture.repeatV, 4)
	pm.setAttr(place2dTexture.repeatU, 4)
	
	#Make connections
	checkerTexture.outColor >> checkerMt.color
	place2dTexture.outUV >> checkerTexture.uvCoord
	place2dTexture.outUvFilterSize >> checkerTexture.uvFilterSize
	
	
	#Assign Mt to sphere
	shadingGroup = pm.sets(renderable=1 , noSurfaceShader = 1 , empty=1 , name='checkerSG')
	pm.select(cl = True)
	pm.connectAttr(checkerMt.outColor, shadingGroup.surfaceShader, f = True)
	
	pm.sets(shadingGroup,edit=1,forceElement=sphereShape)
	
	
	#Create Sphere to visualize rotate output attrs
	
	#Create Sphere
	sphereTransConstrained = pm.polySphere(ch = False, r = 1)[0]
	pm.select(cl = True)
	sphereShapeConstrained = sphereTransConstrained.getShape()
	pm.select(cl = True)
	
	#point constrain sphereConstrained to joint
	pm.pointConstraint(root_j, sphereTransConstrained, mo = False)
	pm.select(cl = True)
	
	
	#Connect rotate output values
	pm.connectAttr(rbRollNode.rotateX, sphereTransConstrained.rotateX , f = True)
	pm.connectAttr(rbRollNode.rotateY, sphereTransConstrained.rotateY , f = True)
	pm.connectAttr(rbRollNode.rotateZ, sphereTransConstrained.rotateZ , f = True)
	pm.select(cl = True)
	
	
	
except:
	print('Error creating sphere and deformer')

'''







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
	
	
