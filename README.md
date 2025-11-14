# Mini Text Editor

A lightweight terminal-based text editor written in modern C++.  
The project implements an efficient text buffer using a **Piece Table** and supports undo/redo and macro recording through a **Command Pattern** architecture.

## Demo Video

<video src="demo/video_demo.mp4" controls width="640">
  Your browser does not support video playback.
</video>

---

## Features

### Version 1
* **File Management**: Open an existing file or create a new one.
* **Saving**: Persist changes (`CTRL+W`).
* **Text Editing**: Insert characters and line breaks.
* **Navigation**: Move the cursor horizontally (`←`/`→`) and vertically (`↑`/`↓`).
* **Deletion**: Forward delete (`Delete`), backward delete (`Backspace`), and delete line (`CTRL+K`).
* **Selection Mode**: Highlight text with `CTRL+S`.
* **Clipboard**: Copy (`CTRL+C`), Cut (`CTRL+X`), Paste (`CTRL+V`).
* **Utilities**: Select all (`CTRL+A`).

### Version 2
* **History**: Undo (`CTRL+Z`) and Redo (`CTRL+Y`).
* **Macros**: Record (`CTRL+R`) and replay (`CTRL+P`) sequences of actions.
* **File Status Indicator**: A `*` marks unsaved changes.

---

## Architecture & Design

* **Language**: C++
* **Terminal UI**: `ncurses`
* **Build System**: `CMake`
* **Core Data Structure**: Piece Table for fast, non-destructive text editing
* **Design Patterns**:
  * **Command Pattern** – Used for encapsulating editing actions to support undo/redo and macro replay.
  * **Singleton** – Used for the text clipboard.
  * **Model–View–Controller** – Separation of concerns between logic (`Editor`), data (`PieceTable`), and rendering (`NcursesView`).

---

## Installation & Build

### Requirements
* A C++ compiler (g++)
* `ncurses` (`libncurses-dev`)
* `CMake` and `make`

### Linux / macOS

1. Install ncurses:

    **Linux**
    ```bash
    sudo pacman -S ncurses
    sudo apt install libncurses5-dev libncursesw5-dev
    ```

    **macOS**
    ```bash
    brew install ncurses
    ```

2. Clone the repository:
    ```bash
    git clone git@gitlab2.istic.univ-rennes1.fr:pbertholom/mini-editeur-tp2-omd.git
    cd mini-editeur-tp2-omd/
    ```

3. Build:
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```

### Windows (supported but not recommended)

1. Install MSYS and open **MSYS MINGW64**.
2. Install required packages:
    ```bash
    pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ncurses
    ```
3. Clone the repository:
    ```bash
    git clone git@gitlab2.istic.univ-rennes1.fr:pbertholom/mini-editeur-tp2-omd.git
    cd mini-editeur-tp2-omd/
    ```
4. Update `CMakeLists.txt`:
    ```cmake
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(NCURSES REQUIRED ncursesw)
    include_directories(include)
    include_directories(${NCURSES_INCLUDE_DIRS})

    add_executable(...)

    target_link_libraries(mini_editeur_tp2_omd PRIVATE ${NCURSES_LIBRARIES})
    ```
5. Build:
    ```bash
    mkdir build && cd build
    cmake ..
    ninja
    ```

---

## Running

* Create a new file:
    ```bash
    ./mini_editeur_tp2_omd
    ```
    or
    ```bash
    ./mini_editeur_tp2_omd new_file.txt
    ```

* Open an existing file:
    ```bash
    ./mini_editeur_tp2_omd existing_file.txt
    ```

---

## Controls

The editor provides two modes: **Normal Mode** (default) and **Selection Mode**.

### General & File
| Command | Action |
| :--- | :--- |
| `CTRL+W` | Save file |
| `CTRL+Q` | Quit editor |

### Navigation & Selection
| Command | Normal Mode | Selection Mode |
| :--- | :--- | :--- |
| Arrow Keys | Move cursor | Extend selection |
| `CTRL+S` | Enter Selection Mode | — |
| `Esc` | — | Exit Selection Mode |
| `CTRL+A` | Select all | — |

### Editing & Clipboard
| Command | Action |
| :--- | :--- |
| Characters | Insert text |
| `Enter` | Insert newline |
| `Backspace` | Delete previous character or selection |
| `Delete` | Delete next character or selection |
| `CTRL+K` | Delete current line |
| `CTRL+C` | Copy selection |
| `CTRL+X` | Cut selection |
| `CTRL+V` | Paste clipboard |

### History & Macros (V2)
| Command | Action |
| :--- | :--- |
| `CTRL+Z` | Undo |
| `CTRL+Y` | Redo |
| `CTRL+R` | Start/Stop macro recording |
| `CTRL+P` | Play recorded macro |
