

#include "Raii.h"

namespace modele {

	bool Raii::metPropreRoiEchec() const {
        auto* piece = echiquier_->obtenirPiece(Test_);
        Roi* roi = echiquier_->estEnEchecRoi(piece->obtenirCouleur());

        if (roi != nullptr) {
            if (piece->obtenirCouleur() == roi->obtenirCouleur()) {
                return true;
            }
        }
        return false;
		
	}

} 
