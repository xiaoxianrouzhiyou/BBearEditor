#ifndef BBPHOTONMAP_H
#define BBPHOTONMAP_H


#include <QVector3D>
#include "Utils/BBUtils.h"

struct BBPhoton
{
    QVector3D m_Position;
    // Incident
    QVector3D m_Direction;
    // Color
    QVector3D m_Power;
    int m_Axis;
};

struct BBNearestPhotons
{
    // Detection point position
    QVector3D m_DetectionPosition;
    // Maximum number of photons to find
    int m_nMaxPhotonCount;
    // Number of photons found
    int m_nCurrentCount;
    // Is the maximum heap full, m_nMaxPhotonCount == m_nCurrentCount
    bool m_bFulled;
    // Store the square of the distance in the photon maximum stack between photon and the detection point to avoid repeated calculation each time
    // [0] store the maximum
    float *m_pDistanceSquare;
    // Photons found
    BBPhoton **m_ppPhotons;

    BBNearestPhotons()
    {
        m_nMaxPhotonCount = 0;
        m_nCurrentCount = 0;
        m_bFulled = false;
        m_pDistanceSquare = nullptr;
        m_ppPhotons = nullptr;
    }

    ~BBNearestPhotons()
    {
        BB_SAFE_DELETE_ARRAY(m_pDistanceSquare);
        BB_SAFE_DELETE_ARRAY(m_ppPhotons);
    }
};

class BBPhotonMap
{
public:
    BBPhotonMap(int nMaxPhotonNum = 10000);
    ~BBPhotonMap();

    BBPhotonMap operator=(const BBPhotonMap &photonMap);

    void store(const BBPhoton &photon);
    void balance();
    void getKNearestPhotons(BBNearestPhotons *pNearestPhotons, int nPhotonIndex);

    void debug();

    QVector3D* getPhotonPositions();
    int getPhotonNum() const { return m_nPhotonNum; }
    int getMaxPhotonNum() const { return m_nMaxPhotonNum; }
    BBPhoton* getPhoton() const { return m_pPhoton; }
    QVector3D getBoxMin() const { return m_BoxMin; }
    QVector3D getBoxMax() const { return m_BoxMax; }

private:
    void splitMedian(BBPhoton pPhoton[], int start, int end, int median, int axis);
    void balanceSegment(BBPhoton pPhoton[], int index, int start, int end);

    int m_nPhotonNum;
    int m_nMaxPhotonNum;
    BBPhoton *m_pPhoton;
    QVector3D m_BoxMin;
    QVector3D m_BoxMax;
};

#endif // BBPHOTONMAP_H
