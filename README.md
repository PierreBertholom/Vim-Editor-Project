# Mini-Éditeur (TP2-OMD)

Un mini-éditeur de texte en C++ pour le terminal. Ce projet met en œuvre un buffer de texte performant (basé sur une **Piece Table**) et une gestion de l'historique et des macros via un **Command Pattern**.

## Démo Vidéo

<video src="demo/video_demo.mp4" controls width="640">
  Votre navigateur ne supporte pas la lecture vidéo.
</video>


## Fonctionnalités

### V1
* **Gestion de fichiers** : Ouverture d'un fichier existant ou création d'un nouveau fichier.
* **Sauvegarde** : Enregistrement des modifications (`CTRL+W`).
* **Édition de texte** : Insertion de caractères et de sauts de ligne.
* **Navigation** : Déplacement du curseur au caractère (`←`/`→`) et à la ligne (`↑`/`↓`).
* **Suppression** : Suppression avant (`Delete`) et arrière (`Backspace`), et suppression de ligne (`CTRL+K`).
* **Sélection** : Un "Mode Sélection" (`CTRL+S`) pour surligner du texte.
* **Presse-papiers** : Copier (`CTRL+C`), Couper (`CTRL+X`), et Coller (`CTRL+V`).
* **Utilitaire** : Sélectionner tout (`CTRL+A`).

### V2
* **Historique** : Annuler (`CTRL+Z`) et Refaire (`CTRL+Y`) des actions.
* **Macros** : Enregistrer (`CTRL+R`) et Rejouer (`CTRL+P`) une séquence d'actions.
* **État** : Un indicateur `*` s'affiche lorsque le fichier a des modifications non sauvegardées.

---

## Conception

* **Langage** : C++
* **Interface Terminal** : `ncurses`
* **Compilation** : `Cmake`
* **Structure de Données** : `PieceTable` (pour une édition performante sans copie de buffer)
* **Patrons de Conception** :
    * **Commande** : Pour encapsuler les actions (Insert, Delete) afin de gérer l'historique (Undo/Redo) et les macros.
    * **Singleton** : Pour la classe `Clipboard` (Presse-papiers).
    * **Modèle-Vue-Contrôleur** : Séparation claire entre la logique (`Editor`), les données (`PieceTable`) et l'affichage (`NcursesView`).

---

## Installation et Lancement

### Prérequis
* Un compilateur C++ (g++)
* La bibliothèque `ncurses` (`libncurses-dev`)
* `Cmake/make`

### Compilation
1. Installer la librairie `ncurses` si besoin :
    ```bash
    sudo pacman -S ncurses
    sudo apt install libncurses5-dev libncursesw5-dev
    ```
1.  Clonez le dépôt :
    ```bash
    git clone git@gitlab2.istic.univ-rennes1.fr:pbertholom/mini-editeur-tp2-omd.git
    cd mini-editeur-tp2-omd/
    ```
2.  Compilez le projet :
    ```bash
    mkdir build && cd build 
    cmake ..
    make
    ```

### Lancement
* Pour créer un nouveau fichier :
    ```bash
    ./mini_editeur_tp2_omd
    ```
    ou
    ```bash
    ./mini_editeur_tp2_omd nouveau_fichier.txt
    ```
* Pour ouvrir un fichier existant :
    ```bash
    ./mini_editeur_tp2_omd fichier_existant.txt
    ```

---

## Commandes

L'éditeur possède deux modes : le **Mode Normal** (par défaut, pour l'édition) et le **Mode Sélection** (pour surligner du texte).

### Général et Fichier
| Commande | Action |
| :--- | :--- |
| `CTRL+W` | Sauvegarder le fichier |
| `CTRL+Q` | Quitter l'éditeur |

### Navigation et Sélection
| Commande | Mode Normal | Mode Sélection |
| :--- | :--- | :--- |
| `Flèches` | Déplacer le curseur | Étendre la sélection |
| `CTRL+S` | Passer en Mode Sélection | - |
| `Esc` | - | Quitter le Mode Sélection |
| `CTRL+A` | Sélectionner tout le contenu | - |

### Édition et Presse-papiers
| Commande | Action |
| :--- | :--- |
| `Caractères` | Insérer du texte (remplace la sélection si active) |
| `Entrée` | Insérer une nouvelle ligne |
| `Backspace` | Supprimer le caractère précédent (ou la sélection) |
| `Delete` | Supprimer le caractère suivant (ou la sélection) |
| `CTRL+K` | Supprimer la ligne actuelle |
| `CTRL+C` | Copier la sélection actuelle |
| `CTRL+X` | Couper la sélection actuelle |
| `CTRL+V` | Coller le contenu du presse-papiers |

### Historique et Macros (V2)
| Commande | Action |
| :--- | :--- |
| `CTRL+Z` | Annuler (Undo) la dernière action |
| `CTRL+Y` | Refaire (Redo) la dernière action |
| `CTRL+R` | Démarrer / Arrêter l'enregistrement de la macro |
| `CTRL+P` | Jouer la macro enregistrée |