// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    data.h
//
// = DESCRIPTION
//    Header file for the class containing test constants.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "tao/corba.h"

class Data
{
public:
  Data (CORBA::ORB_var orb);
  ~Data (void);

  CORBA::Boolean        m_bool1,      m_bool2;
  CORBA::Octet          m_octet1,     m_octet2;
  CORBA::Char           m_char1,      m_char2;
  CORBA::Short          m_short1,     m_short2;
  CORBA::Long           m_long1,      m_long2;
  CORBA::UShort         m_ushort1,    m_ushort2;
  CORBA::ULong          m_ulong1,     m_ulong2;
  CORBA::Float          m_float1,     m_float2;
  CORBA::Double         m_double1,    m_double2;
  CORBA::ULongLong      m_ulonglong1, m_ulonglong2;
  char *                m_string1;
  char *                m_string2;
  CORBA::TypeCode_ptr   m_typecode1,  m_typecode2;
  CORBA::WChar          m_wchar1,     m_wchar2;
  CORBA::Any            m_any1,       m_any2;
  CORBA::Object_var     m_objref1,    m_objref2;

const char* labels[16];

private:
  CORBA::ORB_var orb_;
};
