#pragma once
#include <typeinfo>
//
//void gyroInit(gyro& Gyro){
//    //Calibrate the gyro  
//    
//    Gyro.startCalibration();
//    task::sleep(200);
//    Gyro.calibrate();
//    //task::sleep(4000);
//    cout << "Calibrating Gyro..." << flush;
//    while (Gyro.isCalibrating()) {
//        task::sleep(100);
//        cout << "." << flush;
//    }
//    cout << endl;
//    task::sleep(500);
//}
//void gyroInit(inertial& Gyro){
//    if (Gyro.installed()) {
//        //Calibrate the gyro  
//        Gyro.startCalibration();
//        task::sleep(200);
//        Gyro.calibrate();
//        cout << "Calibrating Inertial Sensor..." << flush;
//        while (Gyro.isCalibrating()) {
//            task::sleep(100);
//            cout << "." << flush;
//        }
//        cout << endl;
//        task::sleep(500);
//    }
//    else {
//        cout << "No Inertial Sensor" << endl;
//    }
//}
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
