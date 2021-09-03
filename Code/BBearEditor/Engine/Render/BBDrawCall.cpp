#include "BBDrawCall.h"
#include "BufferObject/BBVertexBufferObject.h"
#include "BufferObject/BBShaderStorageBufferObject.h"
#include "BufferObject/BBElementBufferObject.h"
#include "BufferObject/BBFrameBufferObject.h"
#include "BufferObject/BBAtomicCounterBufferObject.h"
#include "BBCamera.h"
#include "BBRenderPass.h"
#include "Scene/BBSceneManager.h"
#include "Scene/BBScene.h"
#include "Lighting/GameObject/BBLight.h"
#include "Lighting/GameObject/BBDirectionalLight.h"
#include "Base/BBRenderableObject.h"
#include "Render/BBRenderQueue.h"


/**
 * @brief BBDrawCall::BBDrawCall
 */
BBDrawFunc BBDrawCall::m_DrawFunc = &BBDrawCall::renderForwardPass;

BBDrawCall::BBDrawCall()
    : BBBaseRenderComponent()
{
    m_pMaterial = nullptr;

    m_eDrawPrimitiveType = GL_TRIANGLES;
    m_nDrawStartIndex = 0;

    m_pVBO = nullptr;
    m_pSSBO = nullptr;
    m_pACBO = nullptr;
    m_nDrawCount = 3;
    m_pEBO = nullptr;
    m_nIndexCount = 0;

    m_bVisible = true;

    m_UpdateOrderInRenderQueueFunc = &BBDrawCall::updateOrderInOpaqueRenderQueue;
}

void BBDrawCall::setMaterial(BBMaterial *pMaterial)
{
    if (m_pMaterial)
    {
        BBRenderQueue *pRenderQueue = BBSceneManager::getRenderQueue();
        pRenderQueue->switchQueue(m_pMaterial, pMaterial, this);
        if (pMaterial->getBlendState())
        {
            m_UpdateOrderInRenderQueueFunc = &BBDrawCall::updateOrderInTransparentRenderQueue;
        }
        else
        {
            m_UpdateOrderInRenderQueueFunc = &BBDrawCall::updateOrderInOpaqueRenderQueue;
        }
    }

    m_pMaterial = pMaterial;
    m_pMaterial->bindDrawCallInstance(this);
}

void BBDrawCall::updateMaterialBlendState(bool bEnable)
{
    BBRenderQueue *pRenderQueue = BBSceneManager::getRenderQueue();
    pRenderQueue->switchQueue(bEnable, this);
    if (bEnable)
    {
        m_UpdateOrderInRenderQueueFunc = &BBDrawCall::updateOrderInTransparentRenderQueue;
    }
    else
    {
        m_UpdateOrderInRenderQueueFunc = &BBDrawCall::updateOrderInOpaqueRenderQueue;
    }
}

void BBDrawCall::setVBO(BBVertexBufferObject *pVBO, GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount)
{
    m_pVBO = pVBO;
    m_eDrawPrimitiveType = eDrawPrimitiveType;
    m_nDrawStartIndex = nDrawStartIndex;
    m_nDrawCount = nDrawCount;
}

void BBDrawCall::setSSBO(BBShaderStorageBufferObject *pSSBO, GLenum eDrawPrimitiveType, int nDrawStartIndex, int nDrawCount)
{
    m_pSSBO = pSSBO;
    m_eDrawPrimitiveType = eDrawPrimitiveType;
    m_nDrawStartIndex = nDrawStartIndex;
    m_nDrawCount = nDrawCount;
}

void BBDrawCall::setEBO(BBElementBufferObject *pEBO, GLenum eDrawPrimitiveType, int nIndexCount, int nDrawStartIndex)
{
    m_pEBO = pEBO;
    m_eDrawPrimitiveType = eDrawPrimitiveType;
    m_nIndexCount = nIndexCount;
    m_nDrawStartIndex = nDrawStartIndex;
}

void BBDrawCall::updateOrderInRenderQueue(const QVector3D &renderableObjectPosition)
{
    m_RenderableObjectPosition = renderableObjectPosition;
    (this->*m_UpdateOrderInRenderQueueFunc)();
}

float BBDrawCall::getDistanceToCamera(BBCamera *pCamera)
{
    return m_RenderableObjectPosition.distanceToPoint(pCamera->getPosition());
}

void BBDrawCall::draw(BBCamera *pCamera)
{
    (this->*m_DrawFunc)(pCamera);
}

void BBDrawCall::switchRenderingSettings(int nIndex)
{
    switch (nIndex) {
    case 0:
        m_DrawFunc = &BBDrawCall::renderForwardPass;
        break;
    case 1:
        m_DrawFunc = &BBDrawCall::renderViewSpaceFBOPass;
        break;
    case 2:
        m_DrawFunc = &BBDrawCall::renderLightSpaceFBOPass;
        break;
    default:
        break;
    }
}

void BBDrawCall::renderOnePass(BBCamera *pCamera)
{
    QList<BBGameObject*> lights = collectLights();
    renderOnePass(pCamera, lights);
}

