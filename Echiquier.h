

#pragma once
#include <memory>
#include <vector>
#include <qdebug.h>
#include <qmessagebox.h>
#include <utility>
#include <qstring.h>


#include "Coordonnees.h"
#include "Piece.h"
#include "Roi.h"


const int tailleEchiquier = 8;

namespace modele {

    class Raii;

    class Echiquier : public QObject {
        Q_OBJECT
    public:

        Echiquier(bool test = false);
        ~Echiquier();

        CouleurPiece obtenirTour() const{
            return tour_;
        }
        Piece* obtenirPiece(Coordonnees coor) const;

    public slots:
        bool deplacerPiece(Coordonnees de, Coordonnees vers);
        void reinitialiserPartie(const QString& disposition);
        void verifierSiCaseValide(Coordonnees coor);

    signals:
        void deplacementReussi(Coordonnees de, Coordonnees vers);
        void deplacementRefuse(Coordonnees de, Coordonnees vers);
        void echecAuRoi(CouleurPiece couleurRoi);
        void piecePlacee(const QString& symbole, Coordonnees coor);
        void pieceEnlevee(Coordonnees coor);
        void changerDeTour(CouleurPiece tour);
        void metPropreRoiEchec(Coordonnees coor);
        void caseSelectionneeValide(Coordonnees coor);
        void caseSelectionneeInvalide(Coordonnees coor);


    private:

        void placerPiece(std::unique_ptr<Piece> piece, Coordonnees coor);
        Roi* estEnEchecRoi(CouleurPiece couleur);
        std::unique_ptr<Piece> retirerPiece(Coordonnees coor);
        void initialiserPieces(const QString& disposition = "Standard");
        friend class Raii;
        static CouleurPiece tour_;
        std::vector<std::vector<std::unique_ptr<Piece>>> plateau_;
    };
}


