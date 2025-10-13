#include <X11/Xlib.h>
#include <iostream>

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Unable to open X display" << std::endl;
        return 1;
    } else {
        std::cout << "X display opened successfully" << std::endl;
    }

    Window root = DefaultRootWindow(display);

    int x, y;
    Window returned_root, returned_child;
    int win_x, win_y;
    unsigned int mask_return;
    while (true) {
        if (XQueryPointer(display, root, &returned_root, &returned_child, &x, &y, &win_x, &win_y, &mask_return)) {
            std::cout << "Cursor Position: (" << x << ", " << y << ")" << std::endl;
        } else {
            std::cerr << "Failed to query pointer, retrying..." << std::endl;
            break;

        }
    }

    XCloseDisplay(display);

    return 0;
}