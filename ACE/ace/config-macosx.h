#ifndef ACE_CONFIG_MACOSX_ALL_H
#define ACE_CONFIG_MACOSX_ALL_H
#include <Availability.h>

#if   __MAC_OS_X_VERSION_MAX_ALLOWED >= 101400
#include "config-macosx-mojave.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 101300
#include "config-macosx-highsierra.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 101200
#include "config-macosx-sierra.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 101100
#include "config-macosx-elcapitan.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 101000
#include "config-macosx-yosemite.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 1090
#include "config-macosx-mavericks.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 1080
#include "config-macosx-mountainlion.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
#include "config-macosx-lion.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
#include "config-macosx-snowleopard.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
#include "config-macosx-leopard.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 1040
#include "config-macosx-tiger.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 1030
#include "config-macosx-panther.h"
#elif __MAC_OS_X_VERSION_MAX_ALLOWED >= 1020
#include "config-macosx-jaguar.h"
#else
#error Cannot detect MacOSX SDK version
#endif

#endif // ACE_CONFIG_MACOSX_ALL_H
