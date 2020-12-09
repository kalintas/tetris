#include "tetris.h"

#include <new>

tetris::tetris(
    const window& w, 
    const vec2<int>& gridSize, 
    const vec2<int>& gridCornerSpace,
    const uint32_t pieceDropTime
    )
: 
mGridSize(gridSize),
mGridPos(gridCornerSpace)
{

    mBlockSize = (w.mGetSize() - (gridCornerSpace * 2)) / gridSize;
    

    mPieceSpawnPoint = vec2<int>(mGridSize.x / 2, -2);
    mCreateNewPiece(pieceDropTime);

    try
    {
        mGrid = new gridState[mGridSize.x * mGridSize.y];
    }
    catch (std::bad_alloc& ba)
    {
        window::mPrintError(std::string("bad_alloc caught: ") + ba.what());
        return;
    }
    
    int i = 0;

    while(i < mGridSize.x * mGridSize.y)
    {   
        mGrid[i] = gridEmpty;
        ++i;
    }

    mReady = true;
}

tetris::~tetris()
{
    if(mGrid) delete mGrid;
}


inline void tetris::mCreateNewPiece(const uint32_t& timeLimit)
{
    mCurrPiece = DroppingPiece(
        timeLimit, 
        DroppingPiece::BlockType(rand() % 7), 
        mPieceSpawnPoint, 
        rand() % mUsedColorsSize
    );
}

inline bool tetris::mIsInsideGrid(const vec2<int>& v) const
{
    return v.x >= 0 && v.y >= 0 && v.x < mGridSize.x && v.y < mGridSize.y;
}
inline int tetris::mGetPosInGrid(const vec2<int>& v) const
{
    return (v.y * mGridSize.x) + v.x;
}

inline tetris::gridState tetris::mGetFromGridTetromino(const vec2<int>& v) const
{
    if(v.x >= 0 && v.x < mGridSize.x && v.y < mGridSize.y)
    {
        return (v.y <= 0) ? gridEmpty : mGrid[mGetPosInGrid(v)];
    }

    return gridNULL;
}
inline void tetris::mSetGrid(const vec2<int>& v, const gridState& s) 
{ 
    if(mIsInsideGrid(v))
    {
        int&& index = mGetPosInGrid(v); 
        mGrid[index] = s;
    }
}
inline void tetris::mSetBlockPosGrid(const DroppingPiece::DPCord& b, const gridState& s)
{
    mSetGrid(b[0], s);
    mSetGrid(b[1], s);
    mSetGrid(b[2], s);
    mSetGrid(b[3], s);
}

const size_t tetris::mUsedColorsSize = 10;

const tetris::gridState tetris::gridNULL  = -1;
const tetris::gridState tetris::gridEmpty = mUsedColorsSize;

const window::colorRGBA<uint8_t> tetris::mUsedColors[11] = 
{
    //grid colors
    { 247,  37, 133, 0xff },
    { 181,  23, 158, 0xff },
    { 114,   9, 183, 0xff },
    {  86,  11, 173, 0xff },
    {  72,  12, 168, 0xff },
    {  58,  12, 163, 0xff },
    {  63,  55, 201, 0xff },
    {  67,  97, 238, 0xff },
    {  72, 149, 239, 0xff },
    {  76, 201, 240, 0xff },
    //background color
    {  27,   6,  76, 0xff },
};

bool tetris::mDrawGridAt(window& w, const int& i, const window::colorRGBA<uint8_t>& c) const
{
    if(SDL_SetRenderDrawColor(w.mSDL_renderer, c.r, c.g, c.b, c.a) == -1)
    {
        window::mPrintError("SDL_SetRenderDrawColor returned -1");
        return false;
    }

    const SDL_Rect rect = { 
    mGridPos.x + (i % mGridSize.x) * mBlockSize.x,
    mGridPos.y + (i / mGridSize.x) * mBlockSize.y,
    mBlockSize.x, 
    mBlockSize.y 
    };
    
    if(SDL_RenderFillRect(w.mSDL_renderer, &rect) == -1)
    {
        window::mPrintError("SDL_RenderFillRect returned -1");
        return false;
    } 

    return true;
}

bool tetris::mDraw(window& w) const
{

    for(int i = 0; i < mGridSize.x * mGridSize.y; ++i)
    {      

        if(!mDrawGridAt(w, i, mUsedColors[mGrid[i]])) return false;        
    }

    //drawing the dropping piece 
    const DroppingPiece::DPCord& pieceCord = mCurrPiece.mGetCord();
    for(size_t i = 0; i < 4; ++i)
    {
        if(mIsInsideGrid(pieceCord[i]))
        {
            if(!mDrawGridAt(w, mGetPosInGrid(pieceCord[i]),
            mUsedColors[mCurrPiece.mGridState]
            )) return false;
        }
    }

    return true;
}


