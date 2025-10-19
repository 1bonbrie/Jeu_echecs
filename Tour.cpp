

#include "Tour.h"
#include "Echiquier.h"

namespace modele {


    const QString Tour::obtenirSymboleUnicode() const {
        return (couleur_ == CouleurPiece::Blanc) ? "♖" : "♜";
    }

    bool Tour::estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const {
        const int pasAvant = 1;
        const int pasArriere = -1;

        if (!((de.x == vers.x && de.y != vers.y) || (de.y == vers.y && de.x != vers.x))) {
            return false;
        }
        
        Piece* cible = echiquier.obtenirPiece(vers);
        if (cible != nullptr && cible->obtenirCouleur() == couleur_) {
            return false;
        }

        if (de.x == vers.x) {
            int pas = (vers.y > de.y) ? pasAvant : pasArriere;
            for (int y = de.y + pas; y != vers.y; y += pas) {
                if (echiquier.obtenirPiece({ de.x, y }) != nullptr) {
                    return false;
                }
            }
            return true;
        }

        if (de.y == vers.y) {
            int pas = (vers.x > de.x) ? pasAvant : pasArriere;
            for (int x = de.x + pas; x != vers.x; x += pas) {
                if (echiquier.obtenirPiece({ x, de.y }) != nullptr) {
                    return false;
                }
            }
            return true;
        }
        
        return false;
    }

}
