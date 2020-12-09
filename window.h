#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <iostream>

#include <stdint.h>

#include "vec2.h"



class window
{
public:

    window(const std::string& sTitle, const vec2<int>& windowSize, const uint32_t initFlags = 0, const uint32_t windowFlags = 0, const uint32_t rendererFlags = 0);

    ~window();

    template<typename T>
    struct colorRGBA
    {
        T r = 0;
        T g = 0;
        T b = 0;
        T a = 0;

        colorRGBA() {}
        colorRGBA(const T& red, const T& green, const T& blue, const T& alpha)
        {
            r = red, g = green, b = blue, a = alpha;
        }
        colorRGBA(const colorRGBA<T>& c)
        {
            *this = c;
        }

        ~colorRGBA() {}

    };


    bool mClearWindow(const colorRGBA<uint8_t>& c) const;

    inline void mShow() { SDL_RenderPresent(mSDL_renderer); }

    

    inline void mStartFrameTime() { mFrameStart = 0; }

    bool mSleepAsNeeded(const uint32_t frameLock = 60);

public:

    void mPollEvents(SDL_Event& e);

    static void mPrintError(const std::string& sError, const char* funcName = __builtin_FUNCTION());

public:

    SDL_Renderer* mSDL_renderer = nullptr;

public:

    inline const bool&      mIsOpen () const { return mOpen;       }
    inline const vec2<int>& mGetSize() const { return mWindowSize; }

private:

    vec2<int> mWindowSize;

    SDL_Window* mSDL_window = nullptr;

    bool mOpen = false;

private:

    uint32_t mFrameStart = 0;

private:

    bool mInitSDL(const uint32_t flags = 0) const;
};
