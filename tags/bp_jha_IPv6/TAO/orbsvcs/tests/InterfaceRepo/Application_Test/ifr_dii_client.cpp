// -*- C++ -*-
// $Id$

#include "ifr_dii_client.h"

ACE_RCSID(Application_Test, ifr_dii_client, "$Id$")

IFR_DII_Client::IFR_DII_Client (void)
  : namespace_name (CORBA::string_dup ("warehouse")),
    interface_name (CORBA::string_dup ("inventory")),
    op_name (CORBA::string_dup ("getCDinfo"))
{
}

IFR_DII_Client::~IFR_DII_Client (void)
{
}

int 
IFR_DII_Client::init (int argc,
                      char *argv[],
                      CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc, 
                                argv, 
                                0, 
                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("InterfaceRepository",
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->repo_ = IR_Repository::_narrow (obj.in (),
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // In a reall application, we would get the scoped or
  // local name from the Interface Repository and use that
  // to get the object reference of the target via the Naming 
  // Service. Since we're not testing the Naming Service here,
  // we just use the IOR which is stored in a file by the server.
  this->target_ = 
    this->orb_->string_to_object ("file://iorfile",
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int 
IFR_DII_Client::run (CORBA::Environment &ACE_TRY_ENV)
{
  this->find_interface_def (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->get_operation_def (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->create_dii_request (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->invoke_and_display (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

void 
IFR_DII_Client::find_interface_def (CORBA::Environment &ACE_TRY_ENV)
{
  // Is there a contained object of some kind at any level in the
  // repository called "warehouse"?
  IR_ContainedSeq_var candidates =
    this->repo_->lookup_name (this->namespace_name.in (),
                              -1,       // Unlimited level recursion.
                              dk_all,   // Any type of contained object.
                              1,        // Exclude parents of interfaces.
                              ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::ULong length = candidates->length ();
  IR_Container_var candidate;
  IR_ContainedSeq_var interfaces;
  CORBA::ULong n_interfaces = 0;
  CORBA::String_var name;

  // The length is 1 in this case, but in general, it could
  // be any length.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      candidate =
        IR_Container::_narrow (candidates[i],
                               ACE_TRY_ENV);
      ACE_CHECK;

      // Is this contained item itself a container?
      if (!CORBA::is_nil (candidate.in ()))
        {
          // Does this container contain any interfaces?
          interfaces = candidate->contents (dk_Interface,
                                            1,    // Exclude parents.
                                            ACE_TRY_ENV);
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
      name = interfaces[j]->name (ACE_TRY_ENV);
      ACE_CHECK;

      if (!ACE_OS::strcmp (name.in (), this->interface_name.in ()))
        {
          this->target_def_ = 
            IR_InterfaceDef::_narrow (interfaces[j].in (),
                                      ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
}

void 
IFR_DII_Client::get_operation_def (CORBA::Environment &ACE_TRY_ENV)
{
  // What operation(s) does this interface contain?
  IR_ContainedSeq_var operations = 
    this->target_def_->contents (dk_Operation,
                                 1,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::ULong n_operations = operations->length ();

  // The length is 1 in this case, but in general, it could
  // be any length.
  for (CORBA::ULong i = 0; i < n_operations; ++i)
    {
      op_name = operations[i]->name (ACE_TRY_ENV);
      ACE_CHECK;

      if (!ACE_OS::strcmp (op_name.in (), this->op_name.in ()))
        {
          this->op_ =
            IR_OperationDef::_narrow (operations[i],
                                      ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
}

void 
IFR_DII_Client::create_dii_request (CORBA::Environment &ACE_TRY_ENV)
{
#if (TAO_HAS_MINIMUM_CORBA == 0)

  this->req_ = this->target_->_request (this->op_name.in (),
                                        ACE_TRY_ENV);
  ACE_CHECK;

  this->result_ = this->op_->result (ACE_TRY_ENV);
  ACE_CHECK;

  this->req_->set_return_type (this->result_.in ());

  IR_ParDescriptionSeq_var params = this->op_->params (ACE_TRY_ENV);
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
        case PARAM_IN:
          if (params[i].type->kind () == CORBA::tk_string
              && ACE_OS::strcmp (params[i].name, "artist") == 0)
            {
              // The servant will match the substring 'Beatles'.
              this->req_->add_in_arg (params[i].name) <<= "the Beatles";
            }

          break;
        case PARAM_INOUT:
          if (params[i].type->kind () == CORBA::tk_string
              && ACE_OS::strcmp (params[i].name, "title") == 0)
            {
              // This isn't the exact title, but the servant will find the
              // partial match, and return the full, correct title.
              this->req_->add_inout_arg (params[i].name) <<= "Sgt. Pepper's";
            }

          break;
        case PARAM_OUT:
          {
            // It doesn't matter for basic types, like float, but for
            // cases where it does, this is an alternative method of 
            // adding an OUT argument without initializing it.
            if (params[i].type->kind () == CORBA::tk_float
                && ACE_OS::strcmp (params[i].name, "price") == 0)
              {
                CORBA::Any any (CORBA::_tc_float);

                // The servant will return 0.0 if the title is not found.
                this->req_->arguments ()->add_value (params[i].name,
                                                     any,
                                                     CORBA::ARG_OUT,
                                                     ACE_TRY_ENV);
                ACE_CHECK;
              }

            break;
          }
      }
    }

#else

  ACE_UNUSED_ARG (ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
}

void 
IFR_DII_Client::invoke_and_display (CORBA::Environment &ACE_TRY_ENV)
{
#if (TAO_HAS_MINIMUM_CORBA == 0)

  this->req_->invoke (ACE_TRY_ENV);
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

#else

  ACE_UNUSED_ARG (ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
}


