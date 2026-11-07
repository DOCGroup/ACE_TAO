
//=============================================================================
/**
 *  @file     test.cpp
 *
 *   This program verifies that a POA using the NON_RETAIN policy creates
 *   unique system IDs, also that objects created with a USER id policy is
 *   not given a bo
 *
 *  @author  Phil Mesnier
 */
//=============================================================================

#include "testS.h"
#include "tao/PortableServer/POA_Current.h"

#include <cstddef>
#include <string>
#include <vector>

class ID_Check_i : public POA_ID_Check
{
public:
  ID_Check_i (std::size_t num, PortableServer::Current_ptr current);
  ~ID_Check_i () override = default;

  ID_Check_i (ID_Check_i const &) = delete;
  ID_Check_i &operator= (ID_Check_i const &) = delete;
  ID_Check_i (ID_Check_i &&) = delete;
  ID_Check_i &operator= (ID_Check_i &&) = delete;

  void set_id (std::size_t index, PortableServer::ObjectId_var oid);

  CORBA::Boolean check_servant_id (CORBA::Short index) override;

private:
  std::vector<PortableServer::ObjectId_var> oids_;
  PortableServer::Current_var current_;
};

ID_Check_i::ID_Check_i (std::size_t num, PortableServer::Current_ptr current)
  : oids_ (num),
    current_ (PortableServer::Current::_duplicate (current))
{
}

void
ID_Check_i::set_id (std::size_t index, PortableServer::ObjectId_var oid)
{
  this->oids_[index] = oid;
}

CORBA::Boolean
ID_Check_i::check_servant_id (CORBA::Short index)
{
  if (index < 0 || static_cast<std::size_t> (index) >= this->oids_.size ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("check_servant_id: requested index outside of range\n")));
      return false;
    }
  PortableServer::ObjectId_var const oid = this->current_->get_object_id ();
  std::size_t const object_index = static_cast<std::size_t> (index);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("check_servant_id: index = %d current objectid ")
              ACE_TEXT ("len %u octets: "),
              index, oid->length ()));
  for (std::size_t i = 0; i < oid->length () && i < 16; ++i)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%02x "),
                static_cast<unsigned int> (oid[i])));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  if (oid->length () != this->oids_[object_index]->length ())
    return false;
  for (std::size_t i = 0; i < oid->length (); ++i)
    {
      if (oid[i] != this->oids_[object_index][i])
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("check_servant_id: id[%d] mismatch at position %B, ")
                      ACE_TEXT ("got %d, expected %d\n"),
                      index, i, oid[i], this->oids_[object_index][i]));
          return false;
        }
    }
  return true;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      // Narrow to POA.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::Object_var const current_object =
        orb->resolve_initial_references ("POACurrent");
      PortableServer::Current_var const current =
        PortableServer::Current::_narrow (current_object.in ());

      // Policies for the new POA.
      CORBA::PolicyList policies (4);
      policies.length (3);

      // Request Processing Policy.
      policies[0] =
        root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT);

      // Id Uniqueness Policy.
      policies[1] =
        root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

      // Servant Retention Policy.
      policies[2] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN);

      // Create POA to host default servant.
      PortableServer::POA_var sys_id_poa =
        root_poa->create_POA ("System IDs",
                              poa_manager.in (),
                              policies);

      policies.length (4);
      policies[3] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var user_id_poa =
        root_poa->create_POA ("User IDs",
                              poa_manager.in (),
                              policies);

      // Destroy policies.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr const policy = policies[i];
          policy->destroy ();
        }

      std::size_t const objcount = 30;

      ID_Check_i servant (objcount, current.in ());
      sys_id_poa->set_servant (&servant);
      user_id_poa->set_servant (&servant);

      // Activate POA manager.
      poa_manager->activate ();

      // Create object array.
      std::vector<ID_Check_var> refs (objcount);
      PortableServer::ObjectId_var obj_id;

      for (std::size_t i = 0; i < objcount; ++i)
        {
          if (i < objcount / 2)
            {
              object = sys_id_poa->create_reference ("IDL:ID_Check:1.0");
              refs[i] = ID_Check::_narrow (object.in ());
              obj_id = sys_id_poa->reference_to_id (object.in());
              servant.set_id (i, obj_id);
            }
          else
            {
              std::string const user_id = "id " + std::to_string (i);
              obj_id = PortableServer::string_to_ObjectId (user_id.c_str ());
              object = user_id_poa->create_reference_with_id (obj_id.in(),
                                                              "IDL:ID_Check:1.0");
              refs[i] = ID_Check::_narrow (object.in ());
              servant.set_id (i, obj_id);
            }
        }

      std::size_t successes {};
      for (std::size_t i = 0; i < objcount; ++i)
        {
          successes += refs[i]->check_servant_id (static_cast<CORBA::Short> (i)) ? 1 : 0;
        }

      orb->destroy ();

      if (successes != objcount)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("FAILURE: only got %B ids correct out of %B\n"),
                      successes, objcount));
          return -1;
        }
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Success: All ids match\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
