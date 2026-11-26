#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/Reactor.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"

int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  if (ACE_Service_Config::open (argv[0], ACE_DEFAULT_LOGGER_KEY, true, true) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to initialize service configuration\n")), 1);
    }

  if (ACE_Service_Config::process_directive (
        ACE_TEXT ("dynamic Remote_Dgram Service_Object * ")
        ACE_TEXT ("./IPC_Tests_Server:remote_dgram \"-p 0\"")) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to load remote datagram service\n")), 1);
    }

  ACE_Service_Object * const service =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (ACE_TEXT ("Remote_Dgram"));
  if (service == nullptr || service->get_handle () == ACE_INVALID_HANDLE
      || ACE_Reactor::instance ()->find_handler (service->get_handle ()) != service)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Remote datagram service was not registered\n")), 1);
    }

  ACE_HANDLE const handle = service->get_handle ();
  if (ACE_Service_Config::remove (ACE_TEXT ("Remote_Dgram")) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to remove remote datagram service\n")), 1);
    }
  if (ACE_Reactor::instance ()->find_handler (handle) != nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Remote datagram handler is still registered\n")), 1);
    }
  return 0;
}
