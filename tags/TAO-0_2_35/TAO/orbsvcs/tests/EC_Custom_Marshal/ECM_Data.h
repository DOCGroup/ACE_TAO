/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   Simple C++ data to show how custom marshalling can be used for
//   non-IDL types.
//
// ============================================================================

#ifndef ECM_DATA_H
#define ECM_DATA_H

#include "ace/Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dataC.h"

struct ECM_Data
{
  typedef ACE_Map_Manager<CORBA::ULong,CORBA::Double,ACE_Null_Mutex> Inventory;
  // Yes, an STL type could be used here and yes, we could write
  // generic algorithms for marshalling and demarshalling using
  // iterators.
  // But ACE (and TAO) must be ported to platforms whose C++ compilers
  // will crash and burn when faced with an STL construct.

  CORBA::String_var description;
  Inventory inventory;
};

extern CORBA::Boolean
operator<< (TAO_OutputCDR& cdr, ECM_Data& x);

extern CORBA::Boolean
operator>> (TAO_InputCDR& cdr, ECM_Data& x);

#endif /* ECM_SUPPLIER_H */
