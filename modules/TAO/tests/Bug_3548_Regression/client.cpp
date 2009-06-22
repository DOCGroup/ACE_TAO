// $Id$

//---------------------------------------------------------------------------
#include "memeaterC.h"
#include "ace/OS_NS_unistd.h"

//---------------------------------------------------------------------------

MEMEATER::StringSeq* message_buffer = 0;
#define MAX_MSG_BUF 100

void eat_my_memory()
{
  const char * output_string = "Eat my Memory! Eat my Memory! Eat my Memory! Eat my Memory!";

  CORBA::ULong msg_count = message_buffer->length();
  if (msg_count < MAX_MSG_BUF)
    {
      message_buffer->length(msg_count+1);
      (*message_buffer)[msg_count] = CORBA::string_dup( output_string);
    }
  else
    {
      message_buffer->length(1);
      (*message_buffer)[0] = CORBA::string_dup( output_string );
    }
}

int ACE_TMAIN(int, ACE_TCHAR*[])
{
  message_buffer = new MEMEATER::StringSeq;
  message_buffer->length(0);

  for (int i = 0; i < 1000; i++)
    {
      eat_my_memory();
    }

  delete message_buffer;

  return 0;
}
//---------------------------------------------------------------------------
