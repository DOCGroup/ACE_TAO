// $Id$
#ifndef DYNSERVER_H
#define DYNSERVER_H

#include "dynserver_export.h"

#include "testS.h"

#include "tao/Object_Loader.h"

#include "ace/Auto_Ptr.h"

// Trivial test corba object
class DynServer_Export DynServer
  : public POA_test
{
  int n_;
public:
  DynServer(void);
  virtual ~DynServer();
  virtual CORBA::Long get();
};

class DynServer_ORB_Runner;

// This dll supports the service configurator framework
class DynServer_Export DynServer_Loader : public TAO_Object_Loader
{
public:
  DynServer_Loader(void);

  // spawns a thread to run an internal orb which has activated
  // a single DynServer servant.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  // Allows the service configurator to shutdown the orb
  virtual int fini (void);

  // Not supported
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv[]);

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  DynServer service_;
  ACE_Auto_Ptr<DynServer_ORB_Runner> runner_;

private:
  ACE_UNIMPLEMENTED_FUNC (DynServer_Loader (const DynServer_Loader &))
  ACE_UNIMPLEMENTED_FUNC (DynServer_Loader &operator = (const DynServer_Loader &))
};

ACE_FACTORY_DECLARE (DynServer, DynServer_Loader)

#endif
