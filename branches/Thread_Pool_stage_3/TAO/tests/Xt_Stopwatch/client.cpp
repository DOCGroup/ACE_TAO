// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Xt_Stopwatch, client, "$Id$")

#if !defined (ACE_HAS_XT)

int 
main (int, char *[])
{
  ACE_ERROR ((LM_INFO,
              "Xt not supported on this platform\n"));
  return 0;
}

#else
#include "Control.h"
#include "Client.h"


int
main (int argc, char *argv[])
{
  XtAppContext app;
  Widget toplevel = XtAppInitialize (&app,
                                     "Start & Stop",
                                     NULL,
                                     0,
                                     &argc,
                                     argv,
                                     NULL,
                                     NULL,
                                     0);

  TAO_XT_Resource_Factory::set_context (app);

  Control control (toplevel);

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Client client (orb.in ());

      client.parse_args (argc, argv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      client.add_callback (control);

      // Manage the widgets
      control.manage ();
      XtRealizeWidget (toplevel);
      XtAppMainLoop (app);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;
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
                    char *argv[],
                    CORBA::Environment &ACE_TRY_ENV)
{
  const char *ior = "file://test.ior";

  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
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
    this->orb_->string_to_object (ior, ACE_TRY_ENV);
  ACE_CHECK;

  this->server_ =
    Stopwatch::_narrow (object.in (), ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil(this->server_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n The server value is nil "));
    }
}

void
Client::add_callback (Control &ctrl)
{
  XtPointer client_data = ACE_static_cast (XtPointer, this);

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
  Client *self = ACE_static_cast (Client *,
                                  client_data);
  self->start_hook ();
}

void
Client::stop_callback (Widget /*widget*/,
                       XtPointer client_data,
                       XtPointer )
{
  Client *self = ACE_static_cast (Client *,
                                  client_data);
  self->stop_hook ();
}

void
Client::start_hook (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->server_->start (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Caught an exception in the start button callback");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
Client::stop_hook (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->server_->stop (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught an exception in the stop button callback");
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

#endif /* ACE_HAS_FL */
