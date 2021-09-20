#include "BBBoundingBox.h"
#include "Utils/BBUtils.h"
#include <cfloat>
#include "Render/BufferObject/BBVertexBufferObject.h"
#include "Render/BBMaterial.h"
#include "Render/BBDrawCall.h"
#include <Eigen/Eigen>
#include "Render/BBCamera.h"


using namespace Eigen;


/**
 * @brief BBBoundingBox::BBBoundingBox
 */
BBBoundingBox::BBBoundingBox()
    : BBBoundingBox(0, 0, 0, 0, 0, 0, 1, 1, 1)
{

}

BBBoundingBox::BBBoundingBox(float px, float py, float pz,
                             float rx, float ry, float rz,
                             float sx, float sy, float sz)
    : BBRenderableObject(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_nBoxVertexCount = 0;
    m_pOriginalBoxVertexes = NULL;
    m_pTransformedBoxVertexes = NULL;
    m_DefaultColor = QVector3D(0.909804f, 0.337255f, 0.333333f);
}

BBBoundingBox::~BBBoundingBox()
{
    BB_SAFE_DELETE_ARRAY(m_pOriginalBoxVertexes);
    BB_SAFE_DELETE_ARRAY(m_pTransformedBoxVertexes);
}

bool BBBoundingBox::hit(const BBRay &ray, float &fDistance)
{
    Q_UNUSED(ray);
    Q_UNUSED(fDistance);
    return m_bActive;
}

QVector3D BBBoundingBox::getCenter()
{
    return QVector3D(m_Center[0], m_Center[1], m_Center[2]);
}

void BBBoundingBox::setModelMatrix(float px, float py, float pz,
                                   const QQuaternion &r,
                                   float sx, float sy, float sz)
{
    // When the corresponding model is transformed, invoke
    BBGameObject::setModelMatrix(px, py, pz, r, sx, sy, sz);
    for (int i = 0; i < m_nBoxVertexCount; i++)
    {
        m_pTransformedBoxVertexes[i] = m_ModelMatrix * m_pOriginalBoxVertexes[i];
    }
}


/**
 * @brief BBRectBoundingBox3D::BBRectBoundingBox3D
 * @param fCenterX
 * @param fCenterY
 * @param fCenterZ
 * @param fHalfLengthX
 * @param fHalfLengthY
 * @param fHalfLengthZ
 */
BBRectBoundingBox3D::BBRectBoundingBox3D(float fCenterX, float fCenterY, float fCenterZ,
                                         float fHalfLengthX, float fHalfLengthY, float fHalfLengthZ)
    : BBBoundingBox()
{
    m_nBoxVertexCount = 4;
    m_pOriginalBoxVertexes = new QVector3D[m_nBoxVertexCount];
    m_pTransformedBoxVertexes = new QVector3D[m_nBoxVertexCount];

    m_Center[0] = fCenterX;
    m_Center[1] = fCenterY;
    m_Center[2] = fCenterZ;
    if (fHalfLengthX == 0)
    {
        m_pOriginalBoxVertexes[0] = QVector3D(fCenterX, fCenterY + fHalfLengthY, fCenterZ + fHalfLengthZ);
        m_pOriginalBoxVertexes[1] = QVector3D(fCenterX, fCenterY - fHalfLengthY, fCenterZ + fHalfLengthZ);
        m_pOriginalBoxVertexes[2] = QVector3D(fCenterX, fCenterY - fHalfLengthY, fCenterZ - fHalfLengthZ);
        m_pOriginalBoxVertexes[3] = QVector3D(fCenterX, fCenterY + fHalfLengthY, fCenterZ - fHalfLengthZ);
    }
    else if (fHalfLengthY == 0)
    {
        m_pOriginalBoxVertexes[0] = QVector3D(fCenterX + fHalfLengthX, fCenterY, fCenterZ + fHalfLengthZ);
        m_pOriginalBoxVertexes[1] = QVector3D(fCenterX - fHalfLengthX, fCenterY, fCenterZ + fHalfLengthZ);
        m_pOriginalBoxVertexes[2] = QVector3D(fCenterX - fHalfLengthX, fCenterY, fCenterZ - fHalfLengthZ);
        m_pOriginalBoxVertexes[3] = QVector3D(fCenterX + fHalfLengthX, fCenterY, fCenterZ - fHalfLengthZ);
    }
    else if (fHalfLengthZ == 0)
    {
        m_pOriginalBoxVertexes[0] = QVector3D(fCenterX + fHalfLengthX, fCenterY + fHalfLengthY, fCenterZ);
        m_pOriginalBoxVertexes[1] = QVector3D(fCenterX - fHalfLengthX, fCenterY + fHalfLengthY, fCenterZ);
        m_pOriginalBoxVertexes[2] = QVector3D(fCenterX - fHalfLengthX, fCenterY - fHalfLengthY, fCenterZ);
        m_pOriginalBoxVertexes[3] = QVector3D(fCenterX + fHalfLengthX, fCenterY - fHalfLengthY, fCenterZ);
    }
}

