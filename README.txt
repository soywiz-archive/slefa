Scanline edge-flag algorithm for antialiasing
Source code release v. 1.0
2007-07-29

This is a source code release for algorithm described in paper "Scanline
edge-flag algorithm for antialiasing" by Kiia Kallio. The paper is available
from the same place as the source code, i.e.
http://mlab.uiah.fi/~kkallio/antialiasing/. You might want to read it first
to better understand the algorithm.

The quick evalution of the algorithm in action can be done by running the
pre-compiled test binaries from the bin directory.

The code is distributed under the three-clause BSD license. Read the LICENSE
file or visit the URL above for details.

The source code is experimental, but it is useful if someone wishes to use
the algorithm in a graphics library.

In addition to the implementation (actually, six evolutional stages of
the implementation), the package contains performance test framework, and
tests also for GDI+ and Antigrain Geometry (AGG).

The build environment is available for Visual Studio 2003. It can be imported
to newer versions of Visual Studio, but compiling on other environments
requires setting up the build environment.

The code is portable C++ (C++ features have been used very sparingly, for
instance no exceptions or templates). There are some inline assembler
optimizations (in SubPolygon.h) but these are behind a define and can be
disabled. It should be possible to compile the code on just about any
platform with C++ support (also mobile platforms such as Symbian).

Since the C++ features are used so sparingly, it should also be possible to
convert the code to plain C (or some other language) rather easily.

What the code does?
-------------------

The code implements an algorithm for filling antialiased polygons. It's just
polygons (for instance no bezier curves, stroking etc. is supported). Making
this into a useful graphics library requires considerable amount of work, but
this is a good beginning anyway.

Also a simple framework for loading SVG files is included. This uses TinyXML
library, so a simple and small SVG viewer (for instance for SDL-based programs)
can be implemented using the code.

However, only some SVG features are supported. Getting SVG files to load may
require some tweaking in the authoring tool (for instance converting strokes
to paths etc.).

Also, the SVG library does the conversion of bezier curves to polygons using a
static amount of subdivision. There is no dynamic subdivision algorithm in use.
This is on purpose, since the tests must use polygonal if polygon processing
performance is to be measured. Otherwise the differences in the bezier
subdivision algorithms would influence the results, and this is not desired.

About the code
--------------

The final version of the algorithm is implemented in PolygonVersionF.cpp.
Earlier version are the evolutionary steps (also described in the article).
In a similar fashion, the final version of the implementation for tracking
masks with non-zero winding fill rule is implemented in NonZeroMaskC.h.
The other versions are evolutionary steps.

An interesting file in experimenting is PolygonFiller.h. This has the
defines for the mask sizes and sample positions. Most notably, define
SUBPIXEL_SHIFT (that can have values 3,4 or 5) defines the amount of
samples per pixel (respectively, 8, 16 or 32).

The code is written so that it should be pretty easy to extract it from the
test framework and port it to some other framework. For instance vector and
matrix classes along with their functionality are used with VECTOR2D and
MATRIX2D defines throughout the implementation, and shoud be simple to change
them to use the similar functionality of another library. Also math data
types and functions are wrapped with RATIONAL define. These definitions can
be found from Platform.h file. Currently RATIONAL is defined as float.

There is also doxygen documentation of the code included in the source
package. Reading it may give better view of the relations of various
classes.

About the tests
---------------

Various tests are constructed with code, but some use SVG files. These files
are in SVG directory. Most of the files are public domain, downloaded from
http://www.artfavor.com/, but some ("froggy.svg" and "froggy-simple.svg") are
my own creations. They are licensed under the Creative Commons
Attribution-Noncommercial-Share Alike 3.0 License. To view a copy of this
license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/.

There are actually three sets of tests available. These can be changed with
defines at the beginning of the TestApp.cpp (or TestAppAGG.cpp or
TestAppGDIplus.cpp). The available defines are STANDARD_TEST, QUALITY_TEST
and SQUARE_TEST.

Also the test size can be set with define GLOBAL_SCALE at the beginning of
the TestApp[AGG|GDIplus].cpp. Available values are 1,2 and 4 (for 320x240,
640x480 and 1280x960).

For each test, the same image is drawn for a number of times while its
transformation is modified (in order to prevent the underlying implementation
to cache anything). The average time for this is recorded and displayed.

The images are also saved as BMP files for later inspection. The test results
are also saved to a text file.

About the libraries
-------------------

The project has dependencies to TinyXML, AGG and SDL. TinyXML version 2.3.2,
AGG version 2.3 and SDL version 1.2.7 have been used.

More information about the libraries:
http://www.grinninglizard.com/tinyxml/
http://www.antigrain.com/
http://www.libsdl.org/

Direct download links to the versions in use are:
http://downloads.sourceforge.net/tinyxml/tinyxml_2_3_2.zip
http://www.antigrain.com/agg23.zip
http://www.libsdl.org/release/SDL-devel-1.2.7-VC6.zip

Also newer versions than the versions listed here may work.

Depending on your compiler and platform, compiling the GDI+ tests may also
require downloading some SDK from Microsoft.

Using the code
--------------

As the LICENSE states, you may use the code rather freely, as long as you give
appropriate credit for me.

However, I would very much like to know if the code ends up in use somewhere.
You may try to reach me from kkallio@uiah.fi - but please be aware that 
A) I don't read this account very often and B) the account is so heavily
spammed these days that my spam filter may eat your mails.

If I don't reply within a couple of week or so, please retry - and preferably
not by sending the same mail again; if the spam filter is the culprit, it will
eat it again...
