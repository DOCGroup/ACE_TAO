// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Cubit
// 
// = FILENAME
//    RTI_IO.h
//
// = AUTHOR
//    Dave Meyer <dmeyer@std.saic.com>
// 
// ============================================================================

#if !defined (_RTIFUNCS_H)
#define _RTIFUNCS_H

#include <iostream.h>
#include "cubitC.h"

ostream &operator<< (ostream &o, Cubit::RtiPacket const &arg);

#endif /* _RTIFUNCS_H */
