// $Id$

#include "Command_Handler.h"

// %% yikes!!!
#include "ctr.cpp"

Command_Handler::Command_Handler (ACE_HANDLE command_handle)
  : command_handle_ (command_handle)
{
}

ACE_HANDLE
Command_Handler::get_handle (void) const
{
  return this->command_handle_;
}


// handle the command sent to us by the GUI process 
// this is a reactor callback method
int
Command_Handler::handle_input (ACE_HANDLE fd)
{

  {
    unsigned char cmd;
    int val;
    val = OurCmdRead ((char*)&cmd, 1);
    ::TimerProcessing ();

    // if we get an interrupt while reading we go back to the event loop    
    if (val == 1)
      return 0;

    FILE * fp = NULL;   /* file pointer for experiment plan */
    usr1_flag = 0;
    
    //    fprintf(stderr, "CTR: cmd received - %d\n", cmd);
    
    switch (cmd)
      {
      case CmdINIT:
        init();
        
        /* following for automatic expriment plan when invoked by Developer */
        if (getuid() == DEVELOPER_UID && videoSocket >= 0) {
          fp = fopen(EXP_PLAN_FILE, "r");
          if (fp != NULL) {
            static char expCmd[6] = {CmdPOSITIONrelease, 0, 0, 0, 0, CmdPLAY};
            fprintf(stderr,
                    "Warning: Auto-exp plan is to be conducted as instructed by file %s\n",
                    EXP_PLAN_FILE);
            cmdBuffer = expCmd;
            cmdBytes = 6;
            cmdAcks = 2;
          }
        }
        else fp = NULL;
      
        break;
      case CmdSTOP:
        stop();
        break;
      case CmdFF:
        ff();
        break;
      case CmdFB:
        fb();
        break;
      case CmdSTEP:
        step();
        break;
      case CmdPLAY:

        /* following is for automatic experiment plan */
        if (fp != NULL) {
          char buf[64];
          while (fgets(buf, 64, fp) != NULL) {
            if (!strncmp("Delay", buf, 5)) {
              int val;
              sscanf(strchr(buf, ' '), "%d", &val);
              if (val < 0) val = 1;
              else if (val > 60) val = 60;
              fprintf(stderr, "Auto-exp: Delay for %d seconds\n", val);
              usleep(val * 1000000);
            }
            else if (!strncmp("Experiment", buf, 5)) {
              fprintf(stderr, "Auto-exp: to perform an experiment\n");
              while (fgets(buf, 64, fp) != NULL && buf[0] > ' ') {
                if (!strncmp("playSpeed", buf, 5)) {
                  double fps;
                  sscanf(strchr(buf, ' '), "%lf", &fps);
                  /* following code is copied from definition of set_speed(void) */
                  shared->framesPerSecond = (int)fps;
                  shared->usecPerFrame = (int) (1000000.0/fps);
                  if (audioSocket >= 0)
                    {
                      double sps = shared->audioPara.samplesPerSecond *
                        fps / shared->pictureRate;
                      shared->samplesPerSecond = (int)sps;
                      shared->usecPerSample = (int)(1000000.0/sps);
                    }
                }
                else if (!strncmp("frameRateLimit", buf, 5)) {
                  sscanf(strchr(buf, ' '), "%f", &shared->config.frameRateLimit);
                }
                else if (!strncmp("maxSPframes", buf, 5)) {
                  sscanf(strchr(buf, ' '), "%d", &shared->config.maxSPframes);
                }
                else if (!strncmp("filterPara", buf, 5)) {
                  sscanf(strchr(buf, ' '), "%d", &shared->config.filterPara);
                }
                else if (!strncmp("collectStat", buf, 5)) {
                  sscanf(strchr(buf, ' '), "%d", &shared->config.collectStat);
                }
                else if (!strncmp("qosEffective", buf, 5)) {
                  sscanf(strchr(buf, ' '), "%d", &shared->config.qosEffective);
                }
                else if (!strncmp("syncEffective", buf, 5)) {
                  sscanf(strchr(buf, ' '), "%d", &shared->config.syncEffective);
                }
              }
              usleep(500000);
              shared->loopBack = 1;
              break;
            }
            else if (!strncmp("EndExperiment", buf, 5)) {
              fprintf(stderr, "Auto-exp ends.\n");
              usleep(2000000);
              fclose(fp);
              exit(0);
            }
          }
        }

        play(fp != NULL);
        break;
      case CmdPOSITION:
        position();
        break;
      case CmdPOSITIONrelease:
        position_release();
        break;
      case CmdVOLUME:
        volume();
        break;
      case CmdBALANCE:
        balance();
        break;
      case CmdSPEED:
        speed();
        break;
      case CmdLOOPenable:
        {
          shared->loopBack = 1;
          break;
        }
      case CmdLOOPdisable:
        {
          shared->loopBack = 0;
          break;
        }
      default:
        fprintf(stderr, "CTR: unexpected command from UI: cmd = %d.\n", cmd);
        exit(1);
        break;
      }
  }
  return 0;
}

