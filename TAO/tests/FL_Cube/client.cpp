#include "tao/FlResource/FlResource_Loader.h"
#include "testC.h"
#include "ace/Get_Opt.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Roller.H>
#include <memory>

class Client
{
public:
  Client (CORBA::ORB_ptr orb, Fl_Window *parent);
  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  ~Client () = default;

  void show ();
  // Call show on all the window objects

  int parse_args (int argc, ACE_TCHAR *argv[]);

private:
  static void x_cb (Fl_Widget *, void *cookie);
  static void y_cb (Fl_Widget *, void *cookie);
  // The callbacks

  void x_changed ();
  void y_changed ();
  // The methods for the callbacks

private:
  CORBA::ORB_var const orb_;
  // The ORB

  std::unique_ptr<Fl_Roller> x_roller_;
  std::unique_ptr<Fl_Roller> y_roller_;
  // The rollers

  Simple_Server_var server_ {};
  // The server.
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::FlResource_Loader fl_loader;

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Fl_Window window (300, 100);

      Client client (orb.in (), &window);

      window.end ();

      window.show ();

      client.show ();

      if (client.parse_args (argc, argv) != 0)
        return 1;

      Fl::run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}

Client::Client (CORBA::ORB_ptr orb, Fl_Window *parent)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    x_roller_ (std::make_unique<Fl_Roller> (10, parent->h () / 2 - 20,
                                            parent->w () / 2 - 10, 40)),
    y_roller_ (std::make_unique<Fl_Roller> (3 * parent->w () / 4 - 20, 10,
                                            40, parent->h () - 20))
{
  this->x_roller_->type (FL_HORIZONTAL);
  this->x_roller_->callback (Client::x_cb, this);
  this->x_roller_->range (-360, 360);
  this->x_roller_->step (1);

  this->y_roller_->type (FL_VERTICAL);
  this->y_roller_->callback (Client::y_cb, this);
  this->y_roller_->range (-360, 360);
  this->y_roller_->step (1);
}

void
Client::show ()
{
  this->x_roller_->show ();
  this->y_roller_->show ();
}

void
Client::x_cb (Fl_Widget *, void *cookie)
{
  Client * const self = static_cast<Client *> (cookie);
  self->x_changed ();
}

void
Client::y_cb (Fl_Widget *, void *cookie)
{
  Client * const self = static_cast<Client *> (cookie);
  self->y_changed ();
}

void
Client::x_changed ()
{
  try
    {
      CORBA::Long const x = static_cast<CORBA::Long> (this->x_roller_->value ());
      this->server_->set_x_angle (x);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client::x_changed");
    }
}

void
Client::y_changed ()
{
  try
    {
      CORBA::Long const y = static_cast<CORBA::Long> (this->y_roller_->value ());
      this->server_->set_y_angle (y);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client::y_changed");
    }
}

int
Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior>\n"),
                           argv [0]),
                          -1);
      }

  CORBA::Object_var const object =
    this->orb_->string_to_object (ior);

  this->server_ = Simple_Server::_narrow (object.in ());
  if (CORBA::is_nil (this->server_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Object reference <%s> is nil.\n"), ior), -1);
  return 0;
}
