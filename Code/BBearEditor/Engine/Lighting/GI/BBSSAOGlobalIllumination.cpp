#include "BBSSAOGlobalIllumination.h"
#include "BBGlobalIllumination.h"
#include "Scene/BBScene.h"
#include "2D/BBFullScreenQuad.h"
#include "Render/BBMaterial.h"
#include "Render/Texture/BBTexture.h"
#include "Math/BBMath.h"


void BBSSAOGlobalIllumination::open(BBScene *pScene)
{
    BBGlobalIllumination::setGBufferPass(pScene);
    setSSAOPass(pScene);
    setSSAOBlurPass(pScene);
}

void BBSSAOGlobalIllumination::setSSAOPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(0);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_SSAO", BB_PATH_RESOURCE_SHADER(GI/FullScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI/SSAO.frag));

    pMaterial->setSampler2D("NormalTex", pScene->getColorFBO(0, 1));
    pMaterial->setSampler2D("PositionTex", pScene->getColorFBO(0, 2));
    pMaterial->setSampler2D("NoiseTex", BBTexture().createTexture2D(generateNoise(), 4, 4, GL_RGBA32F));
    pMaterial->setVector4Array("Samples[0]", generateKernel(), 64);

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}

void BBSSAOGlobalIllumination::setSSAOBlurPass(BBScene *pScene)
{
    BBFullScreenQuad *pFullScreenQuad = pScene->getFullScreenQuad(1);
    BBMaterial *pMaterial = new BBMaterial();
    pMaterial->init("GI_SSAO_Blur", BB_PATH_RESOURCE_SHADER(GI/FullScreenQuad.vert), BB_PATH_RESOURCE_SHADER(GI/SSAO_Blur.frag));

    pMaterial->setSampler2D("SSAOTex", pScene->getColorFBO(1, 0));
    pMaterial->setSampler2D("AlbedoTex", pScene->getColorFBO(0, 0));

    pFullScreenQuad->setCurrentMaterial(pMaterial);
}

float* BBSSAOGlobalIllumination::generateKernel()
{
    // range : 0.0~1.0
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    float *pKernel = new float[64 * 4];;
    for (unsigned int i = 0; i < 64; i++)
    {
        QVector3D sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
        sample.normalize();
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        pKernel[i * 4] = sample.x();
        pKernel[i * 4 + 1] = sample.y();
        pKernel[i * 4 + 2] = sample.z();
        pKernel[i * 4 + 3] = 1.0f;
    }
    return pKernel;
}

float* BBSSAOGlobalIllumination::generateNoise()
{
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;
    float *pNoise = new float[16 * 4];
    for (unsigned int i = 0; i < 16; i++)
    {
        // rotate around z-axis (in tangent space)
        pNoise[i * 4] = randomFloats(generator) * 2.0f - 1.0f;
        pNoise[i * 4 + 1] = randomFloats(generator) * 2.0f - 1.0f;
        pNoise[i * 4 + 2] = 0.0f;
        pNoise[i * 4 + 3] = 1.0f;
    }
    return pNoise;
}
