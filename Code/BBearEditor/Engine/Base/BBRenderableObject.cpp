#include "BBRenderableObject.h"
#include "Render/BBDrawCall.h"
#include "Render/BBMaterial.h"
#include "Utils/BBUtils.h"
#include "Render/BBCamera.h"
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BufferObject/BBShaderStorageBufferObject.h"
#include "Render/BufferObject/BBElementBufferObject.h"
#include "Render/BBUniformUpdater.h"
#include "Render/BBRenderPass.h"
#include "Render/BBRenderQueue.h"
#include "Scene/BBSceneManager.h"


/**
 * @brief BBRenderableObject::BBRenderableObject            3D
 */
BBRenderableObject::BBRenderableObject()
    : BBRenderableObject(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBRenderableObject::BBRenderableObject(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale)
    : BBRenderableObject(position.x(), position.y(), position.z(),
                         rotation.x(), rotation.y(), rotation.z(),
                            scale.x(),    scale.y(),    scale.z())
{

}

BBRenderableObject::BBRenderableObject(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
    : BBGameObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    sharedInit();
}

/**
 * @brief BBRenderableObject::BBRenderableObject            2D
 * @param x
 * @param y
 * @param nWidth
 * @param nHeight
 */
BBRenderableObject::BBRenderableObject(int x, int y, int nWidth, int nHeight)
    : BBGameObject(x, y, nWidth, nHeight)
{
    sharedInit();
}

BBRenderableObject::~BBRenderableObject()
{
    BB_SAFE_DELETE(m_pVBO);
    BB_SAFE_DELETE(m_pSSBO);
    BB_SAFE_DELETE(m_pACBO);
    BB_SAFE_DELETE(m_pEBO);
    BB_SAFE_DELETE_ARRAY(m_pIndexes);
}

void BBRenderableObject::init()
{
    m_pCurrentMaterial->setMatrix4(LOCATION_MODELMATRIX, m_ModelMatrix.data());

    if (m_nIndexCount > 0)
    {
        m_pEBO = new BBElementBufferObject(m_nIndexCount);
        m_pEBO->submitData(m_pIndexes, m_nIndexCount);
    }
    closeLight();

    if (m_pVBO)
        m_pVBO->submitData();

    if (m_pSSBO)
        m_pSSBO->submitData();
}

void BBRenderableObject::render(BBCamera *pCamera)
{
    render(m_ModelMatrix, pCamera);
}

void BBRenderableObject::render(const QMatrix4x4 &modelMatrix, BBCamera *pCamera)
{
    if (m_bActive && m_bVisible)
    {
        m_pDrawCalls->draw(pCamera);
    }
}

void BBRenderableObject::insertInRenderQueue(BBRenderQueue *pQueue)
{
    if (m_bActive && m_bVisible)
    {
        bool bTransparent = m_pDrawCalls->getMaterial()->getBlendState();
        if (bTransparent)
        {
            pQueue->appendTransparentDrawCall(m_pDrawCalls);
        }
        else
        {
            pQueue->appendOpaqueDrawCall(m_pDrawCalls);
        }
        m_bInRenderQueue = true;
        // init
        m_pDrawCalls->updateOrderInRenderQueue(m_Position);
    }
}

void BBRenderableObject::removeFromRenderQueue(BBRenderQueue *pQueue)
{
    pQueue->removeOpaqueDrawCall(m_pDrawCalls);
    m_bInRenderQueue = false;
}

void BBRenderableObject::setPosition(const QVector3D &position, bool bUpdateLocalTransform)
{
    BBGameObject::setPosition(position, bUpdateLocalTransform);
    if (m_bInRenderQueue)
    {
        m_pDrawCalls->updateOrderInRenderQueue(m_Position);
    }
}

void BBRenderableObject::setModelMatrix(float px, float py, float pz,
                                        const QQuaternion &r,
                                        float sx, float sy, float sz)
{
    BBGameObject::setModelMatrix(px, py, pz, r, sx, sy, sz);
}

void BBRenderableObject::setVisibility(bool bVisible)
{
    BBGameObject::setVisibility(bVisible);
    m_pDrawCalls->setVisibility(bVisible);
}

void BBRenderableObject::setCurrentMaterial(BBMaterial *pMaterial)
{
    m_pCurrentMaterial = pMaterial;
    m_pCurrentMaterial->setMatrix4(LOCATION_MODELMATRIX, m_ModelMatrix.data());
    m_pCurrentMaterial->setMatrix4(LOCATION_VIEWMODELMATRIX_IT, m_ViewModelMatrix_IT.data());
    m_pDrawCalls->setMaterial(m_pCurrentMaterial);
}

void BBRenderableObject::restoreMaterial()
{
    setCurrentMaterial(m_pDefaultMaterial);
}

void BBRenderableObject::setMatrix4(const std::string &uniformName, const float *pMatrix4)
{
    m_pCurrentMaterial->setMatrix4(uniformName, pMatrix4);
}

void BBRenderableObject::setVector4(const std::string &uniformName, float x, float y, float z, float w)
{
    m_pCurrentMaterial->setVector4(uniformName, x, y, z, w);
}

void BBRenderableObject::setArrayVector4(const std::string &uniformName, const float *pArrayVector4, int nArrayCount)
{
    m_pCurrentMaterial->setArrayVector4(uniformName, pArrayVector4, nArrayCount);
}

void BBRenderableObject::setTexture(const std::string &uniformName, GLuint textureName)
{
    m_pCurrentMaterial->setVector4(LOCATION_TEXTURE_SETTING0, 1.0f, 0.0f, 0.0f, 0.0f);
    m_pCurrentMaterial->setSampler2D(uniformName, textureName);
}

void BBRenderableObject::openLight()
{
    // x: is there a light
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_SETTINGS(0), 1.0f, 0.0f, 0.0f, 0.0f);
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_SETTINGS(1), 0.0f, 0.0f, 0.0f, 0.0f);
}

