// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    TkReactor_Test.cpp
//
// = DESCRIPTION
//      This is a simple test that illustrates the possibility to integrate
//      ACE to the Tk Main Loop. This program uses ACE_TkReactor class to
//      schedule three additional event sources:
//      1. Events from button "Stop Test" (registed with Tk_CreateEventHandler)
//      2. Events from button "Press Me" (registed with Tk_CreateEventHandler)
//      3. Events from X timer (registed with Tk_CreateTimerHandler)
//      4. Events from ACE timer (registed with ACE_TkReactor::schedule_timer)
//      5. Events from the TCP/IP channel using ACE_Acceptor
//      No command line arguments are needed to run the test.
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"

ACE_RCSID (tests,
           TkReactor_Test,
           "$Id$")

#if !defined (ACE_HAS_TK)

int ACE_TMAIN (int, ACE_TCHAR*[])
{
  ACE_START_TEST (ACE_TEXT ("TkReactor_Test"));

  ACE_ERROR ((LM_ERROR,
              "Tk not supported on this platform\n"));

  ACE_END_TEST;
  return 0;
}

#else

#include "ace/Event_Handler.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "ace/TkReactor.h"

#include <tcl.h>
#include <tk.h>

Tcl_Interp* tcl_interp;
void eval (const char *s)
{
  char buf[BUFSIZ];
  strcpy (buf,s);
  int st = Tcl_GlobalEval(tcl_interp,buf);
  if (st != TCL_OK)
    {
      int n =  strlen(s);
      char* wrk = new char[n + 80];
      sprintf(wrk, "tkerror \"%s\"", s);
      Tcl_GlobalEval(tcl_interp, wrk);
      delete wrk;
      //exit(1);
    }
}

// Port we listen on.
static const u_short SERV_TCP_PORT = 6670;

// counter for events from "Press Me" button.
static int count1 = 0;

// counter for events from Tk Timer.
static int count2 = 0;

// counter for events from ACE Timer.
static int count3 = 0;

static int quit = 0;
// Callback for "Stop Test" buton - quit the program.
void
Quit (ClientData, XEvent *)
{
  cerr << "quit called";
  ACE_DEBUG ((LM_DEBUG,"Quit called\n"));
  quit = 1;
}

static void *
client (void *)
{
  char buf[100];
  size_t mes_len;
  ACE_OS::sleep (1);

  ACE_DEBUG ((LM_DEBUG,
              " (%P) Client: Starting...\n"));

  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connector;
  sprintf (buf, "Client: the life was good!");

  mes_len = (int) htonl (ACE_OS::strlen (buf) + 1);

  if (connector.connect (stream,
                         ACE_INET_Addr (SERV_TCP_PORT,
                                        ACE_DEFAULT_SERVER_HOST)) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P) %p\n",
                "Socket open"));

  if (stream.send (4,
                   (void *) &mes_len,
                   sizeof (size_t),
                   (void *)buf,
                   ACE_OS::strlen (buf) + 1) == -1)

    ACE_ERROR ((LM_ERROR,
                "(%P) %p\n",
                "Socket send"));

  if (stream.close () == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P) %p\n",
                "Socket close"));

  ACE_DEBUG ((LM_DEBUG,
              "(%P) Client: Message has been sent, about to exit...\n"));
  return 0;
}

// Callback for "Press Me" button.

static int
inc_count (ClientData client_data, Tcl_Interp *interp,int, char **)
{
  ACE_DEBUG ((LM_DEBUG,"inc_count "));
  char new_string[80];

  sprintf (new_string,
           "Events: [%d] [%d] [%d]",
           count1++,
           count2,
           count3);

  //  sprintf (command,"set %s %s",(char *)client_data,new_string);
  // eval (command);
  char *varValue = Tcl_SetVar (interp,(char *)client_data,new_string,TCL_LEAVE_ERR_MSG);
  if (varValue == NULL)
    return TCL_ERROR;
  return TCL_OK;
}

// Callback for X Timer.

