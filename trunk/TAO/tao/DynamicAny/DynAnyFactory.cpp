//=============================================================================
/**
 *  @file    DynAnyFactory.cpp
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/DynamicAny/DynAnyFactory.h"

#include "tao/DynamicAny/DynAny_i.h"
#include "tao/DynamicAny/DynStruct_i.h"
#include "tao/DynamicAny/DynSequence_i.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynUnion_i.h"
#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor from typecode
TAO_DynAnyFactory::TAO_DynAnyFactory (void)
{
}

DynamicAny::DynAny_ptr
TAO_DynAnyFactory::create_dyn_any (const CORBA::Any & value)
{
  return
    TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
      value._tao_get_typecode (),
      value,
      true );  // Allow truncation
}

DynamicAny::DynAny_ptr
TAO_DynAnyFactory::create_dyn_any_from_type_code (CORBA::TypeCode_ptr type)
{
  // Second arg is typed in the template parameter, repeating it
  // this way allows cleaner template code.
  return
    TAO::MakeDynAnyUtils::make_dyn_any_t<CORBA::TypeCode_ptr> (
      type,
      type,
      true );  // Allow truncation
}

DynamicAny::DynAny_ptr
TAO_DynAnyFactory::create_dyn_any_without_truncation (
  const CORBA::Any &value)
{
  return
    TAO::MakeDynAnyUtils::make_dyn_any_t<const CORBA::Any&> (
      value._tao_get_typecode (),
      value,
      false );  // Do NOT allow truncation
}

DynamicAny::DynAnySeq *
TAO_DynAnyFactory::create_multiple_dyn_anys (
    const DynamicAny::AnySeq &values,
    ::CORBA::Boolean allow_truncate)
{
  // NOTE: Since each any is self contained and holds a streamed
  // representation of the DynAny contents, it is not possiable
  // to make the collection of the anys we are creating to
  // refer to the same duplicate DynValue if it crops up in
  // seporate enteries of the values sequence. Internally
  // within each any, indirection will occur if a DynValue
  // self references with one of its own members.

  const CORBA::ULong length = values.length ();

  DynamicAny::DynAnySeq_var retseq;
  ACE_NEW_THROW_EX (
    retseq.out (),
    DynamicAny::DynAnySeq (length),
    CORBA::NO_MEMORY ());
  retseq->length (length);

  for (CORBA::ULong i= 0u; i < length; ++i)
    {
      retseq[i]=
        (allow_truncate ?
        this->create_dyn_any (values[i]) :
        this->create_dyn_any_without_truncation (values[i]));
    }

  return retseq._retn ();
}

DynamicAny::AnySeq *
TAO_DynAnyFactory::create_multiple_anys (
    const DynamicAny::DynAnySeq &values)
{
  // NOTE: Since each any is self contained and holds a streamed
  // representation of the DynAny contents, it is not possiable
  // to make the collection of the anys we are creating to
  // refer to the same duplicate DynValue if it crops up in
  // seporate enteries of the values sequence. Internally
  // within each any, indirection will occur if a DynValue
  // self references with one of its own members.

  const CORBA::ULong length = values.length ();

  DynamicAny::AnySeq_var retseq;
  ACE_NEW_THROW_EX (
    retseq.out (),
    DynamicAny::AnySeq (length),
    CORBA::NO_MEMORY ());
  retseq->length (length);

  for (CORBA::ULong i= 0u; i < length; ++i)
    {
      retseq[i]= *values[i]->to_any ();
    }

  return retseq._retn ();
}

// Utility function called by all the DynAny classes
// to extract the TCKind of possibly aliased types.
CORBA::TCKind
TAO_DynAnyFactory::unalias (CORBA::TypeCode_ptr tc)
{
  CORBA::TCKind tck = tc->kind ();

  while (tck == CORBA::tk_alias)
    {
      CORBA::TypeCode_var temp = tc->content_type ();

      tck = TAO_DynAnyFactory::unalias (temp.in ());
    }

  return tck;
}

// Same as above, but returns the type code.
CORBA::TypeCode_ptr
TAO_DynAnyFactory::strip_alias (CORBA::TypeCode_ptr tc)
{
  CORBA::TypeCode_var retval = CORBA::TypeCode::_duplicate (tc);
  CORBA::TCKind tck = retval->kind ();

  while (tck == CORBA::tk_alias)
    {
      retval = retval->content_type ();

      tck = retval->kind ();
    }

  return retval._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
