// $Id$

#include "HelloS.h"

ACE_RCSID (Reference_Counting,
           test,
           "$Id$")

class Hello_impl :
  virtual public POA_Hello
{
public:
  Hello_impl ()
  {
    ACE_DEBUG ((LM_DEBUG, "Hello_impl::Hello_impl()\n"));
  }

  ~Hello_impl ()
  {
    ACE_DEBUG ((LM_DEBUG, "Hello_impl::~Hello_impl()\n"));
  }

  virtual void moo (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
  {
  }
};

CORBA::ULong
getRefCount (PortableServer::ServantBase * sb ACE_ENV_ARG_DECL)
{
  return sb->_refcount_value (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
main (int argc, char * argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      Hello_impl * h = 0;
      ACE_NEW_RETURN (h,Hello_impl, 1);

      CORBA::ULong before_act = h->_refcount_value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Before activation: %d\n", before_act));

      PortableServer::ObjectId_var oid = poa->activate_object (h ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong after_act = h->_refcount_value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "After activation: %d\n", after_act));
        {
          /*
           * C++ Language Mapping (formal/03-06-03), section 1.37.3 (Servant
           * Memory Management Considerations), first bullet on page 1-136:
           *
           *   POA::id_to_servant returns a Servant. The POA invokes _add_ref
           *   once on the Servant before returning it; the caller of
           *   id_to_servant is responsible for invoking _remove_ref on the
           *   returned servant when it is finished with it.
           */

          CORBA::ULong refCountBeforeIdToServant =
            h->_refcount_value (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG, "Before id_to_servant:  %d\n", refCountBeforeIdToServant));

          PortableServer::ServantBase_var srv = poa->id_to_servant (oid.in() ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::ULong refCountAfterIdToServant =
            srv->_refcount_value (ACE_ENV_SINGLE_ARG_PARAMETER);;
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG, "After id_to_servant:  %d\n", refCountAfterIdToServant));

           /*
           * According to the above quote, this assertion shall be true.
           */
          ACE_ASSERT (refCountAfterIdToServant == refCountBeforeIdToServant + 1);

          /*
           * At the end of this scope, "srv" is destructed, which decrements
           * the servant's reference count.
           */
        }

      CORBA::ULong before_deact = h->_refcount_value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Before deactivate_object: %d\n", before_deact));

      poa->deactivate_object (oid.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /*
       * Because id_to_servant did not increment the reference count, but
       * the reference count was decremented by the "srv" destructor, the
       * reference count, using TAO 1.4.5, is now 0, and the servant has
       * been destructed. So the following will crash, despite being
       * correct.
       */

      CORBA::ULong after_deact = h->_refcount_value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "After deactivate_object: %d\n", after_deact));

      h->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->shutdown (1
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
