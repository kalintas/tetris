#include <iostream>

#include "window.h"
#include "tetris.h"

#undef main


static void PollEvents(window& w, tetris& t)
{
    SDL_Event events;

    if(SDL_PollEvent(&events) != 0)
    { 
        w.mPollEvents(events);

        t.mPollEvents(events);
    }
}


int main()
{       

    const vec2<int> ScreenSize = { 540, 540 };

    window w("selamlar", ScreenSize);

    

    tetris t(w, vec2<int>(20, 20), vec2<int>(20, 20));

    while(w.mIsOpen())
    {   
        w.mStartFrameTime();

        PollEvents(w, t);
        
        t.mUpdate();

        w.mClearWindow(window::colorRGBA<uint8_t>(0, 0, 0, 0xff));
        
        t.mDraw(w);

        w.mShow();

        w.mSleepAsNeeded();
        //SDL_Delay(16);
    }
    
    return 0;
}