
#pragma once
#include "Piece.h"
#include "Echiquier.h"


namespace modele {

    class Raii {
    public:
        Raii(Coordonnees de, Coordonnees vers, Echiquier* ech)
              : Originales_(de), Test_(vers), echiquier_(ech)
        {
            if (echiquier_->obtenirPiece(Test_) != nullptr) {
                temporaire_ = echiquier_->retirerPiece(Test_);
            }
            std::unique_ptr<Piece> piece = echiquier_->retirerPiece(Originales_);
            echiquier_->placerPiece(move(piece), Test_);
        };

        ~Raii()
        {
            std::unique_ptr<Piece> piece = echiquier_->retirerPiece(Test_);
            echiquier_->placerPiece(move(piece) , Originales_);

            if (temporaire_ != nullptr) {
                echiquier_->placerPiece(move(temporaire_), Test_);
            }
        }

        bool metPropreRoiEchec() const;

        Raii(const Raii&) = delete;
        Raii& operator=(const Raii&) = delete;

    private:
        Echiquier* echiquier_;
        Coordonnees Test_;
        Coordonnees Originales_;
        std::unique_ptr<Piece> temporaire_;
    };

} 

