// $Id$

#ifndef bug_3251_PersistentPoa_h
#define bug_3251_PersistentPoa_h

#include <string>

#include "ace/Service_Object.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "DllOrb.h"
#include "bug_3251_export.h"


class bug_3251_Export PersistentPoa:  public ACE_Service_Object
{
  // public types and methods
  public:
    PersistentPoa ( );

    ~PersistentPoa ( )
      throw ();

  // protected types and methods
  protected:
    virtual int init (int argc, char *argv[]);

    virtual int fini (void);

  // private methods and instance variables
  private:
    std::string                    m_poaName;
    CORBA::ORB_var                 mv_orb;
    PortableServer::POA_var        mv_rootPOA;
    PortableServer::POA_var        mv_thisPOA;
    PortableServer::POAManager_var mv_poaManager;
}; /* end of class PersistentPoa */


ACE_FACTORY_DECLARE (bug_3251, PersistentPoa)


#endif /* bug_3251_PersistentPoa_h */
