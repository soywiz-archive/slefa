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
 * \brief A utility class for dumping the polygon data to SVG file.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#include "polygon/base/Base.h"
#include "polygon/svg/DumpSVG.h"

//! Writes out the data as an SVG file.
bool DumpSVG::write(const char *aFileName, const PolygonData * const *aPolygons,
                    const unsigned long *aColors, const RENDERER_FILLMODE *aFillModes,
                    int aPolygonCount)
{
    TEXTFILE_OBJECT *fp = TEXTFILE_OPEN_FOR_WRITE(aFileName);

    if (!fp)
        return false;

    // Calculate the bounding box for the vector graphic and use it as the viewbox

    RATIONAL minX = MAX_RATIONAL,maxX = MIN_RATIONAL,minY = MAX_RATIONAL, maxY = MIN_RATIONAL;

    int p;
    for (p = 0; p < aPolygonCount; p++)
    {
        const PolygonData *poly = aPolygons[p];
        int subPolyCount = poly->getSubPolygonCount();
        int s;
        for (s = 0; s < subPolyCount; s++)
        {
            int vertexCount = poly->getVertexCount(s);
            const RATIONAL *vertices = poly->getVertexData(s);
            int v;
            for (v = 0; v < vertexCount; v++)
            {
                RATIONAL x = vertices[v*2];
                RATIONAL y = vertices[v*2 + 1];

                if (x < minX)
                    minX = x;
                if (x > maxX)
                    maxX = x;
                if (y < minY)
                    minY = y;
                if (y > maxY)
                    maxY = y;
            }
        }
    }

    RATIONAL width = maxX - minX;
    RATIONAL height = maxY - minY;

    TEXTFILE_WRITE_STRING(fp,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    TEXTFILE_WRITE_STRING(fp,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n");
    
    TEXTFILE_WRITE_STRING(fp,"<svg width=\"");
    TEXTFILE_WRITE_RATIONAL(fp,width);
    TEXTFILE_WRITE_STRING(fp,"\" height=\"");
    TEXTFILE_WRITE_RATIONAL(fp,height);
    TEXTFILE_WRITE_STRING(fp,"\" viewBox=\"");
    TEXTFILE_WRITE_RATIONAL(fp,minX);
    TEXTFILE_WRITE_STRING(fp," ");
    TEXTFILE_WRITE_RATIONAL(fp,minY);
    TEXTFILE_WRITE_STRING(fp," ");
    TEXTFILE_WRITE_RATIONAL(fp,width);
    TEXTFILE_WRITE_STRING(fp," ");
    TEXTFILE_WRITE_RATIONAL(fp,height);
    TEXTFILE_WRITE_STRING(fp,"\" overflow=\"visible\">\n");

    //<path fill-rule="evenodd" fill="#00ff00" d="M 0 0 L 0 100 L 100 100 L 100 0 z" />
    for (p = 0; p < aPolygonCount; p++)
    {

        TEXTFILE_WRITE_STRING(fp,"    <path");

        if (aFillModes[p] == RENDERER_FILLMODE_EVENODD)
        {
            TEXTFILE_WRITE_STRING(fp," fill-rule=\"evenodd\"");
        }
        else
        {
            TEXTFILE_WRITE_STRING(fp," fill-rule=\"nonzero\"");
        }
        
        unsigned long color = aColors[p];

        char colString[7];
        colString[0] = (char)((color & 0xf00000) >> 20);
        colString[0] += colString[0] < 10 ? '0' : 'a' - 10;
        colString[1] = (char)((color & 0x0f0000) >> 16);
        colString[1] += colString[1] < 10 ? '0' : 'a' - 10;
        colString[2] = (char)((color & 0x00f000) >> 12);
        colString[2] += colString[2] < 10 ? '0' : 'a' - 10;
        colString[3] = (char)((color & 0x000f00) >> 8);
        colString[3] += colString[3] < 10 ? '0' : 'a' - 10;
        colString[4] = (char)((color & 0x0000f0) >> 4);
        colString[4] += colString[4] < 10 ? '0' : 'a' - 10;
        colString[5] = (char)(color & 0x00000f);
        colString[5] += colString[5] < 10 ? '0' : 'a' - 10;
        colString[6] = 0;

        TEXTFILE_WRITE_STRING(fp," fill=\"#");
        TEXTFILE_WRITE_STRING(fp,colString);
        TEXTFILE_WRITE_STRING(fp,"\"");

        const PolygonData *poly = aPolygons[p];

        TEXTFILE_WRITE_STRING(fp," d=\"");

        int subPolyCount = poly->getSubPolygonCount();
        int s;
        for (s = 0; s < subPolyCount; s++)
        {
            int vertexCount = poly->getVertexCount(s);
            const RATIONAL *vertices = poly->getVertexData(s);
            if (vertexCount > 0)
            {
                RATIONAL x = vertices[0];
                RATIONAL y = vertices[1];

                TEXTFILE_WRITE_STRING(fp,"M ");
                TEXTFILE_WRITE_RATIONAL(fp,x);
                TEXTFILE_WRITE_STRING(fp," ");
                TEXTFILE_WRITE_RATIONAL(fp,y);
                TEXTFILE_WRITE_STRING(fp," ");

                int v;
                for (v = 0; v < vertexCount; v++)
                {
                    x = vertices[v*2];
                    y = vertices[v*2 + 1];

                    TEXTFILE_WRITE_STRING(fp,"L ");
                    TEXTFILE_WRITE_RATIONAL(fp,x);
                    TEXTFILE_WRITE_STRING(fp," ");
                    TEXTFILE_WRITE_RATIONAL(fp,y);
                    TEXTFILE_WRITE_STRING(fp," ");
                }

                TEXTFILE_WRITE_STRING(fp,"Z ");
            }
        }

        TEXTFILE_WRITE_STRING(fp,"\" />\n");
    }


    TEXTFILE_WRITE_STRING(fp,"</svg>\n");
    TEXTFILE_CLOSE(fp);

    return true;
}


//! Writes out the data as a binary shape data file.
bool DumpSVG::writeBinary(const char *aFileName, const PolygonData * const *aPolygons,
                          const unsigned long *aColors, const RENDERER_FILLMODE *aFillModes,
                          int aPolygonCount)
{
    BINARYFILE_OBJECT *fp = BINARYFILE_OPEN_FOR_WRITE(aFileName);

    if (!fp)
        return false;

    const char *header = "POLYSHPE";
    int p;
    for (p = 0; p < 8; p++)
        BINARYFILE_WRITE_DATA(&header[p],sizeof(char),1,fp);

    unsigned long t = aPolygonCount;
    BINARYFILE_WRITE_DATA(&t,sizeof(unsigned long),1,fp);

    for (p = 0; p < aPolygonCount; p++)
    {
        BINARYFILE_WRITE_DATA(&aColors[p],sizeof(unsigned long),1,fp);

        const PolygonData *poly = aPolygons[p];

        unsigned long subPolyCount = poly->getSubPolygonCount();

        BINARYFILE_WRITE_DATA(&subPolyCount,sizeof(unsigned long),1,fp);

        int s;
        for (s = 0; s < (int)subPolyCount; s++)
        {
            unsigned long vertexCount = poly->getVertexCount(s);
            BINARYFILE_WRITE_DATA(&vertexCount,sizeof(unsigned long),1,fp);

            const RATIONAL *vertices = poly->getVertexData(s);
            if (vertexCount > 0)
            {                

                int v;
                for (v = 0; v < (int)vertexCount; v++)
                {
                    float tf[2];
                    tf[0] = (float)vertices[v*2];
                    tf[1] = (float)vertices[v*2 + 1];

                    BINARYFILE_WRITE_DATA(tf,sizeof(float),2,fp);
                }
            }
        }
    }

    BINARYFILE_CLOSE(fp);

    return true;
}

