// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    PredefinedType_Seq_Tmplinst.cpp
//
// = DESCRIPTION
//   Explicit instantiation of the TAO sequence template classes for all
//   the IDL basic types. These instantiations free us from having to
//   generate them over and over with the IDL compiler.
//
// = AUTHOR
//     Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#include "tao/Sequence_T.h"
#include "tao/ValueBase.h"
#include "tao/AbstractBase.h"
#include "tao/Any.h"

ACE_RCSID (tao, 
           PredefinedType_Seq_Tmplinst, 
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Unbounded_Sequence<CORBA::Boolean>;
template class TAO_Unbounded_Sequence<CORBA::Char>;
template class TAO_Unbounded_Sequence<CORBA::WChar>;
template class TAO_Unbounded_Sequence<CORBA::Short>;
template class TAO_Unbounded_Sequence<CORBA::UShort>;
template class TAO_Unbounded_Sequence<CORBA::Long>;
template class TAO_Unbounded_Sequence<CORBA::ULong>;
template class TAO_Unbounded_Sequence<CORBA::LongLong>;
template class TAO_Unbounded_Sequence<CORBA::ULongLong>;
template class TAO_Unbounded_Sequence<CORBA::Float>;
template class TAO_Unbounded_Sequence<CORBA::Double>;
template class TAO_Unbounded_Sequence<CORBA::LongDouble>;
template class TAO_Unbounded_Pseudo_Sequence<CORBA::Object, 
                                             CORBA::Object_var>;
template class TAO_Unbounded_Valuetype_Sequence<CORBA::ValueBase, 
                                                CORBA::ValueBase_var, 
                                                CORBA::tao_ValueBase_life>;
template class TAO_Unbounded_Pseudo_Sequence<CORBA::AbstractBase, 
                                             CORBA::AbstractBase_var>;
template class TAO_Unbounded_Sequence<CORBA::Any>;
template class TAO_Unbounded_Pseudo_Sequence<CORBA::TypeCode, 
                                             CORBA::TypeCode_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Unbounded_Sequence<CORBA::Boolean>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Char>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::WChar>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Short>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::UShort>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Long>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::ULong>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::LongLong>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::ULongLong>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Float>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Double>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::LongDouble>
#pragma instantiate TAO_Unbounded_Pseudo_Sequence<CORBA::Object, \
                                                  CORBA::Object_var>
#pragma instantiate TAO_Unbounded_Valuetype_Sequence<CORBA::ValueBase, \
                                                     CORBA::ValueBase_var, \
                                                     CORBA::tao_ValueBase_life>
#pragma instantiate TAO_Unbounded_Pseudo_Sequence<CORBA::AbstractBase, \
                                                  CORBA::AbstractBase_var>
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Any>
#pragma instantiate TAO_Unbounded_Pseudo_Sequence<CORBA::TypeCode, \
                                                  CORBA::TypeCode_var>

#endif /* !ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */ 

