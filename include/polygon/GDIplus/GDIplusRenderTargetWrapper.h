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
 * \brief A class for wrapping the GDI+ render target implementation for the tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef GDI_PLUS_RENDER_TARGET_WRAPPER_H_INCLUDED
#define GDI_PLUS_RENDER_TARGET_WRAPPER_H_INCLUDED

//! A class for wrapping the GDI+ render target implementation for the tests.
class GDIplusRenderTargetWrapper : public RenderTargetWrapper
{
public:
    //! Constructor.
    GDIplusRenderTargetWrapper(Graphics * aGraphics)
    {
        mGraphics = aGraphics;
    }

    //! Inline getter for the native implementation.
    inline Graphics * getGraphics()
    {
        return mGraphics;
    }

protected:
    Graphics *mGraphics;
};

#endif // !GDI_PLUS_RENDER_TARGET_WRAPPER_H_INCLUDED
