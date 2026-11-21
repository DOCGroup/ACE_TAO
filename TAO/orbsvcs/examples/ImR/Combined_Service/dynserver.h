#ifndef DYNSERVER_H
#define DYNSERVER_H

#include "dynserver_export.h"

#include "testS.h"

#include "tao/Object_Loader.h"

#include <atomic>
#include <memory>

// Trivial test corba object
class DynServer_Export DynServer : public POA_test
{
public:
  DynServer () = default;
  ~DynServer () override = default;

  DynServer (DynServer const &) = delete;
  DynServer &operator= (DynServer const &) = delete;
  DynServer (DynServer &&) = delete;
  DynServer &operator= (DynServer &&) = delete;

  CORBA::Long get () override;
private:
  std::atomic<CORBA::Long> n_ {CORBA::Long {}};
};

class DynServer_ORB_Runner;

// This dll supports the service configurator framework
class DynServer_Export DynServer_Loader : public TAO_Object_Loader
{
public:
  DynServer_Loader () = default;
  ~DynServer_Loader () override = default;

  DynServer_Loader (DynServer_Loader const &) = delete;
  DynServer_Loader &operator= (DynServer_Loader const &) = delete;
  DynServer_Loader (DynServer_Loader &&) = delete;
  DynServer_Loader &operator= (DynServer_Loader &&) = delete;

  // spawns a thread to run an internal orb which has activated
  // a single DynServer servant.
  int init (int argc, ACE_TCHAR *argv[]) override;

  // Allows the service configurator to shutdown the orb
  int fini () override;

  // Not supported
  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc,
                                   ACE_TCHAR *argv[]) override;

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  std::unique_ptr<DynServer_ORB_Runner> runner_;
};

ACE_FACTORY_DECLARE (DynServer, DynServer_Loader)

#endif
