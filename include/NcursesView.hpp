#ifndef NCURSES_VIEW_HPP
#define NCURSES_VIEW_HPP

#include <string>

// fwd decl
class Editor;

class NcursesView {
private:
    Editor& editor;
    bool isRunning;

    void initialize(); // Initialise ncurses
    void shutdown();   // Restaure le terminal
    void display();    // Affiche le buffer, la sélection...
    void handleInput(); // Gère les entrées clavier
    std::string askFilename();

public:
    NcursesView(Editor& editor);
    ~NcursesView();
    void run();
};

#endif // NCURSES_VIEW_HPP
