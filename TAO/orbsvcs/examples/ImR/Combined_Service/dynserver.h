#ifndef DYNSERVER_H
#define DYNSERVER_H

#include "dynserver_export.h"

#include "testS.h"

#include "tao/Object_Loader.h"

#include <memory>

// Trivial test corba object
class DynServer_Export DynServer
  : public POA_test
{
  int n_;
public:
  DynServer();
  virtual ~DynServer();
  virtual CORBA::Long get();
};

class DynServer_ORB_Runner;

// This dll supports the service configurator framework
class DynServer_Export DynServer_Loader : public TAO_Object_Loader
{
public:
  DynServer_Loader();

  // spawns a thread to run an internal orb which has activated
  // a single DynServer servant.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  // Allows the service configurator to shutdown the orb
  virtual int fini ();

  // Not supported
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv[]);

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  DynServer service_;
  std::unique_ptr<DynServer_ORB_Runner> runner_;

private:
  DynServer_Loader (const DynServer_Loader &) = delete;
  DynServer_Loader &operator = (const DynServer_Loader &) = delete;
};

ACE_FACTORY_DECLARE (DynServer, DynServer_Loader)

#endif
