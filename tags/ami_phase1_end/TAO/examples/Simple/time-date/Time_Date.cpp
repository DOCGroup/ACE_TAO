// $Id$

#include "tao/TAO.h"
#include "../Simple_util.h"
#include "Time_Date_i.h"

class DLL_ORB : public ACE_Task_Base
{
public:
  virtual int init (int argc, char *argv[]);
  // Initialize the <TAO_ORB_Manager>.

  virtual int fini (void);
  // Shutdown the <TAO_ORB_Manager>.
  
  virtual int svc (void);
  // Concurrency hook.

private:
  TAO_ORB_Manager orb_manager_;
  // ORB manager.
  
  Server<Time_Date_i> server_;
  // Servant for the <Time_Date> object.
};

int 
DLL_ORB::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_DEBUG ((LM_DEBUG,
              "\n\trunning ORB event loop\n\n"));

  ACE_TRY
    {
      this->server_.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      ACE_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      ACE_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  ACE_ENDTRY;
}

int 
DLL_ORB::init (int argc, char *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              "\n\tTime_Date server\n\n"));

  // Initialize the ORB.
  if (this->orb_manager_.init (argc,
                               argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n", 
                       "ORB_Manager::init"),
                      1);
  // Initialize the <Time_Date_i> servant.
  else if (this->server_.init ("Time_Date",
                               argc,
                               argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n", 
                       "Server::init"),
                      1);
  else
    // Become an Active Object so that the ORB
    // will execute in a separate thread.
    return this->activate ();
}

int 
DLL_ORB::fini (void)
{
  return 0;
}
