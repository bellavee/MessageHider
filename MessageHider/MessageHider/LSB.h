#pragma once

#include "Steganography.h"

// ----- Explications -----
// On récupère chaque pixel de l'image et on modifie
// la composante B du code (R, G, B).
// Il faut d'abord masquer le bit de poids faible avec AND
// Puis insérer le bit à cacher avec OR.

class LSB : public Steganography
{
public:
	bool Encode(std::string message) override;
	std::string Decode() override;
};