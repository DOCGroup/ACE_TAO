#include "Dynamic_Component_Activator.h"
#include "CIAO_common.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (ciao,
           Dynamic_Component_Activator,
           "$Id$")

namespace CIAO
{
  Dynamic_Component_Activator::Dynamic_Component_Activator (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o))
  {
  }

  Dynamic_Component_Activator::~Dynamic_Component_Activator (void)
  {
  }

  PortableServer::Servant
  Dynamic_Component_Activator::incarnate (const PortableServer::ObjectId &oid,
                                PortableServer::POA_ptr
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest))
  {
  }

  void
  Dynamic_Component_Activator::etherealize (const PortableServer::ObjectId &,
                                  PortableServer::POA_ptr ,
                                  PortableServer::Servant ,
                                  CORBA::Boolean ,
                                  CORBA::Boolean
                                  ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
  {
    /// Need to investigate what needs to be handled here..
  }

}
