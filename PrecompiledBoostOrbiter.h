//----------------------------------------------------------------------------
// This file is part of the BoostOrbiter project, and is subject to the terms
// and conditions defined in file 'license.txt'. Full list of contributors is
// available in file 'contributors.txt'.
//----------------------------------------------------------------------------

#ifndef __BOOSTORBITER_PRECOMPILEDBOOSTORBITER_H__
#define __BOOSTORBITER_PRECOMPILEDBOOSTORBITER_H__

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

#include <Orbiterapi.h>
#include <Orbitersdk.h>

#include <map>
#include <vector>
#include <string>
#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <WTL/atlapp.h>

#endif // __BOOSTORBITER_PRECOMPILEDBOOSTORBITER_H__
