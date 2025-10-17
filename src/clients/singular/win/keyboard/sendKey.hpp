#ifndef sendKey
#define sendKey

#include <string>

#include <winsock2.h>
#include <windows.h>

class sendInput {
    public:
        std::string key;
        SOCKET socket;
        const char* mssg = key.c_str();
        char keyInp = key[0];
        int vKey;
        sendInput(std::string str, SOCKET hsock, int VK) {
            key = str;
            socket = hsock;
            vKey = VK;
        }

        bool sendChar() {
            if ((GetAsyncKeyState(keyInp) & 0x8000) && send(socket, mssg, (int)strlen(mssg), 0) > 0) {
                return true;
            } else {
                return false;
            }
        }

        bool sendFunc() {
            if ((GetAsyncKeyState(vKey) & 0x8000) && send(socket, mssg, (int)strlen(mssg), 0) > 0) {
                return true;
            } else {
                return false;
            }
        }
};

#endif // sendKey