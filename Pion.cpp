
#include "Pion.h"
#include "Echiquier.h"

namespace modele {

    Pion::Pion(const CouleurPiece& couleur)
        : Piece(couleur), estPremierMouvement_(true)  {};

    const QString Pion::obtenirSymboleUnicode() const {
        return (couleur_ == CouleurPiece::Blanc) ? "♙" : "♟";
    }

    void Pion::setPremierMouvementFaux() {
        estPremierMouvement_ = false;
    }

    bool Pion::estPremierMouvement() const {
        return estPremierMouvement_;
    }


    bool Pion::estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const {

        const int pasAvant = 1;
        const int pasArriere = -1;
        const int longPas = 2;

        int direction = couleur_ == CouleurPiece::Blanc ? pasArriere : pasAvant;
        Piece* cible = echiquier.obtenirPiece(vers);

        int dx = vers.x - de.x;
        int dy = vers.y - de.y;

        if (dy != direction && (!estPremierMouvement_ || dy != longPas * direction)) {
            return false;
        }
        if (cible != nullptr && cible->obtenirCouleur() == couleur_) {
            return false;
        }
        if (dx == 0) {
            if (cible != nullptr) {
                return false;
            }
            if (abs(dy) == longPas) {
                int yIntermediaire = de.y + direction;
                if (echiquier.obtenirPiece({ de.x, yIntermediaire }) != nullptr) {
                    return false;
                }
            }
            if (estPremierMouvement_) {
                const_cast<Pion*>(this)->setPremierMouvementFaux();
            }
            return true;
        }
        else if (abs(dx) == pasAvant && dy == direction) {
            if (cible == nullptr) {
                return false;
            }
            if (estPremierMouvement_) {
                const_cast<Pion*>(this)->setPremierMouvementFaux();
            }
            return true;
        }
        return false;
    }


}
