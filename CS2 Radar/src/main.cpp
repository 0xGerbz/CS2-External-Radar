#include "gui.h"
#include "memory.h"
#include "game.h"

int main(){
    //Needed for WICTextureLoader
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    //Initialize GUI and call our render function
    GUI gui;
    gui.initializeGUI();
    //Attach to our process to prepare for memory reads
    memory::attachToProcess();
    //Testing our getMap function
    gui.Render();
    CoUninitialize();
    return 0;
}