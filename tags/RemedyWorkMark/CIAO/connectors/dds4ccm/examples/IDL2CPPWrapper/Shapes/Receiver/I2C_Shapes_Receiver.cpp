// $Id$

#include "Common/I2C_Shapes_Common.h"
#include "connectors/dds4ccm/impl/ndds/DataReader_T.h"

#include "ace/OS_NS_unistd.h"

typedef ::CIAO::NDDS::DataReader_T<ShapeType_DDS_Traits::datareader_type,
                     ShapeType_DDS_Traits::typed_reader_type,
                     ShapeType_DDS_Traits::value_type,
                     ShapeTypeSeq,
                     ShapeType_DDS_Traits::dds_seq_type>
        ShapesDataReader;

void
read (ShapesDataReader* shapes_dr)
{
  int samples_received = 0;
  if (shapes_dr)
    {
      while (samples_received < 1000)
        {
          //start to read
          ShapeTypeSeq data;
          ::DDS::SampleInfoSeq  info;
          ::DDS::ReturnCode_t retcode = shapes_dr->read (
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
      ::DDS::DataReader_var dr;
      ::DDS::DataReaderQos dqos;
      dr = subscriber->create_datareader (topic.in (),
                                      dqos,
                                      ::DDS::DataReaderListener::_nil (),
                                      0);

      ShapesDataReader * shapes_dr = dynamic_cast <ShapesDataReader *>(dr.in ());
      read (shapes_dr);
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
