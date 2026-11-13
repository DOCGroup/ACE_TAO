#include "dynserver.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"

#include "ace/Dynamic_Service.h"
#include "ace/Task.h"

using namespace CORBA;
using namespace PortableServer;

Long DynServer::get ()
{
  CORBA::Long const result = ++n_;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("dynserver: get() %d\n"), result));
  return result;
}

namespace {
  POA_ptr create_persistent_poa (char const *name,
                                 POA_ptr root_poa,
                                 POAManager_ptr poaman)
  {
    PolicyList policies (2);
    policies.length (2);
    policies[0] = root_poa->create_id_assignment_policy(USER_ID);
    policies[1] = root_poa->create_lifespan_policy(PERSISTENT);
    POA_var poa = root_poa->create_POA(name, poaman, policies);
    policies[0]->destroy();
    policies[1]->destroy();
    return poa._retn();
  }
}

class DynServer_ORB_Runner : public ACE_Task_Base
{
  ORB_var orb_;
public:
  explicit DynServer_ORB_Runner (ORB_ptr orb)
    : orb_ (ORB::_duplicate (orb))
  {
  }

  DynServer_ORB_Runner (DynServer_ORB_Runner const &) = delete;
  DynServer_ORB_Runner &operator= (DynServer_ORB_Runner const &) = delete;
  DynServer_ORB_Runner (DynServer_ORB_Runner &&) = delete;
  DynServer_ORB_Runner &operator= (DynServer_ORB_Runner &&) = delete;

  void end ()
  {
    if (!is_nil (orb_.in ()))
      {
        orb_->shutdown (true);
        this->wait ();
      }
  }

  int svc () override
  {
    orb_->run ();
    return 0;
  }
};

int
DynServer_Loader::init (int argc, ACE_TCHAR* argv[])
{
  try {
    orb_ = ORB_init (argc, argv, "DynServer");

    Object_var obj = orb_->resolve_initial_references ("RootPOA");
    root_poa_ = POA::_narrow (obj.in ());
    POAManager_var poaman = root_poa_->the_POAManager ();
    obj = this->orb_->resolve_initial_references ("IORTable");
    IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in ());
    ACE_ASSERT (!is_nil (ior_table.in ()));

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("dynserver: creating poas. (Registers with ImR)\n")));

    POA_var poa1 = create_persistent_poa ("DynObject1", root_poa_.in (), poaman.in ());
    POA_var poa2 = create_persistent_poa ("DynObject2", root_poa_.in (), poaman.in ());

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("dynserver: activating objects.\n")));

    DynServer *svt1 = new DynServer;
    ServantBase_var scoped_svt1 (svt1);
    DynServer *svt2 = new DynServer;
    ServantBase_var scoped_svt2 (svt2);

    ObjectId_var id = string_to_ObjectId ("myobject");

    poa1->activate_object_with_id (id.in (), svt1);
    poa2->activate_object_with_id (id.in (), svt2);

    TAO_Root_POA * const poa1_impl = dynamic_cast<TAO_Root_POA *> (poa1.in ());
    obj = poa1_impl->id_to_reference_i (id.in (), false);
    String_var ior = orb_->object_to_string (obj.in ());
    ior_table->bind ("DynObject1", ior.in ());

    TAO_Root_POA * const poa2_impl = dynamic_cast<TAO_Root_POA *> (poa2.in ());
    obj = poa2_impl->id_to_reference_i (id.in (), false);
    ior = orb_->object_to_string (obj.in ());
    ior_table->bind ("DynObject2", ior.in ());

    poaman->activate ();

    runner_ = std::make_unique<DynServer_ORB_Runner> (orb_.in ());
    runner_->activate ();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("dynserver: running.\n")));
  } catch (Exception const &e) {
    e._tao_print_exception ("DynServer::init()");
  }
  return 0;
}

int
DynServer_Loader::fini ()
{
  ACE_ASSERT (runner_ != nullptr);
  try {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("dynserver: shutting down.\n")));

    runner_->end ();
    runner_.reset ();

    root_poa_->destroy (1, 1);
    orb_->destroy ();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("dynserver: shut down successfully.\n")));

    return 0;
  } catch (Exception const &e) {
    e._tao_print_exception ("DynServer::fini()");
  }
  return -1;
}

Object_ptr
DynServer_Loader::create_object (ORB_ptr,
                                 int,
                                 ACE_TCHAR **)
{
  throw NO_IMPLEMENT ();
}

ACE_FACTORY_DEFINE (DynServer, DynServer_Loader)

