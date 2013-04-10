// $Id$

#include "Common/I2C_Shapes_Common.h"
#include "connectors/dds4ccm/impl/ndds/DataReader_T.h"

#include "ace/OS_NS_unistd.h"

typedef ::I2C_Shapes::DataReader TypedDataReader;

void
read (TypedDataReader::_ptr_type shapes_dr)
{
  int samples_received = 0;
  if (shapes_dr)
    {
      while (samples_received < 1000)
        {
          // Start to read
          ShapeTypeSeq data;
          ::DDS::SampleInfoSeq  info;
          ::DDS::ReturnCode_t retcode = shapes_dr->take (
                                                    data,
                                                    info,
                                                    ::DDS::LENGTH_UNLIMITED,
                                                    ::DDS::NOT_READ_SAMPLE_STATE,
                                                    ::DDS::ANY_VIEW_STATE,
                                                    ::DDS::ALIVE_INSTANCE_STATE);
          samples_received += data.length ();
          if (retcode == ::DDS::RETCODE_OK)
            {
              for (::CORBA::ULong i = 0; i < data.length (); ++i)
                {
                  ACE_DEBUG ((LM_DEBUG, "read - "
                            "Received sample: x <%u> - y <%u>\n",
                            data[i].x,
                            data[i].y));
                }
            }
          else if (retcode != ::DDS::RETCODE_NO_DATA)
            {
              ACE_ERROR ((LM_ERROR, "Receiver_exec_i::start_read_using_idl_proxy - "
                        "ERROR: Error during read\n"));
            }
          ACE_Time_Value tv (0, 50);
          ACE_OS::sleep (tv);
        }
      ACE_DEBUG ((LM_DEBUG, "read - Ready\n"));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "read- ERROR: DataReader seems to be nil\n"));
    }
}

int ACE_TMAIN (int , ACE_TCHAR *[])
{
  int main_result = 0;

  I2C_Shapes_Common common;

  try
    {
      ::DDS::Subscriber_var subscriber;
      subscriber = common.create_subscriber ();
      if (::CORBA::is_nil (subscriber.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ACE_TMAIN - Subscriber seems to be nil\n"));
          throw ::CORBA::INTERNAL ();
        }

      ::DDS::Topic_var topic = common.get_topic ();
      ::DDS::DataReader_var dr = subscriber->create_datareader_with_profile (
                                      topic.in (),
                                      QOS_PROFILE,
                                      ::DDS::DataReaderListener::_nil (),
                                      0);

      TypedDataReader::_var_type shapes_dr = TypedDataReader::_narrow (dr.in ());
      if (shapes_dr->enable () != DDS_RETCODE_OK)
        {
          ACE_ERROR ((LM_ERROR, "ACE_TMAIN - Unable to enable the datareader.\n"));
          throw ::CORBA::INTERNAL ();
        }
      read (shapes_dr.in ());
    }
  catch (::CORBA::Exception &e)
    {
      e._tao_print_exception ("ACE_TMAIN RECEIVER");
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN RECEIVER - "
                "Error: unexpected exception caught\n"));
    }

  return main_result;
}
