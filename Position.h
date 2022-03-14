//#include "files.h"
//#include "cmath"
//#pragma once
//using namespace vex;
//struct Position {
//    //The sensor
//    //inertial& mainSensor;
//    accelerometer& sensX;
//    accelerometer& sensY;
//    gyro& angler;
//    //Keeps track of current acceleration
//    double accX = 0;
//    double accY = 0;
//    //Keeps track of current velocity
//    double velX = 0;
//    double velY = 0;
//    //Keeps track of actual position
//    double posX = 0;
//    double posY = 0;
//
//    double refresh = 0.05;
//    double baseRemX;
//    double baseRemY;
//
//    //Initialize at position 0,0
//    Position(accelerometer& sX, accelerometer& sY, gyro& ang) : sensX(sX), sensY(sY), angler(ang) {
//        /*mainSensor.startCalibration();
//        mainSensor.calibrate();
//        while(mainSensor.isCalibrating()){
//          task::sleep(10);
//        }*/
//    }
//    //Initialize at custom position
//    //Position(inertial& main, double x, double y) : mainSensor(main) {
//        /*mainSensor.startCalibration();
//        mainSensor.calibrate();
//        while(mainSensor.isCalibrating()){
//          task::sleep(10);
//        }*/
//        /*posX = x;
//        posY = y;
//    }*/
//    void setBase() {
//        baseRemX = sensX.acceleration();
//        baseRemY = sensY.acceleration();
//    }
//    double getAcc(double acc, double rem) {
//        if (abs(acc) < abs(rem)) {
//            return 0;
//        }
//        if (acc > 0) {
//            return acc - abs(rem);
//        }
//        else {
//            return -(abs(acc) - abs(rem));
//        }
//    }
//    void update(int& t, int change) {
//        double xAcc = getAcc(sensX.acceleration(), baseRemX) * 32.17405;// *refresh;//mainSensor.acceleration(axisType::xaxis);
//        double yAcc = getAcc(sensY.acceleration(), baseRemY) * 32.17405;// *refresh;//mainSensor.acceleration(axisType::yaxis);
//        //cout << xAcc << ", " << yAcc << endl;
//        //double deltaAccX = xAcc - accX;
//        //double deltaAccY = yAcc - accY;
//        accX = xAcc;
//        accY = yAcc;
//        velX += accX * refresh;
//        velY += accY * refresh;
//        double angle = angler.value(rotationUnits::deg);//mainSensor.yaw();
//        //cout << angle << endl;
//        if (++t == change) {
//            cout << accX << ", " << accY << endl;
//            cout << velX << ", " << velY << endl;
//            cout << angle << endl;
//        }
//        double distX = velX * refresh;
//        double distY = velY * refresh;
//        double sinAngle = sin(angle);
//        double cosAngle = cos(angle);
//        posX += sinAngle * distY;
//        posY += cosAngle * distY;
//        posX += cosAngle * distX;
//        posY += sinAngle * distX;
//        if(t == change)
//        cout << posX << ", " << posY << endl << endl;
//        if (t == change)
//            t = 0;
//    }
//
//};