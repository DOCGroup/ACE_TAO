// $Id$
#include "tao/FlResource/FlResource_Loader.h"
#include "testC.h"
#include "ace/Get_Opt.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Roller.H>

class Client
{
  // = TITLE
  //   Run the client thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run the client threads.
  //
public:
  Client (CORBA::ORB_ptr orb,
          Fl_Window* parent);
  // ctor

  ~Client (void);

  void show (void);
  // Call show on all the window objects

  void parse_args (int argc, ACE_TCHAR *argv[]);

private:
  static void x_cb (Fl_Widget *widget, void* cookie);
  static void y_cb (Fl_Widget *widget, void* cookie);
  // The callbacks

  void x_changed (void);
  void y_changed (void);
  // The methods for the callbacks

private:
  CORBA::ORB_var orb_;
  // The ORB

  Fl_Roller* x_roller_;
  Fl_Roller* y_roller_;
  // The rollers

  Simple_Server_var server_;
  // The server.
};

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  TAO::FlResource_Loader fl_loader;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Fl_Window window (300, 100);

      Client client (orb.in (), &window);

      window.end ();

      window.show ();

      client.show ();

      client.parse_args (argc, argv);

      Fl::run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}

Client::Client (CORBA::ORB_ptr orb,
                Fl_Window* parent)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
  this->x_roller_ =
    new Fl_Roller (10, parent->h () / 2 - 20,
                   parent->w () / 2 - 10, 40);
  this->x_roller_->type (FL_HORIZONTAL);
  this->x_roller_->callback (Client::x_cb, this);
  this->x_roller_->range (-360, 360);
  this->x_roller_->step (1);

  this->y_roller_ =
    new Fl_Roller (3 * parent->w () / 4 - 20, 10,
                   40, parent->h () - 20);
  this->y_roller_->type (FL_VERTICAL);
  this->y_roller_->callback (Client::y_cb, this);
  this->y_roller_->range (-360, 360);
  this->y_roller_->step (1);
}

Client::~Client (void)
{
  delete x_roller_;
  delete y_roller_;
}

void
Client::show (void)
{
  this->x_roller_->show ();
  this->y_roller_->show ();
}

void
Client::x_cb (Fl_Widget*, void* cookie)
{
  Client *self = static_cast<Client*> (cookie);
  self->x_changed ();
}

void
Client::y_cb (Fl_Widget*, void* cookie)
{
  Client *self = static_cast<Client*> (cookie);
  self->y_changed ();
}

void
Client::x_changed (void)
{
  try
    {
      CORBA::Long x = CORBA::Long (this->x_roller_->value ());
      this->server_->set_x_angle (x);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client::x_changed");
    }
}

void
Client::y_changed (void)
{
  try
    {
      CORBA::Long y = CORBA::Long (this->y_roller_->value ());
      this->server_->set_y_angle (y);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client::x_changed");
    }
}

void
Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

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
    Simple_Server::_narrow (object.in ());
}
