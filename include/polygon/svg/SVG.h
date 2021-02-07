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
 * \brief A set of classes for handling SVG files.
 *
 *
 * $Id: $
 * $Date: $
 * $Revision: $
 */

#ifndef SVG_H_INCLUDED
#define SVG_H_INCLUDED

class ShapeMaker;

//! Base class for nodes in a stack.
class StackNode
{
public:
    StackNode *mNext;
};


//! Stack node for storing transformations.
class SVGStackNodeTransformation : public StackNode 
{
public:
    MATRIX2D mMatrix;
};


//! A class for representing paint in SVG images.
class SVGPaint
{
public:
    unsigned int mColor;
    bool mNone;
};


//! Stack node for storing paint.
class SVGStackNodePaint : public StackNode
{
public:
    SVGPaint mPaint;
};


//! Stack node for storing fill rule.
class SVGStackNodeFillRule : public StackNode
{
public:
    RENDERER_FILLMODE mFillRule;
};


//! A pool for stack nodes.
class StackNodePool
{
public:
    //! Constructor.
    StackNodePool();

    //! Initializer.
    bool init(int aInitialSize, int aIncrement);

    //! Virtual destructor.
    virtual ~StackNodePool();

    //! Returns a new stack node from the pool.
    StackNode *get();

    //! Puts a stack node back to the pool.
    void put(StackNode *aNode);

    //! Puts the stack node and all its siblings back to the pool.
    void putNodes(StackNode *aNode);

protected:
    //! Enlarges the pool by aSize.
    bool grow(int aSize);

    //! Virtual method for implementations for creating the nodes.
    virtual StackNode *createNode() = 0;

    int mGrowIncrement;
    StackNode *mFirst;
};


//! A pool for transformation stack nodes.
class SVGStackNodeTransformationPool : public StackNodePool 
{
protected:
    //! Virtual method for implementations for creating the nodes.
    virtual StackNode *createNode();
};


//! A pool for paint stack nodes.
class SVGStackNodePaintPool : public StackNodePool
{
protected:
    //! Virtual method for implementations for creating the nodes.
    virtual StackNode *createNode();
};


//! A pool for fill rule stack nodes.
class SVGStackNodeFillRulePool : public StackNodePool
{
protected:
    //! Virtual method for implementations for creating the nodes.
    virtual StackNode *createNode();
};


//! A stack used while processing the SVG document.
class SVGStack
{
public:
    //! Constructor.
    SVGStack();

    //! Destructor.
    ~SVGStack();

    //! Initializer.
    bool init(int aStackSize, int aStackIncrement);

    //! Pops transformation from the stack to aMatrix.
    bool popTransformation(MATRIX2D &aMatrix);

    //! Pushes the matrix from aMatrix to the stack.
    bool pushTransformation(const MATRIX2D &aMatrix);

    //! Pops paint from the stack to aPaint.
    bool popPaint(SVGPaint &aPaint);

    //! Pushes the paint from aPaint to the stack.
    bool pushPaint(const SVGPaint &aPaint);

    //! Pops fill rule from the stack to aFillRule.
    bool popFillRule(RENDERER_FILLMODE &aFillRule);

    //! Pushes the fill rule from aFillRule to the stack.
    bool pushFillRule(const RENDERER_FILLMODE &aFillRule);

protected:
    SVGStackNodeTransformation *mTransformationStack;
    SVGStackNodePaint *mPaintStack;
    SVGStackNodeFillRule *mFillRuleStack;

    SVGStackNodeTransformationPool mTransformationPool;
    SVGStackNodePaintPool mPaintPool;
    SVGStackNodeFillRulePool mFillRulePool;
};


//! A class that is used for decomposing the SVG tree to polygons.
class SVGContext
{
public:
    //! Constructor.
    SVGContext(ShapeMaker *aShapeMaker);

    //! Initializer.
    bool init(int aStackSize, int aStackIncrement);

    //! Resets the state of the context.
    void reset();

    //! Pushes the state of the context to the stack.
    bool pushState();

    //! Pops the state of the context from the stack.
    bool popState();

    //! Sets the current transformation in the context.
    void setCurrentTransformation(const MATRIX2D &aTransformation);

    //! Sets the current color.
    void setCurrentColor(unsigned int aColor);

    //! Sets the current opacity.
    void setCurrentOpacity(RATIONAL aOpacity);

    //! Sets the current fill rule.
    void setCurrentFillRule(RENDERER_FILLMODE aFillRule);

    //! Sets the fill off.
    void setFillOff();

    //! Returns the current transformation.
    const MATRIX2D & getCurrentTransformation();

    //! Returns the current color.
    unsigned int getCurrentColor();

    //! Returns the current opacity.
    RATIONAL getCurrentOpacity();

    //! Returns the current fill rule.
    RENDERER_FILLMODE getCurrentFillRule();

    //! Returns true if fill is off.
    bool isFillOff();

    //! Returns a pointer to the shape maker.
    ShapeMaker * getShapeMaker();

protected:
    MATRIX2D mCurrentTransformation;
    SVGPaint mCurrentPaint;
    RENDERER_FILLMODE mCurrentFillRule;
    ShapeMaker *mShapeMaker;
    SVGStack mStack;
};


