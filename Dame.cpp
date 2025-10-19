

#include "Dame.h"
#include "Echiquier.h"

namespace modele {

    const QString Dame::obtenirSymboleUnicode() const {
        return (couleur_ == CouleurPiece::Blanc)? "♕":"♛";
    }

    bool Dame::estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const {

        const int pasAvant = 1;
        const int pasArriere = -1;

        int dx = vers.x - de.x;
        int dy = vers.y - de.y;

        if (!(
            (de.x == vers.x && de.y != vers.y) ||   
            (de.y == vers.y && de.x != vers.x) ||   
            (abs(dx) == abs(dy) && dx != 0)         
            )) {
            return false;
        }

        auto* cible = echiquier.obtenirPiece(vers);
        if (cible != nullptr && cible->obtenirCouleur() == couleur_) {
            return false;
        }

        int pasX = (dx == 0) ? 0 : (dx > 0 ? pasAvant : pasArriere);
        int pasY = (dy == 0) ? 0 : (dy > 0 ? pasAvant : pasArriere);

        int cx = de.x + pasX, cy = de.y + pasY;
        while (cx != vers.x || cy != vers.y) {
            if (echiquier.obtenirPiece({ cx, cy })) {
                return false;
            }
            cx += pasX;
            cy += pasY;
        }
        
        return true;
    }
}
