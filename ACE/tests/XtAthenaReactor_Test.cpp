/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    XtReactor_Test.cpp
//
// = DESCRIPTION
//      This is a simple test that illustrates the possibility to integrate
//      ACE to the X Main Loop. This program uses ACE_XtReactor class to
//      schedule three additional event sources:
//      1. Events from button "Stop Test" (registed with XtAddCallback)
//      2. Events from button "Press Me" (registed with XtAddCallback)
//      3. Events from X timer (registed with XtAppAddTimeOut)
//      4. Events from ACE timer (registed with ACE_XtReactor::schedule_timer)
//      5. Events from the TCP/IP channel using ACE_Acceptor
//      No command line arguments are needed to run the test.
//      Programs needs Athena Widgets to be compiled and run.
//
// = AUTHOR
//    Kirill Rybaltchenko <Kirill.Rybaltchenko@cern.ch>
//
// ============================================================================

#include "test_config.h"
#include "ace/XtReactor/XtReactor.h"
#include "ace/Event_Handler.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"

#include "ace/OS_NS_unistd.h"

#include /**/ <X11/Intrinsic.h>
#include /**/ <X11/Xatom.h>
#include /**/ <X11/Shell.h>
#include /**/ <X11/StringDefs.h>

#if defined (ACE_HAS_ATHENA3D)
#  include /**/ <X11/Xaw3d/Command.h>
#  include /**/ <X11/Xaw3d/Label.h>
#  include /**/ <X11/Xaw3d/Box.h>
#else
#  include /**/ <X11/Xaw/Command.h>
#  include /**/ <X11/Xaw/Label.h>
#  include /**/ <X11/Xaw/Box.h>
#endif

static void set_label(Widget w, const char *p)
{
    XtVaSetValues (w, XtNlabel, p, static_cast<void *>(0));
}
#define LABEL_WIDGET labelWidgetClass
#define BUTTON_WIDGET commandWidgetClass
#define PRESS_ME_CALLBACK XtNcallback
static Widget create_box(Widget parent, const char * name)
{
    return XtCreateWidget( (char*) name, boxWidgetClass, parent, 0, 0);
}

// Port we listen on.
static const u_short SERV_TCP_PORT = 6670;

// counter for events from "Press Me" button.
static int count1 = 0;

// counter for events from X Timer.
static int count2 = 0;

// counter for events from ACE Timer.
static int count3 = 0;

// Callback for "Stop Test" buton - quit the program.
void
Quit (Widget, XtPointer, XtPointer)
{
  ACE_OS::exit (0);
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
  ACE_OS::sprintf (buf, "Client: the life was good!");

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

static void
inc_count (Widget, XtPointer client_data, XtPointer)
{
  char new_string[80];

  ACE_OS::sprintf (new_string,
                   "Events: [%d] [%d] [%d]",
                   count1++,
                   count2,
                   count3);
  set_label((Widget) client_data, new_string);
}

// Callback for X Timer.

static void
inc_tmo (void *w,XtIntervalId *)
{
  char new_string[80];

  if (count2 > 10)
    ACE_OS::exit (0);
  ACE_OS::sprintf (new_string,
                   "Events: [%d] [%d] [%d]",
                   count1,
                   count2++,
                   count3);

  set_label((Widget) w, new_string);

  (void) XtAppAddTimeOut (XtWidgetToApplicationContext ((Widget) w),
                          1000,
                          inc_tmo,
                          (Widget) w);
}

class EV_handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg)
  {
    char new_string[80];
    ACE_OS::sprintf (new_string,
                     "Events: [%d] [%d] [%d]",
                     count1,
                     count2,
                     count3++);
    set_label((Widget) arg, new_string);
    return 0;
  }
};

class Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *)
  {
  //FUZZ: enable check_for_lack_ACE_OS
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

#if defined (HummingBird_X)
extern "C" void HCLXmInit (void);
#endif /* HummingBird_X */

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("XtAthenaReactor_Test"));

  XtAppContext app_context;
  Widget topLevel, goodbye, PressMe, lbl, digits_rc;
  Widget children[5];

#if defined (HummingBird_X)
  HCLXmInit ();
#endif /* HummingBird_X */
  topLevel = XtVaAppInitialize (&app_context,
                                "XTReactor_Test",
                                0,
                                0,
                                &argc,
                                argv,
                                0,
                                static_cast<void *>(0));

  digits_rc = create_box(topLevel, "digits_rc");

  //"Stop Test" button.
  goodbye = XtCreateWidget ( (char *) "goodbye",
                             BUTTON_WIDGET,
                             digits_rc,
                             0,
                             0);
  set_label(goodbye, "Stop Test");

  //"Press Me" button
  PressMe = XtCreateWidget ((char *) "PressMe",
                            BUTTON_WIDGET,
                            digits_rc,
                            0,
                            0);

  //Display for event counter
  lbl = XtCreateWidget ((char *) "label_for_event_one",
                        LABEL_WIDGET,
                        digits_rc,
                        0,
                        0);
  set_label(lbl, "label_for_all_events");
  int ac = 0;
  children[ac++] = goodbye;
  children[ac++] = PressMe;
  children[ac++] = lbl;
  XtManageChildren (children, ac);
  XtManageChild (digits_rc);

  //Register callback for "Stop Test" button
  XtAddCallback (goodbye, PRESS_ME_CALLBACK, Quit, 0);

  //Register callback for "Press Me" button
  XtAddCallback (PressMe,
                 PRESS_ME_CALLBACK,
                 inc_count,
                 (XtPointer) lbl);

  // Register callback for X Timer
  (void) XtAppAddTimeOut (app_context,
                          1000,
                          inc_tmo,
                          (XtPointer) lbl);

  XtRealizeWidget (topLevel);

  // It will perform X Main Loop
  ACE_XtReactor reactor (app_context);

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
                              (const void *) lbl,
                              ACE_Time_Value (2),
                              ACE_Time_Value (2))==-1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) can't register with reactor\n"),
                      -1);

  ACE_Thread_Manager::instance ()->spawn ((ACE_THR_FUNC) client,
                                          0,
                                          THR_NEW_LWP | THR_DETACHED);

  XtAppMainLoop (XtWidgetToApplicationContext (topLevel));

  ACE_END_TEST;
  return 0;
}

