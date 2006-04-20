//$Id$

//===================================================================
/**
 * @file TSEC_CheckPoint_exec.h
 *
 * Header file for the Executor implementation.
 */
//===================================================================

#ifndef TSEC_CheckPoint_EXEC_H
#define TSEC_CheckPoint_EXEC_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------

#include "ace/Thread_Manager.h"
#include "ace/Task.h"

#include "tao/LocalObject.h"

#include "ciao/Container_Base.h"

#include "ENWS.h"
#include "TSEC_CheckPointEIC.h"
#include "TSEC_CheckPoint_exec_export.h"


//-------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------

#define TSEC_SESSION_ID_MAX    0xffffffff
#define TSEC_SESSION_MAX_IDENT 2


//-------------------------------------------------------------------
// Type definitions
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Forward declarations
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Classes
//-------------------------------------------------------------------

namespace Impl
{
  typedef ACE_SYNCH_MUTEX TMutex;

  class TSEC_CheckPoint_exec_i;

  class TSEC_Session_impl :
    public virtual POA_ENW::TSession,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    TSEC_Session_impl
    (
      TSEC_CheckPoint_exec_i& Parent,
      CORBA::Long             Ident
    );

    virtual ~TSEC_Session_impl();

    CORBA::Long getIdent() ACE_THROW_SPEC( ( CORBA::SystemException ) );
    CORBA::Boolean isInUse() ACE_THROW_SPEC( ( CORBA::SystemException ) );
    void isInUse( CORBA::Boolean Flag ) { _isInUse = Flag; }

  protected:
    CORBA::Boolean _isInUse;
    CORBA::Long    _ident;

    TSEC_CheckPoint_exec_i& _parent;

    CORBA::ULong hash
    (
      CORBA::ULong Maximum
      ACE_ENV_ARG_DECL_NOT_USED
    );

    TSEC_CheckPoint_exec_i& getParent() { return _parent; }

  };

  struct TSEC_SessionEntry
  {
    TSEC_SessionEntry();

    TSEC_SessionEntry
    (
      TSEC_Session_impl* pSessionImpl,
      ENW::TSession_ptr  Session
    );

    TSEC_Session_impl* _p_sessionImpl;
    ENW::TSession_var  _session;
  };

  class TSEC_CHECKPOINT_EXEC_Export TSEC_CheckPoint_exec_i :
    public virtual ENW::TSEC_CheckPoint_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    TSEC_CheckPoint_exec_i();
    TSEC_CheckPoint_exec_i( const char* Version );

    virtual ~TSEC_CheckPoint_exec_i();

    // Attribute Operationen:
    //


    // Interface ISessionService
    //

    virtual ENW::CCM_ISessionService_ptr get_sessionService
    (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException ) );


    virtual ENW::TSession_ptr createSession
    (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException ) );

    virtual CORBA::Boolean destroySession
    (
      ENW::TSession_ptr Session
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException ) );

    virtual ENW::TSession_ptr acquireSession
    (
      CORBA::Long Ident
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        ENW::EUnknownIdent ) );

    virtual CORBA::Boolean releaseSession
    (
      CORBA::Long       Ident,
      ENW::TSession_ptr Session
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        ENW::EUnknownSession ) );


    // Events
    //

    virtual void push_lifeTokenIn
    (
      ENW::ET_LifeToken* Event
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException ) );


    // Operations from Components::SessionComponent
    //

    virtual void set_session_context
    (
      Components::SessionContext_ptr ctx
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        Components::CCMException ) );

    virtual void ciao_preactivate
    (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        Components::CCMException ) );

    virtual void ccm_activate
    (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        Components::CCMException ) );

    virtual void ciao_postactivate
    (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        Components::CCMException ) );


    virtual void ccm_passivate
    (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        Components::CCMException ) );

    virtual void ccm_remove
    (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        Components::CCMException ) );

    CORBA::Object_ptr getObjRef
    (
      PortableServer::Servant Servant
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException ) );

    int init( ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )ACE_THROW_SPEC( ( CORBA::SystemException ) );

    CORBA::Object_ptr installServant
    (
      PortableServer::Servant Servant
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException ) );

    void uninstallServant
    (
      CORBA::Object_ptr ObjRef
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException ) );

    TMutex& getMutex() { return _mutex; }

  protected:
    // Copmponent specific context

    ENW::CCM_TSEC_CheckPoint_Context_var _context;
    ::CIAO::Session_Container*           _p_sessionContainer;
    TMutex                               _mutex;
    CORBA::ORB_var                       _orb;

  private:
    bool              _awaitingLifeToken;
    bool              _isActivated;

    //TSEC_Session_impl* sessionVector[TSEC_SESSION_MAX_IDENT];
    TSEC_SessionEntry sessionVector[TSEC_SESSION_MAX_IDENT];
  };

  /**
    *  home executor implementation class.
    */
  class TSEC_CHECKPOINT_EXEC_Export TSEC_CheckPointHome_exec_i :
    public virtual ENW::CCM_TSEC_CheckPointHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    TSEC_CheckPointHome_exec_i();

    virtual ~TSEC_CheckPointHome_exec_i();

    // Explicit home operations.
    virtual ::Components::EnterpriseComponent_ptr new_TSEC_CheckPoint
    (
      const char* Version
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
      ) ACE_THROW_SPEC(( CORBA::SystemException));

    // Implicit home operations.
    virtual ::Components::EnterpriseComponent_ptr create
    (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC( ( CORBA::SystemException,
                        Components::CCMException ) );
  };
}  // Impl

extern "C" TSEC_CHECKPOINT_EXEC_Export ::Components::HomeExecutorBase_ptr
createTSEC_CheckPointHome_Impl();

#endif // TSEC_CheckPoint_EXEC_H
