// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "tao/LocalObject.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"

#include "ace/OS_NS_stdio.h"

#include "StateTransferS.h"

using namespace CORBA;
using namespace PortableServer;
using namespace PortableInterceptor;

//
//
PortableInterceptor::SlotId slot_id;


//
//
class StateTransferImpl: public virtual POA_StateTransfer
{
public:
  StateTransferImpl (ORB_ptr orb)
      : orb_ (ORB::_duplicate (orb))
  {
  }

  virtual Short
  number () throw (SystemException)
  {
    // Prepare state update.
    //
    Any state;
    CORBA::Long number = 5;
    state <<= number;

    Object_var obj (orb_->resolve_initial_references ("PICurrent"));
    PortableInterceptor::Current_var pic (
      PortableInterceptor::Current::_narrow (obj.in ()));

    pic->set_slot (slot_id, state);

    return 1;
  }

  virtual void
  shutdown () throw (SystemException)
  {
    ACE_DEBUG ((LM_DEBUG, "Server is shutting down.\n"));
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
public:
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
  tao_ft_interception_point (ServerRequestInfo_ptr, OctetSeq_out)
    throw (SystemException, ForwardRequest)
  {
  }
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

  virtual void
  receive_request_service_contexts (ServerRequestInfo_ptr)
    throw (SystemException, ForwardRequest)
  {
  }

  virtual void
  receive_request (ServerRequestInfo_ptr)
    throw (SystemException, ForwardRequest)
  {
  }

  virtual void
  send_reply (ServerRequestInfo_ptr ri) throw (SystemException)
  {
    Any_var state (ri->get_slot (slot_id));
    CORBA::Long n (0);
    state >>= n;

    if (n == 5)
      ACE_DEBUG ((LM_DEBUG, "State value is correctly %d.\n", n));
    else
      ACE_ERROR ((LM_ERROR, "ERROR: State value is incorrectly %d.\n", n));
  }

  virtual void
  send_exception (ServerRequestInfo_ptr)
    throw (SystemException, ForwardRequest)
  {
  }

  virtual void
  send_other (ServerRequestInfo_ptr) throw (SystemException, ForwardRequest)
  {
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
    slot_id = info->allocate_slot_id ();
    ACE_DEBUG ((LM_DEBUG, "Allocated slot with id %d.\n", slot_id));


    ServerRequestInterceptor_var interceptor (new ReplicaController ());
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

  StateTransferImpl* impl = new StateTransferImpl (orb.in ());
  ServantBase_var impl_var (impl);

  StateTransfer_var ref (impl->_this ());
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
