#pragma once

#include "Steganography.h"

// ----- Explications -----
// Création d'une matrice pour diviser le support en blocs :
// Chaque bloc contiendra une petite partie du message secret
// On encode les bits grâce aux codes de Hamming, qui permet de
// minimiser les modifications et erreurs d'encodage sur le support,
// et également de les rendre plus difficilement détectables.
// Seulement un ou deux bits par bloc sont modifiés.

// message transformé en matrice
// recalcul pour intégrer message dans image

class MatrixEmbedding : public Steganography
{
public:
	bool Encode(std::string message) override;
	std::string Decode() override;
};