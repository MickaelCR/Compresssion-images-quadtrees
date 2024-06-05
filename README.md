# STRUCTURE PROJET 

## MODEL

### Pixel

Le modèle `Pixel` contient la structure des pixels ainsi que les fonctions nécessaires pour manipuler les pixels d'une image.

- **Fichier :** `src/model/Pixel.c`
- **En-tête :** `include/model/Pixel.h`
- **Fonctions :**
  - `double distance(pixel pixel1, pixel pixel2);` : Calcule la distance euclidienne entre deux pixels.
  - `pixel read_image(MLV_Image *image, int startX, int startY, int width, int height);` : Lit les pixels d'une image et retourne la couleur moyenne d'une zone spécifiée.

### QuadTree

Le modèle `QuadTree` contient la structure de l'arbre quartique et les fonctions pour créer et manipuler cet arbre pour la compression d'images.

- **Fichier :** `src/model/QuadTree.c`
- **En-tête :** `include/model/QuadTree.h`
- **Fonctions :**
  - `double calculate_error(MLV_Image *image, int startX, int startY, int width, int height, pixel average);` : Calcule l'erreur de compression pour une zone spécifique de l'image.
  - `quadnode* create_quadnode(MLV_Image *image, int startX, int startY, int width, int height);` : Crée un nœud de quadtree pour une zone spécifique de l'image.

## CONTROLLER

### Controller

Le contrôleur gère les interactions entre le modèle et la vue, y compris le chargement de l'image, la création du quadtree et l'affichage des résultats.

- **Fichier :** `src/controller/Controller.c`
- **En-tête :** `include/controller/Controller.h`
- **Fonctions :**
  - `void process_image(const char *filename);` : Traite une image en la chargeant, en créant un quadtree et en affichant les résultats.

## VIEW

### View

La vue gère l'affichage graphique de l'image et les interactions avec l'utilisateur.

- **Fichier :** `src/view/View.c`
- **En-tête :** `include/view/view.h`
- **Fonctions :**
  - `void initialize_window();` : Initialise la fenêtre graphique.
  - `void draw_image(MLV_Image *image);` : Dessine l'image dans la fenêtre.
  - `void update_window();` : Met à jour la fenêtre.
  - `void free_window();` : Libère les ressources de la fenêtre graphique.

# NEXT STEP

1. Tâches binaires pour le max
2. Charger une image en QuadTree
3. Ajouter des fonctionnalités pour l'encodage et le décodage des images en utilisant des quadtrees
4. Implémenter des boutons dans l'interface pour :
    - Lancer l'approximation sous forme de quadtree
    - Sauvegarder une image en noir et blanc au format binaire
    - Sauvegarder une image en couleur au format binaire
    - Lancer la minimisation des quadtrees
    - Sauvegarder une image minimisée en noir et blanc
    - Sauvegarder une image minimisée en couleur
    - Ouvrir une image à partir de son nom
5. Implémenter la minimisation des quadtrees sans perte et avec perte
6. Ajouter des tests et valider le bon fonctionnement de chaque fonctionnalité
7. Générer une documentation détaillée avec doxygen
8. Préparer l'archive tar.gz pour le rendu final