void BBDrawCall::renderOnePass(BBCamera *pCamera, QList<BBGameObject*> lights)
{
    m_pVBO->bind();
    BBRenderPass *pBaseRenderPass = m_pMaterial->getBaseRenderPass();
    for (int i = 0; i < lights.count(); i++)
    {
        BBLight *pLight = (BBLight*)lights[i];
        pLight->setRenderPass(pBaseRenderPass);
        pBaseRenderPass->bind(pCamera);
        if (m_pEBO == nullptr)
        {
            m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
        }
        else
        {
            m_pEBO->bind();
            m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
            m_pEBO->unbind();
        }
    }
    pBaseRenderPass->unbind();
    m_pVBO->unbind();
}

void BBDrawCall::renderOnePassSSBO(BBCamera *pCamera)
{
    m_pSSBO->bind();
    BBRenderPass *pBaseRenderPass = m_pMaterial->getBaseRenderPass();
    pBaseRenderPass->bind(pCamera);

    m_pEBO->bind();
    m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
    m_pEBO->unbind();

    pBaseRenderPass->unbind();
    m_pSSBO->unbind();
}

void BBDrawCall::renderForwardPass(BBCamera *pCamera)
{
    if (m_bVisible)
    {
        QList<BBGameObject*> lights = collectLights();

        m_pVBO->bind();
        if (m_pACBO)
            m_pACBO->bind();

        // base
        if (lights.count() > 0)
        {
            // render the first light
            BBLight *pLight = (BBLight*)lights[0];
            pLight->setRenderPass(m_pMaterial->getBaseRenderPass());
        }
        m_pMaterial->getBaseRenderPass()->bind(pCamera);
        if (m_pEBO == nullptr)
        {
            m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
        }
        else
        {
            m_pEBO->bind();
            m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
            m_pEBO->unbind();
        }
        m_pMaterial->getBaseRenderPass()->unbind();

        // additive
        BBRenderPass *pAdditiveRenderPass = m_pMaterial->getAdditiveRenderPass();
        if (lights.count() > 1 && pAdditiveRenderPass)
        {
            for (int i = 1; i < lights.count(); i++)
            {
                BBLight *pLight = (BBLight*)lights[i];
                pLight->setRenderPass(pAdditiveRenderPass);
                pAdditiveRenderPass->bind(pCamera);
                if (m_pEBO == nullptr)
                {
                    m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
                }
                else
                {
                    m_pEBO->bind();
                    m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
                    m_pEBO->unbind();
                }
            }
            pAdditiveRenderPass->unbind();
        }

        if (m_pACBO)
            m_pACBO->unbind();
        m_pVBO->unbind();
    }

    if (m_pNext != nullptr)
    {
        next<BBDrawCall>()->draw(pCamera);
    }
}

void BBDrawCall::renderUIPass(BBCanvas *pCanvas)
{
    m_pVBO->bind();
    BBRenderPass *pRenderPass = m_pMaterial->getBaseRenderPass();
    pRenderPass->bind(pCanvas);
    pRenderPass->setupStencilBuffer();
    if (m_pEBO == nullptr)
    {
        m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
    }
    else
    {
        m_pEBO->bind();
        m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
        m_pEBO->unbind();
    }
    pRenderPass->restoreStencilBuffer();
    pRenderPass->unbind();
    m_pVBO->unbind();
}

void BBDrawCall::renderViewSpaceFBOPass(BBCamera *pCamera)
{
    BB_PROCESS_ERROR_RETURN(m_pMaterial->isWriteFBO());
    renderForwardPass(pCamera);
}

void BBDrawCall::renderLightSpaceFBOPass(BBCamera *pCamera)
{
    QList<BBGameObject*> lights = collectLights();
    BB_PROCESS_ERROR_RETURN(lights.count() > 0);

    m_pVBO->bind();
    BBRenderPass *pBaseRenderPass = m_pMaterial->getBaseRenderPass();
    pBaseRenderPass->setCullState(true);
    pBaseRenderPass->setCullFace(GL_FRONT);

    // Only directional light is considered for the time
    BBDirectionalLight *pLight = (BBDirectionalLight*)lights[0];
    pLight->setRenderPass(pBaseRenderPass);

    BBCamera *pLightSpaceCamera = pLight->getLightSpaceCamera();
    pBaseRenderPass->bind(pLightSpaceCamera);
    if (m_pEBO == nullptr)
    {
        m_pVBO->draw(m_eDrawPrimitiveType, m_nDrawStartIndex, m_nDrawCount);
    }
    else
    {
        m_pEBO->bind();
        m_pEBO->draw(m_eDrawPrimitiveType, m_nIndexCount, m_nDrawStartIndex);
        m_pEBO->unbind();
    }

    pBaseRenderPass->setCullState(false);
    pBaseRenderPass->setCullFace(GL_BACK);
    pBaseRenderPass->unbind();
    BB_SAFE_DELETE(pLightSpaceCamera);
    m_pVBO->unbind();
}

void BBDrawCall::updateOrderInOpaqueRenderQueue()
{
    BBRenderQueue *pRenderQueue = BBSceneManager::getRenderQueue();
    pRenderQueue->updateOpaqueDrawCallOrder(this);
}

void BBDrawCall::updateOrderInTransparentRenderQueue()
{
    BBRenderQueue *pRenderQueue = BBSceneManager::getRenderQueue();
    pRenderQueue->updateTransparentDrawCallOrder(this);
}

QList<BBGameObject*> BBDrawCall::collectLights()
{
    return BBSceneManager::getScene()->getLights();
}


