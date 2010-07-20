//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------
#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOSOUND
#define NOKANJI
#define NOIMAGE
#define NOTAPE

#define STRICT

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <atlbase.h>

#include <Orbitersdk.h>

#include <map>
#include <vector>
#include <string>
#include <deque>
#include <sstream>
#include <iomanip>

#include <WTL/atlapp.h>

#include <boost/ptr_container/ptr_container.hpp>
