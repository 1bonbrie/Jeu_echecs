

#pragma once

#include "Piece.h"
#include <memory>

namespace modele {

    class Echiquier;

    class Cavalier : virtual public Piece {
    public:
        Cavalier(CouleurPiece couleur)
            : Piece(couleur)
        {}

        const QString obtenirSymboleUnicode() const override;

        bool estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const override;

        std::unique_ptr<Piece> copier() const override {
            return std::make_unique<Cavalier>(*this);
        }
    };

}