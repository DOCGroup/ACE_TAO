// $Id$

#ifndef PersistentPoa_h
#define PersistentPoa_h

#include "ace/Service_Object.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "bug2936_export.h"


class bug2936_Export PersistentPoa
:
  public ACE_Service_Object
{
  // public types and methods
  public:
    /// Default constructor.
    PersistentPoa ( );

    /// Destructor.
    ~PersistentPoa ( )
      throw ();

  // protected types and methods
  protected:
    virtual int init (int argc, char *argv[]);

    virtual int fini (void);

  // private methods and instance variables
  private:
    CORBA::ORB_var                 mv_orb;
    PortableServer::POA_var        mv_rootPOA;
    PortableServer::POA_var        mv_thisPOA;
    PortableServer::POAManager_var mv_poaManager;
}; /* end of class PersistentPoa */


ACE_FACTORY_DECLARE (bug2936, PersistentPoa)


#endif /* PersistentPoa_h */
