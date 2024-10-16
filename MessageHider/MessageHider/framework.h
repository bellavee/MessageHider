// header.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets
//

#pragma once

#include "targetver.h"
//#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows
#include <windows.h>
// Fichiers d'en-tête C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>


#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "zlib/zlib.h"
#pragma comment(lib, "libz-static.lib")