bool tetris::mCurrPieceMoveIsLegal(const DroppingPiece::DPCord& b) const
{   
    for(size_t i = 0; i < 4; ++i)
    {

        if(mGetFromGridTetromino(b[i]) != gridEmpty) return false;
    }

    return true;
}

bool tetris::mMoveCurrPieceIfValid(const vec2<int>& mov)
{
    DroppingPiece::DPCord newCord;

    mCurrPiece.mFillBlockCord(newCord, mCurrPiece.mGetBaseBlockPos() + mov);

    if(!mCurrPieceMoveIsLegal(newCord)) return false;

    mCurrPiece.mSetBlockCord(newCord);
    
    return true;
}

void tetris::mHardDropPiece()
{

    const vec2<int> piecePos = mCurrPiece.mGetBaseBlockPos();

    int newPosY = piecePos.y;

    while(newPosY < mGridSize.y)
    {   

        DroppingPiece::DPCord newCord;

        mCurrPiece.mFillBlockCord(newCord, vec2<int>(piecePos.x, newPosY));

        if(!mCurrPieceMoveIsLegal(newCord)) break;
        
        ++newPosY;
    }

    --newPosY;

    mCurrPiece.mSetBlockCord(vec2<int>(piecePos.x, newPosY));

    mCurrPiece.mTimer = SDL_GetTicks();
    mCurrPiece.mDropped = true;
}

bool tetris::mFlipCurrPieceIfValid()
{
    DroppingPiece::DPCord newPos;

    mCurrPiece.mFillFlippedBlockCord(newPos);

    if(!mCurrPieceMoveIsLegal(newPos)) return false;

    mCurrPiece.mFlipPiece();
    mCurrPiece.mSetBlockCord(newPos);
    
    return true;
}

void tetris::mClearBlocksIfNeeded()
{
    //meh

    int i = 0;

    while(i < mGridSize.y)
    {

        bool colIsFull = true;

        for(int t = 0; t < mGridSize.x; ++t)
        {

            if(mGrid[(i * mGridSize.x) + t] == gridEmpty)
            {
                colIsFull = false;
                break;
            }
        }

        if(colIsFull)
        {   
            
            if(i > 0)
            {
                int j = i;

                while(j > 0)
                {   
                    for(int t = 0; t < mGridSize.x; ++t)
                    {
                        mGrid[(j * mGridSize.x) + t] = mGrid[((j - 1) * mGridSize.x) + t];
                    }

                    --j;
                }
            }
            else
            {
                for(int t = 0; t < mGridSize.x; ++t)
                {
                    mGrid[(i * mGridSize.x) + t] = gridEmpty;
                }
            }
        }

        ++i;
    }

}

bool tetris::mIsGameOver() const
{
    DroppingPiece::DPCord a;

    vec2<int> v = mPieceSpawnPoint;

    if(v.y < 0) v.y = 0;

    mCurrPiece.mFillBlockCord(a, v);

    return !mCurrPieceMoveIsLegal(a);
}

void tetris::mUpdate()
{
    if(SDL_GetTicks() - mCurrPiece.mTimer < mCurrPiece.mTimeLimit) return;

    mCurrPiece.mTimer = SDL_GetTicks();

    if(mCurrPiece.mDropped)
    {
        mSetBlockPosGrid(mCurrPiece.mGetCord(), mCurrPiece.mGridState);

        mClearBlocksIfNeeded();

        mCreateNewPiece(mCurrPiece.mTimeLimit);

        if((mGameOver = mIsGameOver()))
        { 
            std::cout << "selamm\n";
            system("pause");
        }
        return;
    }

    //if one down is not valid
    //that means piece is falled down
    mCurrPiece.mDropped = !mMoveCurrPieceIfValid(vec2<int>(0, 1));
}

void tetris::mPollEvents(SDL_Event& e)
{   
    if(mCurrPiece.mDropped) return;
    
    if(e.type == SDL_KEYDOWN)
    {
        switch(e.key.keysym.sym)
        {
        case SDLK_z:
        mFlipCurrPieceIfValid();
        break;
        case SDLK_LEFT:
        mMoveCurrPieceIfValid(vec2<int>(-1, 0));
        break;
        case SDLK_RIGHT:
        mMoveCurrPieceIfValid(vec2<int>(1, 0));
        break;
        case SDLK_DOWN:
        mMoveCurrPieceIfValid(vec2<int>(0, 1));
        break;
        case SDLK_SPACE:
        mHardDropPiece();
        break;
        }
    }

}