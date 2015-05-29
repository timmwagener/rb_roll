
#Setup Script
#-------------------------------------------------------------------------------------

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