BBRectBoundingBox3D::~BBRectBoundingBox3D()
{

}

void BBRectBoundingBox3D::init()
{
    m_pVBO = new BBVertexBufferObject(m_nBoxVertexCount);
    for (int i = 0; i < m_nBoxVertexCount; i++)
    {
        m_pVBO->setPosition(i, m_pOriginalBoxVertexes[i].x(),
                               m_pOriginalBoxVertexes[i].y(),
                               m_pOriginalBoxVertexes[i].z());
        m_pVBO->setColor(i, m_DefaultColor);
    }

    m_nIndexCount = 8;
    unsigned short indexes[] = {0, 1, 1, 2, 2, 3, 3, 0};
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < m_nIndexCount; i++)
    {
        m_pIndexes[i] = indexes[i];
    }

    m_pCurrentMaterial->init("UI",
                             BB_PATH_RESOURCE_SHADER(UI.vert),
                             BB_PATH_RESOURCE_SHADER(UI.frag));
    m_pCurrentMaterial->setVector4(LOCATION_TEXTURE_SETTING0, 0.0f, 0.0f, 0.0f, 0.0f);
    m_pCurrentMaterial->setVector4(LOCATION_CAMERA_PARAMETERS, 800.0f, 600.0f, 0.0f, 0.0f);

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_LINES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

bool BBRectBoundingBox3D::hit(const BBRay &ray, float &fDistance)
{
    // If it is not activated, no collision occurs
    if (!BBBoundingBox::hit(ray, fDistance))
        return false;

    QVector3D intersection;
    bool result = false;
    fDistance = FLT_MAX;

    if (ray.computeIntersectWithRectangle(m_pTransformedBoxVertexes[0],
                                          m_pTransformedBoxVertexes[1],
                                          m_pTransformedBoxVertexes[2],
                                          m_pTransformedBoxVertexes[3], intersection))
    {
        float tmp = ray.computeIntersectDistance(intersection);
        if (tmp >= 0.0f)
        {
            result = true;
            fDistance = tmp;
        }
    }

    return result;
}


/**
 * @brief BBTriangleBoundingBox3D::BBTriangleBoundingBox3D
 * @param point1
 * @param point2
 * @param point3
 */
BBTriangleBoundingBox3D::BBTriangleBoundingBox3D(const QVector3D &point1,
                                                 const QVector3D &point2,
                                                 const QVector3D &point3)
    : BBBoundingBox()
{
    m_nBoxVertexCount = 3;
    m_pOriginalBoxVertexes = new QVector3D[m_nBoxVertexCount];
    m_pTransformedBoxVertexes = new QVector3D[m_nBoxVertexCount];

    m_pOriginalBoxVertexes[0] = point1;
    m_pOriginalBoxVertexes[1] = point2;
    m_pOriginalBoxVertexes[2] = point3;
}

