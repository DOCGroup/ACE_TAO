// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(FL_Cube, client, "$Id$")

#if !defined (ACE_HAS_FL)

int main (int, char *[])
{
  ACE_ERROR ((LM_INFO,
              "FL not supported on this platform\n"));
  return 0;
}

#else

#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Roller.h>

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

  void parse_args (int argc, char *argv[],
                   CORBA::Environment &ACE_TRY_ENV);

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

int main (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Fl_Window window (300, 100);

      Client client (orb.in (), &window);

      window.end ();

      window.show ();

      client.show ();

      client.parse_args (argc, argv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Fl::run ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;
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
  Client *self = ACE_static_cast(Client*,cookie);
  self->x_changed ();
}

void
Client::y_cb (Fl_Widget*, void* cookie)
{
  Client *self = ACE_static_cast(Client*,cookie);
  self->y_changed ();
}

void
Client::x_changed (void)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Long x = CORBA::Long (this->x_roller_->value ());
      this->server_->set_x_angle (x, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Client::x_changed");
    }
  ACE_ENDTRY;
}

void
Client::y_changed (void)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Long y = CORBA::Long (this->y_roller_->value ());
      this->server_->set_y_angle (y, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Client::x_changed");
    }
  ACE_ENDTRY;
}

void
Client::parse_args (int argc, char *argv[],
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
    Simple_Server::_narrow (object.in (), ACE_TRY_ENV);
  ACE_CHECK;
}
#endif /* ACE_HAS_FL */
