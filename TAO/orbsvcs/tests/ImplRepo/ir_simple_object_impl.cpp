// $Id$

#include "ir_simple_object_impl.h"
#include "tao/corba.h"

// Constructor
IR_Simple_Impl::IR_Simple_Impl (CORBA::ORB_ptr orb_ptr,
                                PortableServer::POA_ptr poa_ptr,
                                CORBA::Object_ptr forward_to_ptr)
  : orb_var_ (CORBA::ORB::_duplicate (orb_ptr)),
    poa_var_ (PortableServer::POA::_duplicate (poa_ptr)),
    forward_to_var_ (CORBA::Object::_duplicate (forward_to_ptr))
{
  if (CORBA::is_nil (this->forward_to_var_))
    ACE_DEBUG ((LM_DEBUG, "Forward_to is nil!\n"));
}

// Destructor

IR_Simple_Impl::~IR_Simple_Impl (void)
{
}

// Cube a long

CORBA::Long
IR_Simple_Impl::simple_method (CORBA::Long l, CORBA::Environment &env)
{
  if (!CORBA::is_nil (this->forward_to_var_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, "Forwarding...\n"));
    
      PortableServer::ObjectId_var oid =
        this->poa_var_->servant_to_id (this, env);
      
      if (env.exception () != 0)
        return -1;

      PortableServer::Servant servant = this->poa_var_->_servant ();
      if (servant == 0)
        {
//          CORBA::Exception *exception = new Foo::Cannot_Forward;
//          env.exception (exception);
          return -1;
        }
      
      void *ptr = servant->_downcast ("IDL:PortableServer/POA:1.0");
      POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
      TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);
      
      tao_poa->forward_object (oid.in (),
                               this->forward_to_var_.in (),
                               env);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Forward_to refenence is nil.\n"));
//      CORBA::Exception *exception = new Foo::Cannot_Forward;
//      env.exception (exception);
      return -1;
    }
  return 0;
}

// Shutdown.

void IR_Simple_Impl::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG, "%s\n", "IR_Simple_Impl is shutting down"));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

