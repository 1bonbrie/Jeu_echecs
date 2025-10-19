#pragma once
#include <QObject>
#include <memory>
#include "Coordonnees.h"
#include "Echiquier.h"


namespace controleur {


    class VueEchiquier;

    class ControleurEchiquier : public QObject
    {
        Q_OBJECT

    public:
        explicit ControleurEchiquier(VueEchiquier* vue);

    public slots:
        void gererClic(modele::Coordonnees coor);
        void demarrerNouvellePartie();

    private:
        std::shared_ptr<modele::Echiquier> echiquier_;
        VueEchiquier* vue_;
        bool estPremierClic_;
        modele::Coordonnees src_;
        CouleurPiece tour_;
    };


}