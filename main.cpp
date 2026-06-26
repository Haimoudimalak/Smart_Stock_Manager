#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. (Product)
// ==========================================
class Product {
private:
    int id;
    string name;
    int quantity;
public:
    Product(int i, string n, int q) : id(i), name(n), quantity(q) {}
    int getId() { return id; }
    string getName() { return name; }
    int getQuantity() { return quantity; }
    void setQuantity(int q) { quantity = q; } 
    
    void display() {
        cout << "ID: " << id << " | Produit: " << name << " | Stock: " << quantity << endl;
    }
};

// ==========================================
// 2. (Commande)
// ==========================================
class Commande {
private:
    int id;
    string nomClient;
    string nomProduit;
public:
    Commande(int i, string client, string produit) : id(i), nomClient(client), nomProduit(produit) {}
    int getId() { return id; }
    string getNomClient() { return nomClient; }
    string getNomProduit() { return nomProduit; }
    
    void afficher() {
        cout << "No Commande: " << id << " | Client: " << nomClient << " | Demande: " << nomProduit << endl;
    }
};

// ==========================================
// 3. (FileAttente)
// ==========================================
class FileAttente {
private:
    struct cellule {
        Commande* cmd;
        struct cellule* suiv;
    };
    typedef struct cellule* liste;

    liste tete;  
    liste queue; 

public:
    FileAttente() : tete(nullptr), queue(nullptr) {}

    
    void enfiler(Commande* nouvelleCmd) {
        liste nouvelleCellule = new struct cellule;
        nouvelleCellule->cmd = nouvelleCmd;
        nouvelleCellule->suiv = nullptr;

        if (queue == nullptr) {
            tete = queue = nouvelleCellule;
            return;
        }
        queue->suiv = nouvelleCellule;
        queue = nouvelleCellule;
    }

    // permet d'obtenir la commande en tête de la file sans la supprimer
       Commande* obtenirPremier() {
        if (tete == nullptr) return nullptr;
        return tete->cmd;
    }

    // permet de retirer la commande en tête de la file
    void defiler() {
        if (tete == nullptr) return;
        liste temp = tete;
        tete = tete->suiv;
        if (tete == nullptr) queue = nullptr;
        delete temp;
    }

    void afficherFile() {
        if (tete == nullptr) {
            cout << "Aucune commande en attente." << endl;
            return;
        }
        liste courant = tete;
        cout << "\n--- FILE D'ATTENTE DES COMMANDES ---" << endl;
        while (courant != nullptr) {
            courant->cmd->afficher();
            courant = courant->suiv;
        }
        cout << "------------------------------------" << endl;
    }
};

// ==========================================
// 4. (StockTree) 
// ==========================================
class StockTree {
private:
    struct noeud {
        Product* prod;
        struct noeud* gauche;
        struct noeud* droite;
    };
    typedef struct noeud* arbre;
    
    arbre racine;

    arbre insererRec(arbre node, Product* p) {
        if (node == nullptr) {
            arbre nouveau = new struct noeud;
            nouveau->prod = p;
            nouveau->gauche = nouveau->droite = nullptr;
            return nouveau;
        }
        if (p->getId() < node->prod->getId()) node->gauche = insererRec(node->gauche, p);
        else if (p->getId() > node->prod->getId()) node->droite = insererRec(node->droite, p);
        return node;
    }

    // chercher un produit par son nom dans l'arbre de manière récursive
    Product* rechercherParNomRec(arbre node, string nom) {
        if (node == nullptr) return nullptr;
        if (node->prod->getName() == nom) return node->prod;
        
        // recherche selon le principe de parcours infixe ABR
        Product* resGauche = rechercherParNomRec(node->gauche, nom);
        if (resGauche != nullptr) return resGauche;
        
        return rechercherParNomRec(node->droite, nom);
    }

    void afficherInfixe(arbre node) {
        if (node != nullptr) {
            afficherInfixe(node->gauche);
            node->prod->display();
            afficherInfixe(node->droite);
        }
    }

public:
    StockTree() : racine(nullptr) {}

    void ajouterProduit(Product* p) { racine = insererRec(racine, p); }
    
    Product* rechercherProduitParNom(string nom) { return rechercherParNomRec(racine, nom); }

    void afficherStock() {
        if (racine == nullptr) { cout << "Stock vide." << endl; return; }
        cout << "\n--- ETAT DU STOCK (ABR) ---" << endl;
        afficherInfixe(racine);
        cout << "----------------------------" << endl;
    }
};

// ==========================================
// 5. LE MAIN 
// ==========================================
int main() {
    cout << "===== SMART STOCK MANAGER : INTEGRATION SYSTEM =====" << endl;

    // 1. Initialisation du Stock (ABR)
    StockTree gestionStock;
    gestionStock.ajouterProduit(new Product(105, "MacBook Air", 5));
    gestionStock.ajouterProduit(new Product(101, "iPhone 13", 2));

    cout << "\n---  Affichage du stock initial ---" << endl;
    gestionStock.afficherStock();

    // 2. Initialisation de la File d'attente (Queue)
    FileAttente fileCommandes;
    fileCommandes.enfiler(new Commande(1, "Malak", "MacBook Air"));
    fileCommandes.enfiler(new Commande(2, "Anas", "iPhone 13"));

    cout << "\n---  Affichage des clients dans la file ---" << endl;
    fileCommandes.afficherFile();

    // 3. Traitement de la première commande (Interconnexion File <-> ABR)
    cout << "\n---  Traitement de la premiere commande ---" << endl;
    Commande* cmdEnCours = fileCommandes.obtenirPremier();
    
    if (cmdEnCours != nullptr) {
        // Recherche du produit demandé dans l'arbre de stock
        Product* prodEnStock = gestionStock.rechercherProduitParNom(cmdEnCours->getNomProduit());
        
        if (prodEnStock != nullptr && prodEnStock->getQuantity() > 0) {
            cout << "-> Succes: Le produit " << prodEnStock->getName() << " est disponible !" << endl;
            // Décrémentation du stock
            prodEnStock->setQuantity(prodEnStock->getQuantity() - 1);
            cout << "-> Stock mis a jour pour " << prodEnStock->getName() << endl;
            
            // Retirer le client de la file car il a été servi
            fileCommandes.defiler();
        }
    }

    // 4. Vérification finale
    cout << "\n--- [Etape 4] Verification finale du Stock et de la File ---" << endl;
    gestionStock.afficherStock(); 
    fileCommandes.afficherFile();  

    return 0;
}