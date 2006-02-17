// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "tao/LocalObject.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "ace/OS_NS_stdio.h"

#include "InvokeMeS.h"

using namespace CORBA;
using namespace PortableServer;
using namespace PortableInterceptor;

namespace
{
  PortableInterceptor::SlotId slot_id;

  void getMySlot(
    PortableInterceptor::ServerRequestInfo_ptr ri,
    PortableInterceptor::Current_ptr pi_current,
    const char *location)
  {
    CORBA::String_var
      op= ri->operation ();

    const bool correctOperation=
      (0 == ACE_OS::strcmp( op, "invoke_me" ));

    if (correctOperation)
    {
      CORBA::Any_var data;

      ACE_DEBUG ((LM_DEBUG, "SERVER %s -> get_slot(TSC) -> is ", location));
      data= pi_current->get_slot( slot_id );
      CORBA::TypeCode_var
        tc= data->type();
      if (tc->kind() == CORBA::tk_null)
        ACE_DEBUG ((LM_DEBUG, "EMPTY\n"));
      else
      {
        ACE_DEBUG ((LM_DEBUG, "UNKNOWN TYPE\n"));
        throw INTERNAL ();
      }

      ACE_DEBUG ((LM_DEBUG, "SERVER %s -> get_slot(RSC) -> is ", location));
      data= ri->get_slot( slot_id );

      tc= data->type();
      if (tc->kind() == CORBA::tk_null)
        ACE_DEBUG ((LM_DEBUG, "EMPTY\n"));
      else
      {
        ACE_DEBUG ((LM_DEBUG, "WRONG TYPE\n"));
        throw INTERNAL();
      }
    }
  }
}

//
//
class InvokeMeImpl: public virtual POA_InvokeMe
{
public:
  InvokeMeImpl (ORB_ptr orb)
      : orb_ (ORB::_duplicate (orb))
  {
  }

  virtual void invoke_me() throw (SystemException)
  {
    ACE_DEBUG ((LM_DEBUG, "invoke_me\n"));
  }

  virtual void
  shutdown () throw (SystemException)
  {
    ACE_DEBUG ((LM_DEBUG, "\nServer is shutting down.\n"));
    this->orb_->shutdown (0);
  }

private:
  ORB_var orb_;
};


//
//
class ReplicaController: public virtual ServerRequestInterceptor,
                         public virtual TAO_Local_RefCounted_Object
{
  PortableInterceptor::Current_ptr pi_current_;
public:
  ReplicaController ( PortableInterceptor::Current_ptr pi_current )
    : pi_current_( pi_current )
  {
  }

  virtual char*
  name () throw (SystemException)
  {
    return string_dup ("ReplicaController");
  }

  virtual void
  destroy () throw (SystemException)
  {
  }

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
  virtual void
  tao_ft_interception_point (ServerRequestInfo_ptr ri, OctetSeq_out)
    throw (SystemException, ForwardRequest)
  {
    getMySlot( ri, this->pi_current_, "tao_ft_interception_point" );
  }
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

  virtual void
  receive_request_service_contexts (ServerRequestInfo_ptr ri)
    throw (SystemException, ForwardRequest)
  {
    getMySlot( ri, this->pi_current_, "receive_request_service_contexts" );
  }

  virtual void
  receive_request (ServerRequestInfo_ptr ri)
    throw (SystemException, ForwardRequest)
  {
    getMySlot( ri, this->pi_current_, "receive_request" );
  }

  virtual void
  send_reply (ServerRequestInfo_ptr ri) throw (SystemException)
  {
    getMySlot( ri, this->pi_current_, "send_reply" );
  }

  virtual void
  send_exception (ServerRequestInfo_ptr ri)
    throw (SystemException, ForwardRequest)
  {
    getMySlot( ri, this->pi_current_, "send_exception" );
  }

  virtual void
  send_other (ServerRequestInfo_ptr ri)
    throw (SystemException, ForwardRequest)
  {
    getMySlot( ri, this->pi_current_, "send_other" );
  }
};


//
//
class ORB_Initializer : public virtual ORBInitializer,
                        public virtual TAO_Local_RefCounted_Object
{
public:
  virtual void
  pre_init (ORBInitInfo_ptr) throw (SystemException)
  {
  }

  virtual void
  post_init (ORBInitInfo_ptr info) throw (SystemException)
  {
    CORBA::Object_var obj =
      info->resolve_initial_references ("PICurrent");

    PortableInterceptor::Current_var pi_current =
      PortableInterceptor::Current::_narrow (obj.in ());

    slot_id = info->allocate_slot_id ();
    ACE_DEBUG ((LM_DEBUG, "Allocated slot with id %d.\n", slot_id));

    ServerRequestInterceptor_var
      interceptor (new ReplicaController (pi_current.in ()));
    info->add_server_request_interceptor (interceptor.in ());
  }
};

int
main (int argc, char *argv[])
{
  ORBInitializer_var orb_initializer (new ORB_Initializer ());
  register_orb_initializer (orb_initializer.in ());

  ORB_var orb (ORB_init (argc, argv));

  Object_var obj (orb->resolve_initial_references ("RootPOA"));

  POA_var root_poa (POA::_narrow (obj.in ()));
  POAManager_var poa_manager (root_poa->the_POAManager ());

  InvokeMeImpl* impl = new InvokeMeImpl (orb.in ());
  ServantBase_var impl_var (impl);

  InvokeMe_var ref (impl->_this ());
  String_var ior (orb->object_to_string (ref.in ()));

  poa_manager->activate ();


  // Dump the ior.
  //
  FILE *output_file= ACE_OS::fopen ("server.ior", "w");
  if (output_file == 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file <%s> for writing "
                       "IOR: %s",
                       ior.in ()),
                      1);
  }

  ACE_OS::fprintf (output_file, "%s", ior.in ());
  ACE_OS::fclose (output_file);

  ACE_DEBUG ((LM_DEBUG, "Server is ready, IOR is in 'server.ior'\n"));


  // Run the ORB event loop.
  //
  orb->run ();

  root_poa->destroy (1, 1);
  orb->destroy ();

  ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));

  return 0;
}
