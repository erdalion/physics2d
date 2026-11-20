#ifndef VIDEO_HANDLER_H
#define VIDEO_HANDLER_H

#include "raylib.h"
#include <algorithm>

class VideoHandler
{
private:
    bool playing = false;
    int frame = 1;
    float frameElapsed = 0.0f;
    Image frameImage{};

    void clearFrame()
    {
        if (frameImage.data)
            UnloadImage(frameImage);
        frameImage = {};
    }

    void loadNextFrame()
    {
        clearFrame();
        frameImage = LoadImage(TextFormat("%sframes/frame_%04d.png", GetApplicationDirectory(), frame));
        if (frameImage.data)
            frame++;
        frameElapsed = 0.0f;
    }

public:
    ~VideoHandler() { clearFrame(); }

    void toggle(float frameDuration)
    {
        playing = !playing;
        frame = 1;
        frameElapsed = playing ? frameDuration : 0.0f;
        clearFrame();
    }

    void reset()
    {
        playing = false;
        frame = 1;
        frameElapsed = 0.0f;
        clearFrame();
    }

    void update(float frameDuration)
    {
        if (!playing)
            return;

        frameElapsed += GetFrameTime();
        if (!frameImage.data || frameElapsed >= frameDuration)
            loadNextFrame();
    }

    Color getColor(float x, float y, float screenWidth, float screenHeight, Color fallback) const
    {
        if (!frameImage.data)
            return fallback;

        int pixelX = std::clamp(static_cast<int>((x / screenWidth) * frameImage.width), 0, frameImage.width - 1);
        int pixelY = std::clamp(static_cast<int>((y / screenHeight) * frameImage.height), 0, frameImage.height - 1);

        Color pixel = GetImageColor(frameImage, pixelX, pixelY);
        return pixel.r > 128 ? WHITE : BLACK;
    }
};

#endif