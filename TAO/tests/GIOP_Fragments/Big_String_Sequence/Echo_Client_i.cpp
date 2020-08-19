#include "Echo_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"
#include <iostream>
#include <cstring>

// This is the interface program that accesses the remote object

// Constructor.
Echo_Client_i::Echo_Client_i (void)
{
  //no-op
}

//Destructor.
Echo_Client_i::~Echo_Client_i (void)
{
  //no-op
}

int
Echo_Client_i::run (const char *name,
                    int argc,
                    ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (client_.init (name, argc, argv) == -1)
    return -1;

  try
  {
     Echo::List_var list = client_->return_list();
     std::cout << "Received list of length " 
               << list->length() << std::endl;
     if (list->length() != 2)
     {
        std::cout << "ERROR: Expected length 2, exiting..." << std::endl;
        client_->shutdown (); 
        return -1;
     }
     const char* value = (*list)[0].in();
     size_t length = std::strlen(value);
     std::cout << "First element has length " 
               << length << std::endl;
     for (size_t n = 0; n < length; ++n)
     {
        if (value[n] != 'A')
        {
           std::cout << "ERROR: Character at position " << n
                       << " should be 'A', but is '" 
                     << value[n] << "'" << std::endl;
           client_->shutdown ();
           return -1;
        }
     }
     value = (*list)[1].in();
     length = std::strlen(value);
     std::cout << "Second element has length " 
               << length << std::endl;
     std::cout << "Value: " << value << std::endl;
     if (std::strcmp(value, "Hello World") != 0)
     {
        std::cout << "ERROR: Expected \"Hello World\", exiting..." 
                  << std::endl; 
        client_->shutdown ();
        return -1;
     }
     client_->shutdown ();
  }
  catch (const CORBA::Exception& ex)
  {
     ex._tao_print_exception ("\n Exception in RMI");
     return -1;
  }

  return 0;
}
