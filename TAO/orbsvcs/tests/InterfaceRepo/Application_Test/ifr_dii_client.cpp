// -*- C++ -*-
// $Id$

#include "ifr_dii_client.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Application_Test, ifr_dii_client, "$Id$")

IFR_DII_Client::IFR_DII_Client (void)
  : namespace_name (CORBA::string_dup ("warehouse")),
    interface_name (CORBA::string_dup ("inventory")),
    op_name (CORBA::string_dup ("getCDinfo")),
    lookup_by_name_ (0)
{
}

IFR_DII_Client::~IFR_DII_Client (void)
{
}

int
IFR_DII_Client::init (int argc,
                      char *argv[]
                      TAO_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                0
                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // In a reall application, we would get the scoped or
  // local name from the Interface Repository and use that
  // to get the object reference of the target via the Naming
  // Service. Since we're not testing the Naming Service here,
  // we just use the IOR which is stored in a file by the server.
  this->target_ =
    this->orb_->string_to_object ("file://iorfile"
                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (this->parse_args (argc, argv) == -1)
    {
      return -1;
    }

  return 0;
}

int
IFR_DII_Client::run (TAO_ENV_SINGLE_ARG_DECL)
{
  if (this->lookup_by_name_)
    {
      this->lookup_interface_def (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  else
    {
      this->find_interface_def (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  this->get_operation_def (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->create_dii_request (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->invoke_and_display (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
IFR_DII_Client::parse_args (int argc,
                            char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "n");
  int c;

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'n':   // Select lookup by name.
        this->lookup_by_name_ = 1;
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

void
IFR_DII_Client::find_interface_def (TAO_ENV_SINGLE_ARG_DECL)
{
  this->target_def_ = 
    this->target_->_get_interface (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
IFR_DII_Client::lookup_interface_def (TAO_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("InterfaceRepository"
                                            TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->repo_ = CORBA::Repository::_narrow (obj.in ()
                                            TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Is there a contained object of some kind at any level in the
  // repository called "warehouse"?
  CORBA::ContainedSeq_var candidates =
    this->repo_->lookup_name (this->namespace_name.in (),
                              -1,               // Unlimited level recursion.
                              CORBA::dk_all,    // Any type of contained object.
                              1                 // Exclude parents of interfaces.
                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = candidates->length ();
  CORBA::Container_var candidate;
  CORBA::ContainedSeq_var interfaces;
  CORBA::ULong n_interfaces = 0;
  CORBA::String_var name;

  // The length is 1 in this case, but in general, it could
  // be any length.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      candidate =
        CORBA::Container::_narrow (candidates[i].in ()
                                   TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Is this contained item itself a container?
      if (!CORBA::is_nil (candidate.in ()))
        {
          // Does this container contain any interfaces?
          interfaces = candidate->contents (CORBA::dk_Interface,
                                            1     // Exclude parents.
                                            TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;

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
  for (CORBA::ULong j = 0; j < n_interfaces; ++j)
    {
      name = interfaces[j]->name (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (!ACE_OS::strcmp (name.in (), this->interface_name.in ()))
        {
          this->target_def_ =
            CORBA::InterfaceDef::_narrow (interfaces[j].in ()
                                          TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
IFR_DII_Client::get_operation_def (TAO_ENV_SINGLE_ARG_DECL)
{
  // What operation(s) does this interface contain?
  CORBA::ContainedSeq_var operations =
    this->target_def_->contents (CORBA::dk_Operation,
                                 0  // Do not exclude inherited operations.
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong n_operations = operations->length ();
  CORBA::String_var operation_name;

  // The length is 1 in this case, but in general, it could
  // be any length.
  for (CORBA::ULong i = 0; i < n_operations; ++i)
    {
      operation_name = operations[i]->name (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (!ACE_OS::strcmp (operation_name.in (), this->op_name.in ()))
        {
          this->op_ =
            CORBA::OperationDef::_narrow (operations[i].in ()
                                          TAO_ENV_ARG_PARAMETER);
          ACE_CHECK;

          break;
        }
    }
}

void
IFR_DII_Client::create_dii_request (TAO_ENV_SINGLE_ARG_DECL)
{
  this->req_ = this->target_->_request (this->op_name.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->result_ = this->op_->result (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->req_->set_return_type (this->result_.in ());

  CORBA::ParDescriptionSeq_var params = this->op_->params (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong length = params->length ();

  // This example of the discovery of parameter information is
  // purposely contrived for the sake of brevity. A real
  // application would have more versatile code here, and much
  // more of it.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      switch (params[i].mode)
      {
        case CORBA::PARAM_IN:
          if (params[i].type->kind () == CORBA::tk_string
              && ACE_OS::strcmp (params[i].name.in (), "artist") == 0)
            {
              // The servant will match the substring 'Beatles'.
              this->req_->add_in_arg (params[i].name.in ()) <<= "the Beatles";
            }

          break;
        case CORBA::PARAM_INOUT:
          if (params[i].type->kind () == CORBA::tk_string
              && ACE_OS::strcmp (params[i].name.in (), "title") == 0)
            {
              // This isn't the exact title, but the servant will find the
              // partial match, and return the full, correct title.
              this->req_->add_inout_arg (params[i].name.in ()) <<= "Sgt. Pepper's";
            }

          break;
        case CORBA::PARAM_OUT:
          {
            // It doesn't matter for basic types, like float, but for
            // cases where it does, this is an alternative method of
            // adding an OUT argument without initializing it.
            if (params[i].type->kind () == CORBA::tk_float
                && ACE_OS::strcmp (params[i].name.in (), "price") == 0)
              {
                CORBA::Any any (CORBA::_tc_float);

                // The servant will return 0.0 if the title is not found.
                this->req_->arguments ()->add_value (params[i].name.in (),
                                                     any,
                                                     CORBA::ARG_OUT
                                                     TAO_ENV_ARG_PARAMETER);
                ACE_CHECK;
              }

            break;
          }
      }
    }
}

void
IFR_DII_Client::invoke_and_display (TAO_ENV_SINGLE_ARG_DECL)
{
  this->req_->invoke (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var tc = this->req_->return_value ().type ();

  if (tc->kind () == CORBA::tk_boolean)
    {
      CORBA::NVList_ptr args = this->req_->arguments ();

      const char *artist = 0;
      *args->item (0)->value () >>= artist;

      const char *title = 0;
      *args->item (1)->value () >>= title;

      CORBA::Float price = 0.0f;
      *args->item (2)->value () >>= price;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s:\t%s\n")
                  ACE_TEXT ("%s:\t%s\n")
                  ACE_TEXT ("%s:\t$%2.2f\n"),
                  args->item (0)->name (),
                  artist,
                  args->item (1)->name (),
                  title,
                  args->item (2)->name (),
                  price));

      CORBA::Boolean in_stock = 0;

      this->req_->return_value () >>= CORBA::Any::to_boolean (in_stock);

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


