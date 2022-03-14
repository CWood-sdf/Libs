#pragma once
#include <typeinfo>
template<class Tp>
void gyroInit(Tp& Gyro) {
    if (Gyro.installed()) {
        //Calibrate the gyro  
        Gyro.startCalibration();
        task::sleep(200);
        Gyro.calibrate();
        cout << "Calibrating Sensor..." << flush;
        while (Gyro.isCalibrating()) {
            task::sleep(100);
            cout << "." << flush;
        }
        cout << endl;
        task::sleep(500);
    }
    else {
        cout << "No Sensor" << endl;
    }
}