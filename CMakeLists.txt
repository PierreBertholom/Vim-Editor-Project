cmake_minimum_required(VERSION 3.30)
project(mini_editeur_tp2_omd)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(/opt/homebrew/opt/ncurses/include)
link_directories(/opt/homebrew/opt/ncurses/lib)

add_executable(mini_editeur_tp2_omd
        include/Buffer.hpp
        include/Clipboard.hpp
        include/Editor.hpp
        include/NcursesView.hpp
        include/Selection.hpp
        include/Piece.hpp
        include/PieceTable.hpp
        include/Command.hpp
        include/CommandManager.hpp
        include/TextCommands.hpp
        src/Buffer.cpp
        src/Clipboard.cpp
        src/Editor.cpp
        src/main.cpp
        src/NcursesView.cpp
        src/Selection.cpp
        src/PieceTable.cpp
        src/CommandManager.cpp
        src/TextCommands.cpp
)

target_link_libraries(mini_editeur_tp2_omd ncurses)
