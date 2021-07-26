#ifndef BBGROUPMANAGER_H
#define BBGROUPMANAGER_H


#include <QWidget>


class BBLineEditFactory;
class QToolButton;
class QPushButton;
class QMenu;
class BBGameObject;
class BBVector3DFactory;
class BBScene;
class BBEnumFactory;
class BBPreviewOpenGLWidget;
class BBMaterial;
class BBRenderableObject;
class BBPointLight;
class QCheckBox;
class BBDragAcceptedFactory;


// Manage a group of property
class BBGroupManager : public QWidget
{
    Q_OBJECT
public:
    explicit BBGroupManager(const QString &groupName, const QString &iconPath, QWidget *pParent = nullptr);
    ~BBGroupManager();

    QWidget* addFactory(const QString &name, QWidget *pFactory, int nStretch = 1, const Qt::Alignment &alignment = Qt::Alignment());
    QWidget* addFactory(const QString &name, QWidget *pFactory1, QWidget *pFactory2, int nStretch = 1);
    QWidget* addFactory(QWidget *pFactory);
    BBLineEditFactory* addFactory(const QString &name, float fValue);

    void addMargin(int nHeight);

public slots:
    void setContainerExpanded(bool bExpanded);

protected:
    QToolButton *m_pMainButton;
    QPushButton *m_pMenuButton;
    QMenu *m_pMenu;
    QWidget *m_pContainer;
};


enum BBReferenceSystem
{
    Global = 0,
    Local = 1
};


class BBTransformGroupManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBTransformGroupManager(BBGameObject *pGameObject, QWidget *pParent = 0);
    ~BBTransformGroupManager();

    void updatePositionValue();
    void updateRotationValue();
    void updateScaleValue();

private slots:
    void changePosition(const QVector3D &value);
    void changeRotation(const QVector3D &value);
    void changeScale(const QVector3D &value);
    void popMenu();
    void showGlobalCoordinate();
    void showLocalCoordinate();

signals:
    void coordinateSystemUpdated();

private:
    BBVector3DFactory *m_pPositionFactory;
    BBVector3DFactory *m_pRotationFactory;
    BBVector3DFactory *m_pScaleFactory;
    BBGameObject *m_pCurrentGameObject;
    BBReferenceSystem m_eReferenceSystem;
};


class BBGlobalSettingsGroupManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBGlobalSettingsGroupManager(BBScene *pScene, QWidget *pParent = 0);
    ~BBGlobalSettingsGroupManager();

private slots:
    void changeCurrentRenderingAlgorithm(int nIndex);
    void switchRayTracing(bool bEnable);

private:
    void initRenderingAlgorithmEnumFactory();

    static int m_nCurrentRenderingAlgorithmIndex;
    BBScene *m_pScene;
    BBEnumFactory *m_pRenderingAlgorithmEnumFactory;
    QCheckBox *m_pTriggerRayTracing;
};


class BBMaterialPropertyGroupManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBMaterialPropertyGroupManager(BBMaterial *pMaterial, BBPreviewOpenGLWidget *pPreviewOpenGLWidget, QWidget *pParent = 0);
    ~BBMaterialPropertyGroupManager();

private slots:
    void setSampler2D(const QString &uniformName, const QString &texturePath);
    void setFloat(const QString &uniformName, float fValue);

private:
    void addColorItem();
    void addBlendStateItem();
    void addBlendFuncItem();
    void addPropertyItems();

private slots:
    void enableBlendState(bool bEnable);
    void switchSRCBlendFunc(int nIndex);
    void switchDSTBlendFunc(int nIndex);

private:
    unsigned int getBlendFunc(int nIndex);

private:
    BBMaterial *m_pMaterial;
    BBPreviewOpenGLWidget *m_pPreviewOpenGLWidget;
};


class BBRenderManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBRenderManager(BBRenderableObject *pObject, QWidget *pParent = 0);
    ~BBRenderManager();

private slots:
    void changeMaterial(const QString &filePath);
    void popupResourceDialog();

private:
    BBRenderableObject *m_pRenderableObject;
    BBDragAcceptedFactory *m_pMaterialFactory;
};


class BBPointLightManager : public BBGroupManager
{
    Q_OBJECT

public:
    BBPointLightManager(BBPointLight *pLight, QWidget *pParent = 0);
    ~BBPointLightManager();

protected slots:
    void setRadius(float fRadius);
    void setConstantFactor(float fValue);
    void setLinearFactor(float fValue);
    void setQuadricFactor(float fValue);

private:
    BBPointLight *m_pPointLight;
};


#endif // BBGROUPMANAGER_H
