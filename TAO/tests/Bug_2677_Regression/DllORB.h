#ifndef DLLORB_H
#define DLLORB_H

#include "ace/Service_Object.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "bug_2677_regression_export.h"

class bug_2677_regression_Export DllORB: public ACE_Service_Object
{
public:
  DllORB ();
  virtual ~DllORB ();

  CORBA::ORB_ptr orb () const;

  virtual int init (int argc, ACE_TCHAR *argv[]);

  virtual int fini ();

private:
  unsigned int                   failPrePostInit_;
  CORBA::ORB_var                 mv_orb_;
  PortableServer::POA_var        mv_rootPOA_;
  PortableServer::POAManager_var mv_poaManager_;
};

ACE_FACTORY_DECLARE (bug_2677_regression, DllORB)

#endif /* DLLORB_H */