bool BBTriangleBoundingBox3D::hit(const BBRay &ray, float &fDistance)
{
    // If it is not activated, no collision occurs
    if (!BBBoundingBox::hit(ray, fDistance))
        return false;

    QVector3D intersection;
    bool result = false;
    fDistance = FLT_MAX;

    if (ray.computeIntersectWithTriangle(m_pTransformedBoxVertexes[0],
                                         m_pTransformedBoxVertexes[1],
                                         m_pTransformedBoxVertexes[2], intersection))
    {
        float tmp = ray.computeIntersectDistance(intersection);
        if (tmp >= 0.0f)
        {
            result = true;
            fDistance = tmp;
        }
    }
    return result;
}


/**
 * @brief BBQuarterCircleBoundingBox3D::BBQuarterCircleBoundingBox3D
 * @param fCenterX
 * @param fCenterY
 * @param fCenterZ
 * @param fRadius
 * @param ePlaneName
 */
BBQuarterCircleBoundingBox3D::BBQuarterCircleBoundingBox3D(float fCenterX, float fCenterY, float fCenterZ,
                                                           float fRadius, const BBPlaneName &ePlaneName)
    : BBBoundingBox()
{
    m_nBoxVertexCount = 1;
    m_pOriginalBoxVertexes = new QVector3D[m_nBoxVertexCount];
    m_pTransformedBoxVertexes = new QVector3D[m_nBoxVertexCount];

    m_Center[0] = fCenterX;
    m_Center[1] = fCenterY;
    m_Center[2] = fCenterZ;

    m_eSelectedPlaneName = ePlaneName;

    m_QuadrantFlag = QVector3D(0, 0, 0);

    if (ePlaneName == BBPlaneName::YOZ)
    {
        m_pOriginalBoxVertexes[0] = QVector3D(m_Center[0], m_Center[1], m_Center[2]) + QVector3D(0, fRadius, 0);
    }
    else // XOZ or XOY
    {
        m_pOriginalBoxVertexes[0] = QVector3D(m_Center[0], m_Center[1], m_Center[2]) + QVector3D(fRadius, 0, 0);
    }
}

bool BBQuarterCircleBoundingBox3D::hit(const BBRay &ray, float &fDistance)
{
    // If it is not activated, no collision occurs
    if (!BBBoundingBox::hit(ray, fDistance))
        return false;

    QVector3D intersection;
    bool result = false;
    fDistance = FLT_MAX;

    if (ray.computeIntersectWithQuarterCircle(m_Position,
                                              (m_pTransformedBoxVertexes[0] - m_Position).length(),
                                              m_eSelectedPlaneName,
                                              intersection,
                                              m_QuadrantFlag))
    {
        float tmp = ray.computeIntersectDistance(intersection);
        if (tmp >= 0.0f)
        {
            result = true;
            fDistance = tmp;
        }
    }
    return result;
}


/**
 * @brief BBBoundingBox3D::BBBoundingBox3D
 * @param vertexes
 */
BBBoundingBox3D::BBBoundingBox3D(const QList<QVector4D> &vertexes)
    : BBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, vertexes)
{

}

BBBoundingBox3D::BBBoundingBox3D(float px, float py, float pz,
                                 float rx, float ry, float rz,
                                 float sx, float sy, float sz,
                                 const QList<QVector4D> &vertexes)
    : BBBoundingBox(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_nBoxVertexCount = 8;
    m_pOriginalBoxVertexes = new QVector3D[m_nBoxVertexCount];
    m_pTransformedBoxVertexes = new QVector3D[m_nBoxVertexCount];

    m_DefaultColor = QVector3D(0.909804f, 0.337255f, 0.333333f);
    m_Center[0] = 0;
    m_Center[1] = 0;
    m_Center[2] = 0;
    m_HalfLength[0] = 0.5f;
    m_HalfLength[1] = 0.5f;
    m_HalfLength[2] = 0.5f;
    m_Axis[0][0] = 1;
    m_Axis[0][1] = 0;
    m_Axis[0][2] = 0;
    m_Axis[1][0] = 0;
    m_Axis[1][1] = 1;
    m_Axis[1][2] = 0;
    m_Axis[2][0] = 0;
    m_Axis[2][1] = 0;
    m_Axis[2][2] = 1;

    computeBoxVertexes(vertexes);
}

