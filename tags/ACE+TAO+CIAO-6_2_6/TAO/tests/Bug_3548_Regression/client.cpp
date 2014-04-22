// $Id$

//---------------------------------------------------------------------------
#include "memeaterC.h"
#include "ace/OS_NS_unistd.h"

//---------------------------------------------------------------------------

MEMEATER::StringSeq* _message_buffer = 0;
#define MAX_MSG_BUF 100

void eat_my_memory()
{
  const char * output_string = "Eat my Memory! Eat my Memory! Eat my Memory! Eat my Memory!";

  CORBA::ULong msg_count = _message_buffer->length();
  if (msg_count < MAX_MSG_BUF)
    {
      _message_buffer->length(msg_count+1);
      (*_message_buffer)[msg_count] = CORBA::string_dup( output_string);
    }
  else
    {
      _message_buffer->length(1);
      (*_message_buffer)[0] = CORBA::string_dup( output_string );
    }
}

int ACE_TMAIN(int, ACE_TCHAR*[])
{
  _message_buffer = new MEMEATER::StringSeq;
  _message_buffer->length(0);

  try
    {
      for (int i = 0; i < 1000; i++)
        {
          eat_my_memory();
        }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      return 1;
    }

  delete _message_buffer;

  return 0;
}
//---------------------------------------------------------------------------
