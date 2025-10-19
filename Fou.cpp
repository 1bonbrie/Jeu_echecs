

#include "Fou.h"
#include "Echiquier.h"

namespace modele {

    const QString Fou::obtenirSymboleUnicode() const {
        return (couleur_ == CouleurPiece::Blanc) ? "♗" : "♝";
    }

    bool Fou::estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const {
        const int pasAvant = 1;
        const int pasArriere = -1;

       if (abs(vers.x - de.x) != abs(vers.y - de.y) || (de.x == vers.x && de.y == vers.y)) {
            return false;
        }
       
        auto* cible = echiquier.obtenirPiece(vers);
        if (cible != nullptr && cible->obtenirCouleur() == couleur_) {
            return false;
        }

        int dx = (vers.x > de.x) ? pasAvant : pasArriere;
        int dy = (vers.y > de.y) ? pasAvant : pasArriere;

        int cx = de.x + dx, cy = de.y + dy;
        while (cx != vers.x || cy != vers.y) {
            if (echiquier.obtenirPiece({ cx, cy }) != nullptr) {
                return false;
            }
            cx += dx;
            cy += dy;
        }
        return true;
    }
}
