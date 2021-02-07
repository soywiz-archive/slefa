/*!
 * \file
 * Scanline edge-flag algorithm for antialiasing <br>
 * Copyright (c) 2005-2007 Kiia Kallio
 *
 * http://mlab.uiah.fi/~kkallio/antialiasing/
 * 
 * This code is distributed under the three-clause BSD license.
 * Read the LICENSE file or visit the URL above for details.
 *
 * \brief DefaultPolygonFactory is a class for creating the default polygon implementations.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef DEFAULT_POLYGON_FACTORY_H_INCLUDED
#define DEFAULT_POLYGON_FACTORY_H_INCLUDED

#ifdef GATHER_STATISTICS
//! A class for tracking polygon construction.
class PolygonConstructionStatistics
{
public:
    //! Constructos.
    PolygonConstructionStatistics()
    {
        reset();
    }

    //! Resets the statistic counters.
    void reset()
    {
        mPolygonCount = 0;
        mSubPolygonCount = 0;
        mEdgeCount = 0;
        mEdgeLength = 0.0;
    }

    //! Adds the values from the polygondata to the statistics.
    void add(const PolygonData *aData)
    {
        int subCount = aData->getSubPolygonCount();
        int n;
        for (n = 0; n < subCount; n++)
        {
            const RATIONAL *vertices = aData->getVertexData(n);
            int vCount = aData->getVertexCount(n);
            int p;
            for (p = 0; p < vCount; p++)
            {
                int next = (p + 1) % vCount;
                double dx = vertices[next*2] - vertices[p*2];
                double dy = vertices[next*2 + 1] - vertices[p*2 + 1];
                mEdgeLength += sqrt(dx*dx + dy*dy);
            }
            mEdgeCount += vCount;
        }
        mSubPolygonCount += subCount;
        mPolygonCount++;
    }

    int mPolygonCount;
    int mSubPolygonCount;
    int mEdgeCount;
    double mEdgeLength;
};

//! A listener class for tracking polygon construction.
class PolygonConstructionStatisticsListener
{
public:
    virtual void update(const PolygonConstructionStatistics &aStatistics) = 0;
};
#endif


//! A class for creating the default polygon implementations.
class DefaultPolygonFactory : public PolygonFactory
{
public:

#ifdef GATHER_STATISTICS
    DefaultPolygonFactory(PolygonConstructionStatisticsListener *aListener)
    {
        mListener = aListener;
    }

    //! Notifies the factory that a construction of new vector object is starting.
    virtual void beginGraphic()
    {
        mStatistics.reset();
    }

    //! Notifies the factory that a construction of a vector object has ended.
    virtual void endGraphic()
    {
        mListener->update(mStatistics);
    }
#endif

    //! Creates a polygon with the factory.
    virtual PolygonWrapper * createPolygonWrapper(const PolygonData *aData)
    {
#ifdef GATHER_STATISTICS
        mStatistics.add(aData);
#endif
        return Polygon::create(aData);
    }

protected:
#ifdef GATHER_STATISTICS
    PolygonConstructionStatistics mStatistics;
    PolygonConstructionStatisticsListener *mListener;
#endif
};

#endif // !DEFAULT_POLYGON_FACTORY_H_INCLUDED
