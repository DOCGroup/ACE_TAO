/* -*- C++ -*- */
// $Id$
// ========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynUnion_i_T.cpp
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ========================================================================

#if !defined TAO_DYNUNION_I_T_C
#define TAO_DYNUNION_I_T_C

#include "tao/DynUnion_i_T.h"

// Method of template class that serves as a functor for all that
// compares Anys constructed from all legal discriminator
// types - some are specifically defined in DynUnion_i.cpp.

template <class Type>
CORBA::Boolean
DU_Extractor<Type>::check_match (const CORBA_Any& inside_any,
                                 const CORBA_Any& outside_any)
{
  inside_any >>= this->member_index_;
  outside_any >>= this->arg_index_;

#if defined (ACE_LACKS_LONGLONG_T)
  // Oh, this is ugly.  But, it's the only way I can think of
  // to satisfy instantiations of CORBA::LongLong as well as
  // all other (native) types.
  return ! ACE_OS::memcmp (&this->member_index_,
                           &this->arg_index_,
                           sizeof (Type));
#else  /* ! ACE_LACKS_LONGLONG_T */
  return this->member_index_ == this->arg_index_;
#endif /* ! ACE_LACKS_LONGLONG_T */
}

#endif /* TAO_DYNUNION_I_T_C */
