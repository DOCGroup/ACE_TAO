//$Id$

#include "Echo_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"
#include "tao/Messaging/Messaging.h"

// This is the interface program that accesses the remote object

// Constructor.
Echo_Client_i::Echo_Client_i (void) : payload_length_ (0)
{
  //no-op
}

//Destructor.
Echo_Client_i::~Echo_Client_i (void)
{
  //no-op
}

int
Echo_Client_i::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        this->payload_length_ = ACE_OS::atoi(get_opts.opt_arg ()) * 1000000;
        break;
      }
  // Indicates successful parsing of the command line
  return 0;
}


int
Echo_Client_i::run (const char *name,
                    int argc,
                    ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (client_.init (name, argc, argv) == -1)
    return -1;

  if (this->parse_args (argc, argv) == -1)
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

      Echo_var srv(Echo::_narrow(object.in ()));

      char* buf = new char [this->payload_length_+ 1];
      ACE_OS::memset (buf, 'a', this->payload_length_);
      buf[this->payload_length_] = '\0';
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sending len: %d \n"), ACE_OS::strlen (buf)));

      CORBA::String_var s = srv->echo_string (buf);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nString echoed by client has len %d\n"),
                  ACE_OS::strlen(s.in ())));

      delete [] buf;

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
