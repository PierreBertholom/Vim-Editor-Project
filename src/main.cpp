#include "Editor.hpp"
#include "NcursesView.hpp"

int main() {
    Editor myEditor;
    NcursesView view(myEditor);

    view.run(); // Lance l'éditeur

    return 0;
}