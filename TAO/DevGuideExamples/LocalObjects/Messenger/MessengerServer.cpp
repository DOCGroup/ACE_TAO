#include "Messenger_i.h"
#include <iostream>
int ACE_TMAIN (int, ACE_TCHAR **)
{
  try
    {
      // Construct a Messenger object and use it "as if" it's a CORBA object.
      // Put it into a CORBA object reference, comparable to activation,
      // narrowing, etc.
      Messenger_var const messenger (new Messenger_i);

      // Send a message to the Messenger object.
      CORBA::String_var message = CORBA::string_dup ("Hello!");
      messenger->send_message ("TAO User", "TAO Test", message.inout ());

      // Print the Messenger's reply.
      std::cout << "Reply: " << message.in () << std::endl;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught CORBA::Exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}

