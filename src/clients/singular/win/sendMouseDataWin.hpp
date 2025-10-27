#ifndef sendMouse
#define sendMouse

#include <string>

#include <winsock2.h>
#include <windows.h>

class sendMouseData {
    public:
        POINT pt;

        SOCKET socket;
        const char* message;

        sendMouseData(SOCKET hsock) {
            socket = hsock;
        }

        bool sendCursor() {
            if (GetCursorPos(&pt)) {
                message = (std::to_string(pt.x) + "," + std::to_string(pt.y)).c_str();
                if (!send(socket, message, (int)strlen(message), 0) < 1) {
                    return true;
                }
            }
            return false;
        }
};

#endif // sendMouse