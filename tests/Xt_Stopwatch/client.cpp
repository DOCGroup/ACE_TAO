// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"

#include "Control.h"
#include "Client.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  XtAppContext app;
  Widget toplevel = XtAppInitialize (&app,
                                     "Start & Stop",
                                     0,
                                     0,
                                     &argc,
                                     argv,
                                     0,
                                     0,
                                     0);

  TAO::XtResource_Loader xt_loader (app);

  Control control (toplevel);


  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client client (orb.in ());

      client.parse_args (argc, argv);

      client.add_callback (control);

      // Manage the widgets
      control.manage ();
      XtRealizeWidget (toplevel);
      XtAppMainLoop (app);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}

Client::Client (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Client::~Client (void)
{
}

void
Client::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR ((LM_ERROR,
                    "usage:  %s "
                    "-k <ior> "
                    "\n",
                    argv [0]));
      }

  CORBA::Object_var object =
    this->orb_->string_to_object (ior);

  this->server_ =
    Stopwatch::_narrow (object.in ());

  if (CORBA::is_nil(this->server_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n The server value is nil "));
    }
}

void
Client::add_callback (Control &ctrl)
{
  XtPointer client_data = static_cast<XtPointer> (this);

  // Register callbacks, specifying the object's instance pointer as
  // client data.
  XtAddCallback (ctrl.startwidget (),
                 XmNactivateCallback,
                 &Client::start_callback,
                 client_data);

  XtAddCallback (ctrl.stopwidget (),
                 XmNactivateCallback,
                 &Client::stop_callback,
                 client_data);
}

void
Client::start_callback (Widget /*widget*/,
                        XtPointer client_data,
                        XtPointer)
{
  Client *self = static_cast<Client *> (client_data);
  self->start_hook ();
}

void
Client::stop_callback (Widget /*widget*/,
                       XtPointer client_data,
                       XtPointer )
{
  Client *self = static_cast<Client *> (client_data);
  self->stop_hook ();
}

void
Client::start_hook (void)
{
  try
    {
      this->server_->start ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Caught an exception in the start button callback");
      return;
    }
}

void
Client::stop_hook (void)
{
  try
    {
      this->server_->stop ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Caught an exception in the stop button callback");
      return;
    }
}

