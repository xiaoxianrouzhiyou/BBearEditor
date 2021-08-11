#include "BBGlobalSettingsGroupManager.h"
#include "Scene/BBScene.h"
#include "RayTracing/BBRayTracker.h"
#include "../BBPropertyFactory.h"
#include "Render/BBDrawCall.h"
#include <QCheckBox>
#include "Lighting/BBSphericalHarmonicLighting.h"
#include "Scene/BBSceneManager.h"


int BBGlobalSettingsGroupManager::m_nCurrentRenderingAlgorithmIndex = 0;

BBGlobalSettingsGroupManager::BBGlobalSettingsGroupManager(BBScene *pScene, QWidget *pParent)
    : BBGroupManager("Global Settings", BB_PATH_RESOURCE_ICON(earth.png), pParent)
{
    m_pScene = pScene;
    initRenderingAlgorithmFactory();
    initRayTracingFactory();
    initSphericalHarmonicLightingFactory();
    initGlobalIlluminationFactory();
}

BBGlobalSettingsGroupManager::~BBGlobalSettingsGroupManager()
{
    BB_SAFE_DELETE(m_pRenderingAlgorithmFactory);
    BB_SAFE_DELETE(m_pTriggerRayTracing);
    BB_SAFE_DELETE(m_pSphericalHarmonicLightingFactory);
    BB_SAFE_DELETE(m_pGlobalIlluminationFactory);
}

void BBGlobalSettingsGroupManager::changeCurrentRenderingAlgorithm(int nIndex)
{
    // 0 Forward Rendering
    // 1 Deferred Rendering
    BBDrawCall::switchRenderingSettings(nIndex);
    m_nCurrentRenderingAlgorithmIndex = nIndex;
}

void BBGlobalSettingsGroupManager::switchRayTracing(bool bEnable)
{
    if (bEnable)
    {
        m_pScene->getRayTracker()->open();
    }
    else
    {
        m_pScene->getRayTracker()->close();
    }
}

void BBGlobalSettingsGroupManager::bakeSphericalHarmonicLightingMap()
{
    BBSphericalHarmonicLighting::computeLightingData(m_pSphericalHarmonicLightingFactory->getCurrentItemIndex());
    emit updateFileList();
}

void BBGlobalSettingsGroupManager::switchGlobalIllumination(bool bEnable)
{
    BBSceneManager::enableGlobalIllumination(bEnable);
}

void BBGlobalSettingsGroupManager::initRenderingAlgorithmFactory()
{
    QStringList items;
    items.append("Forward Rendering");
    items.append("Deferred Rendering");
    m_pRenderingAlgorithmFactory = new BBEnumFactory("Rendering Algorithm", items, items[m_nCurrentRenderingAlgorithmIndex], this, 1, 1);
    QObject::connect(m_pRenderingAlgorithmFactory, SIGNAL(currentItemChanged(int)),
                     this, SLOT(changeCurrentRenderingAlgorithm(int)));
    addFactory(m_pRenderingAlgorithmFactory);
}

void BBGlobalSettingsGroupManager::initRayTracingFactory()
{
    m_pTriggerRayTracing = new QCheckBox(this);
    QObject::connect(m_pTriggerRayTracing, SIGNAL(clicked(bool)), this, SLOT(switchRayTracing(bool)));
    addFactory("Ray Tracing", m_pTriggerRayTracing);
}

void BBGlobalSettingsGroupManager::initSphericalHarmonicLightingFactory()
{
    QStringList sphericalHarmonicLightingAlgorithmName = {"The Gritty Details",
                                                          "Zonal Harmonics"};
    m_pSphericalHarmonicLightingFactory = new BBEnumExpansionFactory("Spherical Harmonic Lighting", sphericalHarmonicLightingAlgorithmName,
                                                                     "Bake", "Zonal Harmonics", this, 1, 1);
    m_pSphericalHarmonicLightingFactory->enableTrigger(false);
    QObject::connect(m_pSphericalHarmonicLightingFactory, SIGNAL(buttonClicked()), this, SLOT(bakeSphericalHarmonicLightingMap()));
    addFactory(m_pSphericalHarmonicLightingFactory);
}

void BBGlobalSettingsGroupManager::initGlobalIlluminationFactory()
{
    QStringList globalIlluminationAlgorithmName = {"SSAO",
                                                   "SSAO + SSDO"};
    m_pGlobalIlluminationFactory = new BBEnumExpansionFactory("Global Illumination", globalIlluminationAlgorithmName,
                                                              "", "SSAO + SSDO", this, 1, 1);
    m_pGlobalIlluminationFactory->enableButton(false);
    QObject::connect(m_pGlobalIlluminationFactory, SIGNAL(triggerClicked(bool)), this, SLOT(switchGlobalIllumination(bool)));
    addFactory(m_pGlobalIlluminationFactory);
}
