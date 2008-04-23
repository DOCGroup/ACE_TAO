// $Id$
#ifndef FOOSERVANTLIST_H
#define FOOSERVANTLIST_H

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ace/SString.h"


class FooServantList
{
  public:

    FooServantList(const char* prefix,
                   unsigned num_servants,
                   unsigned num_clients,
                   CORBA::ORB_ptr orb);
    ~FooServantList();

    void create_and_activate(PortableServer::POA_ptr poa);

    void client_done(void);


  private:

    typedef TAO_SYNCH_MUTEX     LockType;
    typedef ACE_Guard<LockType> GuardType;

    PortableServer::ServantBase_var* servants_;
    ACE_CString prefix_;
    unsigned num_servants_;

    LockType num_clients_lock_;
    unsigned num_clients_;

    CORBA::ORB_var  orb_;
};

#endif
