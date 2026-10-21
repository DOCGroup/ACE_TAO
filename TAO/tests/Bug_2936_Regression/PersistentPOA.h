#ifndef PersistentPoa_h
#define PersistentPoa_h

#include "ace/Service_Object.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "bug2936_export.h"


class bug2936_Export PersistentPoa : public ACE_Service_Object
{
public:
  /// Default constructor.
  PersistentPoa () = default;

  /// Destructor.
  ~PersistentPoa () noexcept override = default;

protected:
  int init (int argc, ACE_TCHAR *argv[]) override;

  int fini () override;

private:
  CORBA::ORB_var                 mv_orb;
  PortableServer::POA_var        mv_rootPOA;
  PortableServer::POA_var        mv_thisPOA;
  PortableServer::POAManager_var mv_poaManager;
}; /* end of class PersistentPoa */


ACE_FACTORY_DECLARE (bug2936, PersistentPoa)


#endif /* PersistentPoa_h */
