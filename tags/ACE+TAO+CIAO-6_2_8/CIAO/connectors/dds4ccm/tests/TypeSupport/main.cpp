// $Id$

#include "ace/OS_main.h"
#include "ace/Log_Msg.h"
#include "ace/Env_Value_T.h"

#if (CIAO_DDS4CCM_NDDS == 1)

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
#endif

int
ACE_TMAIN (int , ACE_TCHAR **)
{
#if (CIAO_DDS4CCM_NDDS == 1)
  // first turn on or off tracing
  ACE_Env_Value<int> trace (ACE_TEXT("DDS4CCM_TRACE_ENABLE"), 0);
  if (trace)
    {
      DDS4CCM_ENABLE_TRACE ();
    }
  else
    {
      DDS4CCM_DISABLE_TRACE ();
    }
  DDS4CCM_debug_level = 0;

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
      const char * type3 = "DataType3";

      TestTypeFactory * f1 = 0;
      TestTypeFactory * f2 = 0;
      TestTypeFactory * f3 = 0;

      ACE_NEW_THROW_EX (f1,
                        TestTypeFactory (),
                        ::CORBA::NO_MEMORY ());
      ACE_NEW_THROW_EX (f2,
                        TestTypeFactory (),
                        ::CORBA::NO_MEMORY ());
      ACE_NEW_THROW_EX (f3,
                        TestTypeFactory (),
                        ::CORBA::NO_MEMORY ());

      /// Register type 1 with f1
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(dp1.in(), type1, f1))
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> and Factory <%@> for DP1 successfully registered\n",
                    type1, f1));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C> and Factory <%@> for DP1 could not be registered\n",
                    type1, f1));
          ++ret;
        }
      /// DP1 should now contain one type-factory combination

      /// Register the same factory again
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(dp1.in(), type1, f1))
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C> and Factory <%@> for DP1 could be registered\n",
                    type1, f1));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> and Factory <%@> for DP1 could not be registered\n",
                    type1, f1));
        }
      /// DP1 should now contain one type-factory combination

      /// Register the same factory again but with another name
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(dp1.in(), type2, f1))
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> and Factory <%@> for DP1 could be registered\n",
                    type2, f1));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C> and Factory <%@> for DP1 could not be registered\n",
                    type2, f1));
          ++ret;
        }
      /// DP1 should now contain two type-factory combinations

      /// Register the same type with another factory
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(dp1.in(), type2, f2))
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C>  and Factory <%@> for DP1 could be registered\n",
                    type2, f2));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> and Factory <%@> for DP1 could not be registered\n",
                    type2, f2));
        }
      /// DP1 should now contain two type-factory combinations

      /// Just register type 3 with f3
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(dp1.in(), type3, f3))
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> and Factory <%@> for DP1 successfully registered\n",
                    type3, f3));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C> and Factory <%@> for DP1 could not be registered\n",
                    type3, f3));
          ++ret;
        }
      /// DP1 should now contain three type-factory combinations

      /// Unregister an unused factory by using an unused domain participant
      bool tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(dp2.in(), type1);

      if (tmp)
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Unregistered type <%C> for DP2 could be unregistered\n",
                    type1));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Unregistered type <%C> for DP2 could not be unregistered\n",
                    type1));
        }

      /// Unregister a registered type by using a used domain participant
      tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(dp1.in(), type1);

      if (tmp)
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Unregistered type <%C> for DP1 could be unregistered\n",
                    type1));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Unregistered type <%C> for DP1 could not be unregistered\n",
                    type1));
          ++ret;
        }
      /// DP1 should now contain two type-factory combinations

      /// Unregister the same type and domain participant again
      tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(dp1.in(), type1);

      if (tmp)
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Unregistered type <%C> for DP1 could be unregistered\n",
                    type1));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Unregistered type <%C> for DP1 could not be unregistered\n",
                    type1));
          ++ret;
        }
      /// DP1 should now contain two type-factory combinations

      /// Call Close on TypeSupport. This should remove the last remaining entries.
      ::CIAO::NDDS::DDS_TypeSupport_i::close();

      /// Unregister the type and domain participant which should be removed by 'close'
      tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(dp1.in(), type2);

      if (tmp)
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Deleted type <%C> for DP1 could be unregistered\n",
                    type2));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Deleted type <%C> for DP1 could not be unregistered\n",
                    type2));
        }

      /// Unregister the type and domain participant which should be removed by 'close'
      tmp = ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type(dp1.in(), type3);

      if (tmp)
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Deleted type <%C> for DP1 could be unregistered\n",
                    type3));
          ++ret;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Deleted type <%C> for DP1 could not be unregistered\n",
                    type3));
        }

      /// Since we've invoked ::close f3 has been deleted. Therefor we need to create it again.
      ACE_NEW_THROW_EX (f3,
                        TestTypeFactory (),
                        ::CORBA::NO_MEMORY ());


      /// We should now be able to register type 3 again
      if (::CIAO::NDDS::DDS_TypeSupport_i::register_type(dp1.in(), type3, f3))
        {
          ACE_DEBUG ((LM_DEBUG, "OK - Type <%C> and Factory <%@> for DP1 successfully registered\n",
                    type3, f3));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR - Type <%C> and Factory <%@> for DP1 could not be registered\n",
                    type3, f3));
          ++ret;
        }
      /// DP1 should now contain one type-factory combination

      /// Call Close on TypeSupport. This should remove the last remaining entries.
     ::CIAO::NDDS::DDS_TypeSupport_i::close();

      /// No need to delete f1, f2, and f3 since ::close will delete them.
      pf.delete_participant(dp1);
      pf.delete_participant(dp2);

#if (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipantFactory::finalize_instance ();
#endif

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
#else
  ACE_DEBUG ((LM_DEBUG, "NDDS only test\n"));
  return 0;
#endif
}
