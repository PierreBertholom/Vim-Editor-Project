#include "Editor.hpp"
#include "NcursesView.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    //freopen( "output.txt", "w", stdout );
    Editor myEditor;

    if (argc > 1) {
        std::string filepath = argv[1];
        
        if (!myEditor.loadFile(filepath)) {
            myEditor.saveFileAs(filepath);
        }
    }
    
    NcursesView view(myEditor);
    view.run();

    return 0;
}