#include "MatrixEmbedding.h"

bool MatrixEmbedding::Encode(std::string message)
{
    if (!Steganography::Encode(message)) return false;

    //
    
    Reset();
    return true;
}

std::string MatrixEmbedding::Decode()
{
    return std::string();
}