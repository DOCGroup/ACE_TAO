// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Seq_Tmplinst.cpp
//
// = DESCRIPTION
//   Explicit instantiation of the TAO sequence template classes for ValueBase
//   and AbstractBase. These instantiations free us from having to
//   generate them over and over with the IDL compiler.
//
// = AUTHOR
//     Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================
#include "ace/config-all.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || \
    defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#include "tao/Sequence_T.h"
#include "tao/Any.h"
#include "ValueBase.h"
#include "AbstractBase.h"
#include "Sequence_T.h"
#include "ValueFactory.h"
#endif /*if ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA*/

ACE_RCSID (Valuetype,
           Seq_Tmplinst,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Valuetype_Manager<CORBA::ValueBase,
                                     CORBA::ValueBase_var>;

template class TAO_Unbounded_Valuetype_Sequence<CORBA::ValueBase,
                                                CORBA::ValueBase_var>;

template class TAO_Unbounded_Pseudo_Sequence<CORBA::AbstractBase>;

template class TAO_Value_Var_T <CORBA::ValueFactoryBase>;


template class TAO_Value_Var_T <CORBA::ValueBase>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Valuetype_Manager<CORBA::ValueBase,
                                          CORBA::ValueBase_var,
                                          CORBA::tao_ValueBase_life>
#pragma instantiate TAO_Unbounded_Valuetype_Sequence<CORBA::ValueBase, \
                                                     CORBA::ValueBase_var, \
                                                     CORBA::tao_ValueBase_life>
#pragma instantiate TAO_Unbounded_Pseudo_Sequence<CORBA::AbstractBase, \
                                                  CORBA::AbstractBase_var>

#pragma instantiate TAO_Value_Var_T <CORBA::ValueFactoryBase,
                                      CORBA::tao_ValueFactoryBase_life>

#pragma instantiate TAO_Value_Var_T <CORBA::ValueBase,
                                      CORBA::tao_ValueBase_life>

#endif /* !ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
