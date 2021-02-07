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
 * \brief A base class for defining render targets. The actual implementation of the render target
 *        depends on the platform and fillers.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */


#ifndef RENDER_TARGET_WRAPPER_H_INCLUDED
#define RENDER_TARGET_WRAPPER_H_INCLUDED

//! A base class for defining render targets.
/*! The actual implementation of the render target depends on the platform and fillers.
 */
class RenderTargetWrapper
{
public:
    //! A callback for notifying that the rendering has finished.
    virtual void onRenderDone() {}
};

#endif // !RENDER_TARGET_WRAPPER_H_INCLUDED
