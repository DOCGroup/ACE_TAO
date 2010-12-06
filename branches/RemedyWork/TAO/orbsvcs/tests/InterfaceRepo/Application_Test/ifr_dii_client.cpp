// -*- C++ -*-
// $Id$

#include "ifr_dii_client.h"
#include "ace/Get_Opt.h"

IFR_DII_Client::IFR_DII_Client (void)
  : namespace_name (CORBA::string_dup ("warehouse")),
    interface_name (CORBA::string_dup ("inventory")),
    op_name (CORBA::string_dup ("getCDinfo")),
    lookup_by_name_ (false),
    debug_ (false),
    ior_output_file_(ACE_TEXT("file://iorfile"))
{
}

IFR_DII_Client::~IFR_DII_Client (void)
{
}

int
IFR_DII_Client::init (int argc,
                      ACE_TCHAR *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  if (this->parse_args (argc, argv) == -1)
  {
        return -1;
  }
  // In a reall application, we would get the scoped or
  // local name from the Interface Repository and use that
  // to get the object reference of the target via the Naming
  // Service. Since we're not testing the Naming Service here,
  // we just use the IOR which is stored in a file by the server.
  this->target_ = this->orb_->string_to_object (ior_output_file_);

  if (CORBA::is_nil (this->target_.in ()))
  {
     ACE_ERROR_RETURN ((
        LM_ERROR,
        "Unable to find interface repository in: %s\n", ior_output_file_),
        -1);
  }

  return 0;
}

int
IFR_DII_Client::run (void)
{
  int result = 0;

  if (this->lookup_by_name_)
    {
      result = this->lookup_interface_def ();

      if (result == -1)
        {
          return -1;
        }
    }
  else
    {
      result = this->find_interface_def ();

      if (result == -1)
        {
          return (-1);
        }
    }

  this->get_operation_def ();

  this->create_dii_request ();

  this->invoke_and_display ();

  return 0;
}

int
IFR_DII_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("k:nd"));
  int c;

  while ((c = opts ()) != -1)
    switch (c)
      {
        case 'd':
          this->debug_ = true;
          break;
        case 'n':   // Select lookup by name.
          this->lookup_by_name_ = true;
          break;
        case 'k':
          this->ior_output_file_ = opts.opt_arg ();
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "usage: %s"
                            " [-n]"
                            "\n",
                            argv [0]),
                            -1);
      }

  return 0;
}

int
IFR_DII_Client::find_interface_def (void)
{
  this->target_def_ =
    this->target_->_get_interface ();

  if (CORBA::is_nil (this->target_def_.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Unable to find interface def\n"),
                        -1);
    }

  return 0;
}

int
IFR_DII_Client::lookup_interface_def (void)
{
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("InterfaceRepository");

  this->repo_ = CORBA::Repository::_narrow (obj.in ());

  // Is there a contained object of some kind at any level in the
  // repository called "warehouse"?
  CORBA::ContainedSeq_var candidates =
    this->repo_->lookup_name (this->namespace_name.in (),
                              -1,            // Unlimited level recursion.
                              CORBA::dk_all, // Any type of contained object.
                              1);              // Exclude parents of interfaces.

  CORBA::ULong length = candidates->length ();
  CORBA::Container_var candidate;
  CORBA::ContainedSeq_var interfaces;
  CORBA::ULong n_interfaces = 0;
  CORBA::String_var name;

  // No point continuing; theres nothing to look at.
  if (length == 0)
  {
     return -1;
  }

  // The length is 1 in this case, but in general, it could
  // be any length.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      candidate =
        CORBA::Container::_narrow (candidates[i]);

      // Is this contained item itself a container?
      if (!CORBA::is_nil (candidate.in ()))
        {
          // Does this container contain any interfaces?
          interfaces = candidate->contents (CORBA::dk_Interface,
                                            1);     // Exclude parents.

          n_interfaces = interfaces->length ();

          // Here we are just getting out of the loop (which
          // only has length 1 anyway) when we see the first
          // container that contains at least one interface.
          // In a real application, we'd probably have a more
          // useful criterion,
          if (n_interfaces > 0)
            {
              break;
            }
        }
    }

  // The length is 1 in this case, but in general, it could
  // be any length.
  for (CORBA::ULong j = 0; j < n_interfaces  ; ++j)
    {
      name = interfaces[j]->name ();

      if (!ACE_OS::strcmp (name.in (), this->interface_name.in ()))
        {
          this->target_def_ =
            CORBA::InterfaceDef::_narrow (interfaces[j]);
        }
    }
  return 0;
}

