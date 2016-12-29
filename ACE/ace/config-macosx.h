#ifndef ACE_CONFIG_MACOSX_ALL_H
#define ACE_CONFIG_MACOSX_ALL_H
#include <Availability.h>

#if   __MAC_OS_X_VERSION_MAX_ALLOWED >= 100900
#include "config-macosx-mavericks.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 100800
#include "config-macosx-mountainlion.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 100700
#include "config-macosx-lion.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 100600
#include "config-macosx-snowleopard.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 100500
#include "config-macosx-leopard.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 100400
#include "config-macosx-tiger.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 100300
#include "config-macosx-panther.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 100200
#include "config-macosx-jaguar.h"
#endif

#endif // ACE_CONFIG_MACOSX_ALL_H
