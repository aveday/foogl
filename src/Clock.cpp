#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

#include "Clock.h"

Clock::Clock()
{
    startTime = glfwGetTime();
    time = startTime;
    
    fps_sample_index = 0;
    for(int i = 0; i < SAMPLES_FOR_AVERAGE; i++)
        fps_samples[i] = 0;
}

float Clock::tick() {

    if(glfwGetTime() - time < 1.0f/FPS_CAP)
    {
        int ms = 1000 * ( time + 1.0f/FPS_CAP - glfwGetTime() );
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    float newTime = glfwGetTime();
    float dt = newTime - time;
    time = newTime;

    if(LOG_FPS)
    {
        // log FPS
        fps_samples[fps_sample_index] = 1.0f / dt;
        fps_sample_index = (fps_sample_index + 1)%SAMPLES_FOR_AVERAGE;

        // calculate average FPS
        if(!fps_sample_index)
        {
            float sum = 0;
            for(int i = 0; i < SAMPLES_FOR_AVERAGE; i++)
                sum += fps_samples[i];
            std::cout << (int)(sum / SAMPLES_FOR_AVERAGE) << std::endl;
        }
    }
    return dt;
}
