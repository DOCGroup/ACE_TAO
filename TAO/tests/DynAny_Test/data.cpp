//=============================================================================
/**
 *  @file    data.cpp
 *
 *  Implementation file for the class containing test constants.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "data.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Log_Msg.h"

Data::Data (CORBA::ORB_var orb)
  : m_bool1 (1),                                m_bool2 (0),
    m_octet1 (8),                               m_octet2 (0),
    m_char1 ('z'),                              m_char2 (0),
    m_short1 (-5),                              m_short2 (0),
    m_long1 (-123456),                          m_long2 (0),
    m_ushort1 (5),                              m_ushort2 (0),
    m_ulong1 (123456),                          m_ulong2 (0),
    m_float1 (0.142857f),                       m_float2 (0.0f),
    m_double1 (3.14159),                        m_double2 (0.0),
    m_ulonglong1 (654321),                      m_ulonglong2 (0),
    m_string1 (CORBA::string_dup ("upchuck")),

    m_typecode1 (CORBA::TypeCode::_duplicate (CORBA::_tc_long)),
    m_typecode2 (CORBA::TypeCode::_duplicate (CORBA::_tc_null)),

    m_wchar1 (666),                             m_wchar2 (0),
    orb_ (orb)
{
    ACE_CDR_LONG_DOUBLE_ASSIGNMENT (m_longdouble1, 88888888888.8888);
    ACE_CDR_LONG_DOUBLE_ASSIGNMENT (m_longdouble2, 99999999999.9999);

  try
    {
      m_shortseq1.length (3UL);
      m_shortseq1[0UL] = 0;
      m_shortseq1[1UL] = 1;
      m_shortseq1[2UL] = 2;

      // Getting the RootPOA so we can generate object references.
      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("RootPOA");


      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Unable to get root poa reference.\n"));
        }

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());


      // Generate values for the member variables.
     this->m_objref1 =
        root_poa->create_reference ("foo");


      this->m_objref2 =
        root_poa->create_reference ("foo");

      // Clean up after the POA
      root_poa->destroy (true, true);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in ORB/POA init\n");
    }
}
