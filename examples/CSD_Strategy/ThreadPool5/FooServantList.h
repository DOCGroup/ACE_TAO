// $Id$
#ifndef FOOSERVANTLIST_H
#define FOOSERVANTLIST_H

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ace/SString.h"


class ClientTask;
class Foo_i;

class FooServantList
{
  public:

    FooServantList(const ACE_TCHAR* prefix,
                   unsigned num_servants,
                   unsigned num_clients,
                   int      collocated_test,
                   int      servant_to_deactivate,
                   CORBA::ORB_ptr orb);
    ~FooServantList();

    void create_and_activate(CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa);

    void client_done(void);

    ClientTask* collocated_client () const;

    void deactivate_servant (void);

  private:
    Foo_i** servants_;
    PortableServer::ServantBase_var* safe_servants_;
    ACE_TString prefix_;
    unsigned num_servants_;

    TAO_SYNCH_MUTEX num_clients_lock_;
    unsigned num_clients_;
    unsigned init_num_clients_;
    int      collocated_test_;
    // The servant_to_be_deactivate_ should be an integer greater than -2.
    // -1  => defatult value, DONT deactivate servant.
    // 0   => deactivate all servants.
    // > 0 => the index of the servant to be deactivated.
    int      servant_to_deactivate_;

    ClientTask* collocated_client_;

    /// The poa to activate and deactivate servant.
    PortableServer::POA_var poa_;

    CORBA::ORB_var  orb_;
};

#endif