static void
inc_tmo (ClientData client_data)
{
  char new_string[80];

  if (count2 > 10)
    ACE_OS::exit (0);
  sprintf (new_string,
           "Events: [%d] [%d] [%d]",
           count1,
           count2++,
           count3);

  //  sprintf (command,"set %s %s",(char *)client_data,new_string);
  //  eval (command);
  char *varValue = Tcl_SetVar (tcl_interp,(char *)client_data,new_string,TCL_LEAVE_ERR_MSG);
  if (varValue == NULL)
    ACE_ERROR ((LM_ERROR,"Tcl_SetVar failed in inc_tmo\n"));

  (void) Tk_CreateTimerHandler (1000,
                                inc_tmo,
                                client_data);
}

class EV_handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg)
  {
    char new_string[80];
    sprintf (new_string,
             "Events: [%d] [%d] [%d]",
             count1,
             count2,
             count3++);

    //    sprintf (command,"set %s %s",(char *)arg,new_string);
    //    eval (command);
    char *varValue = Tcl_SetVar (tcl_interp,(char *)arg,new_string,TCL_LEAVE_ERR_MSG);
    if (varValue == NULL)
      ACE_ERROR_RETURN ((LM_ERROR,"Tcl_SetVar failed in handle_timeout\n"),-1);

    return 0;
  }
};

class Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  virtual int open (void *)
  {
    char buf[100];
    int head;
    ssize_t ret = this->peer ().recv_n ((char *) &head,
                                        sizeof (int));
    if (ret != sizeof (int))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P) %p\n",
                         "read header"),
                        -1);

    ret = this->peer ().recv_n (buf,
                                (int) ntohl (head));

    if (ret != (int) ntohl (head))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P) %p\n",
                         "read message"),
                        -1);
    ACE_DEBUG ((LM_DEBUG,
                " (%P)Server (ACE_SOCKET channel message): [%s]\n",
                buf));
    return 0;
  }
};

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

int
init (Tcl_Interp *interp)
{
  if (Tcl_Init (interp) == TCL_ERROR)
    return TCL_ERROR;
  if (Tk_Init (interp) == TCL_ERROR)
    return TCL_ERROR;
  return TCL_OK;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("TkReactor_Test"));

  tcl_interp   = Tcl_CreateInterp ();

  if (init (tcl_interp) != TCL_OK) {
    cerr << "error: " << tcl_interp->result << "\n";
    exit (1);
  }

  Tk_Window tk = 0;
  tk = Tk_MainWindow(tcl_interp);
  if (tk == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Tk_Reactor_Test: %s\n", tcl_interp->result),1);
    }

  char tcl_cmd[] = "source TkReactor_Test.tcl";
  if (Tcl_Eval (tcl_interp, tcl_cmd) != TCL_OK) {
    cerr << "error: " << tcl_interp->result << "\n";
    exit (1);
  }
  // set up callback
  char label_var_name[] = "label_var";
  char pressme[] = "pressme";
  Tcl_CreateCommand (tcl_interp,
                     pressme,
                     inc_count,
                     label_var_name,
                     0);

  // Register callback for X Timer
  (void) Tk_CreateTimerHandler (1000,
                                inc_tmo,
                                label_var_name);

  // It will perform Tk Main Loop
  ACE_TkReactor reactor;
  ACE_Reactor r (&reactor);

  //Event Handler for ACE Timer.
  EV_handler evh;

  ACE_Acceptor <Connection_Handler, ACE_SOCK_ACCEPTOR> acceptor;

  if (acceptor.open (ACE_INET_Addr ((u_short) SERV_TCP_PORT),
                     &r) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  if (reactor.schedule_timer (&evh,
                              (const void *) "label_var",
                              ACE_Time_Value (2),
                              ACE_Time_Value (2))==-1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) can't register with reactor\n"),
                      -1);

  ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) client,
                                          NULL,
                                          THR_NEW_LWP | THR_DETACHED);

  while (!quit)
    {
      int result = reactor.handle_events ();
      switch (result)
        {
        case 0:
          //          ACE_DEBUG ((LM_DEBUG,"handle_events timed out\n"));
          break;
        case -1:
          ACE_DEBUG ((LM_DEBUG,"handle_events returned -1\n"));
          quit = 1;
          break;
        }
    }

  ACE_END_TEST;
  return 0;
}

#endif /* ACE_HAS_TK */
