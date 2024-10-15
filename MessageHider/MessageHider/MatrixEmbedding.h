#pragma once

#include "Steganography.h"

// ----- Explications -----
// Cr�ation d'une matrice pour diviser le support en blocs :
// Chaque bloc contiendra une petite partie du message secret
// On encode les bits gr�ce aux codes de Hamming, qui permet de
// minimiser les modifications et erreurs d'encodage sur le support,
// et �galement de les rendre plus difficilement d�tectables.
// Seulement un ou deux bits par bloc sont modifi�s.

// message transform� en matrice
// recalcul pour int�grer message dans image

class MatrixEmbedding : public Steganography
{
public:
	bool Encode(std::string message) override;
	std::string Decode() override;
};