void
IFR_DII_Client::get_operation_def (void)
{
  // What operation(s) does this interface contain?
  CORBA::ContainedSeq_var operations =
    this->target_def_->contents (CORBA::dk_Operation,
                                 0);  // Do not exclude inherited operations.

  CORBA::ULong n_operations = operations->length ();
  CORBA::String_var operation_name;

  // The length is 1 in this case, but in general, it could
  // be any length.
  for (CORBA::ULong i = 0; i < n_operations; ++i)
    {
      operation_name = operations[i]->name ();

      if (!ACE_OS::strcmp (operation_name.in (), this->op_name.in ()))
        {
          this->op_ =
            CORBA::OperationDef::_narrow (operations[i]);

          break;
        }
    }
}

void
IFR_DII_Client::create_dii_request (void)
{
  this->req_ = this->target_->_request (this->op_name.in ());

  this->result_ = this->op_->result ();

  this->req_->set_return_type (this->result_.in ());

  CORBA::ParDescriptionSeq_var params =
    this->op_->params ();

  CORBA::ULong length = params->length ();

  // This example of the discovery of parameter information is
  // purposely contrived for the sake of brevity. A real
  // application would have more versatile code here, and much
  // more of it.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      CORBA::TCKind const kind =
        params[i].type->kind ();

      switch (params[i].mode)
      {
        case CORBA::PARAM_IN:
          if (kind == CORBA::tk_string
              && ACE_OS::strcmp (params[i].name.in (), "artist") == 0)
            {
              // The servant will match the substring 'Beatles'.
              this->req_->add_in_arg (params[i].name.in ()) <<= "the Beatles";
            }

          break;
        case CORBA::PARAM_INOUT:
          if (kind == CORBA::tk_string
              && ACE_OS::strcmp (params[i].name.in (), "title") == 0)
            {
              // This isn't the exact title, but the servant will find the
              // partial match, and return the full, correct title.
              this->req_->add_inout_arg (params[i].name.in ()) <<= "Sgt. Pepper's";
            }

          break;
        case CORBA::PARAM_OUT:
          {
            if (kind == CORBA::tk_float
                && ACE_OS::strcmp (params[i].name.in (), "price") == 0)
              {
                CORBA::Float tmp = -1.0f;
                CORBA::Any any;
                any <<= tmp;

                // The servant will return 0.0 if the title is not found.
                this->req_->arguments ()->add_value (params[i].name.in (),
                                                     any,
                                                     CORBA::ARG_OUT);
              }

            break;
          }
      }
    }
}

void
IFR_DII_Client::invoke_and_display (void)
{
  this->req_->invoke ();

  CORBA::TypeCode_var tc = this->req_->return_value ().type ();

  CORBA::TCKind const kind = tc->kind ();

  if (kind == CORBA::tk_boolean)
    {
      CORBA::NVList_ptr args = this->req_->arguments ();

      const char *artist = 0;

      ACE_ASSERT ((*args->item (0)->value () >>= artist) == true);

      ACE_ASSERT (ACE_OS::strcmp (artist, "the Beatles") == 0);

      const char *title = 0;

      ACE_ASSERT ((*args->item (1)->value () >>= title) == true);

      const char *correct = "Sgt. Pepper's Lonely Hearts Club Band";
      ACE_ASSERT (ACE_OS::strcmp (title, correct) == 0);
      ACE_UNUSED_ARG (correct);

      CORBA::Float price = 0.0f;

      ACE_ASSERT ((*args->item (2)->value () >>= price) == true);

      ACE_ASSERT (price == 13.49f);

      if (this->debug_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("%C:\t%C\n")
                      ACE_TEXT ("%C:\t%C\n")
                      ACE_TEXT ("%C:\t$%2.2f\n"),
                      args->item (0)->name (),
                      artist,
                      args->item (1)->name (),
                      title,
                      args->item (2)->name (),
                      price));
        }

      CORBA::Boolean in_stock = 0;

      CORBA::Boolean ret_status =
        (this->req_->return_value () >>= CORBA::Any::to_boolean (in_stock));
      ACE_UNUSED_ARG (ret_status);

      ACE_ASSERT (ret_status == 1);
      ACE_ASSERT (in_stock == 1);

      if (this->debug_)
        {
          if (in_stock)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("status: in stock\n")));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("status: out of stock\n")));
            }
        }
    }
}
