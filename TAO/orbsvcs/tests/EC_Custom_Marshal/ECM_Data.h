/* -*- C++ -*- */
/**
 * @file ECM_Data.h
 * @brief Data types used to demonstrate custom marshaling of non-IDL data.
 */

#ifndef ECM_DATA_H
#define ECM_DATA_H

#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dataC.h"

struct ECM_Data
{
  using Inventory = ACE_Map_Manager<CORBA::ULong, CORBA::Double, ACE_Null_Mutex>;

  CORBA::String_var description;
  Inventory inventory;
};

extern CORBA::Boolean
operator<< (TAO_OutputCDR& cdr, ECM_Data& x);

extern CORBA::Boolean
operator>> (TAO_InputCDR& cdr, ECM_Data& x);

#endif /* ECM_SUPPLIER_H */
