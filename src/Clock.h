#pragma once

static const bool LOG_FPS = false;
static const int SAMPLES_FOR_AVERAGE = 10;
static const int FPS_CAP = 60;

class Clock
{
    public:
        Clock();
        float tick();

    private:
        float startTime, time;
        int fps_sample_index;
        float fps_samples[SAMPLES_FOR_AVERAGE];
};

