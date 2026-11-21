#ifndef FOOSERVANTLIST_H
#define FOOSERVANTLIST_H

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ace/SString.h"
#include <memory>
#include <vector>

class ClientTask;
class Foo_i;

class FooServantList
{
  public:
    FooServantList(ACE_TCHAR const *prefix,
                   unsigned num_servants,
                   unsigned num_clients,
                   bool     collocated_test,
                   int      servant_to_deactivate,
                   CORBA::ORB_ptr orb);
    ~FooServantList();

    FooServantList (FooServantList const &) = delete;
    FooServantList (FooServantList &&) = delete;
    FooServantList &operator= (FooServantList const &) = delete;
    FooServantList &operator= (FooServantList &&) = delete;

    void create_and_activate (PortableServer::POA_ptr poa);

    void client_done();

    ClientTask *collocated_client () const;

    void deactivate_servant ();

  private:
    std::vector<PortableServer::ServantBase_var> safe_servants_;
    ACE_TString prefix_;
    unsigned num_servants_;

    TAO_SYNCH_MUTEX num_clients_lock_;
    unsigned num_clients_;
    unsigned init_num_clients_;
    bool     collocated_test_;
    /// The value of servant_to_deactivate_ should be greater than -2.
    /// -1 means the default: do not deactivate a servant.
    /// 0 means deactivate all servants.
    /// > 0 is the index of the servant to deactivate.
    int      servant_to_deactivate_;

    std::unique_ptr<ClientTask> collocated_client_;

    /// The poa to activate and deactivate servant.
    PortableServer::POA_var poa_;

    CORBA::ORB_var  orb_;
};

#endif
