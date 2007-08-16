// -*- C++ -*-

//=============================================================================
/**
 * @file   Interpreter_Utils.h
 *
 * $Id$
 */
//=============================================================================

#ifndef TAO_INTERPRETER_UTILS_H
#define TAO_INTERPRETER_UTILS_H

#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/Functor.h"
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/CORBA_String.h"
#include "tao/SystemException.h"

#include "orbsvcs/Trader/trading_serv_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<>
class TAO_Trading_Serv_Export ACE_Hash<CORBA::String_var>
{
public:
  unsigned long operator () (const CORBA::String_var& string) const
  {
    unsigned long ret = ACE::hash_pjw (string.in ());
    return ret;
  }
};

template<>
class TAO_Trading_Serv_Export ACE_Less_Than<CORBA::String_var>
{
public:
  int operator () (const CORBA::String_var &lhs,
                   const CORBA::String_var &rhs) const
  {
    return ACE_OS::strcmp (lhs.in (), rhs.in ()) < 0;
  }
};

template<>
class TAO_Trading_Serv_Export ACE_Equal_To<CORBA::String_var>
{
public:
  int operator () (const CORBA::String_var &lhs,
                   const CORBA::String_var &rhs) const
  {
    return ACE_OS::strcmp (lhs.in (), rhs.in ()) == 0;
  }
};

typedef ACE_Hash_Map_Manager_Ex<CORBA::String_var,
                                int,
                                ACE_Hash<CORBA::String_var>,
                                ACE_Equal_To<CORBA::String_var>,
                                ACE_Null_Mutex>
  TAO_Lookup_Table;
typedef ACE_Hash_Map_Manager_Ex<CORBA::String_var,
                                CORBA::TypeCode_ptr,
                                ACE_Hash<CORBA::String_var>,
                                ACE_Equal_To<CORBA::String_var>,
                                ACE_Null_Mutex>
  TAO_Typecode_Table;

ACE_END_VERSIONED_NAMESPACE_DECL
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// *************************************************************
// TAO_Sequence_Extracter_Base
// *************************************************************

class TAO_Sequence_Extracter_Base
// = TITLE
//   Static methods used by the TAO_Sequence_Extracter class.
{
public:

  static CORBA::TCKind sequence_type (CORBA::TypeCode* code);
  // Return the sequence element type for the sequence whose typecode
  // is <code>.
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_INTERPRETER_UTILS_H */
