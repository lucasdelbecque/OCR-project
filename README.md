# 🧠 Glass Box OCR : Reconnaissance Manuscrite Hybride (Python/C++)

![Badge C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg) ![Badge Python](https://img.shields.io/badge/Python-3.x-yellow.svg) ![Badge OpenGL](https://img.shields.io/badge/OpenGL-Legacy-green.svg) ![Badge PyTorch](https://img.shields.io/badge/PyTorch-Train-orange.svg)

**Glass Box OCR** est un moteur d'intelligence artificielle transparent capable de reconnaître des chiffres manuscrits en temps réel.

Contrairement aux solutions "boîte noire" classiques, ce projet révèle les entrailles du réseau de neurones. Il entraîne un modèle robuste en **Python (PyTorch)**, exporte ses "connaissances" (poids synaptiques), et les injecte dans un moteur d'inférence personnalisé écrit entièrement en **C++ & OpenGL**.

---

## Fonctionnalités Clés

* **Approche "Glass Box" (Boîte Transparente) :** Visualisez non seulement le résultat, mais aussi ce que l'IA "voit" à travers ses filtres de convolution (Feature Maps) en temps réel.
* **Moteur d'Inférence "From Scratch" :** Aucune librairie de Deep Learning en C++. Les convolutions, le pooling, les couches denses et l'activation ReLU sont codés à la main via une librairie matricielle personnalisée.
* **Robustesse aux Dessins Imparfaits :** Le modèle Python utilise la *Data Augmentation* (rotations, zooms, décalages) pour comprendre l'écriture humaine naturelle et imparfaite.
* **Interface Temps Réel :** Dessinez à la souris et voyez les jauges de probabilités réagir instantanément (60 FPS).
---

## Architecture du Projet

Le projet est divisé en deux cerveaux qui collaborent :

### 1. Python
* **Rôle :** Apprendre et Concevoir.
* **Outil :** PyTorch.
* **Fichier :** `train.py`.
* **Action :** Il télécharge le dataset MNIST (60 000 images), applique des déformations aléatoires (Data Augmentation), entraîne un CNN (Convolutional Neural Network) à 8 filtres, et sauvegarde les poids dans des fichiers `.txt`.

### 2. C++
* **Rôle :** Exécuter et Afficher.
* **Outil :** C++, OpenGL (GLFW), CMake.
* **Fichiers :** `main.cpp`, `matrix.cpp`, `matrix.h`.
* **Action :** Il lit les fichiers `.txt`, reconstruit le réseau en mémoire, ouvre une fenêtre graphique, et effectue les calculs mathématiques en direct pendant que l'utilisateur dessine.

---

## Structure des Fichiers

Voici le rôle détaillé de chaque fichier :

### `train.py` (L'Entraînement)
Le script de conception du cerveau :
1.  **Data Augmentation** : Utilise `transforms.RandomAffine` pour créer des variantes (rotations, zooms) des chiffres MNIST.
2.  **Modèle (`RobustCNN`)** : Architecture avec 1 couche de convolution (8 filtres), ReLU, Max Pooling, et 1 couche dense.
3.  **Export** : Convertit les tenseurs PyTorch en fichiers textes bruts (`conv_k0.txt`, `dense_weights.txt`...).

### `matrix.h` / `matrix.cpp` (Le Moteur Mathématique)
Une librairie d'algèbre linéaire faite maison :
* **`convolve`** : Applique les filtres sur l'image (produit de convolution 2D).
* **`maxPooling`** : Réduit la taille de l'image (Downsampling).
* **`softmax`** : Convertit les scores bruts en probabilités (0-100%).
* **`Matrix`** : Classe gérant la mémoire et les opérations sur les tableaux 2D.

### `main.cpp` (L'Application Finale)
Le chef d'orchestre graphique :
1.  **Chargement** : Importe les poids `.txt` au démarrage.
2.  **Game Loop** : Boucle infinie qui gère l'affichage à 60 FPS.
3.  **Input** : Gère le dessin à la souris (conversion coordonnées écran -> OpenGL).
4.  **Rendu** : Dessine l'interface (Grille noire, Feature Maps cyan, Jauges) en utilisant OpenGL (mode immédiat).

---

## Installation & Compilation

### Pré-requis
* **Python 3.x** (avec `pip`)
* **Compilateur C++** (GCC, Clang ou MSVC)
* **CMake** (pour construire le projet)
* **Librairie GLFW** (pour la fenêtre graphique)

### Étape 1 : Entraîner le Modèle (Python)
Générez les "cerveaux" de l'IA (fichiers de poids).

```bash
# 1. Installez les dépendances
pip install torch torchvision

# 2. Lancez l'entraînement
python train.py
```
**Résultat :** Plusieurs fichiers `.txt` (`conv_k0.txt`, `dense_weights.txt`, etc.) apparaissent dans le dossier.

### Étape 2 : Installer GLFW (Si nécessaire)
* **Mac (Homebrew) :** `brew install glfw`
* **Linux (Debian/Ubuntu) :** `sudo apt-get install libglfw3-dev`
* **Windows :** Téléchargez les binaires sur [glfw.org](https://www.glfw.org/).

### Étape 3 : Compiler le C++

```bash
# 1. Créez un dossier de build
mkdir cmake-build-debug
cd cmake-build-debug

# 2. Configurez avec CMake
cmake ..

# 3. Compilez
make 
```

### Étape 4 : Lancer !
Assurez-vous que les fichiers `.txt` générés par Python sont bien dans le dossier de l'exécutable.

```bash
./OCR
```

---

## Comment Utiliser

Une fois l'application lancée :

1.  **Dessinez** un chiffre (0-9) dans la grande zone noire à gauche avec la **Souris (Clic Gauche)**.
2.  **Observez** la zone centrale : c'est la "vision" interne de l'IA (ce que le premier filtre détecte).
3.  **Regardez** la colonne de droite : la barre verte indique la prédiction de l'IA.
4.  **Effacez** tout en cliquant sur le bouton **X Rouge** en bas.

---

##  Détails Techniques

### Pourquoi 8 Filtres ?
Un seul filtre rendait l'IA "myope". Avec 8 filtres, le réseau apprend 8 caractéristiques différentes simultanément (boucles, lignes verticales, diagonales...), rendant la reconnaissance beaucoup plus fiable.

### Pourquoi pas de GPU en C++ ?
Pour un projet éducatif, coder les opérations matricielles sur CPU permet de comprendre *exactement* l'algorithme sous-jacent. La taille réduite des images (28x28 pixels) permet une exécution fluide sur CPU.

---

## Auteur

Projet réalisé par **Lucas Delbecque**.

* *Technologies : Deep Learning, Computer Vision, C++ Graphics, Systems Engineering.*