#include "HelloS.h"

class Hello_impl :
  public virtual POA_Hello
{
public:
  Hello_impl ()
  {
    ACE_DEBUG ((LM_DEBUG, "Hello_impl::Hello_impl()\n"));
  }

  ~Hello_impl () override
  {
    ACE_DEBUG ((LM_DEBUG, "Hello_impl::~Hello_impl()\n"));
  }

  Hello_impl (Hello_impl const &) = delete;
  Hello_impl (Hello_impl &&) = delete;
  Hello_impl &operator= (Hello_impl const &) = delete;
  Hello_impl &operator= (Hello_impl &&) = delete;

  void moo () override
  {
  }
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      Hello_impl *servant {};
      ACE_NEW_RETURN (servant, Hello_impl, 1);
      PortableServer::ServantBase_var const servant_owner (servant);

      CORBA::ULong const before_act = servant->_refcount_value ();

      ACE_DEBUG ((LM_DEBUG, "Before activation: %d\n", before_act));

      PortableServer::ObjectId_var const oid = poa->activate_object (servant);

      CORBA::ULong const after_act = servant->_refcount_value ();

      ACE_DEBUG ((LM_DEBUG, "After activation: %d\n", after_act));
      /*
       * C++ Language Mapping (formal/03-06-03), section 1.37.3 (Servant
       * Memory Management Considerations), first bullet on page 1-136:
       *
       *   POA::id_to_servant returns a Servant. The POA invokes _add_ref once
       *   on the Servant before returning it; the caller of id_to_servant is
       *   responsible for invoking _remove_ref on the returned servant when
       *   it is finished with it.
       */
      {
        CORBA::ULong const ref_count_before_id_to_servant =
          servant->_refcount_value ();
        ACE_DEBUG ((LM_DEBUG, "Before id_to_servant:  %d\n", ref_count_before_id_to_servant));

        PortableServer::ServantBase_var const returned_servant =
          poa->id_to_servant (oid.in ());
        CORBA::ULong const ref_count_after_id_to_servant =
          returned_servant->_refcount_value ();
        ACE_DEBUG ((LM_DEBUG, "After id_to_servant:  %d\n", ref_count_after_id_to_servant));

        // According to the above quote, this assertion shall be true.
        ACE_ASSERT (ref_count_after_id_to_servant == ref_count_before_id_to_servant + 1);
      }

      CORBA::ULong const before_deact = servant->_refcount_value ();

      ACE_DEBUG ((LM_DEBUG, "Before deactivate_object: %d\n", before_deact));

      poa->deactivate_object (oid.in());

      // The local servant_owner keeps the servant alive after POA deactivation.
      CORBA::ULong const after_deact = servant->_refcount_value ();

      ACE_DEBUG ((LM_DEBUG, "After deactivate_object: %d\n", after_deact));

      orb->shutdown (true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
