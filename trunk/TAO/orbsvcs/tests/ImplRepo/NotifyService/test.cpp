// $Id$

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "ace/Log_Msg.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("NotifyService");

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Could not resolve Notify Service"),
                          1);
      try
        {
          CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_ =
            CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());
        }
      catch (CORBA::OBJECT_NOT_EXIST &)
        {
          ACE_DEBUG ((LM_DEBUG, "Test caught Object Not Exist, retrying\n"));
          CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_ =
            CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());
        }

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Test Successful\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test");
      return 1;
    }

  return 0;
}
