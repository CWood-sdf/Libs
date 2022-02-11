//#include "robot-config.h"
//#pragma once
int stillChoosing;
string driveMode;
string confirm;

string autonType;

const color Green = color(0, 255, 0);
const color green2 = color(0x00ff00);
color gray = color(150, 150, 150);
Button2 setRedBig = Button2(Brain, 0, 0, 150, 100, autonType, "red-big(left)", red, "big");
Button2 setBlueBig = Button2(Brain, 0, 100, 150, 100, autonType, "blue-big", blue, "big");
Button2 setRedSmall = Button2(Brain, 150, 0, 100, 100, autonType, "red-small(right)", red, "small");
Button2 setBlueSmall = Button2(Brain, 150, 100, 100, 100, autonType, "blue-small", blue, "small");
Button2 setSkills = Button2(Brain, 250, 100, 100, 100, autonType, "skills", gray, "skills");
Button2 setDriveNorm = Button2(Brain, 0, 0, 240, 272, driveMode, "normal", red, "normal");
Button2 setDriveAuton = Button2(Brain, 240, 0, 240, 272, driveMode, "auton", blue, "auton");
Button2 setConfirm = Button2(Brain, 240, 0, 240, 272, confirm, "c", green, "confirm");
Button2 setDeny = Button2(Brain, 0, 0, 240, 272, confirm, "d", red, "deny");

void initialize() {
    
    cout << "// selecting auton type ..." << endl;
    //int h = 0;
AUTON_START:
    while (Brain.Screen.pressing());
    task::sleep(500);
    Brain.Screen.clearScreen(black);
    Brain.Screen.waitForRefresh();
    setBlueBig.draw();
    setBlueSmall.draw();
    setRedSmall.draw();
    setRedBig.draw();
    setSkills.draw();
    while (1) {
        if (Brain.Screen.pressing()) {
            int x = Brain.Screen.xPosition();
            int y = Brain.Screen.yPosition();
            cout << "// " << x << ", " << y << endl;
            setBlueBig.click();
            setBlueSmall.click();
            setRedSmall.click();
            setRedBig.click();
            setSkills.click();
            if (autonType != " ")
                break;
        }
    }

    cout << "// auton type selected: " << autonType << endl;
    //
    while (Brain.Screen.pressing());
    task::sleep(500);
    Brain.Screen.clearScreen(black);
    Brain.Screen.waitForRefresh();
    
    setConfirm.draw();
    setDeny.draw();
    Brain.Screen.setFillColor(gray);
    Brain.Screen.printAt(200, 100, autonType.data());
    while (1) {
        if (Brain.Screen.pressing()) {
            int x = Brain.Screen.xPosition();
            int y = Brain.Screen.yPosition();
            cout << "// " << x << ", " << y << endl;
            if (x > 240) {
                goto DRIVER_START;
            }
            else {
                goto AUTON_START;
            }
        }
    }
DRIVER_START:
    while (Brain.Screen.pressing());
    task::sleep(500);
    setDriveAuton.draw();
    setDriveNorm.draw();

    
    //Brain.Screen.printAt(10, 50, "normal");
    //Brain.Screen.printAt(300, 50, "auton");
    while (1) {
        if (Brain.Screen.pressing()) {
            int x = Brain.Screen.xPosition();
            int y = Brain.Screen.yPosition();
            cout << "// " << x << ", " << y << endl;
            setDriveNorm.click();
            setDriveAuton.click();
            if (driveMode != " ")
                break;
        }
    }

    cout << "// drive mode selected: " << driveMode << endl;
    
    while (Brain.Screen.pressing()) {}
    task::sleep(500);
    Brain.Screen.clearScreen(black);
    setConfirm.draw();
    setDeny.draw();
    Brain.Screen.setFillColor(gray);
    Brain.Screen.printAt(200, 100, driveMode.data());
    while (1) {
        if (Brain.Screen.pressing()) {
            int x = Brain.Screen.xPosition();
            int y = Brain.Screen.yPosition();
            std::cout << "// " << x << ", " << y << std::endl;
            if (x > 240)
                break;
            else
                goto DRIVER_START;
        }
    }
    
    task::sleep(1000);
    while (Brain.Screen.pressing());
    stillChoosing++;
cout << driveMode << endl;
}
