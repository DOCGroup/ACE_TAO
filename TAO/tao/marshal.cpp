// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    marshal.cpp
//
// = DESCRIPTION
//   Implements the MarshalObject class and the factory
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

#include <assert.h>
#include <limits.h>
#include <string.h>

#include        "orb.h"
#include        "align.h"
#include	"cdr.h"

#if	defined (HAVE_WIDEC_H)
#		include <widec.h>
#else
extern "C" 
{
  u_int wslen (const CORBA_WChar *);
  CORBA_WChar *wscpy (CORBA_WChar *, const CORBA_WChar *);
}
#endif

// cosntructor for the factory
TAO_MarshalFactory::TAO_MarshalFactory()
#if 0
  : m_primitive_(TAO_MARSHAL_PRIMITIVE::instance()),
    m_any_(TAO_MARSHAL_ANY::instance()),
    m_typecode_(TAO_MARSHAL_TYPECODE::instance()),
    m_principal_(TAO_MARSHAL_PRINCIPAL::instance()),
    m_objref_(TAO_MARSHAL_OBJREF::instance()),
    m_struct_(TAO_MARSHAL_STRUCT::instance()),
    m_union_(TAO_MARSHAL_UNION::instance()),
    m_string_(TAO_MARSHAL_STRING::instance()),
    m_sequence_(TAO_MARSHAL_SEQUENCE::instance()),
    m_array_(TAO_MARSHAL_ARRAY::instance()),
    m_alias_(TAO_MARSHAL_ALIAS::instance()),
    m_except_(TAO_MARSHAL_EXCEPT::instance()),
    m_wstring_(TAO_MARSHAL_WSTRING::instance())
#endif
{
  // initialize the mobj table
  mobj_table_[tk_null].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_void].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_short].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_long].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_ushort].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_ulong].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_float].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_double].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_boolean].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_char].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_octet].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_any].obj_ = TAO_MARSHAL_ANY::instance();
  mobj_table_[tk_TypeCode].obj_ = TAO_MARSHAL_TYPECODE::instance();
  mobj_table_[tk_Principal].obj_ = TAO_MARSHAL_PRINCIPAL::instance();
  mobj_table_[tk_objref].obj_ = TAO_MARSHAL_OBJREF::instance();
  mobj_table_[tk_struct].obj_ = TAO_MARSHAL_STRUCT::instance();
  mobj_table_[tk_union].obj_ = TAO_MARSHAL_UNION::instance();
  mobj_table_[tk_enum].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_string].obj_ = TAO_MARSHAL_STRING::instance();
  mobj_table_[tk_sequence].obj_ = TAO_MARSHAL_SEQUENCE::instance();
  mobj_table_[tk_array].obj_ = TAO_MARSHAL_ARRAY::instance();
  mobj_table_[tk_alias].obj_ = TAO_MARSHAL_ALIAS::instance();
  mobj_table_[tk_except].obj_ = TAO_MARSHAL_EXCEPT::instance();
  mobj_table_[tk_longlong].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_ulonglong].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_longdouble].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_wchar].obj_ = TAO_MARSHAL_PRIMITIVE::instance();
  mobj_table_[tk_wstring].obj_ = TAO_MARSHAL_WSTRING::instance();
}

TAO_MarshalFactory::~TAO_MarshalFactory()
{
}

#if 0
// factory method
//
// Based on the kind of the typecode, return the appropriate marshal object
TAO_MarshalObject* TAO_MarshalFactory::make_marshal_object(CORBA_TypeCode_ptr tc,
							   CORBA_Environment &env)
{
  env.clear();
  if (tc)
    {
      switch(tc->_kind)
	{
	case tk_null:
	case tk_void:
	case tk_short:
	case tk_ushort:
	case tk_long:
	case tk_ulong:
	case tk_float:
	case tk_enum:
	case tk_double:
	case tk_longlong:
	case tk_ulonglong:
	case tk_boolean:
	case tk_char:
	case tk_octet:
	case tk_longdouble:
	case tk_wchar:
	  return m_primitive_;
	case tk_any:
	  return m_any_;
	case tk_TypeCode:
	  return m_typecode_;
	case tk_Principal:
	  return m_principal_;
	case tk_objref:
	  return m_objref_;
	case tk_struct:
	  return m_struct_;
	case tk_union:
	  return m_union_;
	case tk_string:
	  return m_string_;
	case tk_sequence:
	  return m_sequence_;
	case tk_array:
	  return m_array_;
	case tk_alias:
	  return m_alias_;
	case tk_except:
	  return m_except_;
	case tk_wstring:
	  return m_wstring_;
	default:
	  // anything else is an error
	  env.exception (new CORBA_BAD_TYPECODE(COMPLETED_NO));
	  return (TAO_MarshalObject *)0;
	}
    }
  else
    {
      env.exception (new CORBA_BAD_TYPECODE(COMPLETED_NO));
      return (TAO_MarshalObject *)0;
    }
}
#endif

// define a default factory
TAO_MarshalFactory *TAO_DEFAULT_MARSHAL_FACTORY = TAO_MARSHAL_FACTORY::instance();