//! A class for storing the tokens in the SVG path data.
class SVGPathToken
{
    friend class SVGPathTokenizer;
public:
    //! Returns a pointer to the next token.
    SVGPathToken *getNext();

    //! Returns a pointer to the data of the token.
    const char *getBuffer();

protected:
    //! Protected constructor
    SVGPathToken(char *aBuffer);

    //! Protected destructor.
    ~SVGPathToken();

    SVGPathToken *mNext;
    char *mBuffer;
};


//! A class for tokenizing the SVG path data.
class SVGPathTokenizer
{
public:
    //! Constructor.
    SVGPathTokenizer();

    //! Destructor.
    ~SVGPathTokenizer();

    //! Tokenizes the input string.
    bool tokenize(const char *aInput);

    //! Returns a pointer to the first token (call this after calling tokenize()).
    SVGPathToken *getFirstToken();

protected:
    //! Adds a token to the list of tokens.
    bool addToken(const char *aBuffer);

    SVGPathToken *mFirstToken;
    SVGPathToken *mLastToken;
};


//! Base class for commands.
class SVGCommand
{
    friend class SVGCommandSet;
public:
    //! Constructor.
    SVGCommand();

    //! Virtual destructor.
    virtual ~SVGCommand();

    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext) = 0;

    //! Returns the next command.
    SVGCommand *getNextSibling();

protected:
    SVGCommand *mNextSibling;
};


//! A class for handling a set of SVG commands.
class SVGCommandSet : public SVGCommand
{
public:
    //! Constructor.
    SVGCommandSet(bool aUseStack = false);

    //! Virtual destructor.
    virtual ~SVGCommandSet();

    //! Executes the commands.
    virtual bool execute(SVGContext *aContext);

    //! Adds a new command to the set as the last child node.
    void addLastChild(SVGCommand *aCommand);

    //! Returns the first command in the set.
    SVGCommand *getFirstChild();

protected:
    bool mUseStack;
    SVGCommand *mFirstChild;
    SVGCommand *mLastChild;
};


//! A color definition command in SVG command tree.
class SVGColorCommand : public SVGCommand
{
public:
    //! Constructor. (Paint set to none.)
    SVGColorCommand();

    //! Constructor. (Paint set to given color.)
    SVGColorCommand(unsigned int aColor);

    //! Builds a color command from the given string.
    static SVGCommand * build(const char *aColor);

    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext);

protected:
    unsigned int mColor;
    bool mNone;
};


//! An opacity definition command in SVG command tree.
class SVGOpacityCommand : public SVGCommand
{
public:
    //! Constructor.
    SVGOpacityCommand(RATIONAL aOpacity);

    //! Builds an opacity command from the given string.
    static SVGCommand * build(const char *aOpacity);

    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext);

protected:
    RATIONAL mOpacity;
};


//! A fill rule definition command in SVG command tree.
class SVGFillRuleCommand : public SVGCommand
{
public:
    //! Constructor.
    SVGFillRuleCommand(RENDERER_FILLMODE aFillRule);

    //! Builds a fill rule command from the given string.
    static SVGCommand * build(const char *aFillRule);

    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext);

protected:
    RENDERER_FILLMODE mFillRule;
};


//! A begin path command in SVG command tree.
class SVGBeginPathCommand : public SVGCommand
{
public:
    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext);
};


//! An end path command in SVG command tree.
class SVGEndPathCommand : public SVGCommand
{
public:
    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext);
};


//! A command representing the path data in SVG command tree.
class SVGPathCommand : public SVGCommand
{
public:
    //! Constructor.
    SVGPathCommand();

    //! Virtual destructor.
    virtual ~SVGPathCommand();

    //! Returns a list of path commands, NULL if fails
    static SVGCommand * build(const char *aPathData);

    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext);

protected:
    char mCommand;
    RATIONAL *mData;
};


//! A transformation command in SVG command tree.
class SVGTransformationCommand : public SVGCommand
{
public:
    //! Constructor.
    SVGTransformationCommand(const MATRIX2D &aMatrix);

    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext);

protected:
    MATRIX2D mMatrix;
};


//! A command representing a document in SVG command tree.
class SVGDocument : public SVGCommandSet
{
public:
    //! Constructor.
    SVGDocument();

    //! Loads the SVG document from a file.
    static SVGDocument *load(const char *aFileName);

    //! Sets the root level transformation for the SVG document.
    void setTransformation(const MATRIX2D &aMatrix);

    //! Executes the command, i.e. applies it to the context.
    virtual bool execute(SVGContext *aContext);

protected:
    //! Creates the SVG tree from the XML DOM.
    static bool createTree(class TiXmlElement *aElement, SVGCommandSet *aSet);

    //! Creates a path command set from strings.
    static SVGCommand * createPath(const char *aFill, const char *aFillOpacity, const char *aFillRule, const char *aPath);

    MATRIX2D mMatrix;
};

//! A static method for loading an SVG file (and optionally creating a tesselated dump of it) into a VectorGraphic object.
VectorGraphic * loadSVGtoVectorGraphic(PolygonFactory *aFactory, const char *aPath, const char *aSVGName, const char *aSVGDumpName, const char *aBinaryDumpName);

#endif // SVG_H_INCLUDED
