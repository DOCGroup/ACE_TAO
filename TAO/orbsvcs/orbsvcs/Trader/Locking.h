/* -*- C++ -*- */

// =========================================================================
// $Id$
//
// = LIBRARY
//    Trading
//
// = FILENAME
//    Locking.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_LOCKING_H
#define TAO_LOCKING_H

#include "tao/corba.h"

// Macros for obtaining read/write locks that are
// automatically released at the end of scope.
// In case of failure, CORBA::SystemException is thrown.

#define TAO_WRITE_GUARD(MUTEX,OBJ,LOCK) \
ACE_Write_Guard<MUTEX> OBJ (LOCK); \
if (OBJ.locked () == 0) \
  TAO_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_NO));

#define TAO_READ_GUARD(MUTEX,OBJ,LOCK) \
ACE_Read_Guard<MUTEX> OBJ (LOCK); \
if (OBJ.locked () == 0) \
  TAO_THROW (CORBA::UNKNOWN (CORBA::COMPLETED_NO));

#define TAO_WRITE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
ACE_Write_Guard<MUTEX> OBJ (LOCK); \
if (OBJ.locked () == 0) \
  TAO_THROW_RETURN (CORBA::UNKNOWN (CORBA::COMPLETED_NO), RETURN);

#define TAO_READ_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
ACE_Read_Guard<MUTEX> OBJ (LOCK); \
if (OBJ.locked () == 0) \
  TAO_THROW_RETURN (CORBA::UNKNOWN (CORBA::COMPLETED_NO), RETURN);

#endif /* TAO_LOCKING_H */

