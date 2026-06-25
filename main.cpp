#include <iostream>
#include <string>

using namespace std;

class Commande {
private:
    int id;
    string nomClient;
    string nomProduit;

public:
    Commande(int i, string client, string produit) {
        id = i;
        nomClient = client;
        nomProduit = produit;
    }

    int getId() { return id; }
    string getNomClient() { return nomClient; }
    string getNomProduit() { return nomProduit; }

    void afficher() {
        cout << "No: " << id << " | Client: " << nomClient << " | Produit: " << nomProduit << endl;
    }
};

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
    FileAttente() {
        tete = nullptr;
        queue = nullptr;
    }

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

    void defiler() {
        if (tete == nullptr) {
            cout << "La file est vide !" << endl;
            return;
        }

        liste temp = tete;
        cout << "-> [POO Processing] ";
        temp->cmd->afficher(); 

        tete = tete->suiv;

        if (tete == nullptr) {
            queue = nullptr;
        }

        delete temp;
    }

    void afficherFile() {
        if (tete == nullptr) {
            cout << "Aucune commande en attente." << endl;
            return;
        }

        liste courant = tete;
        cout << "\n=== FILE D'ATTENTE DES COMMANDES ===" << endl;
        while (courant != nullptr) {
            courant->cmd->afficher();
            courant = courant->suiv;
        }
        cout << "====================================\n" << endl;
    }
};

int main() {
    cout << "--- Verification du code POO & Structure de Donnees ---" << endl;

    FileAttente maFile;

    Commande* c1 = new Commande(1, "Malak", "MacBook Air");
    Commande* c2 = new Commande(2, "Anas", "iPhone 13");

    maFile.enfiler(c1);
    maFile.enfiler(c2);

    maFile.afficherFile();
    maFile.defiler();
    maFile.afficherFile();

    return 0;
}