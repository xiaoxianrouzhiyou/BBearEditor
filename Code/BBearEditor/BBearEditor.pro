#-------------------------------------------------
#
# Project created by QtCreator 2021-05-09T15:51:16
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BBearEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    Editor/FileSystem/BBFileListWidget.cpp \
    Editor/FileSystem/BBFilePathBarWidget.cpp \
    Editor/FileSystem/BBFileSystemDataManager.cpp \
    Editor/FileSystem/BBFileSystemDockWidget.cpp \
    Editor/FileSystem/BBFileSystemManager.cpp \
    Editor/FileSystem/BBFolderTreeWidget.cpp \
    Editor/PropertyManager/BBFactoryComponent.cpp \
    Editor/PropertyManager/GroupManager/BBGroupManager.cpp \
    Editor/PropertyManager/BBPropertyFactory.cpp \
    Editor/PropertyManager/BBPropertyManager.cpp \
    Editor/Dialog/BBConfirmationDialog.cpp \
    Editor/MainInterface/BBMainWindow.cpp \
    Editor/Dialog/BBProjectDialog.cpp \
    Editor/MainInterface/BBTitleBar.cpp \
    Editor/SceneManager/BBHierarchyTreeWidget.cpp \
    Editor/ObjectList/BBObjectListWidget.cpp \
    Editor/Common/BBResizableWidget.cpp \
    Editor/Common/BBTreeWidget.cpp \
    Editor/Render/BBEditViewDockWidget.cpp \
    Editor/Render/BBEditViewOpenGLWidget.cpp \
    Editor/Render/BBOpenGLWidget.cpp \
    Editor/Common/NcFramelessHelper.cpp \
    Engine/3D/Mesh/BBMesh.cpp \
    Engine/3D/BBModel.cpp \
    Engine/3D/BBSkyBox.cpp \
    Engine/3D/Mesh/BBStaticMesh.cpp \
    Engine/Base/BBGameObject.cpp \
    Engine/Base/BBGameObjectSet.cpp \
    Engine/Base/BBRenderableObject.cpp \
    Engine/Geometry/BBBoundingBox.cpp \
    Engine/Geometry/BBRay.cpp \
    Engine/Render/BBCamera.cpp \
    Engine/Scene/BBScene.cpp \
    Engine/Allocator/tlsf/tlsf.c \
    Engine/Allocator/BBAllocator.cpp \
    Engine/Profiler/BBProfiler.cpp \
    Engine/Scene/BBSceneManager.cpp \
    Engine/Serializer/BBGameObject.pb.cc \
    Engine/Serializer/BBHierarchyTreeWidgetItem.pb.cc \
    Engine/Serializer/BBVector.pb.cc \
    Engine/Serializer/BBScene.pb.cc \
    Engine/Render/BufferObject/BBVertexBufferObject.cpp \
    Engine/Render/BufferObject/BBElementBufferObject.cpp \
    Engine/Render/BBRenderState.cpp \
    Engine/Render/BBMaterial.cpp \
    Engine/Render/BufferObject/BBBufferObject.cpp \
    Engine/Render/BBAttribute.cpp \
    Engine/Render/BBLinkedList.cpp \
    Engine/Render/Shader/BBShader.cpp \
    Engine/Render/BBUniformUpdater.cpp \
    Engine/Render/BBDrawCall.cpp \
    Engine/Render/BBMaterialProperty.cpp \
    Engine/Render/BBRenderPass.cpp \
    Engine/Render/Lighting/GameObject/BBLight.cpp \
    Engine/Render/BufferObject/BBFrameBufferObject.cpp \
    Engine/Render/Texture/BBTexture.cpp \
    Engine/Render/Lighting/GameObject/BBDirectionalLight.cpp \
    Engine/Render/Lighting/GameObject/BBPointLight.cpp \
    Engine/Render/Lighting/GameObject/BBSpotLight.cpp \
    Engine/3D/BBIcon.cpp \
    Engine/Scene/CoordinateSystem/BBCoordinateSystem.cpp \
    Engine/3D/BBHorizontalPlane.cpp \
    Engine/2D/BBSelectionRegion.cpp \
    Engine/Utils/BBUtils.cpp \
    Engine/3D/BBLightIndicator.cpp \
    Engine/2D/BBFullScreenQuad.cpp \
    Editor/Render/BBPreviewOpenGLWidget.cpp \
    Engine/Serializer/BBMaterial.pb.cc \
    Editor/PropertyManager/BBHeadManager.cpp \
    Engine/Geometry/BBPlane.cpp \
    Engine/Geometry/BBFrustum.cpp \
    Engine/2D/BBSprite2D.cpp \
    Engine/2D/BBCanvas.cpp \
    Engine/Scene/CoordinateSystem/BBCoordinateComponent.cpp \
    Engine/Scene/CoordinateSystem/BBCoordinateSystem2D.cpp \
    Engine/Base/BBRenderableObject2D.cpp \
    Engine/Scene/CoordinateSystem/BBCoordinateComponent2D.cpp \
    Engine/Scene/CoordinateSystem/BBTransformCoordinateSystem.cpp \
    Engine/Geometry/BBBoundingBox2D.cpp \
    Engine/2D/BBSpriteObject2D.cpp \
    Engine/Scene/BBRendererManager.cpp \
    Engine/2D/BBClipArea2D.cpp \
    Engine/3D/Mesh/BBTerrain.cpp \
    Engine/Font/BBDynamicFont.cpp \
    Editor/Tools/FBX2BBear/BBFBX2BBear.cpp \
    Editor/Tools/FBX2BBear/BBFBXSkeletonGPU.cpp \
    Engine/Render/RayTracing/BBRayTracker.cpp \
    Editor/Dialog/BBResourceDialog.cpp \
    Engine/Render/BBRenderQueue.cpp \
    Editor/PropertyManager/GroupManager/BBMaterialPropertyGroupManager.cpp \
    Editor/PropertyManager/GroupManager/BBLightManager.cpp \
    Engine/Serializer/BBCubeMap.pb.cc \
    Engine/3D/BBNormalIndicator.cpp \
    Editor/Debugger/BBConsoleDockWidget.cpp \
    Editor/PropertyManager/GroupManager/BBGlobalSettingsGroupManager.cpp \
    Engine/Render/Lighting/BBSphericalHarmonicLighting.cpp \
    Engine/Render/Lighting/GI/BBGlobalIllumination.cpp \
    Engine/Render/Lighting/GI/BBSSAOGlobalIllumination.cpp \
    Engine/Render/Lighting/GI/BBSSDOGlobalIllumination.cpp \
    Engine/Math/BBMath.cpp \
    Engine/Render/RayTracing/BBScreenSpaceRayTracker.cpp \
    Editor/PropertyManager/GroupManager/BBRenderManager.cpp \
    Engine/Render/Texture/BBProcedureTexture.cpp \
    Engine/ParticleSystem/BBParticleSystem.cpp \
    Engine/ParticleSystem/BBParticle.cpp \
    Engine/Render/BufferObject/BBShaderStorageBufferObject.cpp \
    Engine/Render/Shader/BBComputeShader.cpp \
    Engine/Render/Shader/BBBaseShader.cpp \
    Engine/Render/BufferObject/BBTranslateFeedbackObject.cpp \
    Engine/Python/BBPythonVM.cpp \
    Engine/Python/BBPythonModule.c \
    Engine/Render/Shadow/BBShadow.cpp \
    Engine/Render/Lighting/GI/BBFLCGlobalIllumination.cpp \
    Engine/Render/BufferObject/BBAtomicCounterBufferObject.cpp \
    Engine/Render/Volumetric/BBVolumetricCloud.cpp \
    Editor/Render/BBOfflineOpenGLWidget.cpp \
    Engine/Render/OfflineRenderer/BBOfflineRenderer.cpp \
    Engine/Geometry/BBPhotonMap.cpp \
    Engine/Render/Lighting/GameObject/BBAreaLight.cpp \
    Editor/PropertyManager/GroupManager/BBOfflineRendererManager.cpp \
    Engine/Render/OfflineRenderer/BBScatterMaterial.cpp \
    Engine/Math/BBPerlinNoise.cpp \
    Engine/Physics/FluidSystem/BBSPHParticleSystem.cpp \
    Engine/Physics/FluidSystem/BBSPHGridContainer.cpp \
    Engine/Physics/FluidSystem/BBSPHParticleNeighborTable.cpp \
    Engine/Physics/FluidSystem/BBSPHFluidSystem.cpp

