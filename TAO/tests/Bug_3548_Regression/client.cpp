//---------------------------------------------------------------------------
#include "memeaterC.h"
#include "ace/OS_NS_unistd.h"
#include <memory>

//---------------------------------------------------------------------------

std::unique_ptr<MEMEATER::StringSeq> message_buffer;
constexpr CORBA::ULong max_msg_buf = 100;

void eat_my_memory ()
{
  char const *output_string = "Eat my Memory! Eat my Memory! Eat my Memory! Eat my Memory!";

  CORBA::ULong const msg_count = message_buffer->length ();
  if (msg_count < max_msg_buf)
    {
      message_buffer->length (msg_count + 1);
      (*message_buffer)[msg_count] = CORBA::string_dup (output_string);
    }
  else
    {
      message_buffer->length (1);
      (*message_buffer)[0] = CORBA::string_dup (output_string);
    }
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  message_buffer = std::make_unique<MEMEATER::StringSeq> ();
  message_buffer->length (0);

  try
    {
      for (int i = 0; i < 1000; ++i)
        {
          eat_my_memory ();
        }
    }
  catch (::CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR : unexpected CORBA exception caugth :");
      return 1;
    }

  message_buffer.reset ();

  return 0;
}
//---------------------------------------------------------------------------
