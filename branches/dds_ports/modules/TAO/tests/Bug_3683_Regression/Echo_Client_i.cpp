//$Id$

#include "Echo_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"
#include "tao/Messaging/Messaging.h"

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
      CORBA::PolicyList policyList;
      policyList.length(1);
      CORBA::Any objectTimeout;
      TimeBase::TimeT to = 50000;
      to *= 365 * 24 * 3600;
      to *= 100;
      objectTimeout <<= to;
      policyList[0] = client_.orb()->create_policy(
          Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
          objectTimeout);
      CORBA::Object_var object =
        client_->_set_policy_overrides(policyList, CORBA::ADD_OVERRIDE);

      Echo_var srv(Echo::_narrow(object));

/*
      // Get the input message which has to be displayed.
      // When we are reading potentially large inputs like this, we
      // cannot use ACE_Read_Buffer, as it uses recursion, and this might
      // smash the stack when the input is large (>2M or so)
      std::string buf;
      std::string line;
      while (cin.good())
        {
          char* lstring = 0;
          std::cin >> lstring;
          buf += line;
          buf += " ";
        }
*/
      ACE_CString buf;
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("read len: %d \n"), buf.length())); 

      CORBA::String_var s = srv->echo_string (buf.c_str());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nString echoed by client has len %d\n"),
                  ACE_OS::strlen(s.in ())));

      if (client_.do_shutdown () == 1)
        client_->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("\n Exception in RMI");
      return -1;
    }

  return 0;
}