// ----------------------------------------------------------------------

// Client_Sig_Handler methods
// handles the timeout SIGALRM signal
Client_Sig_Handler::Client_Sig_Handler ()
{
}

int
Client_Sig_Handler::register_handler (void)
{
  // Assign the Sig_Handler a dummy I/O descriptor.  Note that even
  // though we open this file "Write Only" we still need to use the
  // ACE_Event_Handler::NULL_MASK when registering this with the
  // ACE_Reactor (see below).
  this->handle_ = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);
  ACE_ASSERT (this->handle_ != -1);

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.
  if (ACE_Reactor::instance ()->register_handler 
      (this, ACE_Event_Handler::NULL_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);

  // Create a sigset_t corresponding to the signals we want to catch.
  ACE_Sig_Set sig_set;

  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);
  sig_set.sig_add (SIGALRM);  
  sig_set.sig_add (SIGUSR1);
  sig_set.sig_add (SIGUSR2);  

  // Register the signal handler object to catch the signals.
  if (ACE_Reactor::instance ()->register_handler (sig_set, 
                                                  this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);
  return 0;
}
// Called by the ACE_Reactor to extract the fd.

ACE_HANDLE
Client_Sig_Handler::get_handle (void) const
{
  return this->handle_;
}

int 
Client_Sig_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handling asynchonrous input...\n"));
  return 0;
}

int 
Client_Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) closing down Sig_Handler...\n"));
  return 0;
}

// This method handles all the signals that are being caught by this
// object.  In our simple example, we are simply catching SIGALRM,
// SIGINT, and SIGQUIT.  Anything else is logged and ignored.
//
// There are several advantages to using this approach.  First, 
// the behavior triggered by the signal is handled in the main event
// loop, rather than in the signal handler.  Second, the ACE_Reactor's 
// signal handling mechanism eliminates the need to use global signal 
// handler functions and data. 

int
Client_Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  int status;
  pid_t pid;
  //  ACE_DEBUG ((LM_DEBUG, "(%t) received signal %S\n", signum));

  switch (signum)
    {
    case SIGALRM:
      // Handle the timeout
      ::TimerHandler (signum);
      // %% ??!!!
      break;
    case SIGUSR1:
      usr1_handler (signum);
      break;
    case SIGUSR2:
      default_usr2_handler (signum);
      break;
    case SIGCHLD:
      pid = ACE_OS::wait (&status);
      if (pid == UIpid)
        ACE_Reactor::instance ()->end_event_loop ();
    default: 
      ACE_DEBUG ((LM_DEBUG, 
                  "(%t) %S: not handled, returning to program\n", 
                  signum));
      break;
    }
  TimerProcessing ();
  return 0;
}
