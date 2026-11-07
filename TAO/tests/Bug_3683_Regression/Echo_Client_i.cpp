#include "Echo_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/Messaging/Messaging.h"

#include <cstring>
#include <string>

// This is the interface program that accesses the remote object

int
Echo_Client_i::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'p':
        this->payload_length_ = ACE_OS::atoi (get_opts.opt_arg ()) * 1000000;
        break;
      }
  // Indicates successful parsing of the command line
  return 0;
}


int
Echo_Client_i::run (char const *name,
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
      CORBA::PolicyList policy_list {};
      policy_list.length (1);
      CORBA::Any object_timeout {};
      TimeBase::TimeT to {50000};
      to *= 365 * 24 * 3600;
      to *= 100;
      object_timeout <<= to;
      policy_list[0] = client_.orb ()->create_policy (
        Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
        object_timeout);
      CORBA::Object_var const object =
        client_->_set_policy_overrides (policy_list, CORBA::ADD_OVERRIDE);

      Echo_var const srv (Echo::_narrow (object.in ()));

      std::string const buf (this->payload_length_, 'a');
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sending len: %B \n"), buf.length ()));

      CORBA::String_var const s = srv->echo_string (buf.c_str ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nString echoed by client has len %B\n"),
                  std::strlen (s.in ())));

      if (client_.do_shutdown ())
        client_->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("\n Exception in RMI");
      return -1;
    }

  return 0;
}
