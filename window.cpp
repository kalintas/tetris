#include "window.h"


window::window(const std::string& sTitle, const vec2<int>& windowSize, const uint32_t initFlags, const uint32_t windowFlags, const uint32_t rendererFlags)
{

    if(!mInitSDL(initFlags)) return;
    
    mSDL_window = SDL_CreateWindow(
        sTitle.c_str(), 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        windowSize.x,
        windowSize.y,
        windowFlags
    );

    if(!mSDL_window)
    {   
        mPrintError("Cant Create SDL_Window");
        return;
    }

    mSDL_renderer = SDL_CreateRenderer(
        mSDL_window,
        -1,
        rendererFlags
    );

    if(!mSDL_renderer)
    {
        mPrintError("Cant Create SDL_Renderer");
        return;
    }

    mWindowSize = windowSize;

    mOpen = true;
}

window::~window()
{
    SDL_Quit();
    if(mSDL_window)   SDL_DestroyWindow(mSDL_window);
    if(mSDL_renderer) SDL_DestroyRenderer(mSDL_renderer);
}


bool window::mInitSDL(const uint32_t flags) const
{

    if(SDL_Init(flags) != 0)
    {
        mPrintError("SDL_Init failed");
        return false;
    }

    return true;
}


bool window::mClearWindow(const colorRGBA<uint8_t>& c) const
{
    if(SDL_SetRenderDrawColor(mSDL_renderer, c.r, c.g, c.b, c.a) == -1) 
    {
        mPrintError("SDL_SetRenderDrawColor returned -1");
        return false;
    }

    if(SDL_RenderClear(mSDL_renderer) == -1)
    {
        mPrintError("SDL_RenderClear returned -1");
        return false;
    }

    return true;
}


void window::mPrintError(const std::string& sError, const char* funcName)
{
    std::cerr 
    << "Error: function " << funcName 
    << "gived: " << sError 
    << " SDL_GetError() =" << SDL_GetError() << "\n"; 
}


void window::mPollEvents(SDL_Event& e)
{

    mOpen = !(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE));
}

bool window::mSleepAsNeeded(const uint32_t frameLock)
{   
    if(frameLock == 0) return false;

    const uint32_t timePerFrame = 1000 / frameLock;

    if(mFrameStart < timePerFrame) SDL_Delay(timePerFrame - mFrameStart);

    return true;
}