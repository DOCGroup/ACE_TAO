#include "LB_MemberLocator.h"
#include "LB_LoadManager.h"

ACE_RCSID (LoadBalancing,
           LB_MemberLocator,
           "$Id$")

TAO_LB_MemberLocator::TAO_LB_MemberLocator (
  TAO_LB_LoadManager * lm)
  : load_manager_ (lm)
{
  ACE_ASSERT (lm != 0);
}

PortableServer::Servant
TAO_LB_MemberLocator::preinvoke (
    const PortableServer::ObjectId & oid,
    PortableServer::POA_ptr /* adapter */,
    const char * /* operation */,
    PortableServer::ServantLocator::Cookie & /* the_cookie */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  CORBA::Object_var member =
    this->load_manager_->member (oid
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

//   ACE_DEBUG ((LM_DEBUG,
//               "FORWARDED\n"));

  // Throw a forward exception to force the client to redirect its
  // requests to the member chosen by the LoadBalancer.
  ACE_THROW_RETURN (PortableServer::ForwardRequest (member.in ()),
                    0);
}

void
TAO_LB_MemberLocator::postinvoke (
    const PortableServer::ObjectId & /* oid */,
    PortableServer::POA_ptr /* adapter */,
    const char * /* operation */,
    PortableServer::ServantLocator::Cookie /* the_cookie */,
    PortableServer::Servant /* the_servant */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
