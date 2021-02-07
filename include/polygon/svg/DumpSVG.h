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


#ifndef DUMP_SVG_H_INCLUDED
#define DUMP_SVG_H_INCLUDED

class VectorGraphic;

//! A utility class for dumping the polygon data to SVG file.
class DumpSVG
{
public:
    //! Writes out the data as an SVG file.
    static bool write(const char *aFileName, const PolygonData * const *aPolygons,
                      const unsigned long *aColors, const RENDERER_FILLMODE *aFillModes,
                      int aPolygonCount);

    //! Writes out the data as a binary shape data file.
    static bool writeBinary(const char *aFileName, const PolygonData * const *aPolygons,
                            const unsigned long *aColors, const RENDERER_FILLMODE *aFillModes,
                            int aPolygonCount);
};

#endif // !DUMP_SVG_H_INCLUDED
