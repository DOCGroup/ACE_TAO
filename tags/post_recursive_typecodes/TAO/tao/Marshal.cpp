// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    marshal.cpp
//
// = DESCRIPTION
//   Implements the Marshal_Object class and the factory
//
//   The original encoder and decoder code now appears in files encode.cpp and
//   decode.cpp
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and
//     Aniruddha Gokhale
//
// ============================================================================

#include "tao/Marshal.h"
#include "tao/singletons.h"
#include "tao/CDR.h"
#include "tao/Environment.h"

#if !defined (__ACE_INLINE__)
# include "tao/Marshal.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Marshal, "$Id$")

TAO_Marshal_Factory* TAO_Marshal::DEFAULT_MARSHAL_FACTORY = 0;

void
TAO_Marshal::init (void)
{
  DEFAULT_MARSHAL_FACTORY = TAO_MARSHAL_FACTORY::instance();
}

// cosntructor for the factory
TAO_Marshal_Factory::TAO_Marshal_Factory (void)
{
  // initialize the mobj table
  mobj_table_[CORBA::tk_null].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_void].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_short].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_long].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_ushort].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_ulong].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_float].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_double].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_boolean].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_char].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_octet].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_any].obj_ = TAO_MARSHAL_ANY::instance ();
  mobj_table_[CORBA::tk_TypeCode].obj_ = TAO_MARSHAL_TYPECODE::instance ();
  mobj_table_[CORBA::tk_Principal].obj_ = TAO_MARSHAL_PRINCIPAL::instance ();
  mobj_table_[CORBA::tk_objref].obj_ = TAO_MARSHAL_OBJREF::instance ();
  mobj_table_[CORBA::tk_struct].obj_ = TAO_MARSHAL_STRUCT::instance ();
  mobj_table_[CORBA::tk_union].obj_ = TAO_MARSHAL_UNION::instance ();
  mobj_table_[CORBA::tk_enum].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_string].obj_ = TAO_MARSHAL_STRING::instance ();
  mobj_table_[CORBA::tk_sequence].obj_ = TAO_MARSHAL_SEQUENCE::instance ();
  mobj_table_[CORBA::tk_array].obj_ = TAO_MARSHAL_ARRAY::instance ();
  mobj_table_[CORBA::tk_alias].obj_ = TAO_MARSHAL_ALIAS::instance ();
  mobj_table_[CORBA::tk_except].obj_ = TAO_MARSHAL_EXCEPT::instance ();
  mobj_table_[CORBA::tk_longlong].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_ulonglong].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_longdouble].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_wchar].obj_ = TAO_MARSHAL_PRIMITIVE::instance ();
  mobj_table_[CORBA::tk_wstring].obj_ = TAO_MARSHAL_WSTRING::instance ();
}

TAO_Marshal_Factory::~TAO_Marshal_Factory (void)
{
}

// Factory method that returns the appropriate marshal object based on
// the kind of the typecode.
TAO_Marshal_Object *
TAO_Marshal_Factory::make_marshal_object (CORBA::TypeCode_ptr tc,
                                          CORBA::Environment &TAO_IN_ENV)
{
  if (tc && tc->kind_ >= 0 && tc->kind_ < CORBA::TC_KIND_COUNT)
    return this->mobj_table_[tc->kind_].obj_;
  TAO_THROW_RETURN (CORBA::BAD_TYPECODE (), 0);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<TAO_Marshal_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Primitive, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Any, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_TypeCode, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Principal, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_ObjRef, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Struct, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Union, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_String, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Sequence, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Array, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Alias, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Except, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_Marshal_WString, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<TAO_Marshal_Factory, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Primitive, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Any, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_TypeCode, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Principal, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_ObjRef, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Struct, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Union, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_String, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Sequence, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Array, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Alias, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Except, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_WString, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
