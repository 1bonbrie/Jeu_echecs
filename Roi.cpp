

#include "Roi.h"
#include "Echiquier.h"
#include "qdebug.h"



namespace modele {

    int Roi::nRois_ = 0;

    Roi::Roi(CouleurPiece couleur)
        : Piece(couleur)
    {

        nRois_++;
        if (nRois_ > nMax_) {
            --nRois_;
            throw TropDeRoisException();
        }
    }

    Roi::~Roi() 
    {
        --nRois_;
    }

    const QString Roi::obtenirSymboleUnicode() const {
        return (couleur_ == CouleurPiece::Blanc) ? "♔" : "♚";
    }


    bool Roi::estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const {
        
        const int pasAvant = 1;

        Piece* cible = echiquier.obtenirPiece(vers);
        if (cible != nullptr && cible->obtenirCouleur() == couleur_) {
            return false;
        }
        
        int dx = abs(vers.x - de.x);
        int dy = abs(vers.y - de.y);
        return dx <= pasAvant && dy <= pasAvant && !(dx == 0 && dy == 0);
    }

    const int Roi::obtenirNombreRois() const {
        return nRois_;
    }


}


