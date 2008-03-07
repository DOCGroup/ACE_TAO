// $Id$
#include "ApplicationActuator_exec.h"
#include "ciao/CIAO_common.h"
#include <sstream>

namespace CIAO
{
  namespace RACE
  {
    namespace Effector
    {
      namespace CIDL_EUCON_Effector_Impl
      {
        ApplicationActuator_exec_i::ApplicationActuator_exec_i
        (EUCON_Effector_Context *context,
         Logger &logger)
          : context_ (context),
            logger_ (logger)

        {
        }

        ApplicationActuator_exec_i::~ApplicationActuator_exec_i (void)
        {
        }

        ::CORBA::Boolean
        ApplicationActuator_exec_i::init
        (const ::CIAO::RACE::OperationalStrings & opstrings)
        {

          std::stringstream msg ;
          bool ret = false;
          msg << "Entering ApplicationActuator_exec_i::init ().\n";
          this->logger_.log (msg.str());
          msg.str ();

          // First clear the local callback_map.
          this->callback_map_.unbind_all ();

          if (this->resolve_naming_service ())
            {
              ret = this->populate_callback_map (opstrings);
            }
          msg << "Exiting ApplicationActuator_exec_i::init ().\n"
              << "Return value is " << ret <<std::endl;
          this->logger_.log (msg.str());
          return ret;
        }

        ::CORBA::Boolean
        ApplicationActuator_exec_i::modifyApplications (
          const ::CIAO::RACE::OperationalStrings & opstrings)
        {
          ACE_DEBUG ((LM_DEBUG, "In "
                      "ApplicationActuator_exec_i::modifyApplications ().\n"));
          //          return true;

          std::stringstream msg ;
          msg << "Entering "
              << "ApplicationActuator_exec_i::modifyApplications ().\n";

          for (::CORBA::ULong itr = 0; itr < opstrings.length (); ++itr)
            {
              msg << "Trying to modify rate of application: \n"
                  << opstrings[itr].ID.in()
                  << ".\n";

              msg << "Trying to retrieve the callback object from the "
                  << "internal data-structure...";
              this->logger_.log (msg.str ());
              msg.str ("");

              MAP::ENTRY *entry = 0;
              ACE_CString str (opstrings[itr].ID.in());
              if (this->callback_map_.find (str, entry) == 0)
                {
                  msg << "done!\nNow making invocations....";
                  try
                    {
                      bool ret =
                        entry->int_id_.in ()->set_rate
                        (opstrings[itr].rate.currRate);
                      msg << "done!\n";
                      this->logger_.log (msg.str ());
                      msg.str ("");
                    }
                  catch (::CORBA::Exception &ex)
                    {
                      ACE_PRINT_EXCEPTION (ex, "Exception caught!\n");
                      msg << "Exception caught:\n ";
                      msg << ex._info ().c_str();
                      this->logger_.log (msg.str ());
                      return false;
                    }

                }
              else
                {
                  msg << "Given ID is not present in the internal "
                      << "map!... bailing out.....\n";
                  return false;
                }
            }
          msg << "Exiting "
              << "ApplicationActuator_exec_i::modifyApplications ().\n";
          this->logger_.log (msg.str());
          return true;
        }


        bool
        ApplicationActuator_exec_i::resolve_naming_service ()
        {
          std::stringstream msg ;
          msg << "Entering ApplicationActuator_exec_i::"
              << "resolve_naming_service().\n";
          msg << " Trying to obtain the object "
              << "reference of the NamingService...";
          this->logger_.log (msg.str ());
          msg.str ("");
          try
            {
              ::CORBA::ORB_ptr orb =
                this->context_->_ciao_the_Container()->the_ORB();
              /// Trying to get the object reference from the Naming Service.
              CORBA::Object_var naming_obj =
                orb->resolve_initial_references ("NameService");
              if (CORBA::is_nil (naming_obj.in ()))
                {
                  msg << "Oops! Unable to resolve reference "
                      << "to the Name Service.\n";
                  this->logger_.log (msg.str());
                  return false;
                }
              else
                {
                  this->naming_context_ =
                    CosNaming::NamingContext::_narrow (naming_obj.in ());
                }
              msg << "done!\nExiting "
                  << "ApplicationActuator_exec_i::resolve_naming_service().\n";
              this->logger_.log (msg.str());
              return true;
            }
          catch (CORBA::Exception &ex)
            {
              msg << "Exception caught\n" << ex._info ().c_str();
              msg <<  "\nUnable to resolve reference to the naming service\n";
              msg << "Exiting ApplicationActuator_exec_i::"
                  << "resolve_naming_service ().\n";
              this->logger_.log (msg.str());
              return false;
            }
        }

        bool
        ApplicationActuator_exec_i::populate_callback_map
        (const ::CIAO::RACE::OperationalStrings & opstrings)
        {
          std::stringstream msg ;
          msg << "Entering "
              << "ApplicationActuator_exec_i::populate_callback_map ().\n";

          msg << "Trying to resolve the callback objects.\n";
          for (::CORBA::ULong itr = 0; itr < opstrings.length (); ++itr)
            {
              msg << "Working on "
                  << opstrings[itr].ID.in()
                  << ".\n";
              this->logger_.log (msg.str ());
              msg.str ("");
              try
                {
                  CosNaming::Name name (1);
                  name.length (1);
                  name[0].id =
                    CORBA::string_dup (opstrings[itr].effectorID.in ());
                  CORBA::Object_var component =
                    this->naming_context_->resolve (name);
                  Callback_var callback =
                    Callback::_narrow (component.in ());

                  if (CORBA::is_nil (callback.in ()))
                    {
                      msg << "Oops! Couldn't obtain the "
                          << "reference  to the callback object\n";
                      this->logger_.log (msg.str ());
                      ACE_ERROR ((LM_ERROR, "Oops! Couldn't obtain the "
                                  "reference  to the callback object for "
                                  "component %s.\n", opstrings[itr].ID.in()));
                      return false;
                    }
                  else
                    {
                      msg << "Successfully obtained the reference to the "
                          << "callback object.\n";
                      // Now inserting the objref into the internal hashtable.
                      ACE_CString str (opstrings[itr].ID.in());
                      if (this->callback_map_.rebind (str, callback) < 0)
                        {
                          msg << "Error while inserting it into the map!\n";
                          this->logger_.log (msg.str ());
                          return false;
                        }
                    }
                }
              catch (::CORBA::Exception &ex)
                {
                ACE_PRINT_EXCEPTION (ex, "Exception caught!\n");
                msg << "Exception caught:\n ";
                msg << ex._info ().c_str();
                this->logger_.log (msg.str ());
                return false;
              }
            }
          msg << "Exiting "
              << "ApplicationActuator_exec_i::populate_callback_map ().\n";
          this->logger_.log (msg.str());
          return true;
        }
      }
    }
  }
}
