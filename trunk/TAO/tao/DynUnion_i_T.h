// $Id$

// ====================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynUnion_i_T.h
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ====================================================================

#ifndef TAO_DYNUNION_I_T_H
#define TAO_DYNUNION_I_T_H

#include "tao/corbafwd.h"

class DU_Extractor_base
{
  // = TITLE
  //    DU_Extractor_base
  //
  // = DESCRIPTION
  //    Base class of template class below
  //
public:
  virtual CORBA::Boolean check_match (const CORBA_Any& inside_any,
                                      const CORBA_Any& outside_any) = 0;
};

template <class Type>
class DU_Extractor : public DU_Extractor_base
{
  // = TITLE
  //    DU_Extractor
  //
  // = DESCRIPTION
  //    Template class functor to compare Anys in TAO_DynUnion_i
  //    constructor with an Any arg and member function from_any().
  //    Some compilers won't accept these eclarations inside
  //    TAO_DynUnion_i or even at global scope in the same file.
  //
public:
  virtual ~DU_Extractor ();

  virtual CORBA::Boolean check_match (const CORBA_Any& inside_any,
                                      const CORBA_Any& outside_any);
private:
  Type arg_index_;
  Type member_index_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/DynUnion_i_T.cpp"
// On Win32 platforms, this code will be included as template source
// code and will not be inlined. Therefore, we first turn off
// ACE_INLINE, set it to be nothing, include the code, and then turn
// ACE_INLINE back to its original setting. All this nonsense is
// necessary, since the generic template code that needs to be
// specialized cannot be inlined, else the compiler will ignore the
// specialization code. Also, the specialization code *must* be
// inlined or the compiler will ignore the specializations.
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DynUnion_i_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_DYNUNION_I_T_H */
