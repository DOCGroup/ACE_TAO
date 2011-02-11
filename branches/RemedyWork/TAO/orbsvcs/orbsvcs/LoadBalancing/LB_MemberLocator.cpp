// $Id$

#include "orbsvcs/LoadBalancing/LB_LoadManager.h"
#include "orbsvcs/LoadBalancing/LB_MemberLocator.h"

#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    PortableServer::ServantLocator::Cookie & /* the_cookie */)
{
  try
    {
      CORBA::Object_var member =
        this->load_manager_->next_member (oid);

//       ACE_DEBUG ((LM_DEBUG, "%N:%l\n"));
//       ACE_DEBUG ((LM_DEBUG,
//                   "FORWARDED\n"));

      ACE_ASSERT (!CORBA::is_nil (member.in ()));
      // Throw a forward exception to force the client to redirect its
      // requests to the member chosen by the LoadBalancer.
      throw PortableServer::ForwardRequest (member.in ());
    }
  catch (const PortableGroup::ObjectGroupNotFound& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception ("LoadManager::next_member()");
    }
  catch (const PortableGroup::MemberNotFound& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception ("LoadManager::next_member()");
    }

  throw CORBA::OBJECT_NOT_EXIST ();
}

void
TAO_LB_MemberLocator::postinvoke (
    const PortableServer::ObjectId & /* oid */,
    PortableServer::POA_ptr /* adapter */,
    const char * /* operation */,
    PortableServer::ServantLocator::Cookie /* the_cookie */,
    PortableServer::Servant /* the_servant */)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
