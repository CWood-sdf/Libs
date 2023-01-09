#ifndef GYROINIT_H
#define GYROINIT_H
#include <iostream>
namespace vex
{
    class task;
}
template <class Tp>
void gyroInit(Tp& Gyro)
{
    if (Gyro.installed())
    {
        // Calibrate the gyro
        Gyro.startCalibration();
        vex::task::sleep(200);
        Gyro.calibrate();
        // Give the nice "..."" in the terminal
        std::cout << "Calibrating Sensor..." << std::flush;
        while (Gyro.isCalibrating())
        {
            vex::task::sleep(100);
            std::cout << "." << std::flush;
        }
        std::cout << std::endl;
        vex::task::sleep(500);
    }
    else
    {
        std::cout << "No Sensor" << std::endl;
    }
}

#endif // GYROINIT_H