BBBoundingBox3D::BBBoundingBox3D(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale,
                                 const QVector3D &center, const QVector3D &halfLength)
    : BBBoundingBox3D(position.x(), position.y(), position.z(),
                      rotation.x(), rotation.y(), rotation.z(),
                      scale.x(), scale.y(), scale.z(),
                      center.x(), center.y(), center.z(),
                      halfLength.x(), halfLength.y(), halfLength.z())
{

}

BBBoundingBox3D::BBBoundingBox3D(float px, float py, float pz,
                                 float rx, float ry, float rz,
                                 float sx, float sy, float sz,
                                 float fCenterX, float fCenterY, float fCenterZ,
                                 float fHalfLengthX, float fHalfLengthY, float fHalfLengthZ)
    : BBBoundingBox(px, py, pz, rx, ry, rz, sx, sy, sz)
{
    m_nBoxVertexCount = 8;
    m_pOriginalBoxVertexes = new QVector3D[m_nBoxVertexCount];
    m_pTransformedBoxVertexes = new QVector3D[m_nBoxVertexCount];

    m_Center[0] = fCenterX;
    m_Center[1] = fCenterY;
    m_Center[2] = fCenterZ;
    m_HalfLength[0] = fHalfLengthX;
    m_HalfLength[1] = fHalfLengthY;
    m_HalfLength[2] = fHalfLengthZ;
    m_Axis[0][0] = 1;
    m_Axis[0][1] = 0;
    m_Axis[0][2] = 0;
    m_Axis[1][0] = 0;
    m_Axis[1][1] = 1;
    m_Axis[1][2] = 0;
    m_Axis[2][0] = 0;
    m_Axis[2][1] = 0;
    m_Axis[2][2] = 1;

    QList<QVector4D> vertexes;
    computeBoxVertexes(vertexes);
}

BBBoundingBox3D::~BBBoundingBox3D()
{

}

void BBBoundingBox3D::init()
{
    m_pVBO = new BBVertexBufferObject(8);
    for (int i = 0; i < 8; i++)
    {
        m_pVBO->setPosition(i, m_pOriginalBoxVertexes[i].x(),
                               m_pOriginalBoxVertexes[i].y(),
                               m_pOriginalBoxVertexes[i].z());
        m_pVBO->setColor(i, m_DefaultColor);
    }

    m_nIndexCount = 24;
    unsigned short indexes[] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
    m_pIndexes = new unsigned short[m_nIndexCount];
    for (int i = 0; i < m_nIndexCount; i++)
    {
        m_pIndexes[i] = indexes[i];
    }

    m_pCurrentMaterial->init("base",
                             BB_PATH_RESOURCE_SHADER(base.vert),
                             BB_PATH_RESOURCE_SHADER(base.frag));

    BBRenderableObject::init();

    BBDrawCall *pDrawCall = new BBDrawCall;
    pDrawCall->setMaterial(m_pCurrentMaterial);
    pDrawCall->setVBO(m_pVBO);
    pDrawCall->setEBO(m_pEBO, GL_LINES, m_nIndexCount, 0);
    appendDrawCall(pDrawCall);
}

bool BBBoundingBox3D::hit(const BBRay &ray, float &fDistance)
{
    // If it is not activated, no collision occurs
    if (!BBBoundingBox::hit(ray, fDistance))
        return false;

    QVector3D intersection;
    bool result = false;
    fDistance = FLT_MAX;
    // Calculate whether to hit the 6 faces of the bounding box
    static unsigned int indexes[] = {0, 1, 2, 3, 4, 5, 6, 7,
                                     1, 2, 6, 5, 0, 3, 7, 4,
                                     0, 1, 5, 4, 3, 2, 6, 7};
    for (int i = 0; i < 24; i += 4)
    {
        if (ray.computeIntersectWithRectangle(m_pTransformedBoxVertexes[indexes[i]],
                                              m_pTransformedBoxVertexes[indexes[i + 1]],
                                              m_pTransformedBoxVertexes[indexes[i + 2]],
                                              m_pTransformedBoxVertexes[indexes[i + 3]], intersection))
        {
            float temp = ray.computeIntersectDistance(intersection);
            if (temp >= 0.0f)
            {
                if (temp < fDistance)
                    fDistance = temp;
                result = true;
            }
        }
    }
    return result;
}

