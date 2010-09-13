//=============================================================================
/**
 *  @file    data.h
 *
 *  $Id$
 *
 *  Header file for the class containing test constants.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/ShortSeqC.h"

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
  CORBA::Object_var     m_objref1,    m_objref2;
  CORBA::ShortSeq       m_shortseq1;
  CORBA::ShortSeq_var   m_shortseq2;
  CORBA::LongDouble     m_longdouble1, m_longdouble2;

const char* labels[20];

private:
  CORBA::ORB_var orb_;
};
