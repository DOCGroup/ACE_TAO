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

#if 0
#include "tao/orb.h"
#include "tao/align.h"
#include "tao/cdr.h"
#endif

#include "tao/corba.h"

#if	defined (HAVE_WIDEC_H)
#		include <widec.h>
#else
extern "C" 
{
  u_int wslen (const CORBA::WChar *);
  CORBA::WChar *wscpy (CORBA::WChar *, const CORBA::WChar *);
}
#endif /* HAVE_WIDEC_H */

TAO_Marshal_Factory* TAO_Marshal::DEFAULT_MARSHAL_FACTORY = 0;

void
TAO_Marshal::initialize (void)
{
  DEFAULT_MARSHAL_FACTORY = TAO_MARSHAL_FACTORY::instance();
}

// cosntructor for the factory
TAO_Marshal_Factory::TAO_Marshal_Factory (void)
#if 0
  : m_primitive_ (TAO_MARSHAL_PRIMITIVE::instance ()),
    m_any_ (TAO_MARSHAL_ANY::instance ()),
    m_typecode_ (TAO_MARSHAL_TYPECODE::instance ()),
    m_principal_ (TAO_MARSHAL_PRINCIPAL::instance ()),
    m_objref_ (TAO_MARSHAL_OBJREF::instance ()),
    m_struct_ (TAO_MARSHAL_STRUCT::instance ()),
    m_union_ (TAO_MARSHAL_UNION::instance ()),
    m_string_ (TAO_MARSHAL_STRING::instance ()),
    m_sequence_ (TAO_MARSHAL_SEQUENCE::instance ()),
    m_array_ (TAO_MARSHAL_ARRAY::instance ()),
    m_alias_ (TAO_MARSHAL_ALIAS::instance ()),
    m_except_ (TAO_MARSHAL_EXCEPT::instance ()),
    m_wstring_ (TAO_MARSHAL_WSTRING::instance ())
#endif /* 0 */
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

#if 0
// factory method
//
// Based on the kind of the typecode, return the appropriate marshal object
TAO_Marshal_Object* TAO_Marshal_Factory::make_marshal_object (CORBA::TypeCode_ptr tc,
							      CORBA::Environment &env)
{
  env.clear ();
  if (tc)
    {
      switch (tc->_kind)
	{
	case CORBA::tk_null:
	case CORBA::tk_void:
	case CORBA::tk_short:
	case CORBA::tk_ushort:
	case CORBA::tk_long:
	case CORBA::tk_ulong:
	case CORBA::tk_float:
	case CORBA::tk_enum:
	case CORBA::tk_double:
	case CORBA::tk_longlong:
	case CORBA::tk_ulonglong:
	case CORBA::tk_boolean:
	case CORBA::tk_char:
	case CORBA::tk_octet:
	case CORBA::tk_longdouble:
	case CORBA::tk_wchar:
	  return m_primitive_;
	case CORBA::tk_any:
	  return m_any_;
	case CORBA::tk_TypeCode:
	  return m_typecode_;
	case CORBA::tk_Principal:
	  return m_principal_;
	case CORBA::tk_objref:
	  return m_objref_;
	case CORBA::tk_struct:
	  return m_struct_;
	case CORBA::tk_union:
	  return m_union_;
	case CORBA::tk_string:
	  return m_string_;
	case CORBA::tk_sequence:
	  return m_sequence_;
	case CORBA::tk_array:
	  return m_array_;
	case CORBA::tk_alias:
	  return m_alias_;
	case CORBA::tk_except:
	  return m_except_;
	case CORBA::tk_wstring:
	  return m_wstring_;
	default:
	  // anything else is an error
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return (TAO_Marshal_Object *)0;
	}
    }
  else
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
      return (TAO_Marshal_Object *)0;
    }
}
#endif /* 0 */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<TAO_Marshal_Factory, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Primitive, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Any, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_TypeCode, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Principal, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_ObjRef, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Struct, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Union, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_String, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Sequence, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Array, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Alias, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_Except, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_Marshal_WString, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<TAO_Marshal_Factory, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Primitive, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Any, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_TypeCode, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Principal, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_ObjRef, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Struct, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Union, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_String, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Sequence, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Array, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Alias, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_Except, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_Marshal_WString, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
