# Compression d'Images avec quadtrees - Projet de Programmation C

## INFO1 - 2024/2024

## Binôme

Mickael RAKOTOARISON, Romain Buges

### Manuel utilisateur

#### Compilation

Pour compiler le projet, utilisez la commande suivante :

```bash
make
```
Cela générera un exécutable dans le dossier `bin/`.

#### Utilisation

Pour utiliser le programme, exécutez la commande suivante :
```bash
./bin/compression <chemin-vers-l-image> [--circles]
```
L'option --circles permet de choisir la forme à utiliser pour la génération de l'image. Par défaut, cette option n'est pas définie.

### État final du projet

Tout ce qui a été demandé a été réalisé avec succès :

- Le programme compresse et décompresse les images en utilisant des quadtrees.
- Gestion des fichiers de sauvegarde minimisée en noir et blanc et en couleur.
- Calcul des erreurs à l'aide d'un heap pour optimiser la compression.

Améliorations effectuées :

- Implantation de la subdivision dynamique basée sur l'erreur.
- Interface graphique pour une meilleure visualisation et interaction.
- Capacité de charger des fichiers de sauvegarde minimisée.
- Affichage à l'aide de forme géometriques

### Détails de l'implémentation

- **Structures de données :**
  - `QuadTree` : Utilisé pour stocker la structure de l'image en zones.
  - `Heap` : Employé pour gérer les erreurs de manière efficace lors de la compression.

- **Fonctions principales :**
  - `create_quadtree` : Construit un quadtree à partir d'une image.
  - `compress_image` : Applique la compression quadtree à l'image.
  - `decompress_image` : Restaure l'image à partir de sa version compressée.
  - `minimize_quadtree` : Réduit la taille du quadtree en minimisant les redondances.
  - `save_compressed_image` : Sauvegarde l'image compressée dans un fichier.
  - `load_compressed_image` : Charge une image compressée à partir d'un fichier.
### Structure du Projet

Voici un diagramme simplifié de la structure de notre projet de compression d'images avec quadtrees :
```
├── Makefile                 # Script pour compiler le projet avec make.
├── README.md                # Fichier Markdown fournissant une description du projet.
├── bin/                     # Contient les fichiers binaires et objets compilés.
├── include/                 # En-têtes des fichiers pour la déclaration des fonctions.
│   ├── controller/
│   │   └── Controller.h     # Déclarations pour les fonctions du contrôleur.
│   ├── model/
│   │   ├── Heap.h           # Déclarations pour les fonctions de gestion du tas.
│   │   ├── Pixel.h          # Déclarations pour les fonctions manipulant les pixels.
│   │   └── QuadTree.h       # Déclarations pour les fonctions du QuadTree.
│   └── view/
│       └── View.h           # Déclarations pour les fonctions de visualisation.
├── resources/               # Ressources utilisées dans le projet (images, données).
└── src/                     # Source du projet contenant les définitions des fonctions.
    ├── controller/
    │   ├── Controller.c     # Implémentation des fonctions de contrôle.
    │   └── Main.c           # Point d'entrée principal du projet.
    ├── model/
    │   ├── Heap.c           # Implémentation de la structure de tas.
    │   ├── Pixel.c          # Implémentation des fonctions de manipulation de pixels.
    │   └── QuadTree.c       # Implémentation des fonctions pour le QuadTree.
    └── view/
        └── View.c           # Implémentation des fonctions de visualisation.

```

### Déroulement du projet

#### Répartition des tâches

- **Mickael RAKOTOARISON :**
  - Mise en place du heap.
  - Sauvegarde minimisée en noir et blanc et en couleur.
  - Calcul des erreurs avec le heap.

- **Romain Buges :**
  - Mise en place de la subdivision.
  - Chargement des fichiers de sauvegarde.
  - Développement de l'interface graphique.

#### Difficultés rencontrées et solutions apportées

- **Problème de chargement des fichiers minimisés :** Nous avons eu des difficultés à identifier chaque nœud lors du parcours du fichier. Une solution a été mise en place pour parcourir correctement le fichier et identifier chaque nœud avec précision.

### Détails Techniques du Projet

#### Opérations bit à bit
- **Difficulté** : Manipuler des bits directement peut introduire des erreurs subtiles qui affectent grandement les images compressées. Les opérations bit à bit sont cruciales dans la compression d'images car elles permettent une gestion plus fine de l'espace de stockage, mais elles requièrent une précision et une attention extrêmes pour éviter toute corruption de données.
- **Solution** : Nous avons utilisé des opérations bit à bit, comme spécifié dans le projet, pour optimiser la compression des données d'image. Cette approche est particulièrement efficace pour réduire la taille des fichiers sans perdre en qualité d'image. Par exemple, pour écrire efficacement les données compressées dans un fichier, nous avons utilisé le code suivant :

