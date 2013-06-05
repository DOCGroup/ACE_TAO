// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "tao/QtResource/QtResource_Loader.h"

#include "client.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  ACE_Argv_Type_Converter ct (argc, argv);
  QApplication app (argc,  ct.get_ASCII_argv ());
  TAO::QtResource_Loader qt_resources (&app);

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client client (orb.in (), app);

      client.parse_args (argc, argv);

      // Creates the Qt widgets
      client.create_widgets ();

      // This may look a bit suspect, but Qt wants the manager widget
      // as the toplevel widget unlike Xt for display purposes.
      app.setActiveWindow (&(client.mainwindow_));

      // Show them on Screen
      client.show ();

      app.exec ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}

Client::Client (CORBA::ORB_ptr orb,
                QApplication &qapp)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    qapp_ (&qapp)
{
}

Client::~Client (void)
{
  delete this->slider_;
  delete this->push_button_;
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
    LCD_Display::_narrow (object.in ());

  if (CORBA::is_nil(this->server_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n The server value is nil "));
    }
}

void
Client::create_widgets (/**/)
{
  // Ewsize the box
  this->mainwindow_.resize (200,120);
  this->mainwindow_.setWindowTitle("QtClient");

  // Make a pushbutton widget
  ACE_NEW (this->push_button_,
           QPushButton ("Quit"));
  box_.addWidget(this->push_button_);

  // Connect the click () SIGNAL routine with the SLOT handler that we
  // have defined
  QObject::connect (this->push_button_,
                    SIGNAL (clicked()),
                    this,
                    SLOT (shutdown_call ()));

  // Create a slider widget
  ACE_NEW (this->slider_,
           QSlider (Qt::Horizontal));

  // Add resource for the slider
  this->slider_->setRange (0, 99);
  this->slider_->setValue (0);

  box_.addWidget(this->slider_);


  // Connect the valuechanged SIGNAL routine with the slot that we
  // have defined. THe slot routine would invoke the remote call.
  QObject::connect (this->slider_,
                    SIGNAL (valueChanged (int)),
                    this,
                    SLOT (remote_call (int)));

  this->mainwindow_.setLayout(&box_);


}

void
Client::show (void)
{
  this->mainwindow_.show ();
}

void
Client::remote_call (int val)
{
  this->server_->send_val (val);
}

void
Client::shutdown_call (void)
{
  this->server_->shutdown ();
}

