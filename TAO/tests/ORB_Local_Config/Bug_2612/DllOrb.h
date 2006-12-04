/*
 * Copyright (c) 2005 Tradescape Inc.  All rights reserved.
 *
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 * @author Lothar Werzinger <lothar@tradescape.biz>
 */

#ifndef tradescape_utility_DllOrb_h
#define tradescape_utility_DllOrb_h

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
  // public types and methods
  public:
    /// Default constructor.
    DllOrb ( );

    /// Destructor.
    ~DllOrb ( )
      throw ();

    CORBA::ORB_ptr orb ( ) const;

    virtual int init (int argc, char *argv[]);

    virtual int fini (void);

  // protected types and methods
  protected:
    virtual int svc (void);

  // private methods and instance variables
  private:
    //    static log4cplus::Logger       ms_logger;

    unsigned int                   m_failPrePostInit;
    ACE_Thread_Barrier *           mp_barrier;
    CORBA::ORB_var                 mv_orb;
    PortableServer::POA_var        mv_rootPOA;
    PortableServer::POAManager_var mv_poaManager;
}; /* end of DllOrb */


ACE_FACTORY_DECLARE (DllOrb, DllOrb)


#endif /* tradescape_utility_DllOrb_h */
