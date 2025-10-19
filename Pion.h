
#pragma once
#include "Piece.h"


namespace modele {


    class Pion : virtual public Piece{
    public:
        Pion(const CouleurPiece& couleur);

        const QString obtenirSymboleUnicode() const override;

        bool estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const override;

        void setPremierMouvementFaux();
        bool estPremierMouvement() const ;

        std::unique_ptr<Piece> copier() const override {
            return std::make_unique<Pion>(*this);
        }

    private:
        bool estPremierMouvement_;
    
    };
}

