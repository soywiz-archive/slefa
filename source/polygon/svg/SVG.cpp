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

#include "polygon/base/Base.h"
#include "polygon/svg/ShapeMaker.h"
#include "polygon/svg/SVG.h"
#include "polygon/svg/DumpSVG.h"
#include "../libs/tinyxml/tinyxml.h"

///////////////////////////////////////////////////////////////////////////////
// A pool for stack nodes.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
StackNodePool::StackNodePool()
{
    mGrowIncrement = 0;
    mFirst = NULL;
}


//! Initializer.
bool StackNodePool::init(int aInitialSize, int aIncrement)
{
    mGrowIncrement = aIncrement;
    if (!grow(aInitialSize))
        return false;
    return true;
}


//! Virtual destructor.
StackNodePool::~StackNodePool()
{
    StackNode *node = mFirst;
    while (node)
    {
        mFirst = mFirst->mNext;
        delete node;
        node = mFirst;
    }
}


//! Returns a new stack node from the pool.
StackNode * StackNodePool::get()
{
    if (mFirst == NULL)
        grow(mGrowIncrement);
    StackNode *node = mFirst;
    if (node)
        mFirst = mFirst->mNext;
    return node;
}


//! Puts a stack node back to the pool.
void StackNodePool::put(StackNode *aNode)
{
    aNode->mNext = mFirst;
    mFirst = aNode;
}


//! Puts the stack node and all its siblings back to the pool.
void StackNodePool::putNodes(StackNode *aNode)
{
    StackNode *node = aNode;
    while (node)
    {
        StackNode *nextNode = node;
        put(node);
        node = nextNode;
    }
}