void BBRenderableObject::closeLight()
{
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_SETTINGS(0), 0.0f, 0.0f, 0.0f, 0.0f);
    m_pCurrentMaterial->setVector4(LOCATION_LIGHT_SETTINGS(1), 0.0f, 0.0f, 0.0f, 0.0f);
}

void BBRenderableObject::appendSSBO(BBShaderStorageBufferObject *pSSBO)
{
    if (m_pSSBO == nullptr)
    {
        m_pSSBO = pSSBO;
    }
    else
    {
        m_pSSBO->pushBack(pSSBO);
    }
}

void BBRenderableObject::removeSSBO(BBShaderStorageBufferObject *pSSBO)
{
    // Head is an ssbo with vertex information and cannot be removed, so start with the second node
    if (m_pSSBO && pSSBO)
    {
        m_pSSBO->remove(pSSBO);
    }
}

void BBRenderableObject::appendACBO(BBAtomicCounterBufferObject *pACBO)
{
    // set ACBO for drawcall
    BBDrawCall *pDrawCall = m_pDrawCalls;
    while (pDrawCall != nullptr)
    {
        pDrawCall->setACBO(pACBO);
        pDrawCall = pDrawCall->next<BBDrawCall>();
    }
}

void BBRenderableObject::removeACBO()
{
    BBDrawCall *pDrawCall = m_pDrawCalls;
    while (pDrawCall != nullptr)
    {
        pDrawCall->removeACBO();
        pDrawCall = pDrawCall->next<BBDrawCall>();
    }
}

void BBRenderableObject::appendDrawCall(BBDrawCall *pDrawCall)
{
    if (m_pDrawCalls == nullptr)
    {
        m_pDrawCalls = pDrawCall;
    }
    else
    {
        m_pDrawCalls->pushBack(pDrawCall);
    }
}

void BBRenderableObject::sharedInit()
{
    m_bInRenderQueue = false;
    m_pDrawCalls = nullptr;
    m_bVisible = true;
    m_pDefaultMaterial = new BBMaterial();
    m_pCurrentMaterial = m_pDefaultMaterial;
    m_pVBO = nullptr;
    m_pSSBO = nullptr;
    m_pACBO = nullptr;
    m_pEBO = nullptr;
    m_pIndexes = nullptr;
    m_nIndexCount = 0;
    m_nVertexCount = 0;
    m_DefaultColor = QVector3D(1.0f, 1.0f, 1.0f);
}
