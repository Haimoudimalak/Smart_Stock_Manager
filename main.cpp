#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. CLASSE PRODUCT
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
    
    void displayJSON() {
        cout << "{\"id\":" << id << ",\"name\":\"" << name << "\",\"quantity\":" << quantity << "}";
    }
};

// ==========================================
// 2. CLASSE COMMANDE
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
    
    void afficherJSON() {
        cout << "{\"id\":" << id << ",\"client\":\"" << nomClient << "\",\"produit\":\"" << nomProduit << "\"}";
    }
};

// ==========================================
// 3. FILE D'ATTENTE (Queue)
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

    Commande* obtenirPremier() { return tete == nullptr ? nullptr : tete->cmd; }

    void defiler() {
        if (tete == nullptr) return;
        liste temp = tete;
        tete = tete->suiv;
        if (tete == nullptr) queue = nullptr;
        delete temp;
    }

    void afficherFileJSON() {
        cout << "[";
        liste courant = tete;
        while (courant != nullptr) {
            courant->cmd->afficherJSON();
            courant = courant->suiv;
            if (courant != nullptr) cout << ",";
        }
        cout << "]";
    }
};

// ==========================================
// 4. ARBRE DE RECHERCHE (ABR)
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

    Product* rechercherParNomRec(arbre node, string nom) {
        if (node == nullptr) return nullptr;
        if (node->prod->getName() == nom) return node->prod;
        Product* resGauche = rechercherParNomRec(node->gauche, nom);
        if (resGauche != nullptr) return resGauche;
        return rechercherParNomRec(node->droite, nom);
    }

    void afficherInfixeJSON(arbre node, bool &premier) {
        if (node != nullptr) {
            afficherInfixeJSON(node->gauche, premier);
            if (!premier) cout << ",";
            node->prod->displayJSON();
            premier = false;
            afficherInfixeJSON(node->droite, premier);
        }
    }

public:
    StockTree() : racine(nullptr) {}

    void ajouterProduit(Product* p) { racine = insererRec(racine, p); }
    Product* rechercherProduitParNom(string nom) { return rechercherParNomRec(racine, nom); }

    void afficherStockJSON() {
        cout << "[";
        bool premier = true;
        afficherInfixeJSON(racine, premier);
        cout << "]";
    }
};

// ==========================================
// 5. FONCTION PRINCIPALE (MAIN)
// ==========================================
int main(int argc, char* argv[]) {
    StockTree gestionStock;
    FileAttente fileCommandes;

    gestionStock.ajouterProduit(new Product(101, "iPhone 13", 3));
    gestionStock.ajouterProduit(new Product(105, "MacBook Air", 5));
    gestionStock.ajouterProduit(new Product(108, "iPad Pro", 2));

    fileCommandes.enfiler(new Commande(1, "Malak", "MacBook Air"));
    fileCommandes.enfiler(new Commande(2, "Anas", "iPhone 13"));

    if (argc > 1) {
        string action = argv[1];
        if (action == "get_stock") {
            gestionStock.afficherStockJSON();
        } else if (action == "get_orders") {
            fileCommandes.afficherFileJSON();
        } else if (action == "process") {
            Commande* cmd = fileCommandes.obtenirPremier();
            if (cmd != nullptr) {
                Product* prod = gestionStock.rechercherProduitParNom(cmd->getNomProduit());
                if (prod != nullptr && prod->getQuantity() > 0) {
                    prod->setQuantity(prod->getQuantity() - 1);
                    fileCommandes.defiler();
                    cout << "{\"status\":\"success\",\"message\":\"Commande traitee avec succes\"}";
                } else {
                    cout << "{\"status\":\"error\",\"message\":\"Produit en rupture de stock\"}";
                }
            } else {
                cout << "{\"status\":\"error\",\"message\":\"Aucune commande dans la file\"}";
            }
        }
    } else {
        cout << "{\"stock\":";
        gestionStock.afficherStockJSON();
        cout << ",\"orders\":";
        fileCommandes.afficherFileJSON();
        cout << "}";
    }

    return 0;
}