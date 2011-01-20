// $Id$

#include "Common/I2C_Shapes_Common.h"
#include "connectors/dds4ccm/impl/ndds/DataWriter_T.h"

#include "ace/OS_NS_unistd.h"

#define MAX_X_Y 250

typedef ::I2C_Shapes::DataWriter TypedDataWriter;

void
write (TypedDataWriter::_ptr_type shapes_dw)
{
  if (shapes_dw)
    {
      ShapeType square;
      square.color = "GREEN";
      square.x = ACE_OS::rand () % MAX_X_Y;
      square.y = ACE_OS::rand () % MAX_X_Y;
      square.shapesize = 25;
      bool x_increasing = true;
      bool y_increasing = true;
      for (::CORBA::ULong i = 0; i < 1000; ++i)
        {
          if (x_increasing)
            {
              ++square.x;
              x_increasing = square.x + 1 <= MAX_X_Y;
            }
          else
            {
              --square.x;
              x_increasing = square.x - 1 < 0;
            }
          if (y_increasing)
            {
              ++square.y;
              y_increasing = square.y + 1 <= MAX_X_Y;
            }
          else
            {
              --square.y;
              y_increasing = square.y - 1 < 0;
            }
          ::DDS::ReturnCode_t retcode = shapes_dw->write (square, ::DDS::HANDLE_NIL);
          if (retcode == ::DDS::RETCODE_OK)
            {
              ACE_DEBUG ((LM_DEBUG, "Updated square - "
                        "x: <%u> - y: <%u>\n",
                        square.x, square.y));
            }
          ACE_Time_Value tv (0, 500000);
          ACE_OS::sleep (tv);
        }
      ACE_DEBUG ((LM_DEBUG, "write - Ready. Written 1000 samples.\n"));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "write - ERROR: DataWriter seems to be nil\n"));
    }
}

int ACE_TMAIN (int , ACE_TCHAR *[])
{
  int main_result = 0;
  I2C_Shapes_Common common;
  try
    {
      ::DDS::Publisher_var publisher = common.create_publisher ();
      if (::CORBA::is_nil (publisher.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ACE_TMAIN - Publisher seems to be nil\n"));
          throw ::CORBA::INTERNAL ();
        }

      ::DDS::Topic_var topic = common.get_topic ();
      ::DDS::DataWriter_var dw = publisher->create_datawriter_with_profile (
                                      topic.in (),
                                      QOS_PROFILE,
                                      ::DDS::DataWriterListener::_nil (),
                                      0);

      TypedDataWriter::_var_type shapes_dw = TypedDataWriter::_narrow (dw.in ());
      write (shapes_dw);
    }
  catch (::CORBA::Exception &e)
    {
      e._tao_print_exception ("ACE_TMAIN SENDER");
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ACE_TMAIN SENDER - "
                "Error: unexpected exception caught\n"));
    }

  return main_result;
}

