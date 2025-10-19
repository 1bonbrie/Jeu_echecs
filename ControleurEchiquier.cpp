#include "ControleurEchiquier.h"
#include "Echiquier.h"

#include <QInputDialog>
#include <QDebug>

ControleurEchiquier::ControleurEchiquier(VueEchiquier* vue)
    : vue_(vue), estPremierClic_(true), tour_(CouleurPiece::Blanc)
{
    echiquier_ = std::make_shared<modele::Echiquier>();

    QObject::connect(vue_, &VueEchiquier::caseCliquee, this, &ControleurEchiquier::gererClic);
    QObject::connect(vue_, &VueEchiquier::nouvellePartieDemandee, this, &ControleurEchiquier::demarrerNouvellePartie);

    vue_->afficherPieces(*echiquier_);
}

void ControleurEchiquier::gererClic(modele::Coordonnees coor)
{
    if (estPremierClic_ && echiquier_->obtenirPiece(coor) != nullptr
        && echiquier_->obtenirPiece(coor)->obtenirCouleur() == tour_) {
        src_ = coor;
        estPremierClic_ = false;
        vue_->faireBrillerCaseSelectionnee(coor);
        return;
    }

    bool deplacementValide = echiquier_->deplacerPiece(src_, coor);

    if (deplacementValide) {
        tour_ = (tour_ == CouleurPiece::Blanc) ? CouleurPiece::Noir : CouleurPiece::Blanc;
    }
    else {
        qDebug() << "D�placement invalide.";
    }

    vue_->desactiverSurbrillance(src_);
    estPremierClic_ = true;
    vue_->afficherPieces(*echiquier_);
}

void ControleurEchiquier::demarrerNouvellePartie()
{
    QStringList choix = { "Standard", "Ouverture francaise", "Ouverture sicilienne" };
    bool ok = false;

    QString reponse = QInputDialog::getItem(vue_, "Nouvelle Partie", "Disposition des pi�ces :", choix, 0, false, &ok);

    if (!ok || reponse.isEmpty()) {
        return;
    }

    modele::DispositionDepart disposition;

    if (reponse == "Standard")
        disposition = modele::DispositionDepart::Standard;
    else if (reponse == "Ouverture francaise")
        disposition = modele::DispositionDepart::DefenseFrancaise;
    else
        disposition = modele::DispositionDepart::DefenseSicilienne;

    echiquier_->reinitialiserPartie(disposition);
    tour_ = CouleurPiece::Blanc;
    estPremierClic_ = true;

    vue_->afficherPieces(*echiquier_);
}
