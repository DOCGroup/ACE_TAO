#include "LB_MemberLocator.h"
#include "LB_LoadManager.h"

#include "tao/debug.h"


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
  ACE_TRY
    {
      CORBA::Object_var member =
        this->load_manager_->next_member (oid
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

//       ACE_DEBUG ((LM_DEBUG, "%N:%l\n"));
//       ACE_DEBUG ((LM_DEBUG,
//                   "FORWARDED\n"));

      ACE_ASSERT (!CORBA::is_nil (member.in ()));
      // Throw a forward exception to force the client to redirect its
      // requests to the member chosen by the LoadBalancer.
      ACE_TRY_THROW (PortableServer::ForwardRequest (member.in ()));
    }
  ACE_CATCH (PortableGroup::ObjectGroupNotFound, ex)
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ex,
                             "LoadManager::next_member()");
    }
  ACE_CATCH (PortableGroup::MemberNotFound, ex)
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ex,
                             "LoadManager::next_member()");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
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
