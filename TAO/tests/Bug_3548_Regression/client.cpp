#include "memeaterC.h"

namespace
{
  constexpr CORBA::ULong max_message_buffer_count {100};
  constexpr int iteration_count {1000};
  char const * const output_string = "Eat my Memory! Eat my Memory! Eat my Memory! Eat my Memory!";
}

void eat_my_memory (MEMEATER::StringSeq &message_buffer)
{
  CORBA::ULong const msg_count = message_buffer.length ();
  if (msg_count < max_message_buffer_count)
    {
      message_buffer.length (msg_count + 1);
      message_buffer[msg_count] = CORBA::string_dup (output_string);
    }
  else
    {
      message_buffer.length (1);
      message_buffer[0] = CORBA::string_dup (output_string);
    }
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  MEMEATER::StringSeq message_buffer {};

  try
    {
      for (int iteration {}; iteration < iteration_count; ++iteration)
        {
          eat_my_memory (message_buffer);
        }
    }
  catch (::CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR: unexpected CORBA exception caught:");
      return 1;
    }

  return 0;
}