HEADERS += \
    Editor/FileSystem/BBFileListWidget.h \
    Editor/FileSystem/BBFilePathBarWidget.h \
    Editor/FileSystem/BBFileSystemDataManager.h \
    Editor/FileSystem/BBFileSystemDockWidget.h \
    Editor/FileSystem/BBFileSystemManager.h \
    Editor/FileSystem/BBFolderTreeWidget.h \
    Editor/PropertyManager/BBFactoryComponent.h \
    Editor/PropertyManager/GroupManager/BBGroupManager.h \
    Editor/PropertyManager/BBPropertyFactory.h \
    Editor/PropertyManager/BBPropertyManager.h \
    Editor/Dialog/BBConfirmationDialog.h \
    Editor/MainInterface/BBMainWindow.h \
    Editor/Dialog/BBProjectDialog.h \
    Editor/MainInterface/BBTitleBar.h \
    Editor/SceneManager/BBHierarchyTreeWidget.h \
    Editor/ObjectList/BBObjectListWidget.h \
    Editor/Common/BBResizableWidget.h \
    Editor/Common/BBTreeWidget.h \
    Editor/Render/BBEditViewDockWidget.h \
    Editor/Render/BBEditViewOpenGLWidget.h \
    Editor/Render/BBOpenGLWidget.h \
    Editor/Common/NcFramelessHelper.h \
    Engine/3D/Mesh/BBMesh.h \
    Engine/3D/BBModel.h \
    Engine/3D/BBSkyBox.h \
    Engine/3D/Mesh/BBStaticMesh.h \
    Engine/Base/BBGameObject.h \
    Engine/Base/BBGameObjectSet.h \
    Engine/Base/BBRenderableObject.h \
    Engine/Geometry/BBBoundingBox.h \
    Engine/Geometry/BBRay.h \
    Engine/Render/BBCamera.h \
    Engine/Scene/BBScene.h \
    Engine/Allocator/tlsf/tlsf.h \
    Engine/Allocator/tlsf/tlsfbits.h \
    Engine/Allocator/BBAllocator.h \
    Engine/Allocator/BBMemoryLabel.h \
    Engine/Profiler/BBProfiler.h \
    Engine/Scene/BBSceneManager.h \
    Engine/Serializer/BBGameObject.pb.h \
    Engine/Serializer/BBHierarchyTreeWidgetItem.pb.h \
    Engine/Serializer/BBVector.pb.h \
    Engine/Serializer/BBScene.pb.h \
    Engine/Render/BufferObject/BBVertexBufferObject.h \
    Engine/Render/BufferObject/BBElementBufferObject.h \
    Engine/Render/BBRenderState.h \
    Engine/Render/BBMaterial.h \
    Engine/Render/BufferObject/BBBufferObject.h \
    Engine/Render/BBAttribute.h \
    Engine/Render/BBBaseRenderComponent.h \
    Engine/Render/BBLinkedList.h \
    Engine/Render/Shader/BBShader.h \
    Engine/Render/BBUniformUpdater.h \
    Engine/Render/BBDrawCall.h \
    Engine/Render/BBMaterialProperty.h \
    Engine/Render/BBRenderPass.h \
    Engine/Render/Lighting/GameObject/BBLight.h \
    Engine/Render/BufferObject/BBFrameBufferObject.h \
    Engine/Render/Texture/BBTexture.h \
    Engine/Render/Lighting/GameObject/BBDirectionalLight.h \
    Engine/Render/Lighting/GameObject/BBPointLight.h \
    Engine/Render/Lighting/GameObject/BBSpotLight.h \
    Engine/3D/BBIcon.h \
    Engine/Scene/CoordinateSystem/BBCoordinateSystem.h \
    Engine/3D/BBHorizontalPlane.h \
    Engine/2D/BBSelectionRegion.h \
    Engine/Utils/BBUtils.h \
    Engine/3D/BBLightIndicator.h \
    Engine/2D/BBFullScreenQuad.h \
    Editor/Render/BBPreviewOpenGLWidget.h \
    Engine/Serializer/BBMaterial.pb.h \
    Editor/PropertyManager/BBHeadManager.h \
    Engine/Geometry/BBPlane.h \
    Engine/Geometry/BBFrustum.h \
    Engine/2D/BBSprite2D.h \
    Engine/2D/BBCanvas.h \
    Engine/Scene/CoordinateSystem/BBCoordinateComponent.h \
    Engine/Scene/CoordinateSystem/BBCoordinateSystem2D.h \
    Engine/Base/BBRenderableObject2D.h \
    Engine/Scene/CoordinateSystem/BBCoordinateComponent2D.h \
    Engine/Scene/CoordinateSystem/BBTransformCoordinateSystem.h \
    Engine/Geometry/BBBoundingBox2D.h \
    Engine/2D/BBSpriteObject2D.h \
    Engine/Scene/BBRendererManager.h \
    Engine/2D/BBClipArea2D.h \
    Engine/3D/Mesh/BBTerrain.h \
    Engine/Font/BBDynamicFont.h \
    Editor/Tools/FBX2BBear/BBFBX2BBear.h \
    Editor/Tools/FBX2BBear/BBFBXSkeletonGPU.h \
    Engine/Render/RayTracing/BBRayTracker.h \
    Editor/Dialog/BBResourceDialog.h \
    Engine/Render/BBRenderQueue.h \
    Editor/PropertyManager/GroupManager/BBMaterialPropertyGroupManager.h \
    Editor/PropertyManager/GroupManager/BBLightManager.h \
    Engine/Serializer/BBCubeMap.pb.h \
    Engine/3D/BBNormalIndicator.h \
    Editor/Debugger/BBConsoleDockWidget.h \
    Editor/PropertyManager/GroupManager/BBGlobalSettingsGroupManager.h \
    Engine/Render/Lighting/BBSphericalHarmonicLighting.h \
    Engine/Render/Lighting/GI/BBGlobalIllumination.h \
    Engine/Render/Lighting/GI/BBSSAOGlobalIllumination.h \
    Engine/Math/BBMath.h \
    Engine/Render/Lighting/GI/BBSSDOGlobalIllumination.h \
    Engine/Render/RayTracing/BBScreenSpaceRayTracker.h \
    Editor/PropertyManager/GroupManager/BBRenderManager.h \
    Engine/Render/Texture/BBProcedureTexture.h \
    Engine/ParticleSystem/BBParticleSystem.h \
    Engine/ParticleSystem/BBParticle.h \
    Engine/Render/BufferObject/BBShaderStorageBufferObject.h \
    Engine/Render/Shader/BBComputeShader.h \
    Engine/Render/Shader/BBBaseShader.h \
    Engine/Render/BufferObject/BBTranslateFeedbackObject.h \
    Engine/Python/BBPythonVM.h \
    Engine/Render/Shadow/BBShadow.h \
    Engine/Render/Lighting/GI/BBFLCGlobalIllumination.h \
    Engine/Render/BufferObject/BBAtomicCounterBufferObject.h \
    Engine/Render/Volumetric/BBVolumetricCloud.h \
    Editor/Render/BBOfflineOpenGLWidget.h \
    Engine/Render/OfflineRenderer/BBOfflineRenderer.h \
    Engine/Geometry/BBPhotonMap.h \
    Engine/Render/Lighting/GameObject/BBAreaLight.h \
    Editor/PropertyManager/GroupManager/BBOfflineRendererManager.h \
    Engine/Render/OfflineRenderer/BBScatterMaterial.h \
    Engine/Math/BBPerlinNoise.h \
    Engine/Physics/FluidSystem/BBSPHParticleSystem.h \
    Engine/Physics/FluidSystem/BBSPHGridContainer.h \
    Engine/Physics/FluidSystem/BBSPHParticleNeighborTable.h \
    Engine/Physics/FluidSystem/BBSPHFluidSystem.h

FORMS += \
    Editor/FileSystem/BBFileSystemDockWidget.ui \
    Editor/Dialog/BBConfirmationDialog.ui \
    Editor/MainInterface/BBMainWindow.ui \
    Editor/Dialog/BBProjectDialog.ui \
    Editor/MainInterface/BBTitleBar.ui \
    Editor/Dialog/BBResourceDialog.ui

INCLUDEPATH += \
    ../BBearEditor/Editor \
    ../BBearEditor/Editor/Common \
    ../BBearEditor/Engine \
    ../BBearEditor/Engine/Render \
    ../../External \
    ../../External/Eigen \
    ../../External/rapidxml \
    ../../External/FBX/include \
    ../../External/lua \
    ../../External/ProtoBuffer/src

LIBS += -lopengl32 -lglu32 -lglut \
    -L../../External/ProtoBuffer/lib -lprotobuf \

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../External/Python37-32/libs/ -lpython37
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../External/Python37-32/libs/ -lpython37d

INCLUDEPATH += $$PWD/../../External/Python37-32/include
DEPENDPATH += $$PWD/../../External/Python37-32/include
