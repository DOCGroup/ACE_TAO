//=============================================================================
/**
 *  @file    data.h
 *
 *  Header file for the class containing test constants.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TESTS_DYNANY_TEST_DATA_H
#define TAO_TESTS_DYNANY_TEST_DATA_H

#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/ShortSeqC.h"
#include <array>

class Data
{
public:
  explicit Data (CORBA::ORB_var orb);
  ~Data () = default;
  Data (Data const&) = delete;
  Data (Data&&) = delete;
  Data& operator= (Data const&) = delete;
  Data& operator= (Data&&) = delete;

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
  CORBA::String_var     m_string1,    m_string2;
  CORBA::TypeCode_var   m_typecode1,  m_typecode2;
  CORBA::WChar          m_wchar1,     m_wchar2;
  CORBA::Object_var     m_objref1,    m_objref2;
  CORBA::ShortSeq       m_shortseq1;
  CORBA::ShortSeq_var   m_shortseq2;
  CORBA::LongDouble     m_longdouble1, m_longdouble2;

  std::array<char const*, 20> labels {{
    "type boolean", "type octet", "type char", "type short", "type long",
    "type ushort", "type ulong", "type float", "type double", "type longlong",
    "type ulonglong", "type string", "type typecode", "type wchar", "type any",
    "type objref", "type short sequence", "type longdouble", "type enum",
    "typedef'd enum in struct"
  }};

private:
  CORBA::ORB_var orb_;
};

#endif /* TAO_TESTS_DYNANY_TEST_DATA_H */