//! Enlarges the pool by aSize.
bool StackNodePool::grow(int aSize)
{
    int n;
    for (n = 0; n < aSize; n++)
    {
        StackNode *node = createNode();
        if (node)
            put(node);
        else
            return false;
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// A pool for transformation stack nodes.
///////////////////////////////////////////////////////////////////////////////

//! Virtual method for implementations for creating the nodes.
StackNode * SVGStackNodeTransformationPool::createNode()
{
    return new SVGStackNodeTransformation();
}


///////////////////////////////////////////////////////////////////////////////
// A pool for paint stack nodes.
///////////////////////////////////////////////////////////////////////////////

//! Virtual method for implementations for creating the nodes.
StackNode * SVGStackNodePaintPool::createNode()
{
    return new SVGStackNodePaint();
}


///////////////////////////////////////////////////////////////////////////////
// A pool for fill rule stack nodes.
///////////////////////////////////////////////////////////////////////////////

//! Virtual method for implementations for creating the nodes.
StackNode * SVGStackNodeFillRulePool::createNode()
{
    return new SVGStackNodeFillRule();
}


///////////////////////////////////////////////////////////////////////////////
//! A stack used while processing the SVG document.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGStack::SVGStack()
{
    mTransformationStack = NULL;
    mPaintStack = NULL;
    mFillRuleStack = NULL;
}


//! Destructor.
SVGStack::~SVGStack()
{
    mTransformationPool.putNodes(mTransformationStack);
    mPaintPool.putNodes(mPaintStack);
    mFillRulePool.putNodes(mFillRuleStack);
}


//! Initializer.
bool SVGStack::init(int aStackSize, int aStackIncrement)
{
    if (!mTransformationPool.init(aStackSize,aStackIncrement) ||
        !mPaintPool.init(aStackSize,aStackIncrement) ||
        !mFillRulePool.init(aStackSize,aStackIncrement))
        return false;

    return true;
}


//! Pops transformation from the stack to aMatrix.
bool SVGStack::popTransformation(MATRIX2D &aMatrix)
{
    SVGStackNodeTransformation *node = mTransformationStack;
    if (node == NULL)
        return false;
    
    mTransformationStack = (SVGStackNodeTransformation *)node->mNext;
    aMatrix = node->mMatrix;
    mTransformationPool.put(node);

    return true;
}


//! Pushes the matrix from aMatrix to the stack.
bool SVGStack::pushTransformation(const MATRIX2D &aMatrix)
{
    SVGStackNodeTransformation *node =
        (SVGStackNodeTransformation *)mTransformationPool.get();
    if (node == NULL)
        return false;
    node->mMatrix = aMatrix;
    node->mNext = mTransformationStack;
    mTransformationStack = node;
    return true;
}


//! Pops paint from the stack to aPaint.
bool SVGStack::popPaint(SVGPaint &aPaint)
{
    SVGStackNodePaint *node = mPaintStack;
    if (node == NULL)
        return false;
    
    mPaintStack = (SVGStackNodePaint *)node->mNext;
    aPaint = node->mPaint;
    mPaintPool.put(node);

    return true;
}


//! Pushes the paint from aPaint to the stack.
bool SVGStack::pushPaint(const SVGPaint &aPaint)
{
    SVGStackNodePaint *node =
        (SVGStackNodePaint *)mPaintPool.get();
    if (node == NULL)
        return false;
    node->mPaint = aPaint;
    node->mNext = mPaintStack;
    mPaintStack = node;
    return true;
}


//! Pops fill rule from the stack to aFillRule.
bool SVGStack::popFillRule(RENDERER_FILLMODE &aFillRule)
{
    SVGStackNodeFillRule *node = mFillRuleStack;
    if (node == NULL)
        return false;
    
    mFillRuleStack = (SVGStackNodeFillRule *)node->mNext;
    aFillRule = node->mFillRule;
    mFillRulePool.put(node);

    return true;
}


//! Pushes the fill rule from aFillRule to the stack.
bool SVGStack::pushFillRule(const RENDERER_FILLMODE &aFillRule)
{
    SVGStackNodeFillRule *node =
        (SVGStackNodeFillRule *)mFillRulePool.get();
    if (node == NULL)
        return false;
    node->mFillRule = aFillRule;
    node->mNext = mFillRuleStack;
    mFillRuleStack = node;
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// A class that is used for decomposing the SVG tree to polygons.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGContext::SVGContext(ShapeMaker *aShapeMaker)
{
    mShapeMaker = aShapeMaker;
    reset();
}


//! Initializer.
bool SVGContext::init(int aStackSize, int aStackIncrement)
{
    return mStack.init(aStackSize,aStackIncrement);
}


//! Resets the state of the context.
void SVGContext::reset()
{
    mCurrentTransformation.makeIdentity();
    mCurrentPaint.mColor = 0xff000000;
    mCurrentPaint.mNone = false;
    mCurrentFillRule = RENDERER_FILLMODE_EVENODD;
}


//! Pushes the state of the context to the stack.
bool SVGContext::pushState()
{
    if (!mStack.pushPaint(mCurrentPaint) ||
        !mStack.pushTransformation(mCurrentTransformation) ||
        !mStack.pushFillRule(mCurrentFillRule))
        return false;

    return true;
}


//! Pops the state of the context from the stack.
bool SVGContext::popState()
{
    if (!mStack.popTransformation(mCurrentTransformation) ||
        !mStack.popPaint(mCurrentPaint) ||
        !mStack.popFillRule(mCurrentFillRule))
        return false;

    mShapeMaker->setTransformation(mCurrentTransformation);
    mShapeMaker->setColor(mCurrentPaint.mColor);
    mShapeMaker->setFillRule(mCurrentFillRule);

    return true;
}


//! Sets the current transformation in the context.
void SVGContext::setCurrentTransformation(const MATRIX2D &aTransformation)
{
    mCurrentTransformation.multiply(aTransformation);
    mShapeMaker->setTransformation(mCurrentTransformation);
}


//! Sets the current color.
void SVGContext::setCurrentColor(unsigned int aColor)
{
    mCurrentPaint.mColor = (mCurrentPaint.mColor & 0xff000000) | (aColor & 0x00ffffff);
    mCurrentPaint.mNone = false;
    mShapeMaker->setColor(mCurrentPaint.mColor);
}


//! Sets the current opacity.
void SVGContext::setCurrentOpacity(RATIONAL aOpacity)
{
    unsigned int opacity = (int)(INT_TO_RATIONAL(255) * aOpacity);
    opacity <<= 24;
    mCurrentPaint.mColor = (mCurrentPaint.mColor & 0x00ffffff) | (opacity & 0xff000000);
    mShapeMaker->setColor(mCurrentPaint.mColor);
}


//! Sets the current fill rule.
void SVGContext::setCurrentFillRule(RENDERER_FILLMODE aFillRule)
{
    mCurrentFillRule = aFillRule;
    mShapeMaker->setFillRule(mCurrentFillRule);
}


//! Sets the fill off.
void SVGContext::setFillOff()
{
    mCurrentPaint.mNone = true;
}


//! Returns the current transformation.
const MATRIX2D & SVGContext::getCurrentTransformation()
{
    return mCurrentTransformation;
}


//! Returns the current color.
unsigned int SVGContext::getCurrentColor()
{
    return mCurrentPaint.mColor & 0xffffff;
}


//! Returns the current opacity.
RATIONAL SVGContext::getCurrentOpacity()
{
    return (RATIONAL)((mCurrentPaint.mColor >> 24) & 0xff) / INT_TO_RATIONAL(255);
}


//! Returns the current fill rule.
RENDERER_FILLMODE SVGContext::getCurrentFillRule()
{
    return mCurrentFillRule;
}


//! Returns true if fill is off.
bool SVGContext::isFillOff()
{
    return mCurrentPaint.mNone;
}


//! Returns a pointer to the shape maker.
ShapeMaker * SVGContext::getShapeMaker()
{
    return mShapeMaker;
}


///////////////////////////////////////////////////////////////////////////////
// A class for storing the tokens in the SVG path data.
///////////////////////////////////////////////////////////////////////////////

//! Returns a pointer to the next token.
SVGPathToken * SVGPathToken::getNext()
{
    return mNext;
}


//! Returns a pointer to the data of the token.
const char * SVGPathToken::getBuffer()
{
    return mBuffer;
}


//! Protected constructor
SVGPathToken::SVGPathToken(char *aBuffer)
{
    mNext = NULL;
    mBuffer = aBuffer;
}


//! Protected destructor.
SVGPathToken::~SVGPathToken()
{
    delete[] mBuffer;
}


///////////////////////////////////////////////////////////////////////////////
// A class for tokenizing the SVG path data.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGPathTokenizer::SVGPathTokenizer()
{
    mFirstToken = mLastToken = NULL;
}


//! Destructor.
SVGPathTokenizer::~SVGPathTokenizer()
{
    SVGPathToken *token = mFirstToken;
    while (token)
    {
        SVGPathToken *nextToken = token->getNext();
        delete token;
        token = nextToken;
    }
}


//! Tokenizes the input string.
bool SVGPathTokenizer::tokenize(const char *aInput)
{
    if (aInput == NULL)
        return false;
    /*
    - spaces are allowed (except within a number) as well as ','
    - a number starts with '-' or number.
    - a number ends when the next character is not a number or '.'
    - a command is a letter. Allowed letters are:
      Z z - closePath (0)
      L l - lineto (2)
      H h - horizontal lineto (1)
      V v - vertical lineto (1)
      C c - curveto (6)
      S s - smooth curveto (4)
      Q q - quadratic bezier curveto (4)
      T t - smooth quadratic bezier curveto (2)
      A a - elliptical arc (7)
    - Each command is followed by n arguments. The arguments can be
      repeated by multiply of n to mark up another similar command.
    */

    char buffer[100];
    int bufferIndex = 0;

    bool parsingNumber = false;
    char i;
    do
    {
        i = *aInput++;

        if (i == ' ' || i == ',' || i == '\r' || i == '\t' || i == '\n')
        {
            // Previous token ends.
            buffer[bufferIndex] = '\0';
            if (!addToken(buffer))
                return false;
            bufferIndex = 0;
            parsingNumber = false;
        }
        else if (i == '-' || (i >= '0' && i <= '9') || i == '.')
        {
            // previous wasn't a number or '-' breaks the number reading
            if (!parsingNumber || i == '-')
            {
                // Previous token ends.
                buffer[bufferIndex] = '\0';
                if (!addToken(buffer))
                    return false;
                bufferIndex = 0;
            }
            buffer[bufferIndex++] = i;
            parsingNumber = true;
        }
        else
        {
            // Any other character. This is interpreted as individual token
            // at this stage.
            buffer[bufferIndex] = '\0';
            if (!addToken(buffer))
                return false;
            bufferIndex = 0;
            buffer[bufferIndex++] = i;
            parsingNumber = false;
        }

        if (bufferIndex == 100)
            return false;
    }
    while(i);

    return true;
}


//! Returns a pointer to the first token (call this after calling tokenize()).
SVGPathToken * SVGPathTokenizer::getFirstToken()
{
    return mFirstToken;
}


//! Adds a token to the list of tokens.
bool SVGPathTokenizer::addToken(const char *aBuffer)
{
    int len = STRLEN(aBuffer);

    if (len == 0)
        return true;

    char *buffer = new char[len+1];

    if (buffer == NULL)
        return false;

    int n;
    for (n = 0; n < len; n++)
        buffer[n] = aBuffer[n];
    buffer[len] = '\0';

    SVGPathToken *token = new SVGPathToken(buffer);
    if (token == NULL)
    {
        delete[] buffer;
        return false;
    }

    if (mLastToken)
        mLastToken->mNext = token;
    else
        mFirstToken = token;
    mLastToken = token;

    return true;
}


///////////////////////////////////////////////////////////////////////////////
// Base class for commands.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGCommand::SVGCommand()
{
    mNextSibling = NULL;
}


//! Virtual destructor.
SVGCommand::~SVGCommand()
{
}


//! Returns the next command.
SVGCommand * SVGCommand::getNextSibling()
{
    return mNextSibling;
}


///////////////////////////////////////////////////////////////////////////////
// A class for handling a set of SVG commands.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGCommandSet::SVGCommandSet(bool aUseStack)
{
    mFirstChild = mLastChild = NULL;
    mUseStack = aUseStack;
}


//! Virtual destructor.
SVGCommandSet::~SVGCommandSet()
{
    SVGCommand *child = mFirstChild;
    while (child)
    {
        SVGCommand *next = child->getNextSibling();
        delete child;
        child = next;
    }
}


//! Executes the commands.
bool SVGCommandSet::execute(SVGContext *aContext)
{
    if (mUseStack)
    {
        if (!aContext->pushState())
            return false;
    }

    SVGCommand *command = mFirstChild;
    while (command)
    {
        if (!command->execute(aContext))
            return false;
        command = command->getNextSibling();
    }

    if (mUseStack)
    {
        if (!aContext->popState())
            return false;
    }

    return true;
}


//! Adds a new command to the set as the last child node.
void SVGCommandSet::addLastChild(SVGCommand *aCommand)
{
    if (aCommand)
    {
        if (mLastChild)
            mLastChild->mNextSibling = aCommand;
        else
            mFirstChild = aCommand;
        mLastChild = aCommand;
    }
}


//! Returns the first command in the set.
SVGCommand * SVGCommandSet::getFirstChild()
{
    return mFirstChild;
}


///////////////////////////////////////////////////////////////////////////////
// A color definition command in SVG command tree.
///////////////////////////////////////////////////////////////////////////////

//! Constructor. (Paint set to none.)
SVGColorCommand::SVGColorCommand()
{
    mNone = true;
}


//! Constructor. (Paint set to given color.)
SVGColorCommand::SVGColorCommand(unsigned int aColor)
{
    mColor = aColor;
    mNone = false;
}


//! Builds a color command from the given string.
SVGCommand * SVGColorCommand::build(const char *aColor)
{
    if (aColor == NULL)
        return NULL;

    SVGColorCommand *cmd;
    if (STREQ(aColor,"none"))
    {
        cmd = new SVGColorCommand();
    }
    else
    {
        unsigned int color;
        int count = SSCANF(aColor,"#%x",&color);
        if (count != 1)
            return NULL;
        cmd = new SVGColorCommand(color);
    }
    return cmd;
}


//! Executes the command, i.e. applies it to the context.
bool SVGColorCommand::execute(SVGContext *aContext)
{
    if (mNone)
        aContext->setFillOff();
    else
        aContext->setCurrentColor(mColor);
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// An opacity definition command in SVG command tree.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGOpacityCommand::SVGOpacityCommand(RATIONAL aOpacity)
{
    mOpacity = aOpacity;
}


//! Builds an opacity command from the given string.
SVGCommand * SVGOpacityCommand::build(const char *aOpacity)
{
    if (aOpacity == NULL)
        return NULL;

    float opacity;
    int count = SSCANF(aOpacity,"%f",&opacity);
    if (count != 1)
        return NULL;

    SVGOpacityCommand *cmd = new SVGOpacityCommand((RATIONAL)opacity);
    return cmd;
}


//! Executes the command, i.e. applies it to the context.
bool SVGOpacityCommand::execute(SVGContext *aContext)
{
    aContext->setCurrentOpacity(mOpacity);
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// A fill rule definition command in SVG command tree.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGFillRuleCommand::SVGFillRuleCommand(RENDERER_FILLMODE aFillRule)
{
    mFillRule = aFillRule;
}


//! Builds a fill rule command from the given string.
SVGCommand * SVGFillRuleCommand::build(const char *aFillRule)
{
    if (aFillRule == NULL)
        return NULL;

    RENDERER_FILLMODE mode;
    if (STREQ(aFillRule,"evenodd"))
        mode = RENDERER_FILLMODE_EVENODD;
    else if (STREQ(aFillRule,"nonzero"))
        mode = RENDERER_FILLMODE_NONZERO;
    else
        return NULL;

    SVGFillRuleCommand *cmd = new SVGFillRuleCommand(mode);
    return cmd;
}


//! Executes the command, i.e. applies it to the context.
bool SVGFillRuleCommand::execute(SVGContext *aContext)
{
    aContext->setCurrentFillRule(mFillRule);
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// A begin path command in SVG command tree.
///////////////////////////////////////////////////////////////////////////////

//! Executes the command, i.e. applies it to the context.
bool SVGBeginPathCommand::execute(SVGContext *aContext)
{
    ShapeMaker *maker = aContext->getShapeMaker();
    return maker->beginPath();
}


///////////////////////////////////////////////////////////////////////////////
// An end path command in SVG command tree.
///////////////////////////////////////////////////////////////////////////////

//! Executes the command, i.e. applies it to the context.
bool SVGEndPathCommand::execute(SVGContext *aContext)
{
    ShapeMaker *maker = aContext->getShapeMaker();
    return maker->endPath();
}


///////////////////////////////////////////////////////////////////////////////
// A command representing the path data in SVG command tree.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGPathCommand::SVGPathCommand()
{
    mCommand = 0;
    mData = NULL;
}


//! Virtual destructor.
SVGPathCommand::~SVGPathCommand()
{
    delete mData;
}


//! Returns a list of path commands, NULL if fails
SVGCommand * SVGPathCommand::build(const char *aPathData)
{
    SVGPathTokenizer tokenizer;
    if (!tokenizer.tokenize(aPathData))
        return NULL;

    SVGCommandSet *commands = new SVGCommandSet();
    if (!commands)
        return NULL;

    SVGBeginPathCommand *begin = new SVGBeginPathCommand();
    if (!begin)
    {
        delete commands;
        return NULL;
    }

    commands->addLastChild(begin);

    SVGPathToken *token = tokenizer.getFirstToken();

    int numberCount = 0;
    int numbersRead = 0;
    char currentCommand = 0;
    RATIONAL numbers[7];
    bool requireNumber = false;
    while(token)
    {
        const char *buffer = token->getBuffer();
        if (((buffer[0] >= 'a' && buffer[0] <= 'z') ||
             (buffer[0] >= 'A' && buffer[0] <= 'Z')) &&
             buffer[1] == '\0')
        {
            // A command was encountered.

            // This should be a number. Fail.
            if (requireNumber)
            {
                delete commands;
                return NULL;
            }

            currentCommand = buffer[0];

            switch(currentCommand)
            {
                // Z z - closePath (0)
                case 'Z':
                case 'z':
                    numberCount = 0;
                    break;
                // H h - horizontal lineto (1)
                case 'H':
                case 'h':
                // V v - vertical lineto (1)
                case 'V':
                case 'v':
                    numberCount = 1;
                    break;
                // M m - moveto (2)
                case 'M':
                case 'm':
                // L l - lineto (2)
                case 'L':
                case 'l':
                // T t - smooth quadratic bezier curveto (2)
                case 'T':
                case 't':
                    numberCount = 2;
                    break;
                // S s - smooth curveto (4)
                case 'S':
                case 's':
                // Q q - quadratic bezier curveto (4)
                case 'Q':
                case 'q':
                    numberCount = 4;
                    break;
                // C c - curveto (6)
                case 'C':
                case 'c':
                    numberCount = 6;
                    break;
                // A a - elliptical arc (7)
                case 'A':
                case 'a':
                    // Unsupported.
                    numberCount = 7;
                    break;
                default:
                    delete commands;
                    return NULL;
            }

            requireNumber = true;
        }
        else
        {
            // No number allowed in the start.
            if (currentCommand == 0)
            {
                delete commands;
                return NULL;
            }

            // Should be a number.
            float num;
            int count = SSCANF(buffer,"%f",&num);
            if (count != 1)
            {
                delete commands;
                return NULL;
            }

            numbers[numbersRead++] = (RATIONAL)num;
        }

        // If enough numbers were read, close the command.
        if (numbersRead == numberCount)
        {
            SVGPathCommand *cmd = new SVGPathCommand();
            if (cmd == NULL)
            {
                delete commands;
                return NULL;
            }

            cmd->mCommand = currentCommand;
            if (numbersRead > 0)
            {
                cmd->mData = new RATIONAL[numbersRead];
                if (cmd->mData == NULL)
                {
                    delete cmd;
                    delete commands;
                    return NULL;
                }
            }

            int n;
            for (n = 0; n < numbersRead; n++)
            {
                cmd->mData[n] = numbers[n];
            }

            commands->addLastChild(cmd);

            // Reset read numbers, but keep the command key. There may be
            // following sequences of data for the same key.
            numbersRead = 0;
            requireNumber = false;
        }

        token = token->getNext();
    }

    // Add close path as the last command.
    if (currentCommand != 'z' && currentCommand != 'Z')
    {
        SVGPathCommand *cmd = new SVGPathCommand();
        if (cmd == NULL)
        {
            delete commands;
            return NULL;
        }

        cmd->mCommand = 'z';
        commands->addLastChild(cmd);
    }

    SVGEndPathCommand *end = new SVGEndPathCommand();
    if (!end)
    {
        delete commands;
        return NULL;
    }

    commands->addLastChild(end);
    return commands;
}


//! Executes the command, i.e. applies it to the context.
bool SVGPathCommand::execute(SVGContext *aContext)
{
    if (aContext->getCurrentOpacity() <= FLOAT_TO_RATIONAL(0.0001f) ||
        aContext->isFillOff())
        return true;

    ShapeMaker *maker = aContext->getShapeMaker();
    bool result = false;
    VECTOR2D p1, p2, p3;
    switch (mCommand)
    {
        // Z z - closePath (0)
        case 'Z':
        case 'z':
            result = maker->closePath();
            break;
        // H h - horizontal lineto (1)
        case 'H':
            result = maker->horizontalLineToAbsolute(mData[0]);
            break;
        case 'h':
            result = maker->horizontalLineToRelative(mData[0]);
            break;
        // V v - vertical lineto (1)
        case 'V':
            result = maker->verticalLineToAbsolute(mData[0]);
            break;
        case 'v':
            result = maker->verticalLineToRelative(mData[0]);
            break;
        // M m - moveto (2)
        case 'M':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            result = maker->moveToAbsolute(p1);
            break;
        case 'm':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            result = maker->moveToRelative(p1);
            break;
        // L l - lineto (2)
        case 'L':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            result = maker->lineToAbsolute(p1);
            break;
        case 'l':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            result = maker->lineToRelative(p1);
            break;
        // T t - smooth quadratic bezier curveto (2)
        case 'T':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            result = maker->smoothQuadraticCurveToAbsolute(p1);
            break;
        case 't':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            result = maker->smoothQuadraticCurveToRelative(p1);
            break;
        // S s - smooth curveto (4)
        case 'S':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            VECTOR2D_SETX(p2,mData[2]);
            VECTOR2D_SETY(p2,mData[3]);
            result = maker->smoothCurveToAbsolute(p1,p2);
            break;
        case 's':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            VECTOR2D_SETX(p2,mData[2]);
            VECTOR2D_SETY(p2,mData[3]);
            result = maker->smoothCurveToRelative(p1,p2);
            break;
        // Q q - quadratic bezier curveto (4)
        case 'Q':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            VECTOR2D_SETX(p2,mData[2]);
            VECTOR2D_SETY(p2,mData[3]);
            result = maker->quadraticCurveToAbsolute(p1,p2);
            break;
        case 'q':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            VECTOR2D_SETX(p2,mData[2]);
            VECTOR2D_SETY(p2,mData[3]);
            result = maker->quadraticCurveToRelative(p1,p2);
            break;
        // C c - curveto (6)
        case 'C':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            VECTOR2D_SETX(p2,mData[2]);
            VECTOR2D_SETY(p2,mData[3]);
            VECTOR2D_SETX(p3,mData[4]);
            VECTOR2D_SETY(p3,mData[5]);
            result = maker->curveToAbsolute(p1,p2,p3);
            break;
        case 'c':
            VECTOR2D_SETX(p1,mData[0]);
            VECTOR2D_SETY(p1,mData[1]);
            VECTOR2D_SETX(p2,mData[2]);
            VECTOR2D_SETY(p2,mData[3]);
            VECTOR2D_SETX(p3,mData[4]);
            VECTOR2D_SETY(p3,mData[5]);
            result = maker->curveToRelative(p1,p2,p3);
            break;
        // A a - elliptical arc (7)
        case 'A':
            // Unsupported. Replace with a line to keep the image closed.
            VECTOR2D_SETX(p1,mData[5]);
            VECTOR2D_SETY(p1,mData[6]);
            result = maker->lineToAbsolute(p1);
            break;
        case 'a':
            // Unsupported. Replace with a line to keep the image closed.
            VECTOR2D_SETX(p1,mData[5]);
            VECTOR2D_SETY(p1,mData[6]);
            result = maker->lineToRelative(p1);
            break;
    }

    if (!result)
        return false;

    return true;
}


///////////////////////////////////////////////////////////////////////////////
// A transformation command in SVG command tree.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGTransformationCommand::SVGTransformationCommand(const MATRIX2D &aMatrix)
{
    mMatrix = aMatrix;
}


//! Executes the command, i.e. applies it to the context.
bool SVGTransformationCommand::execute(SVGContext *aContext)
{
    aContext->setCurrentTransformation(mMatrix);
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// A command representing a document in SVG command tree.
///////////////////////////////////////////////////////////////////////////////

//! Constructor.
SVGDocument::SVGDocument()
{
    mMatrix.makeIdentity();
}


//! Loads the SVG document from a file.
SVGDocument * SVGDocument::load(const char *aFileName)
{
    SVGDocument *svgdoc = NULL;
    TiXmlDocument *xmldoc = new TiXmlDocument();

    TiXmlElement *root = NULL;
    if (xmldoc && xmldoc->LoadFile(aFileName) &&
        (root = xmldoc->RootElement()) != NULL)
    {
        svgdoc = new SVGDocument();
        if (!(svgdoc && createTree(root,svgdoc)))
        {
            delete svgdoc;
            svgdoc = NULL;
        }
    }
    
    delete xmldoc;

    return svgdoc;
}


//! Sets the root level transformation for the SVG document.
void SVGDocument::setTransformation(const MATRIX2D &aMatrix)
{
    mMatrix = aMatrix;
}


//! Executes the command, i.e. applies it to the context.
bool SVGDocument::execute(SVGContext *aContext)
{
    aContext->reset();
    aContext->setCurrentTransformation(mMatrix);
    return SVGCommandSet::execute(aContext);
}


//! Creates the SVG tree from the XML DOM.
bool SVGDocument::createTree(class TiXmlElement *aElement, SVGCommandSet *aSet)
{
    if (STREQ(aElement->Value(),"g"))
    {
        SVGCommandSet *group = new SVGCommandSet(true);
        if (!group)
            return false;

        aSet->addLastChild(group);

        const char *fill = aElement->Attribute("fill");
        const char *fillOpacity = aElement->Attribute("fill-opacity");
        const char *fillRule = aElement->Attribute("fill-rule");

        if (fill)
        {
            SVGCommand *cmd = SVGColorCommand::build(fill);
            if (cmd)
                group->addLastChild(cmd);
        }

        if (fillOpacity)
        {
            SVGCommand *cmd = SVGOpacityCommand::build(fillOpacity);
            if (cmd)
                group->addLastChild(cmd);
        }

        if (fillRule)
        {
            SVGCommand *cmd = SVGFillRuleCommand::build(fillRule);
            if (cmd)
                group->addLastChild(cmd);
        }

        TiXmlElement *child = aElement->FirstChildElement();
        while (child)
        {
            if (!createTree(child,group))
                return false;
            child = child->NextSiblingElement();
        }
    }
    else if (STREQ(aElement->Value(),"path"))
    {
        const char *fill = aElement->Attribute("fill");
        const char *fillOpacity = aElement->Attribute("fill-opacity");
        const char *fillRule = aElement->Attribute("fill-rule");
        const char *data = aElement->Attribute("d");

        SVGCommand *path = createPath(fill,fillOpacity,fillRule,data);

        if (!path)
            return false;

        aSet->addLastChild(path);
    }
    else
    {
        TiXmlElement *child = aElement->FirstChildElement();
        while (child)
        {
            if (!createTree(child,aSet))
                return false;
            child = child->NextSiblingElement();
        }
    }

    return true;
}


//! Creates a path command set from strings.
SVGCommand * SVGDocument::createPath(const char *aFill, const char *aFillOpacity, const char *aFillRule, const char *aPath)
{
    SVGCommandSet *set = new SVGCommandSet(aFill != NULL || aFillOpacity != NULL);

    if (set)
    {
        if (aFill)
        {
            SVGCommand *cmd = SVGColorCommand::build(aFill);
            if (cmd)
                set->addLastChild(cmd);
        }
    
        if (aFillOpacity)
        {
            SVGCommand *cmd = SVGOpacityCommand::build(aFillOpacity);
            if (cmd)
                set->addLastChild(cmd);
        }

        if (aFillRule)
        {
            SVGCommand *cmd = SVGFillRuleCommand::build(aFillRule);
            if (cmd)
                set->addLastChild(cmd);
        }

        if (aPath)
        {
            SVGCommand *cmd = SVGPathCommand::build(aPath);
            if (cmd)
                set->addLastChild(cmd);
            else
            {
                delete set;
                return NULL;
            }
        }
    }

    return set;
}


//! A static method for loading an SVG file (and optionally creating a tesselated dump of it) into a VectorGraphic object.
VectorGraphic * loadSVGtoVectorGraphic(PolygonFactory *aFactory, const char *aPath, const char *aSVGName, const char *aSVGDumpName, const char *aBinaryDumpName)
{
    char *filename = new char[strlen(aPath) + strlen(aSVGName) + 1];

    if (filename == NULL)
        return NULL;

    filename[0] = '\0';

    if (aPath)
    {
        STRCAT(filename,aPath);
    }

    STRCAT(filename,aSVGName);

    SVGDocument *doc = SVGDocument::load(filename);

    delete[] filename;

    if (doc == NULL)
        return NULL;

    ShapeMaker shapeMaker;
    if (!shapeMaker.newShape())
    {
        delete doc;
        return NULL;
    }

    SVGContext context(&shapeMaker);
    
    if (!context.init(100,10))
        return NULL;

    if (!doc->execute(&context))
    {
        delete doc;
        return NULL;
    }

    delete doc;

    const ShapeMaker::ShapeData *shapeData = shapeMaker.getShapeData();

    int polyCount = shapeData->getPolygonCount();

    PolygonWrapper **polys = new PolygonWrapper *[polyCount];
    PaintWrapper **cols = new PaintWrapper *[polyCount];
    RENDERER_FILLMODE *fillModes = new RENDERER_FILLMODE[polyCount];

    bool success = true;
    if (polys == NULL || cols == NULL || fillModes == NULL)
    {
        delete[] polys;
        polys = NULL;
        delete[] cols;
        cols = NULL;
        success = false;
    }

    const PolygonData * const *srcPolys = shapeData->getPolygons();
    const unsigned long *srcCols = shapeData->getColors();
    const RENDERER_FILLMODE *srcFillModes = shapeData->getFillModes();

    if (success && aSVGDumpName != NULL)
    {
        DumpSVG::write(aSVGDumpName,srcPolys,srcCols,srcFillModes,polyCount);
    }

    if (success && aBinaryDumpName != NULL)
    {
        DumpSVG::writeBinary(aBinaryDumpName,srcPolys,srcCols,srcFillModes,polyCount);
    }

    aFactory->beginGraphic();
    int n;
    if (success)
    {
        for (n = 0; n < polyCount; n++)
        {
            polys[n] = aFactory->createPolygonWrapper(srcPolys[n]);
            cols[n] = aFactory->createPaintWrapper(srcCols[n]);
            if (polys[n] == NULL || cols[n] == NULL)
                success = false;
            fillModes[n] = srcFillModes[n];
        }
    }
    aFactory->endGraphic();

    VectorGraphic *graphic = NULL;
    if (success)
    {
        graphic = new VectorGraphic(polys, cols, fillModes, polyCount);
        if (graphic == NULL)
            success = false;
    }

    if (!success)
    {
        if (polys)
        {
            for (n = 0; n < polyCount; n++)
                delete polys[n];
        }
        if (cols)
        {
            for (n = 0; n < polyCount; n++)
                delete cols[n];
        }
        delete[] polys;
        delete[] cols;
        delete[] fillModes;
    }

    return graphic;
}

