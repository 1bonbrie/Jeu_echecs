


#include "EchiquierWindow.h"
#include <qpushbutton.h>
#include <qtimer.h>
#include <qdebug.h>
#include <sstream>
#include <string>
#include <qinputdialog.h>
#include <memory>

namespace vue {

    bool EchiquierWindow::estPremierClic_ = true;
    Coordonnees EchiquierWindow::src = { 0,0 };

    //Slots:
    void EchiquierWindow::traiterDeplacementReussi(Coordonnees de, Coordonnees vers) {
        cases_[de.y][de.x]->setText("");
        desactiverSurbrillance(de);
        estPremierClic_ = true;
    }

    void EchiquierWindow::traiterDeplacementRefuse(Coordonnees de, Coordonnees vers) {
        desactiverSurbrillance(de);
        clignoterCaseErreur(de);
        estPremierClic_ = true;
    }

    void EchiquierWindow::placerPieceDansWindow(const QString& symbole, Coordonnees coor) {

        cases_[coor.y][coor.x]->setText(symbole);
        cases_[coor.y][coor.x]->setFont(QFont("Arial", taillePieces_));
    }

    void EchiquierWindow::retirerPieceDeWindow(Coordonnees coor) {
        cases_[coor.y][coor.x]->setText("");
    }
    
    void EchiquierWindow::traiterEchecAuRoi(CouleurPiece couleurRoi) {

        QString message;
        if (couleurRoi == CouleurPiece::Blanc) {
            message = "Le roi blanc est en échec !";
        }
        else {
            message = "Le roi noir est en échec !";
        }

        QMessageBox* fenetre = new QMessageBox(QMessageBox::NoIcon,"Attention", message, QMessageBox::NoButton, this);
        fenetre->setWindowModality(Qt::NonModal);
        fenetre->show();

    }

    void EchiquierWindow::mettreAJourTour(CouleurPiece couleur) {
        QString texte = (couleur == CouleurPiece::Blanc) ? "Tour : Blanc" : "Tour : Noir";
        tourLabel_->setText(texte);
    }

    void EchiquierWindow::traiterErreurPropreRoiEchec(Coordonnees coor) {
        retirerPieceDeWindow(coor);

        const QString message = "Met le roi du joueur en echec volontairement";
        QMessageBox* fenetre = new QMessageBox(QMessageBox::NoIcon, "Erreur", message, QMessageBox::NoButton, this);
        fenetre->setWindowModality(Qt::NonModal);
        fenetre->show();
    }

    void EchiquierWindow::accepterSelectionCase(Coordonnees coor) {
        src = coor;
        estPremierClic_ = false;
        faireBrillerCaseSelectionnee(src);
    }

    void EchiquierWindow::refuserSelectionCase(Coordonnees coor) {
    }


    //Méthodes de classe:
    void EchiquierWindow::gererClic(Coordonnees coor)
    {
        if (estPremierClic_ == true) {
            emit demanderVerificationCase(coor);
            return;
        }
        else{
            emit demanderDeplacement(src, coor);
        }
    }

    void EchiquierWindow::faireNouvellePartie() {

        QStringList listeChoix;
        listeChoix << "Standard" << "Ouverture francaise" << "Ouverture sicilienne" << "Fin de partie - Option 1"
                   << "Fin de partie - Option 2" << "Fin de partie - Option 3" << "Test M-V";

        bool estFaitChoix;
        QString choix = QInputDialog::getItem(this,
            "Choisir un layout",
            "Disposition des pièces :",
            listeChoix, 0, false, &estFaitChoix);
            estPremierClic_ = true;
            emit demanderNouvellePartie(choix);
    }

    void EchiquierWindow::desactiverSurbrillance(Coordonnees coor) {
        QString couleur = ((coor.x + coor.y) % 2 == 0) ? "#e8e5da" : "#69923e";
        cases_[coor.y][coor.x]->setStyleSheet
        ("background-color: " 
         + couleur 
         + "; border: none;");
    }

    void EchiquierWindow::faireBrillerCaseSelectionnee(Coordonnees coor) {
        bool estBlanc = ((coor.x + coor.y) % 2 == 0);
        QString couleurSurbrillance = estBlanc ? "#f7f6d2" : "#a9c75e";
        cases_[coor.y][coor.x]->setStyleSheet(
            "background-color: " + couleurSurbrillance + ";"
            "border: none;"
        );

    }

    void EchiquierWindow::clignoterCaseErreur(Coordonnees coor) {
        const int dureeClignotement = 300;

        QPushButton* caseBouton = cases_[coor.y][coor.x];

        caseBouton->setStyleSheet("background-color: #d66a5e; border: none;");
        QTimer::singleShot(dureeClignotement, this, [this, coor]() {
            QString couleur = ((coor.x + coor.y) % 2 == 0) ? "#e8e5da" : "#69923e";
            cases_[coor.y][coor.x]->setStyleSheet("background-color: " + couleur + "; border: none;");
            });
    }

