
#pragma once


#include <QScreen>
#include <QGuiApplication>
#include <QMainWindow>
#include <QPushButton>
#include "ui_Livrable1.h"
#include "Echiquier.h"  
#include "qlabel.h"
#include "Piece.h"
#include <qstring.h>



namespace vue {

    struct Resolution {
        int largeur;
        int hauteur;
    };

    class EchiquierWindow : public QMainWindow {
        Q_OBJECT

    public:
        EchiquierWindow(Resolution resolution, int taillePieces, int tailleCases, QWidget* parent = nullptr);
        ~EchiquierWindow();


    public slots:
        void traiterDeplacementReussi(Coordonnees de, Coordonnees vers);
        void traiterDeplacementRefuse(Coordonnees de, Coordonnees vers);

        void retirerPieceDeWindow(Coordonnees coor);
        void traiterErreurPropreRoiEchec(Coordonnees coor);
        void accepterSelectionCase(Coordonnees coor);
        void refuserSelectionCase(Coordonnees coor);

        void placerPieceDansWindow(const QString& symbole, Coordonnees coor);

        void mettreAJourTour(CouleurPiece couleur);
        void traiterEchecAuRoi(CouleurPiece couleurRoi);

    signals:
        void demanderDeplacement(Coordonnees de, Coordonnees vers);
        void demanderNouvellePartie(const QString& disposition);
        void demanderVerificationCase(Coordonnees coor);

    private:
        
        void creerMenu();
        void creerBoutonsEchiquierGraphique();
        void gererClic(Coordonnees coor);
        void faireNouvellePartie();
        void faireBrillerCaseSelectionnee(Coordonnees coor);
        void desactiverSurbrillance(Coordonnees coor);
        void clignoterCaseErreur(Coordonnees coor);

        Ui::Livrable1Class ui;
        static bool estPremierClic_;
        static Coordonnees src;
        QPushButton* cases_[tailleEchiquier][tailleEchiquier];
        std::unique_ptr<modele::Echiquier> echiquierLogique_;
        Resolution resolution_;
        int taillePieces_;
        int tailleCases_;
        QLabel* tourLabel_;

    };

}
