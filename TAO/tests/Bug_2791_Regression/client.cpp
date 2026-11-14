#include "MessageFactory.h"
#include "MessageC.h"
#include "tao/corba.h"
#include "tao/AnyTypeCode/TypeCode.h"
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::ValueFactoryBase_var mf = new MessageFactory ();
      CORBA::String_var const id = _tc_Message->id ();

      ACE_DEBUG ((LM_DEBUG, "Registering factory reference\n"));
      orb->register_value_factory (id.in (), mf);

      ACE_DEBUG ((LM_DEBUG, "Unregistering factory reference\n"));
      orb->unregister_value_factory (id.in ());

      ACE_DEBUG ((LM_DEBUG, "Value factory destructor flag: %i\n",
                  MessageFactory::destructorInvoked_));

      ACE_DEBUG ((LM_DEBUG, "Releasing the local factory reference\n"));
      mf = nullptr;

      ACE_DEBUG ((LM_DEBUG, "Value factory destructor flag: %i\n",
                  MessageFactory::destructorInvoked_));

      ACE_DEBUG ((LM_DEBUG, "Shutting down / destroying ORB\n"));
      orb->shutdown (true);
      orb->destroy ();

      if (MessageFactory::destructorInvoked_ == 1)
        {
          ACE_DEBUG ((LM_ERROR,
                      "Test passed: value factory destructor was invoked as expected\n"));
          return 0;
        }
      else
        {
          ACE_DEBUG ((LM_ERROR,
                      "Test failed: value factory destructor was not invoked\n"));
          return 1;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CORBA exception caught during execution");
      return -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_ERROR, "Non-CORBA exception caught during execution"));
      return -1;
    }
}
