//$Id$

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------

#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
#include "ace/os_include/os_assert.h"

#include "tao/LocalObject.h"
#include <orbsvcs/CosNamingC.h>

#include "ciao/Version.h"
#include "ciao/Containers/Session/Session_Container.h"
#include "TSEC_CheckPoint_exec.h"
#include "TSEC_CheckPoint_svnt.h"


//-------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------

#define SEPARATION_LINE "----------------------------------------" \
                        "----------------------------------------\n"


//-------------------------------------------------------------------
// Statics
//-------------------------------------------------------------------

static const char* argv[] =
{
  "TSEC_CheckPoint"
};


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_SessionEntry::TSEC_SessionEntry()
: _p_sessionImpl( 0 )
{
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_SessionEntry::TSEC_SessionEntry
(
  TSEC_Session_impl* pSessionImpl,
  ENW::TSession_ptr  Session
)
: _p_sessionImpl( pSessionImpl )
{
  if( !CORBA::is_nil( Session ) )
  {
    _session = ENW::TSession::_duplicate( Session );
  }
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_Session_impl::TSEC_Session_impl
(
  TSEC_CheckPoint_exec_i& Parent,
  CORBA::Long             Ident
)
  : _isInUse(0),_ident( Ident ),_parent( Parent )
{
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_Session_impl::~TSEC_Session_impl()
{
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

CORBA::ULong
Impl::TSEC_Session_impl::hash
(
  CORBA::ULong Maximum
)
{
  CORBA::ULong hash =
       static_cast <CORBA::ULong> (reinterpret_cast <ptrdiff_t> (this));

  return hash % Maximum;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

CORBA::Long
Impl::TSEC_Session_impl::getIdent
(
)
{
  //ACE_GUARD_RETURN( TMutex, guard, _parent.getMutex(), -1 );

  return _ident;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

CORBA::Boolean
Impl::TSEC_Session_impl::isInUse
(
)
{
  //ACE_GUARD_RETURN( TMutex, guard, _parent.getMutex(), FALSE );
  return _isInUse;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_CheckPoint_exec_i::TSEC_CheckPoint_exec_i()
: _p_sessionContainer( 0 ),
  _isActivated( false )
{
  ACE_DEBUG( ( LM_DEBUG, "Impl::TSEC_CheckPoint_exec_i::"
                         "TSEC_CheckPoint_exec_i\n" ) );

  this->init();
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_CheckPoint_exec_i::TSEC_CheckPoint_exec_i
(
  const char* /*Version*/
)
: _p_sessionContainer( 0 ),
  _isActivated( false )
{
  ACE_DEBUG( ( LM_DEBUG, "Impl::TSEC_CheckPoint_exec_i::"
                         "TSEC_CheckPoint_exec_i\n" ) );
  this->init();
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_CheckPoint_exec_i::~TSEC_CheckPoint_exec_i()
{
  ACE_DEBUG( ( LM_DEBUG, "Impl::TSEC_CheckPoint_exec_i::"
                         "~TSEC_CheckPoint_exec_i\n" ) );
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

CORBA::Object_ptr
Impl::TSEC_CheckPoint_exec_i::getObjRef
(
  PortableServer::Servant Servant
)
{
  return this->_p_sessionContainer->get_objref( Servant );
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

int
Impl::TSEC_CheckPoint_exec_i::init
(
)
{
  ACE_DEBUG ((LM_DEBUG, "Impl::TSEC_CheckPoint_exec_i::init\n"));

  int argc = sizeof( argv ) / sizeof( argv[0] );

  CORBA::ORB_var orb = CORBA::ORB_init( argc,
                                        const_cast<char **> (argv)
 );
  _orb               = CORBA::ORB::_duplicate( orb.in () );

  return 0;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

CORBA::Object_ptr
Impl::TSEC_CheckPoint_exec_i::installServant
(
  PortableServer::Servant Servant
)
{
  assert( this->_p_sessionContainer );
  PortableServer::ObjectId_var oid;
  return this->_p_sessionContainer->install_servant( Servant,
                             CIAO::Container_Types::COMPONENT_t, oid.out ());
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

void
Impl::TSEC_CheckPoint_exec_i::uninstallServant
(
  CORBA::Object_ptr ObjRef
)
{
  assert( this->_p_sessionContainer );

  this->_p_sessionContainer->uninstall( ObjRef, CIAO::Container_Types::COMPONENT_t);
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

ENW::TSession_ptr
Impl::TSEC_CheckPoint_exec_i::createSession
(
)
{
  static long cnt = 0L;

  ENW::TSession_var session = ENW::TSession::_nil();

  ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::createSession...\n" ) );

  TSEC_Session_impl* p_sessionImpl = new TSEC_Session_impl( *this, ++cnt );
  PortableServer::ServantBase_var safe_servant( p_sessionImpl );
  CORBA::Object_var obj = this->installServant( p_sessionImpl
 );
  session = ENW::TSession::_narrow ( obj.in () );

  ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::createSession...[DONE]\n" ) );

  return session._retn();
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

::CORBA::Boolean
Impl::TSEC_CheckPoint_exec_i::destroySession
(
  ENW::TSession_ptr Session
)
{
  ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::destroySession...\n" ) );

  if( CORBA::is_nil( Session ) )
  {
    ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::destroySession...[ERROR]\n" ) );
    return 0;
  }

  this->uninstallServant( Session );

  return 1;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

ENW::TSession_ptr
Impl::TSEC_CheckPoint_exec_i::acquireSession
(
  CORBA::Long Ident
)
{
  ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::acquireSession...\n" ) );

  if( Ident <= 0  ||  Ident > TSEC_SESSION_MAX_IDENT )
  {
    if( Ident != -1 )
    {
      ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::releaseSession...[ERROR]\n" ) );
      throw ENW::EUnknownIdent();
    }
  }

  ENW::TSession_var  session = ENW::TSession::_nil();
  TSEC_Session_impl* p_sessionImpl = 0;

  try
  {
    {
      //ACE_GUARD_RETURN( TMutex, guard, _mutex, ENW::TSession::_nil() );
      p_sessionImpl = sessionVector[Ident-1]._p_sessionImpl;
    }

    p_sessionImpl->isInUse( 1 );
    CORBA::Object_var obj = this->getObjRef( p_sessionImpl) ;
    session = ENW::TSession::_narrow ( obj.in () );
    //session = sessionVector[Ident-1]._session;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Reason\n");
    ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::releaseSession...[ERROR]\n" ) );

    return session._retn();
  }

  ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::releaseSession...[DONE]\n" ) );

  return session._retn();
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

::CORBA::Boolean
Impl::TSEC_CheckPoint_exec_i::releaseSession
(
  CORBA::Long Ident,
  ENW::TSession_ptr Session
)
{
  ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::releaseSession...\n" ) );

  if( CORBA::is_nil( Session ) )
  {
    ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::releaseSession...[ERROR]\n" ) );
    throw ENW::EUnknownSession();
  }

  //CORBA::Long ident = Session->getIdent();
  CORBA::Long ident = Ident;

  if( ident <= 0  ||  ident > TSEC_SESSION_MAX_IDENT )
  {
    ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::releaseSession...[ERROR]\n" ) );
    return 0;;
  }

  TSEC_Session_impl* p_sessionImpl = 0;

  {
    //ACE_GUARD_RETURN( TMutex, guard, _mutex, FALSE );

    p_sessionImpl = sessionVector[ident-1]._p_sessionImpl;
  }

  assert( p_sessionImpl );


  if( !p_sessionImpl->isInUse() )
  {
    ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::releaseSession...[ERROR]\n" ) );
    return 0;
  }

  p_sessionImpl->isInUse( 0);

  ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) TSEC_CheckPoint_exec_i::releaseSession...[DONE]\n" ) );

  return 1;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

ENW::CCM_ISessionService_ptr
Impl::TSEC_CheckPoint_exec_i::get_sessionService
(
)
{
  return ENW::CCM_ISessionService::_duplicate( this );
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

void
Impl::TSEC_CheckPoint_exec_i::set_session_context
(
  Components::SessionContext_ptr Ctx
)
{
  ACE_DEBUG( ( LM_DEBUG,
               "Impl::TSEC_CheckPoint_exec_i::set_session_context\n" ) );

  this->_context =
    ENW::CCM_TSEC_CheckPoint_Context::_narrow( Ctx
 );

  if( CORBA::is_nil( this->_context.in() ) )
  {
    throw CORBA::INTERNAL();
  }

  CIAO_ENW_TSEC_CheckPoint_Impl::TSEC_CheckPoint_Context* p_checkPoint_Context =
    CIAO_ENW_TSEC_CheckPoint_Impl::TSEC_CheckPoint_Context::_narrow( Ctx );

  if( !p_checkPoint_Context )
  {
    throw CORBA::INTERNAL();
  }

  this->_p_sessionContainer = dynamic_cast < ::CIAO::Session_Container*>(p_checkPoint_Context->_ciao_the_Container());

  if( !_p_sessionContainer )
  {
    throw CORBA::INTERNAL();
  }
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

void
Impl::TSEC_CheckPoint_exec_i::push_lifeTokenIn
(
  ENW::ET_LifeToken* /* Event */
)
{
  _awaitingLifeToken = false;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

void
Impl::TSEC_CheckPoint_exec_i::configuration_complete
(
)
{
  ACE_DEBUG( ( LM_DEBUG, "Impl::TSEC_CheckPoint_exec_i::configuration_complete\n" ) );
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

void
Impl::TSEC_CheckPoint_exec_i::ccm_activate
(
)
{
  ACE_DEBUG( ( LM_DEBUG, "Impl::TSEC_CheckPoint_exec_i::ccm_activate\n" ) );

  TSEC_Session_impl* p_sessionImpl = 0;

  for( CORBA::Long i = 0; i < TSEC_SESSION_MAX_IDENT; ++i )
  {
    p_sessionImpl = new TSEC_Session_impl( *this, i + 1 );
    PortableServer::ServantBase_var safe_servant( p_sessionImpl );
    CORBA::Object_var obj = this->installServant( p_sessionImpl
 );
    ENW::TSession_var session =
                     ENW::TSession::_narrow ( obj.in () );
    sessionVector[i] = TSEC_SessionEntry( p_sessionImpl, session.in () );
  }

  _isActivated = true;
}



//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

void
Impl::TSEC_CheckPoint_exec_i::ccm_passivate
(
)
{
  ACE_DEBUG( ( LM_DEBUG, "Impl::TSEC_CheckPoint_exec_i::ccm_passivate\n" ) );
  _isActivated = false;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

void
Impl::TSEC_CheckPoint_exec_i::ccm_remove
(
)
{
  ACE_DEBUG( ( LM_DEBUG, "Impl::TSEC_CheckPoint_exec_i::ccm_remove\n" ) );
  _isActivated = false;
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_CheckPointHome_exec_i::TSEC_CheckPointHome_exec_i()
{
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

Impl::TSEC_CheckPointHome_exec_i::~TSEC_CheckPointHome_exec_i()
{
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

::Components::EnterpriseComponent_ptr
Impl::TSEC_CheckPointHome_exec_i::new_TSEC_CheckPoint
(
  const char* Version
  )
{
  return new Impl::TSEC_CheckPoint_exec_i( Version );
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

::Components::EnterpriseComponent_ptr
Impl::TSEC_CheckPointHome_exec_i::create
(
)
{

  return new Impl::TSEC_CheckPoint_exec_i();
}


//-------------------------------------------------------------------
// Operation
//-------------------------------------------------------------------

extern "C" TSEC_CHECKPOINT_EXEC_Export ::Components::HomeExecutorBase_ptr
createTSEC_CheckPointHome_Impl()
{
  return new Impl::TSEC_CheckPointHome_exec_i();
}

