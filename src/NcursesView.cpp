#include "NcursesView.hpp"
#include "Editor.hpp"
#include <ncurses.h>
#include <iostream>

NcursesView::NcursesView(Editor& editor) : editor(editor), isRunning(true) {
    initialize();
}

NcursesView::~NcursesView() {
    shutdown();
}

void NcursesView::initialize() {
    initscr();            // Démarre le mode ncurses
    raw();                // Désactive le line buffering (on reçoit les touches direct)
    keypad(stdscr, TRUE); // Active la gestion des touches spéciales (F1, flèches, etc.)
    noecho();             // N'affiche pas les touches tapées par l'utilisateur
    curs_set(1);          // (1 = barre verticale, 2 = bloc, 0 = invisible)
}

void NcursesView::shutdown() {
    endwin();
}

void NcursesView::run() {
    while (isRunning) {
        display();
        handleInput();
    }
}

void NcursesView::display() {
    clear();

    const std::string& content = editor.getBufferContent();
    size_t selStart = editor.getSelectionStart();
    size_t selEnd = editor.getSelectionEnd();
    size_t cursorPos = editor.getCursorPosition();
    bool hasSelection = (selStart != selEnd);

    int cursorScreenY = 0;
    int cursorScreenX = 0;
    
    // Affiche le contenu
    for (size_t i = 0; i < content.length(); ++i) {
        if (hasSelection && i >= selStart && i < selEnd) {
            attron(A_REVERSE);
        }
        
        printw("%c", content[i]);

        if (hasSelection && i >= selStart && i < selEnd) {
            attroff(A_REVERSE);
        }
        
        if (i == cursorPos - 1) {
            getyx(stdscr, cursorScreenY, cursorScreenX);
        }
    }
    
    if (cursorPos == 0) {
        cursorScreenY = 0;
        cursorScreenX = 0;
    }
    
    if (cursorPos == content.length() && content.length() > 0) {
        getyx(stdscr, cursorScreenY, cursorScreenX);
    }

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    attron(A_REVERSE);
    
    // Affiche le mode et le nom du fichier
    std::string status = (editor.isSelecting()) ? "-- SELECTING --" : "-- NORMAL --";
    std::string filename = editor.hasFilename() 
        ? (editor.hasBeenModified() ? "*" + editor.getFilename() : editor.getFilename())
        : "[Nouveau fichier]";
    
    // Affiche le statut à gauche et le nom du fichier à droite
    mvprintw(rows - 1, 0, "%s", status.c_str());
    mvprintw(rows - 1, cols - filename.length() - 1, "%s", filename.c_str());
    attroff(A_REVERSE);
    
    if (hasSelection) {
        curs_set(0);
    } else {
        curs_set(1);
        move(cursorScreenY, cursorScreenX);
    }
    
    refresh();
}

void NcursesView::handleInput() {
    int ch = getch();
    
    switch(ch) {
        case KEY_LEFT:
            if (editor.isSelecting()) {
                editor.extendSelection(-1);
            } else {
                editor.moveCursor(-1);
            }
        break;
        case KEY_RIGHT:
            if (editor.isSelecting()) {
                editor.extendSelection(1);
            } else {
                editor.moveCursor(1);
            }
        break;

        case KEY_UP:
            if (editor.isSelecting()) {
                editor.extendSelectionVertical(-1);
            } else {
                editor.moveCursorVertical(-1);
            }
        break;
        
        case KEY_DOWN:
            if (editor.isSelecting()) {
                editor.extendSelectionVertical(1);
            } else {
                editor.moveCursorVertical(1);
            }
        break;

        // CTRL+S - Mode sélection
        case 19:
            editor.toggleSelectionMode();
            break;

        // Escape - Mode Normal
        case 27:
            editor.toggleNormalMode();
            break;

        // CTRL+Q - quitter
        case 17:
            isRunning = false;
            break;
        
        // CTRL+W - write, sauvegarde
        case 23:
            editor.saveFile();
            break;
        
        // CTRL+C - copier
        case 3:
            editor.copy();
            break;

        // CTRL+X - couper
        case 24:
            editor.cut();
            break;

        // CTRL+V - coller
        case 22:
            editor.paste();
            break;
        
        // CTRL+A - tout sélectionner
        case 1:
            editor.selectAll();
            break;

        // Enter - nouvelle ligne
        case KEY_ENTER:
        case 10: // (LF) - '\n'
        case 13: // (CR) - '\r'
            editor.insertText("\n");
            break;

        // CTRL+K - supprime la ligne actuelle
        case 11:
            editor.deleteCurrentLine();
            break;

        // Backspace - supprime le caractère avant le curseur
        case KEY_BACKSPACE:
        case 127:
        case 8:
            editor.deleteBackward();
            break;

        // Del - supprime le caractère après le curseur
        case KEY_DC:
            editor.deleteForward();
            break;

        // CTRL+Z - défait une action
        case 26:
            editor.undo();
            break;
        
        // CTRL+Y - refait une action
        case 25:
            editor.redo();
            break;

        default:
            // Si ce n'est pas une commande, c'est du texte à insérer
            if (ch >= 32 && ch <= 126) {
                editor.insertText(std::string(1, ch));
            }
            break;
    }
}