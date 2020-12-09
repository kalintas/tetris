#pragma once

#include "window.h"


class tetris
{
public:

    tetris(
        const window& w, 
        const vec2<int>& gridSize, 
        const vec2<int>& gridCornerSpace, 
        const uint32_t pieceDropTime = 300
    );

    ~tetris();


private:    

    typedef int8_t gridState;
private:

//DroppingPiece class declaration
class DroppingPiece
{
public:
    typedef vec2<int> DPCord[4];

    enum BlockType
    {
        Block_I = 0,
        Block_J = 1,
        Block_L = 2,
        Block_O = 3,
        Block_S = 4,
        Block_T = 5,
        Block_Z = 6
    };

    DroppingPiece () {}
    DroppingPiece(
        const uint32_t& timeLimit, 
        const BlockType& blockType,
        const vec2<int>& startingPos,
        const uint8_t& colorIndex
    );

    ~DroppingPiece() {}

    void mSetType(const BlockType& t);

    void mSetBlockCord(const vec2<int>& newPos);
    void mSetBlockCord(const DPCord& a);
    
    void mFillBlockCord       (DPCord& a, const vec2<int>& newPos) const;
    void mFillFlippedBlockCord(DPCord& a) const;
    
    void mFlipPiece();

    const DPCord& mGetCord() const;

    vec2<int> mGetBaseBlockPos() const;

public:

    uint32_t mTimer = 0;
    uint32_t mTimeLimit;

    gridState mGridState;

    bool mDropped = false;

private:

    typedef void (tetris::DroppingPiece::*flipFunc)(vec2<int>&, vec2<size_t>&) const;

    static const size_t mBaseBlockIndex;

    static const vec2<int> TetrominoCords[7][4];

    DPCord mBlockCord;

    // default  = +x, +y
    vec2<int>    mCordSign = { 1, 1 };
    // default =  x,  y
    vec2<size_t> mCordOrder = { 0, 1 };

    //piece type
    BlockType mBlockType;

    flipFunc mFlipFunc;

    static vec2<int> mGetTetrominoPosProcessed(
        const size_t& i,
        const BlockType& mBlockType,
        const vec2<int>& rotation,
        const vec2<size_t>& cordOrder
    );

    void mGetFlippedBlock_O(
        vec2<int>& rotation, 
        vec2<size_t>& cordOrder
    ) const;

    void mGetFlippedGeneral(
        vec2<int>& rotation, 
        vec2<size_t>& cordOrder
    ) const;

};
////DroppingPiece class
    
private:

    bool mDrawGridAt(window& w, const int& i, const window::colorRGBA<uint8_t>& c) const;

public:

    bool mDraw(window& w) const;

    void mUpdate();

    void mPollEvents(SDL_Event& e);

private:

    vec2<int> mGridSize;
    vec2<int> mGridPos;
    vec2<int> mBlockSize;

    vec2<int> mPieceSpawnPoint;

    gridState* mGrid;

    bool mReady = false;

    bool mGameOver = false;

    static const gridState gridNULL;
    static const gridState gridEmpty;

    static const size_t mUsedColorsSize;
    static const window::colorRGBA<uint8_t> mUsedColors[11];

private:
    
    bool mIsInsideGrid(const vec2<int>& v) const;
    int  mGetPosInGrid(const vec2<int>& v) const;

    gridState mGetFromGridTetromino(const vec2<int>& v) const;

    void mSetGrid        (const vec2<int>& v, const gridState& s);
    void mSetBlockPosGrid(const DroppingPiece::DPCord& b, const gridState& s);

    
    void mClearBlocksIfNeeded();
    
    void mCreateNewPiece(const uint32_t& timeLimit);


    bool mIsGameOver() const;

private:

    DroppingPiece mCurrPiece;

    bool mCurrPieceMoveIsLegal(const DroppingPiece::DPCord& b) const;

    bool mMoveCurrPieceIfValid(const vec2<int>& mov);

    bool mFlipCurrPieceIfValid();

    void mHardDropPiece();

};