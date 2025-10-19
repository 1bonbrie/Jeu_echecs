

#include "Echiquier.h"
#include "Piece.h"
#include "Raii.h"

#include "Pion.h"
#include "Tour.h"
#include "Cavalier.h"
#include "Fou.h"
#include "Dame.h"
#include "Roi.h"


namespace modele {

    CouleurPiece Echiquier::tour_ = CouleurPiece::Blanc;

    Echiquier::Echiquier(bool test)
        : plateau_(tailleEchiquier)
    {
        for (auto& ligne : plateau_) {
            ligne.resize(tailleEchiquier);
        }
        if (test == false) {
            initialiserPieces();
        }
    }

    Echiquier::~Echiquier() {}

    void Echiquier::reinitialiserPartie(const QString& disposition) {
        for (int y = 0; y < tailleEchiquier; y++) {
            for (int x = 0; x < tailleEchiquier; x++) {
                retirerPiece({ x,y });
            }
        }
        initialiserPieces(disposition);
    }

    bool Echiquier::deplacerPiece(Coordonnees de, Coordonnees vers) {

        if (vers.x < 0 || vers.y < 0 || de.x < 0 || de.y < 0) {
            emit deplacementRefuse(de, vers);
            return false;
        }

        Piece* piece = obtenirPiece(de);
        if (piece == nullptr) {
            emit deplacementRefuse(de, vers);
            return false;
        }

        if (piece->obtenirCouleur() != tour_) {
            emit deplacementRefuse(de, vers);
            return false;
        }


        Piece* cible = obtenirPiece(vers);
        if (cible != nullptr && cible->obtenirCouleur() == piece->obtenirCouleur()) {
            emit deplacementRefuse(de, vers);
            return false;
        }

        if (piece->estValideMouvement(de, vers, *this) == false) {
            emit deplacementRefuse(de, vers);
            return false;
        }


        bool estOkDeplacementTemp;
        {
            Raii deplacementTemporaire(de, vers, this);
            if (deplacementTemporaire.metPropreRoiEchec() == false) {
                estOkDeplacementTemp = true;
            }
            else {
                estOkDeplacementTemp = false;
                emit metPropreRoiEchec(vers);
            }
        }

        if (estOkDeplacementTemp == true) {
            placerPiece(std::move(plateau_[de.y][de.x]), vers);
        }
        else {
            emit deplacementRefuse(de, vers);
            return false;
        }

        Roi* roi = estEnEchecRoi(tour_ == CouleurPiece::Blanc ? CouleurPiece::Noir : CouleurPiece::Blanc);
        if (roi != nullptr) {
            echecAuRoi(roi->obtenirCouleur());
        }

        if (tour_ == CouleurPiece::Blanc) {
            tour_ = CouleurPiece::Noir;
        }
        else {
            tour_ = CouleurPiece::Blanc;
        }
        emit changerDeTour(tour_);
        emit deplacementReussi(de, vers);
        return true;
    }

    void Echiquier::verifierSiCaseValide(Coordonnees coor) {
        Piece* piece = obtenirPiece(coor);
        if (piece != nullptr && piece->obtenirCouleur() == tour_) {
            emit caseSelectionneeValide(coor);
        }
        else {
            emit caseSelectionneeInvalide(coor);
        }
    }

