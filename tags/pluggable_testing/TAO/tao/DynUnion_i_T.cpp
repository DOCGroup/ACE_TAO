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

#ifndef TAO_DYNUNION_I_T_C
#define TAO_DYNUNION_I_T_C

#include "tao/DynUnion_i_T.h"

// Method of template class that serves as a functor for all that
// compares Anys constructed from all legal discriminator
// types - some are specifically defined in DynUnion_i.cpp.

template <class Type>
DU_Extractor<Type>::~DU_Extractor ()
{
}

template <class Type>
CORBA::Boolean
DU_Extractor<Type>::check_match (const CORBA_Any& inside_any,
                                 const CORBA_Any& outside_any)
{
  inside_any >>= this->member_index_;
  outside_any >>= this->arg_index_;
  return this->member_index_ == this->arg_index_;
}

#endif /* TAO_DYNUNION_I_T_C */
