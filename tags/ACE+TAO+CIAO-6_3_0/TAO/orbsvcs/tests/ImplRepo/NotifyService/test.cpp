// $Id$

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  bool success = false;
  int retries = 5;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("NotifyService");

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Could not resolve Notify Service")),
                          1);

      for (int i = 0; !success && i < retries; i++)
        {
          try
            {
              CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_ =
                CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());
              success = true;
            }
          catch (CORBA::OBJECT_NOT_EXIST &)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Test caught Object Not Exist, retry %d\n"),
                          i));
              ACE_OS::sleep (1);
            }
        }
      orb->destroy ();

      if (success)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test Successful\n")));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test Retries exceeded\n")));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test");
      return 1;
    }

  return success ? 0 : 1;
}