    void EchiquierWindow::creerBoutonsEchiquierGraphique()
    {
        ui.gridLayout->setSpacing(0); 

        for (int y = 0; y < tailleEchiquier; y++) {
            for (int x = 0; x < tailleEchiquier; x++) {
                QPushButton* boutonCase = new QPushButton;
                boutonCase->setFixedSize(tailleCases_, tailleCases_);    
                QString couleur = ((x + y) % 2 == 0) ? "#e8e5da" : "#69923e";
                boutonCase->setStyleSheet("background-color: " + couleur + "; border: none;");

                cases_[y][x] = boutonCase;
                ui.gridLayout->addWidget(boutonCase, y, x);

                connect(boutonCase, &QPushButton::clicked, this, [this, x, y]() {gererClic({ x, y }); });
            }
        }
    }


    void EchiquierWindow::creerMenu() {
        QWidget* central = new QWidget(this);
        setCentralWidget(central);
        QHBoxLayout* layoutPrincipal = new QHBoxLayout(central);

       QWidget* menuWidget = new QWidget(central);
        QVBoxLayout* layoutMenu = new QVBoxLayout(menuWidget);

        QPushButton* boutonNouvellePartie = new QPushButton("Nouvelle partie", menuWidget);
        QPushButton* boutonQuitter = new QPushButton("Quitter", menuWidget);

        layoutMenu->addWidget(boutonNouvellePartie);
        layoutMenu->addWidget(boutonQuitter);
        layoutMenu->addStretch(); 

        QWidget* plateauWidget = new QWidget(central);
        QGridLayout* gridLayout = new QGridLayout(plateauWidget);
        this->ui.gridLayout = gridLayout; 

        plateauWidget->setLayout(gridLayout);

        layoutPrincipal->addWidget(plateauWidget);
        layoutPrincipal->addWidget(menuWidget);

        tourLabel_ = new QLabel("Tour : Blanc", this);
        tourLabel_->setAlignment(Qt::AlignCenter);
        tourLabel_->setStyleSheet("font-size: 18px; font-weight: bold; padding: 10px;");
        tourLabel_->setFont(QFont("Arial"));
        layoutPrincipal->addWidget(tourLabel_);

        connect(boutonNouvellePartie, &QPushButton::clicked, this, &EchiquierWindow::faireNouvellePartie);
        connect(boutonQuitter, &QPushButton::clicked, this, &EchiquierWindow::close);
    }


    EchiquierWindow::EchiquierWindow(Resolution resolution, int taillePieces, int tailleCases, QWidget* parent)
        : QMainWindow(parent), resolution_(resolution), taillePieces_(taillePieces), tailleCases_(tailleCases),
        echiquierLogique_()
    {


        ui.setupUi(this);
        echiquierLogique_ = std::make_unique<modele::Echiquier>();

        connect(echiquierLogique_.get(), &modele::Echiquier::piecePlacee,
            this, &EchiquierWindow::placerPieceDansWindow);

        connect(this, &EchiquierWindow::demanderDeplacement,
            echiquierLogique_.get(), &modele::Echiquier::deplacerPiece);

        connect(this, &EchiquierWindow::demanderNouvellePartie,
            echiquierLogique_.get(), &modele::Echiquier::reinitialiserPartie);

        connect(this, &EchiquierWindow::demanderNouvellePartie,
            echiquierLogique_.get(), &modele::Echiquier::reinitialiserPartie);

        connect(this, &EchiquierWindow::demanderVerificationCase,
            echiquierLogique_.get(), &modele::Echiquier::verifierSiCaseValide);

        connect(echiquierLogique_.get(), &modele::Echiquier::deplacementRefuse,
            this, &EchiquierWindow::traiterDeplacementRefuse);

        connect(echiquierLogique_.get(), &modele::Echiquier::deplacementReussi,
            this, &EchiquierWindow::traiterDeplacementReussi);

        connect(echiquierLogique_.get(), &modele::Echiquier::echecAuRoi,
            this, &EchiquierWindow::traiterEchecAuRoi);


        connect(echiquierLogique_.get(), &modele::Echiquier::changerDeTour,
            this, &EchiquierWindow::mettreAJourTour);

        connect(echiquierLogique_.get(), &modele::Echiquier::metPropreRoiEchec,
            this, &EchiquierWindow::traiterErreurPropreRoiEchec);

        connect(echiquierLogique_.get(), &modele::Echiquier::pieceEnlevee,
            this, &EchiquierWindow::retirerPieceDeWindow);

        connect(echiquierLogique_.get(), &modele::Echiquier::caseSelectionneeInvalide,
            this, &EchiquierWindow::refuserSelectionCase);


        connect(echiquierLogique_.get(), &modele::Echiquier::caseSelectionneeValide,
            this, &EchiquierWindow::accepterSelectionCase);

        resize(resolution_.largeur, resolution_.hauteur);
        creerMenu();
        creerBoutonsEchiquierGraphique();
        emit demanderNouvellePartie("Standard");

    }


    EchiquierWindow::~EchiquierWindow()
    {

    }

}