    void Echiquier::initialiserPieces(const QString& disposition) {
        Echiquier::tour_ = CouleurPiece::Blanc;

        if (disposition == "Standard"){
            for (int x = 0; x < tailleEchiquier; x++) {
                placerPiece(std::make_unique<Pion>(CouleurPiece::Noir), { x, 1 });
            }
            placerPiece(std::make_unique<Tour>(CouleurPiece::Noir), { 0, 0 });
            placerPiece(std::make_unique<Cavalier>(CouleurPiece::Noir), { 1, 0 });
            placerPiece(std::make_unique<Fou>(CouleurPiece::Noir), { 2, 0 });
            placerPiece(std::make_unique<Dame>(CouleurPiece::Noir), { 3, 0 });
            placerPiece(std::make_unique<Roi>(CouleurPiece::Noir), { 4, 0 });
            placerPiece(std::make_unique<Fou>(CouleurPiece::Noir), { 5, 0 });
            placerPiece(std::make_unique<Cavalier>(CouleurPiece::Noir), { 6, 0 });
            placerPiece(std::make_unique<Tour>(CouleurPiece::Noir), { 7, 0 });

            for (int x = 0; x < tailleEchiquier; x++) {
                placerPiece(std::make_unique<Pion>(CouleurPiece::Blanc), { x, 6 });
            }
            placerPiece(std::make_unique<Tour>(CouleurPiece::Blanc), { 0, 7 });
            placerPiece(std::make_unique<Cavalier>(CouleurPiece::Blanc), { 1, 7 });
            placerPiece(std::make_unique<Fou>(CouleurPiece::Blanc), { 2, 7 });
            placerPiece(std::make_unique<Dame>(CouleurPiece::Blanc), { 3, 7 });
            placerPiece(std::make_unique<Roi>(CouleurPiece::Blanc), { 4, 7 });
            placerPiece(std::make_unique<Fou>(CouleurPiece::Blanc), { 5, 7 });
            placerPiece(std::make_unique<Cavalier>(CouleurPiece::Blanc), { 6, 7 });
            placerPiece(std::make_unique<Tour>(CouleurPiece::Blanc), { 7, 7 });
        }
        else if (disposition == "Ouverture francaise") {
            initialiserPieces("Standard");
            deplacerPiece({ 4, 6 }, { 4, 4 });
            deplacerPiece({ 4, 1 }, { 4, 2 });
        }

        else if (disposition == "Ouverture sicilienne") {
            initialiserPieces("Standard");
            deplacerPiece({ 4, 6 }, { 4, 4 });
            deplacerPiece({ 2, 1 }, { 2, 3 });
        }
        
        else if (disposition == "Fin de partie - Option 1") {
            placerPiece(std::make_unique<Roi>(CouleurPiece::Blanc), { 6, 7 });
            placerPiece(std::make_unique<Fou>(CouleurPiece::Blanc), { 4, 6 });

            placerPiece(std::make_unique<Roi>(CouleurPiece::Noir), { 7, 5 });
            placerPiece(std::make_unique<Fou>(CouleurPiece::Noir), { 4, 3 });
            placerPiece(std::make_unique<Pion>(CouleurPiece::Noir), { 7, 3 });
            placerPiece(std::make_unique<Pion>(CouleurPiece::Noir), { 6, 4 });
        }

        else if (disposition == "Fin de partie - Option 2") {
            placerPiece(std::make_unique<Roi>(CouleurPiece::Noir), { 0, 0 });
            placerPiece(std::make_unique<Tour>(CouleurPiece::Noir), { 0, 1 });
            placerPiece(std::make_unique<Pion>(CouleurPiece::Noir), { 1, 6 });

            placerPiece(std::make_unique<Roi>(CouleurPiece::Blanc), { 7, 6 });
            placerPiece(std::make_unique<Dame>(CouleurPiece::Blanc), { 6, 1 });
            placerPiece(std::make_unique<Cavalier>(CouleurPiece::Blanc), { 5, 2 });
        }

        else if (disposition == "Fin de partie - Option 3") {
            placerPiece(std::make_unique<Roi>(CouleurPiece::Blanc), { 0, 0 });
            placerPiece(std::make_unique<Pion>(CouleurPiece::Blanc), { 0, 1 });
            placerPiece(std::make_unique<Tour>(CouleurPiece::Blanc), { 0, 7 });

            placerPiece(std::make_unique<Roi>(CouleurPiece::Noir), { 6, 3 });
            placerPiece(std::make_unique<Pion>(CouleurPiece::Noir), { 5, 4 });
            placerPiece(std::make_unique<Pion>(CouleurPiece::Noir), { 6, 4 });
            placerPiece(std::make_unique<Tour>(CouleurPiece::Noir), { 1, 5 });
        }
        else if (disposition == "Test M-V") {

            placerPiece(std::make_unique<Roi>(CouleurPiece::Blanc), { 4, 4 });

            deplacerPiece({ 4, 4 }, { 7, 7 });  

            placerPiece(std::make_unique<Pion>(CouleurPiece::Blanc), { 2, 2 });
            retirerPiece({ 2, 2 });  

            placerPiece(std::make_unique<Tour>(CouleurPiece::Noir), { 5, 0 });

            tour_ = CouleurPiece::Noir;
            deplacerPiece({ 5, 0 }, { 4, 0 });  
        }
        emit changerDeTour(tour_);
    }

    Piece* Echiquier::obtenirPiece(Coordonnees coor) const {
        if (coor.x < 0 || coor.x >= tailleEchiquier || coor.y < 0 || coor.y >= tailleEchiquier) {
            return nullptr;
        }
            
        return plateau_[coor.y][coor.x].get();
    }

    std::unique_ptr<Piece> Echiquier::retirerPiece(Coordonnees coor) {
        if (obtenirPiece(coor) != nullptr) {
            emit pieceEnlevee(coor);
            return move(plateau_[coor.y][coor.x]);
        }
        else {
            return nullptr;
        }
    }

    void Echiquier::placerPiece(std::unique_ptr<Piece> piece, Coordonnees coor) {
        if (coor.x < 0 || coor.x >= tailleEchiquier || coor.y < 0 || coor.y >= tailleEchiquier) {
            return;
        }
        emit piecePlacee(piece->obtenirSymboleUnicode(), coor);
        plateau_[coor.y][coor.x] = std::move(piece);
    }

    Roi* Echiquier::estEnEchecRoi(CouleurPiece couleur) {
        Roi* roiCible = nullptr;
        Coordonnees posRoi;

        for (int y = 0; y < tailleEchiquier && !roiCible; y++) {
            for (int x = 0; x < tailleEchiquier && !roiCible; x++) {
                Piece* piece = obtenirPiece({ x, y });
                if (auto* roi = dynamic_cast<Roi*>(piece)) {
                    if (roi->obtenirCouleur() == couleur) {
                        roiCible = roi;
                        posRoi = { x, y };
                    }
                }
            }
        }

        if (!roiCible)
            return nullptr;

        CouleurPiece couleurAdverse = (couleur == CouleurPiece::Blanc)
            ? CouleurPiece::Noir
            : CouleurPiece::Blanc;

        for (int y = 0; y < tailleEchiquier; y++) {
            for (int x = 0; x < tailleEchiquier; x++) {
                Piece* piece = obtenirPiece({ x, y });
                if (piece && piece->obtenirCouleur() == couleurAdverse) {
                    if (piece->estValideMouvement({ x, y }, posRoi, *this)) {
                        return roiCible;
                    }
                }
            }
        }

        return nullptr;
    }
}