```c
  void write_node_to_file(quadnode *node, FILE *fptr, unsigned long long int *buffer, int *bufferSize, int isBlackAndWhite) {
      *buffer <<= 1; // Décale le buffer d'un bit à gauche pour faire de la place pour le nouveau bit.
      *bufferSize += 1; // Incrémente la taille du buffer utilisé.
      if (node->northwest == NULL) {
          *buffer += 1; // Ajoute un bit à 1 pour marquer que c'est une feuille.
          if (isBlackAndWhite) {
              *buffer <<= 1; // Décale à nouveau pour le bit de couleur.
              *bufferSize += 1;
              int grayscale = (node->color.red + node->color.green + node->color.blue) / 3;
              *buffer += grayscale / 128; // Ajoute un bit représentant la couleur (noir ou blanc).
          } else {
              *buffer <<= 32; // Décale de 32 bits pour ajouter les couleurs RGBA.
              *bufferSize += 32;
              *buffer += ((long long) (node->color.red) % 256) << 24;
              *buffer += ((long long) (node->color.green) % 256) << 16;
              *buffer += ((long long) (node->color.blue) % 256) << 8;
              *buffer += ((long long) (node->color.alpha) % 256);
          }
      }
      // Écriture des bits dans le fichier dès que le buffer atteint 8 bits.
      while (*bufferSize >= 8) {
          char byte = *buffer >> (*bufferSize - 8);
          fputc(byte, fptr);
          *bufferSize -= 8;
      }
      // Recursion pour les autres quadrants si ce n'est pas une feuille.
      if (node->northwest != NULL) {
          write_node_to_file(node->northwest, fptr, buffer, bufferSize, isBlackAndWhite);
          write_node_to_file(node->northeast, fptr, buffer, bufferSize, isBlackAndWhite);
          write_node_to_file(node->southwest, fptr, buffer, bufferSize, isBlackAndWhite);
          write_node_to_file(node->southeast, fptr, buffer, bufferSize, isBlackAndWhite);
      }
  }
```

#### Modularisation de l'architecture

- **Difficulté** : Assurer une structure de code claire et maintenable dans un projet complexe.
- **Solution** : Nous avons structuré le code selon le modèle MVC (Modèle-Vue-Contrôleur) pour améliorer la clarté et faciliter les modifications futures.

**Modèle (Model)** :
- Gère la logique des données et l'état.
- Inclut la gestion des quadtrees et des tas max.
- Responsable de la manipulation des données d'image et du calcul des erreurs.

**Vue (View)** :
- Génère l'interface utilisateur basée sur les données du modèl.
- Affiche les images et les résultats de la compression.

**Contrôleur (Controller)** :
- Gère les entrées de l'utilisateur et les interactions.
- Interagit avec le modèle pour modifier l'état des données.
- Met à jour la vue pour refléter les changements.


#### Utilisation d'un tas max pour la gestion des erreurs
- **Difficulté** : Prioriser et gérer efficacement les erreurs dans la compression.
- **Solution** : Nous avons utilisé un tas max pour trier les nœuds en fonction de leur erreur de compression, insérant chaque nouveau nœud créé dans le tas :

```c
quadnode * create_quadnode(MLV_Image * image, int x, int y, int size, max_heap * heap) {
  quadnode * node = malloc(sizeof(quadnode));
  node -> color = read_image(image, x, y, size);
  node -> error = calculate_error(image, x, y, size, node -> color); // Calculer l'erreur
  insert_max_heap(heap, node); // Insérer le nœud dans le tas max
  return node;
}
```


#### Gestion des fichiers de sauvegarde
- **Difficulté** : Sauvegarder et charger efficacement les structures de données complexes.
- **Solution** : Nous avons développé des fonctions pour écrire et lire les quadtrees compressés dans des fichiers, en minimisant la perte de données :

```c
void save_quadtree_minimized(quadnode * tree,
  const char * filename, int isBlackAndWhite) {
  FILE * fptr = fopen(filename, "w"); // Ouvrir le fichier
  write_minimized_node_to_file(tree, fptr, isBlackAndWhite); // Écrire le quadtree minimisé
  fclose(fptr);
}
```

### Conclusion
Ce projet a été extrêmement bénéfique pour nous deux, renforçant notre compréhension des opérations bit à bit et leur application pratique dans le traitement des données. Il nous a également permis de mettre en œuvre une architecture bien modularisée, ce qui a facilité la gestion et l'évolution de notre code de manière collaborative. De plus, nous avons pu appliquer concrètement l'utilisation d'un tas max pour la gestion des erreurs, ce qui a enrichi notre expérience pratique des structures de données dans des situations réelles.