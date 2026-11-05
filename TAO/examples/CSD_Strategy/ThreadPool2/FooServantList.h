#ifndef FOOSERVANTLIST_H
#define FOOSERVANTLIST_H

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ace/SString.h"
#include <atomic>
#include <vector>


class FooServantList
{
public:
  FooServantList (ACE_TCHAR const *prefix,
                  unsigned num_servants,
                  unsigned num_clients,
                  CORBA::ORB_ptr orb);
  ~FooServantList () = default;

  FooServantList (FooServantList const &) = delete;
  FooServantList (FooServantList &&) = delete;
  FooServantList & operator= (FooServantList const &) = delete;
  FooServantList & operator= (FooServantList &&) = delete;

  void create_and_activate (PortableServer::POA_ptr poa);

  void client_done ();

private:
  std::vector<PortableServer::ServantBase_var> servants_;
  ACE_TString prefix_;

  std::atomic<unsigned> num_clients_;

  CORBA::ORB_var  orb_;
};

#endif
