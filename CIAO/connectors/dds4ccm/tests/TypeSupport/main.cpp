// $Id$

#include "ace/OS_main.h"
#include "ace/Log_Msg.h"
#include "ace/Env_Value_T.h"

#include "dds4ccm/impl/ndds/TypeSupport.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"

typedef ::CIAO::NDDS::DDS_DomainParticipantFactory_i
    DomainParticipantFactory;

class TestTypeFactory : public ::CIAO::NDDS::DDS_TypeFactory_i
{
  virtual DDS::DataWriter_ptr create_datawriter (DDSDataWriter* ,
                                                  ::DDS::DomainParticipant_ptr ,
                                                  ::DDS::Publisher_ptr )
  {
    return ::DDS::DataWriter::_nil ();
  }

  virtual DDS::DataReader_ptr create_datareader (DDSDataReader* ,
                                                  ::DDS::DomainParticipant_ptr ,
                                                  ::DDS::Subscriber_ptr )
  {
    return ::DDS::DataReader::_nil ();
  }
};

int
ACE_TMAIN (int , ACE_TCHAR **)
{
#if (CIAO_DDS4CCM_NDDS != 1)
  ACE_DEBUG ((LM_DEBUG, "RTI DDS only test\n"));
  return 0;
#endif

  int ret = 0;

  try
    {
      int domain_id = 0;
      ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), domain_id);
      domain_id = id;

      DomainParticipantFactory pf;

      ::DDS::DomainParticipantQos qos;
      ::DDS::DomainParticipant_var dp1 = pf.create_participant (
                                            domain_id,
                                            qos,
                                            ::DDS::DomainParticipantListener::_nil (),
                                            0);

      ::DDS::DomainParticipant_var dp2  = pf.create_participant (
                                            domain_id,
                                            qos,
                                            ::DDS::DomainParticipantListener::_nil (),
                                            0);

      const char * type1 = "DataType1";
      const char * type2 = "DataType2";

      TestTypeFactory * f1 = 0;
      TestTypeFactory * f2 = 0;

      ACE_NEW_THROW_EX (f1,
                        TestTypeFactory (),
                        ::CORBA::NO_MEMORY ());
      ACE_NEW_THROW_EX (f2,
                        TestTypeFactory (),
                        ::CORBA::NO_MEMORY ());


      // Register the the first factory
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(type1, f1, dp1.in()))
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> for DP1 succesfully registered\n",
                    type1));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C> for DP1 could not be registered\n",
                    type1));
          ++ret;
        }
      // Register the same factory again
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(type1, f1, dp1.in()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C> for DP1 could be registered\n",
                    type1));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> for DP1 could not be registered\n",
                    type1));
        }
      // Register the same factory again but with another name
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(type2, f1, dp1.in()))
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> for DP1 could be registered\n",
                    type2));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C> for DP1 could not be registered\n",
                    type2));
          ++ret;
        }

      // Unregister an unused factory by using an unused domain participant
      ::CIAO::NDDS::DDS_TypeFactory_i * tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(type1, dp2.in());

      if (tmp)
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Unregistered type <%C> for DP2 could be unregistered\n",
                    type2));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Unregistered Type <%C> for DP2 could not be unregistered\n",
                    type2));
        }

      // Unregister a registered type by using a used domain participant
      tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(type1, dp1.in());

      if (tmp)
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Unregistered type <%C> for DP1 could be unregistered\n",
                    type2));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Unregistered Type <%C> for DP1 could not be unregistered\n",
                    type2));
          ++ret;
        }

      // Unregister a the same type and domain participant again
      tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(type1, dp1.in());

      if (tmp)
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Unregistered type <%C> for DP1 could be unregistered\n",
                    type2));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Unregistered Type <%C> for DP1 could not be unregistered\n",
                    type2));
        }

      // Call Close on TypeSupport. This should remove the last remaining entry.
      ::CIAO::NDDS::DDS_TypeSupport_i::close();

      // Unregister the type and domain participant which should be removed by 'close'
      tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(type2, dp1.in());

      if (tmp)
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Unregistered type <%C> for DP1 could be unregistered\n",
                    type2));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Unregistered Type <%C> for DP1 could not be unregistered\n",
                    type2));
        }

      delete f1;
      delete f2;
    }
  catch (const ::CORBA::Exception& e)
    {
      e._tao_print_exception ("ACE_TMAIN - Caught unexpected "
                              "CORBA exception\n");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN - ERROR: Caught unexpected "
                            "exception\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Test passed !\n"));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "%d errors found during test. \n",
                  ret));
    }
  return ret;
}