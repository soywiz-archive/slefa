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
 * \brief A class for wrapping the AGG render target implementation for the tests.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef AGG_RENDER_TARGET_WRAPPER_H_INCLUDED
#define AGG_RENDER_TARGET_WRAPPER_H_INCLUDED

//! A class for wrapping the AGG render target implementation for the tests.
class AGGRenderTargetWrapper : public RenderTargetWrapper
{
public:
    //! Constructor.
    AGGRenderTargetWrapper(agg::rendering_buffer *aBuffer)
    {
        mBuffer = aBuffer;
    }

    //! Inline getter for the native implementation.
    inline agg::rendering_buffer * getBuffer()
    {
        return mBuffer;
    }

protected:
    agg::rendering_buffer *mBuffer;
};

#endif // !AGG_RENDER_TARGET_WRAPPER_H_INCLUDED
