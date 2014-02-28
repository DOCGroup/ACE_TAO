
//=============================================================================
/**
 *  @file     test.cpp
 *
 *  $Id$
 *
 *   This program verifies that a POA using the NON_RETAIN policy creates
 *   unique system IDs, also that objects created with a USER id policy is
 *   not given a bo
 *
 *  @author  Phil Mesnier
 */
//=============================================================================


#include "testS.h"
#include "ace/SString.h"
#include "tao/PortableServer/POA_Current.h"

class ID_Check_i : public POA_ID_Check
{
public:
  ID_Check_i (int num);
  ~ID_Check_i (void);

  void set_id (int index, PortableServer::ObjectId_var oid);

  CORBA::Boolean check_servant_id (CORBA::Short index);

private:
  int count_;
  PortableServer::ObjectId_var * oids_;
  PortableServer::Current_var current_;
};

ID_Check_i::ID_Check_i (int num)
  : count_ (num),
    oids_ (0),
    current_ ()
{
  int argc = 0;
  char **argv = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  CORBA::Object_var obj = orb->resolve_initial_references ("POACurrent");
  current_ = PortableServer::Current::_narrow (obj.in());

  oids_ = new PortableServer::ObjectId_var[num];
}

ID_Check_i::~ID_Check_i (void)
{
  delete [] oids_;
}

void
ID_Check_i::set_id (int index, PortableServer::ObjectId_var oid)
{
  this->oids_[index] = oid;
}

CORBA::Boolean
ID_Check_i::check_servant_id (CORBA::Short index)
{
  if (index < 0 || index >= this->count_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("check_servant_id: requested index outside of range\n")));
      return false;
    }
  PortableServer::ObjectId_var oid = this->current_->get_object_id();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("check_servant_id: index = %d current objectid ")
              ACE_TEXT ("len %d octets: "),
              index, oid->length()));
  for (size_t i = 0; i < oid->length () && i < 16; i++)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("%02x "), oid[i]));
  ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("\n")));

  if (oid->length() != this->oids_[index]->length())
    return false;
  for (size_t i = 0; i < oid->length(); i++)
    {
      if (oid[i] != this->oids_[index][i])
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("check_servant_id: id[%d] mismatch at position %d, ")
                      ACE_TEXT ("got %d, expected %d\n"),
                      index, i, oid[i], this->oids_[index][i]));
          return false;
        }
    }
  return true;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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
      ACE_CString name = "System IDs";
      PortableServer::POA_var sys_id_poa =
        root_poa->create_POA (name.c_str (),
                              poa_manager.in (),
                              policies);

      policies.length (4);
      policies[3] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      name = "User IDs";
      PortableServer::POA_var user_id_poa =
        root_poa->create_POA (name.c_str (),
                              poa_manager.in (),
                              policies);

      // Destroy policies.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      int objcount = 30;

      ID_Check_i *servant = new ID_Check_i (objcount);
      sys_id_poa->set_servant (servant);
      user_id_poa->set_servant (servant);

      // Activate POA manager.
      poa_manager->activate ();

      // Create object array.
      ID_Check_var *refs = new ID_Check_var[objcount];
      PortableServer::ObjectId_var obj_id;
      char *user_id_str = new char[10];

      for (int i = 0; i < objcount; i++)
        {
          if (i < objcount / 2)
            {
              object = sys_id_poa->create_reference ("IDL:ID_Check:1.0");
              refs[i] = ID_Check::_narrow (object.in());
              obj_id = sys_id_poa->reference_to_id (object.in());
              servant->set_id (i, obj_id);
            }
          else
            {
              ACE_OS::sprintf (user_id_str, "id %d", i);
              obj_id = PortableServer::string_to_ObjectId (user_id_str);
              object = user_id_poa->create_reference_with_id (obj_id.in(),
                                                              "IDL:ID_Check:1.0");
              refs[i] = ID_Check::_narrow (object.in());
              servant->set_id (i, obj_id);
            }
        }

      int successes = 0;
      for (int i = 0; i < objcount; i++)
        {
          successes += refs[i]->check_servant_id (i) ? 1 : 0;
        }

      orb->destroy ();

      if (successes != objcount)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("FAILURE: only got %d ids correct out of %d\n"),
                      successes, objcount));
          return -1;
        }
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Success: All ids match\n")));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
