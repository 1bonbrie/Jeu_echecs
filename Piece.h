

#pragma once
#include <QString>
#include <string>
#include "Coordonnees.h"
#include <memory>

enum class CouleurPiece { Blanc, Noir };


namespace modele {

    class Echiquier;

    class Piece {
    public:

        Piece(CouleurPiece couleur) : couleur_(couleur) {};

        virtual ~Piece() = default;

        virtual const QString obtenirSymboleUnicode() const = 0;

        virtual bool estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const = 0;

        CouleurPiece obtenirCouleur();

        virtual std::unique_ptr<Piece> copier() const = 0;

    protected:
        CouleurPiece couleur_;
         
    };

}
