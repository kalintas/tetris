#include "tetris.h"


const size_t tetris::DroppingPiece::mBaseBlockIndex = 0;

//tetromino block coordinates based on mBlockCord[mBaseBlockIndex]
const vec2<int> tetris::DroppingPiece::TetrominoCords[7][4] =
{
    { {0,  0}, {-1,  0}, {1 , 0}, {2,  0} }, // I-block
    { {0,  0}, {-1, -1}, {-1, 0}, {1,  0} }, // J-block 
    { {0,  0}, {-1,  0}, {1 , 0}, {1, -1} }, // L-block
    { {0,  0}, {1 ,  0}, {0 , 1}, {1,  1} }, // O-block
    { {0,  0}, {-1,  1}, {0 , 1}, {1,  0} }, // S-block
    { {0,  0}, {-1,  0}, {0 , 1}, {1,  0} }, // T-block
    { {0,  0}, {-1,  0}, {0 , 1}, {1,  1} }  // Z- block
};


tetris::DroppingPiece::DroppingPiece(
    const uint32_t& timeLimit, 
    const BlockType& blockType,
    const vec2<int>& startingPos,
    const uint8_t& colorIndex
) : mTimeLimit(timeLimit), mGridState(colorIndex)
{   
    mSetType(blockType);

    mSetBlockCord(startingPos);

}

void tetris::DroppingPiece::mSetType(const BlockType& t)
{
    mBlockType = t;

    if(mBlockType == Block_O) mFlipFunc = mGetFlippedBlock_O;
    else mFlipFunc = mGetFlippedGeneral;
}

vec2<int> tetris::DroppingPiece::mGetTetrominoPosProcessed(
    const size_t& i,
    const BlockType& mBlockType,
    const vec2<int>& rotation, 
    const vec2<size_t>& cordOrder
)
{
    return 
    vec2<int>(
    TetrominoCords[mBlockType][i][cordOrder.x], 
    TetrominoCords[mBlockType][i][cordOrder.y]) 
    * rotation;
}

void tetris::DroppingPiece::mFillBlockCord(DPCord& a, const vec2<int>& newPos) const
{

    for(size_t i = 0; i < 4; ++i)
    {
        a[i] = newPos + mGetTetrominoPosProcessed(i, mBlockType, mCordSign, mCordOrder);
    }
}

void tetris::DroppingPiece::mFillFlippedBlockCord(DPCord& a) const
{   
    
    vec2<int>    rotation;
    vec2<size_t> cordOrder;

    (this->*mFlipFunc)(rotation, cordOrder);

    for(size_t i = 0; i < 4; ++i)
    {
        a[i] = mBlockCord[mBaseBlockIndex] + mGetTetrominoPosProcessed(i, mBlockType, rotation, cordOrder);
    }
}


void tetris::DroppingPiece::mSetBlockCord(const vec2<int>& newPos)
{
    mFillBlockCord(mBlockCord, newPos);
}

void tetris::DroppingPiece::mSetBlockCord(const DPCord& a)
{
    mBlockCord[0] = a[0];
    mBlockCord[1] = a[1];
    mBlockCord[2] = a[2];
    mBlockCord[3] = a[3];
}

const tetris::DroppingPiece::DPCord& tetris::DroppingPiece::mGetCord() const { return mBlockCord; }

void tetris::DroppingPiece::mFlipPiece()
{
    (this->*mFlipFunc)(mCordSign, mCordOrder);
}

void tetris::DroppingPiece::mGetFlippedBlock_O(
    vec2<int>& rotation, 
    vec2<size_t>& cordOrder
) const
{
    rotation = mCordSign;
    cordOrder = mCordOrder;
}

void tetris::DroppingPiece::mGetFlippedGeneral(
    vec2<int>&    rotation, 
    vec2<size_t>& cordOrder
) const
{
    // x2 = y1, y2 = -x1

    cordOrder = vec2<size_t>(1, 1) - mCordOrder;
    rotation  = vec2<int>(mCordSign.y, -mCordSign.x);
}   


vec2<int> tetris::DroppingPiece::mGetBaseBlockPos() const
{
    return mBlockCord[mBaseBlockIndex];
}