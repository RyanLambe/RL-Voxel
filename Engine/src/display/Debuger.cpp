#include "Debuger.h"

using namespace Display;

bool Debuger::debugMode;
HANDLE Debuger::hOut;

std::ofstream Debuger::file;

void Debuger::start(bool debug)
{
    debugMode = debug;
    if (debugMode) {
        //setup console
        AllocConsole();
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        //setup font
        CONSOLE_FONT_INFOEX font = {};
        font.cbSize = sizeof(font);
        font.dwFontSize.Y = 24;
        font.FontWeight = FW_NORMAL;
        wcscpy_s(font.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hOut, false, &font);
    }

    //file logging
    file.open("log.txt", std::ios::trunc);
}

Debuger::~Debuger()
{
    if (debugMode)
        FreeConsole();
    file.close();
}

//logs
void Debuger::log(std::string out)
{
    std::string line = "INFO " + formatedTime() + out + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(char out[])
{
    std::string line = "INFO " + formatedTime() + out + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(const char out[])
{
    std::string line = "INFO " + formatedTime() + out + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(int out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(unsigned int out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(float out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(long out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(double out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(long long out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(long double out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(unsigned long out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(unsigned long long out)
{
    std::string line = "INFO " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(bool out)
{
    std::string line;
    if(out)
        line = "INFO " + formatedTime() + "True" + "\n";
    else
        line = "INFO " + formatedTime() + "False" + "\n";

    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(Math::Vec2 out)
{
    std::string line = "INFO " + formatedTime();
    line += "x: " + std::to_string(out.x);
    line += ", y: " + std::to_string(out.y) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debuger::log(Math::Vec3 out)
{
    std::string line = "INFO " + formatedTime();
    line += "x: " + std::to_string(out.x);
    line += ", y: " + std::to_string(out.y);
    line += ", z: " + std::to_string(out.z) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

//errors
void Debuger::logError(std::string out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + out + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(char out[], int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + out + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(const char out[], int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + out + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(int out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(unsigned int out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(float out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(long out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(double out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(long long out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(long double out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(unsigned long out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(unsigned long long out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime() + std::to_string(out) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(bool out, int lineNum, std::string file)
{
    std::string line;
    if (out)
        line = "ERROR " + formatedTime() + "True" + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    else
        line = "ERROR " + formatedTime() + "False" + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";

    print(line, FOREGROUND_RED);
}

void Debuger::logError(Math::Vec2 out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime();
    line += "x: " + std::to_string(out.x);
    line += ", y: " + std::to_string(out.y) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logError(Math::Vec3 out, int lineNum, std::string file)
{
    std::string line = "ERROR " + formatedTime();
    line += "x: " + std::to_string(out.x);
    line += ", y: " + std::to_string(out.y);
    line += ", z: " + std::to_string(out.z) + " - [Line: " + std::to_string(lineNum) + ", File: " + file + "]" + "\n";
    print(line, FOREGROUND_RED);
}

void Debuger::logErrorCode(HRESULT code, int lineNum, std::string file)
{
    if (FAILED(code))
        logError(TranslateHResult(code), lineNum, file);
}

//warnings
void Debuger::logWarning(std::string out)
{
    std::string line = "WARNING " + formatedTime() + out + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(char out[])
{
    std::string line = "WARNING " + formatedTime() + out + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(const char out[])
{
    std::string line = "WARNING " + formatedTime() + out + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(int out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(unsigned int out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(float out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(long out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(double out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(long long out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(long double out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(unsigned long out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(unsigned long long out)
{
    std::string line = "WARNING " + formatedTime() + std::to_string(out) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(bool out)
{
    std::string line;
    if (out)
        line = "WARNING " + formatedTime() + "True" + "\n";
    else
        line = "WARNING " + formatedTime() + "False" + "\n";

    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(Math::Vec2 out)
{
    std::string line = "WARNING " + formatedTime();
    line += "x: " + std::to_string(out.x);
    line += ", y: " + std::to_string(out.y) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}

void Debuger::logWarning(Math::Vec3 out)
{
    std::string line = "WARNING " + formatedTime();
    line += "x: " + std::to_string(out.x);
    line += ", y: " + std::to_string(out.y);
    line += ", z: " + std::to_string(out.z) + "\n";
    print(line, FOREGROUND_RED | FOREGROUND_GREEN);
}


//helper functions
std::string Debuger::TranslateHResult(HRESULT code)
{
    //reformat message
    char* msgBuffer = nullptr;
    DWORD msgLength = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&msgBuffer), 0, nullptr);

    //check if there is a message
    if (msgLength == 0)
        return "Unknown Exception";

    //return
    std::string out = msgBuffer;
    LocalFree(msgBuffer);
    return out;
}

std::string Debuger::formatedTime()
{
    //get values
    int secs = (int)Time::getTime();
    int mins = (int)floor(secs / 60);
    secs -= mins * 60;

    //format string
    std::string out = "[";
    out += std::to_string(mins);
    out += ":";

    //make seconds 2 digits
    if (secs < 10)
        out += "0";

    out += std::to_string(secs);
    out += "]: ";

    //return
    return out;
}

void Debuger::print(std::string out, WORD colour)
{
    //write to console
    if (true) {
        //set correct font
        SetConsoleTextAttribute(hOut, colour);

        //print to console
        WriteConsoleA(hOut, out.c_str(), (DWORD)out.size(), nullptr, nullptr);
    }

    //file logging
    file << out;
}