bool BBBoundingBox3D::belongToSelectionRegion(const BBFrustum &frustum)
{
    for (int i = 0; i < m_nBoxVertexCount; i++)
    {
        BB_PROCESS_ERROR_RETURN_FALSE(frustum.contain(m_pTransformedBoxVertexes[i]));
    }
    return true;
}

QVector3D BBBoundingBox3D::getHalfLength()
{
    return QVector3D(m_HalfLength[0], m_HalfLength[1], m_HalfLength[2]);
}

void BBBoundingBox3D::computeBoxVertexes(const QList<QVector4D> &vertexes)
{
    Q_UNUSED(vertexes);
    FloatData temp[8];
    int sign[8][3] = {{1, 1, 1},
                      {-1, 1, 1},
                      {-1, -1, 1},
                      {1, -1, 1},
                      {1, 1, -1},
                      {-1, 1, -1},
                      {-1, -1, -1},
                      {1, -1, -1}};
    for (int index = 0; index < 8; index++)
    {
        for (int i = 0; i < 3; i++)
        {
            temp[index].v[i] = m_Center[i];
            for (int j = 0; j < 3; j++)
            {
                temp[index].v[i] += m_Axis[j][i] * m_HalfLength[j] * sign[index][j];
            }
        }
        m_pOriginalBoxVertexes[index] = QVector3D(temp[index].v[0], temp[index].v[1], temp[index].v[2]);
        m_pTransformedBoxVertexes[index] = m_ModelMatrix * m_pOriginalBoxVertexes[index];
    }
}


/**
 * @brief BBAABBBoundingBox3D::BBAABBBoundingBox3D
 * @param vertexes
 */
BBAABBBoundingBox3D::BBAABBBoundingBox3D(const QList<QVector4D> &vertexes)
    : BBAABBBoundingBox3D(0, 0, 0, 0, 0, 0, 1, 1, 1, vertexes)
{

}

BBAABBBoundingBox3D::BBAABBBoundingBox3D(float px, float py, float pz,
                                         float rx, float ry, float rz,
                                         float sx, float sy, float sz,
                                         const QList<QVector4D> &vertexes)
    : BBBoundingBox3D(px, py, pz, rx, ry, rz, sx, sy, sz, vertexes)
{
    m_HalfLength[0] = 0;
    m_HalfLength[1] = 0;
    m_HalfLength[2] = 0;
    computeBoxVertexes(vertexes);
}

BBAABBBoundingBox3D::BBAABBBoundingBox3D(const QVector3D &position, const QVector3D &rotation, const QVector3D &scale,
                                         const QVector3D &center, const QVector3D &halfLength)
    : BBBoundingBox3D(position, rotation, scale, center, halfLength)
{

}

bool BBAABBBoundingBox3D::computeIntersectWithPlane(const QVector3D &point, const QVector3D &normal)
{
    // The vertex closest to the plane
    QVector3D p = getMin();
    if (normal.x() >= 0)
        p.setX(getMax().x());
    if (normal.y() >= 0)
        p.setY(getMax().y());
    if (normal.z() >= 0)
        p.setZ(getMax().z());
    // The vertex of the farthest diagonal
    QVector3D n = getMax();
    if (normal.x() >= 0)
        n.setX(getMin().x());
    if (normal.y() >= 0)
        n.setY(getMin().y());
    if (normal.z() >= 0)
        n.setZ(getMin().z());

    if (p.distanceToPlane(point, normal) < 0)
        return false;

    if (n.distanceToPlane(point, normal) < 0)
        return true;

    return false;
}

