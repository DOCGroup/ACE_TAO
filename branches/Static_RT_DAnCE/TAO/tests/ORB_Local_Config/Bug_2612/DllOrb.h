/*
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 * @author Lothar Werzinger <lothar@tradescape.biz>
 *
 * $Id$
 */

#ifndef DllOrb_h
#define DllOrb_h

#include "ace/Barrier.h"
#include "ace/Task.h"

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include "DllOrb_Export.h"

/** The CORBA ORB as a dynamic loadable module.
  *
  */
class DllOrb_Export DllOrb
:
  public ACE_Task_Base
{
  public:
    /// Default constructor.
    DllOrb (int nthreads = 1);

    /// Destructor.
    ~DllOrb (void) throw ();

    CORBA::ORB_ptr orb (void) const;

    virtual int init (int argc, char *argv[]);

    virtual int fini (void);

  // protected types and methods
  protected:
    virtual int svc (void);

  // private methods and instance variables
  private:
    int                            m_nthreads_;
    unsigned int                   m_failPrePostInit;

#if defined (ACE_HAS_THREADS)
    ACE_Thread_Barrier *           mp_barrier;
#endif

    CORBA::ORB_var                 mv_orb;
    PortableServer::POA_var        mv_rootPOA;
    PortableServer::POAManager_var mv_poaManager;
}; /* end of DllOrb */


ACE_FACTORY_DECLARE (DllOrb, DllOrb)


#endif /* DllOrb_h */
