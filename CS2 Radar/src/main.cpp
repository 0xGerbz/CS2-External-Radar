#include "gui.h"


int main(){
    //Needed for WICTextureLoader
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    //Initialize GUI and call our render function
    gui::initializeGUI();
    gui::Render();
    CoUninitialize();
    return 0;
}