#pragma once
using namespace std;
#include "fstream"
ofstream auton_code;
fstream byte_code;
#define microsec Brain.Timer.systemHighResolution()
#define microSleep(time)                      \
if(1){int start = microsec;              \
while (microsec - start < time);}
#define autSV 150
#define autS s(autSV); 
#define sm(time) microSleep(time)
#define sampleRate 100
#define APA_TIMER_END    \
if(driveMode == "auton"){   \
    s(sampleRate);   \
    int time = Brain.timer(timeUnits::msec);   \
    APA_TIMER_START \
    if (time == autSV) {   \
        auton_write(false, " autS ");   \
    }   \
    else   \
    auton_write("s(", parseInt(time), ");  ", false);   \
    if (++times == 10) {   \
        auton_write();   \
        times = 0;   \
    }   \
}   \
if (Greg.ButtonX.pressing() && driveMode == "auton" && auton_code.is_open()) {   \
    auton_code.close();   \
}   

#define APA_TIMER_START Brain.setTimer(0, timeUnits::msec);

int times = 0;
template<typename Number>
string parseInt(Number n) {

    char str[20];
    if (n < 0) {
        string ret = "-";
        ret += parseInt(abs(n));
        return ret;
    }
    string buf = itoa(n, str, 10);
    return buf;
}

template<>
string parseInt<string>(string n) {
    return n;
}
template<>
string parseInt<const char*>(const char* n) {
    return n;
}
string nl = "\r\n";

bool inserted = false;
string stream = "";
void streamPush(string data) {
    if(driveMode == "auton"){
        cout << data << flush;
        if(inserted)
        auton_code << data << flush;
        //stream += data;

    }
}
void write() {
    //
    if (inserted) {
        //cout << stream << flush;
        const char* update = stream.data();
        cout << update << flush;
        auton_code << update << flush;
    }
    stream = "";
}
void auton_write (){
    if(!Greg.ButtonA.pressing() && driveMode == "auton"){
        streamPush(nl);
    }
}
template<typename... Args>
void auton_write(bool en, Args... args) {
    vector<string> arr = { parseInt(args)... };
    string write;
    for (auto i : arr) {
        write += i;
    }
    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
        streamPush(write);
    }
}
template<typename... Args>
void auton_write(string en, Args... args) {
    vector<string> arr = { parseInt(args)... };
    string write = nl + en;
    for (auto i : arr) {
        write += i;
    }
    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
        streamPush(write);
    }
}
template<typename... Args>
void auton_write(int en, Args... args) {
    vector<string> arr = { parseInt(args)... };
    string write = nl + parseInt(en);
    for (auto i : arr) {
        write += i;
    }
    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
        streamPush(write);
    }
}
//void auton_write(string arg1, string arg2) {
//    string write = nl + arg1 + arg2;
//    int length = write.length();
//    times = 0;
//    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
//        streamPush(write);
//    }
//}
//void auton_write(string arg1, string arg2, string arg3, bool en) {
//    string write = nl + arg1 + arg2 + arg3;
//    int length = write.length();
//    times = 0;
//    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
//        streamPush(write);
//    }
//}
//void auton_write(string arg1, string arg2, string arg3) {
//    string write = nl + arg1 + arg2 + arg3;
//    int length = write.length();
//    times = 0;
//    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
//        streamPush(write);
//    }
//}
//void auton_write(string arg1, string arg2, string arg3, string arg4) {
//    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
//        streamPush(nl + arg1 + arg2 + arg3 + arg4);
//    }
//}
//
//
//void auton_write(bool en, string arg) {
//    string write = arg;
//    int length = write.length();
//    if (en != false) {
//
//        times = 0;
//    }
//    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
//        streamPush(write);
//    }
//}
//void auton_write(bool en, string arg1, string arg2) {
//    string write = arg1 + arg2;
//    int length = write.length();
//    if (en != false) {
//
//        times = 0;
//    }
//    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
//        streamPush(write);
//    }
//}
//void auton_write(string arg1, string arg2, string arg3, int en) {
//    string write = arg1 + arg2 + arg3;
//    int length = write.length();
//    if (en != false) {
//
//        times = 0;
//    }
//    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
//        streamPush(write);
//    }
//}
//
//void auton_write() {
//    string write = nl + nl;
//    int length = write.length();
//    times = 0;
//    if (!Greg.ButtonA.pressing() && driveMode == "auton") {
//        streamPush(write);
//    }
//}
string accessFile = "";
string byteCodes = "";
void autonCodeSet (string autonType){
    string baseFileName = "Auton/auton_type_";
    string fileType = ".h";
    string fileType2 = ".bin";
    driveMode = "auton";
    //string fileName;
    accessFile = baseFileName + autonType + fileType;
    string access = baseFileName + autonType + fileType2;
    inserted = Brain.SDcard.isInserted();
    Greg.rumble(".");
    cout << accessFile << endl;
    cout << access << endl;
    cout << inserted << endl;
    if(inserted){
      
    
    
        auton_code.open(accessFile.data(), std::fstream::out);
        if(auton_code.is_open()){
            cout << "// all good in the file stream 'hood" << endl;
            Greg.rumble("..");
        }
        else {
            cout << "// THE FILE STREAM FAILED (C++ SUCKS)" << endl;
        }
        auton_code.seekp(0, ios_base::beg);
    }
    else {
        cout << "//STREAM FAIL: NO CARD" << endl;
    }
  
    inserted = inserted && auton_code.good();
    cout << inserted << endl;
}

void endAPA() {
    cout << "Ending APA connection" << endl;
    
    auton_code.close();
    byte_code.close();
    inserted = false;
}
