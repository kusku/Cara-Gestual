#ifndef DEFINES_H
#define DEFINES_H

#include "stdafx.h"

#define CHECKED_DELETE(x) if(x!=NULL) {delete x; x=NULL;}
#define CHECKED_RELEASE(x) if(x!=NULL) {x->Release(); x=NULL;}

#endif