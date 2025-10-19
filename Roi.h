#pragma once

#include "Piece.h"
#include <string>

const int nombreMaxRois = 2;

namespace modele {

    class Echiquier;

    class TropDeRoisException : public std::exception {
    public:
        std::string nature() { return message_; }

    private:
        std::string message_ = "Il est impossible de creer plus de 2 rois sur l'echiquier";
    };


    class Roi : virtual public Piece {
    public:

        Roi(CouleurPiece couleur);

        ~Roi();
            
        const QString obtenirSymboleUnicode() const override;

        bool estValideMouvement(Coordonnees de, Coordonnees vers, const Echiquier& echiquier) const override;

        const int obtenirNombreRois() const;

        static void reinitialiserCompteur() { nRois_ = 0; } 

        std::unique_ptr<Piece> copier() const override {
            return std::make_unique<Roi>(*this);
        }
    private:
        static int nRois_;
        static const int nMax_ = nombreMaxRois;
    };
}
