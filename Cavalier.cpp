

#include "Cavalier.h"
#include "Echiquier.h"

namespace modele {

    const QString Cavalier::obtenirSymboleUnicode() const {
        return (couleur_ == CouleurPiece::Blanc) ? "♘" : "♞";
    }

    bool Cavalier::estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const {

        const int DeplacementCourt = 1;
        const int DeplacementLong = 2;

        int dx = abs(vers.x - de.x);
        int dy = abs(vers.y - de.y);

        if (!((dx == DeplacementCourt && dy == DeplacementLong) || (dx == DeplacementLong && dy == DeplacementCourt))) {
            return false;
        }

        auto* cible = echiquier.obtenirPiece(vers);
        if (cible != nullptr && cible->obtenirCouleur() == couleur_) {
            return false;
        }
        
        return true;
    }


}