void BBAABBBoundingBox3D::computeBoxVertexes(const QList<QVector4D> &vertexes)
{
    int nVertexCount = vertexes.count();

    // init matrix
    MatrixXf m(3, nVertexCount);
    for (int i = 0; i < nVertexCount; i++)
    {
        m(0, i) = vertexes.at(i).x();
        m(1, i) = vertexes.at(i).y();
        m(2, i) = vertexes.at(i).z();
    }
    // Find the mean value of x y z, center
    VectorXf avg = m.rowwise().mean();
    // compute the distance from each point to the center
    m.colwise() -= avg;
    for (int i = 0; i < 3; i++)
    {
        m_Center[i] = avg[i];
        for (int j = 0; j < nVertexCount; j++)
        {
            m_HalfLength[i] = std::max(m_HalfLength[i], std::fabs(m(i, j)));
        }
    }

    BBBoundingBox3D::computeBoxVertexes(vertexes);
}

QVector3D BBAABBBoundingBox3D::getMax()
{
    return m_pTransformedBoxVertexes[0];
}

QVector3D BBAABBBoundingBox3D::getMin()
{
    return m_pTransformedBoxVertexes[6];
}




//void OBBBoundingBox3D::computeBoundingBox()
//{
//    int vertexCount = mVertexes.count();

//    //初始化矩阵
//    MatrixXf input(3, vertexCount);
//    MatrixXf m(3, vertexCount);
//    for (int i = 0; i < vertexCount; i++)
//    {
//        m(0, i) = mVertexes.at(i).x();
//        m(1, i) = mVertexes.at(i).y();
//        m(2, i) = mVertexes.at(i).z();
//    }
//    input = m;
//    //cout << input << endl << endl;
//    //求取x y z均值
//    VectorXf avg = m.rowwise().mean();
//    m.colwise() -= avg;
//    //计算协方差矩阵 adjoint转置
//    MatrixXf covMat = (m * m.adjoint()) / float(vertexCount - 1);
//    //cout << covMat << endl << endl;
//    //求特征向量和特征值
//    SelfAdjointEigenSolver<MatrixXf> eigen(covMat);
//    MatrixXf eigenvectors = eigen.eigenvectors();
//    //cout << eigenvectors << endl << endl;
//    //将各点的（X，Y，Z）坐标投影到计算出的坐标轴上 avg由累加所有点再求均值得到 求出center和半长度
//    VectorXf minExtents(3);
//    VectorXf maxExtents(3);
//    minExtents << FLT_MAX, FLT_MAX, FLT_MAX;
//    maxExtents << -FLT_MAX, -FLT_MAX, -FLT_MAX;
//    for (int i = 0; i < vertexCount; i++)
//    {
//        VectorXf vertex = input.col(i);
//        VectorXf displacement = vertex - avg;
//        for (int j = 0; j < 3; j++)
//        {
//            minExtents[j] = min(minExtents[j], displacement.dot(eigenvectors.col(j)));
//            maxExtents[j] = max(maxExtents[j], displacement.dot(eigenvectors.col(j)));
//        }
//    }
//    VectorXf offset = (maxExtents - minExtents) / 2.0f + minExtents;
//    for (int i = 0; i < 3; i++)
//    {
//        avg += eigenvectors.col(i) * offset[i];
//    }

//    for (int i = 0; i < 3; i++)
//    {
//        center[i] = avg[i];
//        radius[i] = (maxExtents[i] - minExtents[i]) / 2.0f;
//        for (int j = 0; j < 3; j++)
//            axis[i][j] = eigenvectors.col(i)[j];
//    }
//    /*qDebug() << center[0] << center[1] << center[2];
//    qDebug() << radius[0] << radius[1] << radius[2];
//    qDebug() << axis[0][0] << axis[0][1] << axis[0][2];
//    qDebug() << axis[1][0] << axis[1][1] << axis[1][2];
//    qDebug() << axis[2][0] << axis[2][1] << axis[2][2];
//    qDebug() << endl;*/
//    getBoundingBoxVertexes();
//}
