// $Id$
#include "dynserver.h"

#include "tao/IORTable/IORTable.h"
#include "tao/SystemException.h"

#include "ace/Dynamic_Service.h"
#include "ace/Task.h"

DynServer::DynServer()
: n_(0)
{
}

DynServer::~DynServer() {
}

CORBA::Long DynServer::get() ACE_THROW_SPEC ((CORBA::SystemException)) {
  ACE_DEBUG((LM_DEBUG, "dynserver: get() %d\n", ++n_));
  return n_;
}

namespace {
  PortableServer::POA_ptr createPersistPOA(const char* name, PortableServer::POA_ptr root_poa, PortableServer::POAManager_ptr poaman) {
    CORBA::PolicyList policies (2);
    policies.length (2);
    policies[0] = root_poa->create_id_assignment_policy(PortableServer::USER_ID);
    policies[1] = root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
    PortableServer::POA_var poa = root_poa->create_POA(name, poaman, policies);
    policies[0]->destroy();
    policies[1]->destroy();
    return poa._retn();
  }
}

class DynServer_ORB_Runner : public ACE_Task_Base
{
  CORBA::ORB_var orb_;
public:
  DynServer_ORB_Runner(CORBA::ORB_ptr orb)
    : orb_(CORBA::ORB::_duplicate(orb))
  {
  }
  void end() {
    if (! CORBA::is_nil(orb_.in())) {
      orb_->shutdown(1);
      wait();
    }
  }
  virtual int svc()
  {
    orb_->run();
    orb_ = CORBA::ORB::_nil();
    return 0;
  }
};

DynServer_Loader::DynServer_Loader(void)
{
}

int
DynServer_Loader::init (int argc, ACE_TCHAR* argv[] ACE_ENV_ARG_DECL)
{
  try {

    orb_ = CORBA::ORB_init(argc, argv, "DynServer");

    CORBA::Object_var obj = orb_->resolve_initial_references("RootPOA");
    root_poa_ = PortableServer::POA::_narrow(obj.in());
    PortableServer::POAManager_var poaman = root_poa_->the_POAManager();
    obj = this->orb_->resolve_initial_references ("IORTable");
    IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in());
    ACE_ASSERT(! is_nil(ior_table.in()));

    ACE_DEBUG((LM_DEBUG, "dynserver: creating poas. (Registers with ImR)\n"));

    PortableServer::POA_var poa1 = createPersistPOA("DynObject1", root_poa_.in(), poaman.in());
    PortableServer::POA_var poa2 = createPersistPOA("DynObject2", root_poa_.in(), poaman.in());

    ACE_DEBUG((LM_DEBUG, "dynserver: activating objects.\n"));

    DynServer* svt1 = new DynServer;
    PortableServer::ServantBase_var scoped_svt1(svt1);
    DynServer* svt2 = new DynServer;
    PortableServer::ServantBase_var scoped_svt2(svt2);

    PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId("myobject");

    poa1->activate_object_with_id(id.in(), svt1);
    poa2->activate_object_with_id(id.in(), svt2);

    obj = poa1->id_to_reference(id.in());
    CORBA::String_var ior = orb_->object_to_string(obj.in());
    ior_table->bind ("DynObject1", ior.in());
    obj = poa2->id_to_reference(id.in());
    ior = orb_->object_to_string(obj.in());
    ior_table->bind ("DynObject2", ior.in());

    poaman->activate();

    runner_.reset(new DynServer_ORB_Runner(orb_.in()));
    runner_->activate();

    ACE_DEBUG((LM_DEBUG, "dynserver: running.\n"));

  } catch (CORBA::Exception& e) {
    ACE_PRINT_EXCEPTION(e, "DynServer::init()");
  }
  return 0;
}

int
DynServer_Loader::fini (void)
{
  ACE_ASSERT(runner_.get() != 0);
  try {

    ACE_DEBUG((LM_DEBUG, "dynserver: shutting down.\n"));

    runner_->end();
    runner_.reset(0);

    root_poa_->destroy(1, 1);
    orb_->destroy();

    ACE_DEBUG((LM_DEBUG, "dynserver: shut down successfully.\n"));

    return 0;

  } catch (CORBA::Exception& e) {
    ACE_PRINT_EXCEPTION(e, "DynServer::fini()");
  }
  return -1;
}

CORBA::Object_ptr
DynServer_Loader::create_object (CORBA::ORB_ptr,
                                 int,
                                 ACE_TCHAR **
                                 ACE_ENV_ARG_DECL)
                                 ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN(CORBA::NO_IMPLEMENT(), CORBA::Object::_nil());
}

ACE_FACTORY_DEFINE (DynServer, DynServer_Loader)

