
#pragma once
#include "Piece.h"

namespace modele {

    class Echiquier;

    class Dame : virtual public Piece {
    public:
        Dame(const CouleurPiece couleur)
            : Piece(couleur){}

        const QString obtenirSymboleUnicode() const override;

        bool estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier)const override;

        std::unique_ptr<Piece> copier() const override {
            return std::make_unique<Dame>(*this);
        }
    };

}
