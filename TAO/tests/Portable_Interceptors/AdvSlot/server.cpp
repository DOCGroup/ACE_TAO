// author    : Boris Kolpackov <boris@kolpackov.net>
#include "tao/LocalObject.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"

#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

#include "StateTransferS.h"

using namespace CORBA;
using namespace PortableServer;
using namespace PortableInterceptor;

ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


//
PortableInterceptor::SlotId slot_id {};


//
class StateTransferImpl : public virtual POA_StateTransfer
{
public:
  explicit StateTransferImpl (ORB_ptr orb)
      : orb_ (ORB::_duplicate (orb))
  {
  }

  Short number () override
  {
    // Prepare state update.
    //
    Any state;
    CORBA::Long const number = 5;
    state <<= number;

    Object_var const obj (orb_->resolve_initial_references ("PICurrent"));
    PortableInterceptor::Current_var const pic (
      PortableInterceptor::Current::_narrow (obj.in ()));

    pic->set_slot (slot_id, state);

    return 1;
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override
  {
    ACE_DEBUG ((LM_DEBUG, "Server is shutting down.\n"));
    this->orb_->shutdown (false);
  }
  //FUZZ: enable check_for_lack_ACE_OS

  StateTransferImpl (StateTransferImpl const &) = delete;
  StateTransferImpl (StateTransferImpl &&) = delete;
  StateTransferImpl & operator = (StateTransferImpl const &) = delete;
  StateTransferImpl & operator = (StateTransferImpl &&) = delete;

private:
  ORB_var orb_;
};


//
class ReplicaController : public virtual ServerRequestInterceptor,
                          public virtual ::CORBA::LocalObject
{
public:
  ReplicaController () = default;

  char * name () override
  {
    return string_dup ("ReplicaController");
  }

  void destroy () override
  {
  }

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
  void tao_ft_interception_point (ServerRequestInfo_ptr, OctetSeq_out) override
  {
  }
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

  void receive_request_service_contexts (ServerRequestInfo_ptr) override
  {
  }

  void receive_request (ServerRequestInfo_ptr) override
  {
  }

  void send_reply (ServerRequestInfo_ptr ri) override
  {
    Any_var state (ri->get_slot (slot_id));
    CORBA::Long n {};
    state >>= n;

    if (n == 5)
      ACE_DEBUG ((LM_DEBUG, "State value is correctly %d.\n", n));
    else
      ACE_ERROR ((LM_ERROR, "ERROR: State value is incorrectly %d.\n", n));
  }

  void send_exception (ServerRequestInfo_ptr) override
  {
  }

  void send_other (ServerRequestInfo_ptr) override
  {
  }

  ReplicaController (ReplicaController const &) = delete;
  ReplicaController (ReplicaController &&) = delete;
  ReplicaController & operator = (ReplicaController const &) = delete;
  ReplicaController & operator = (ReplicaController &&) = delete;
};


//
class ORB_Initializer : public virtual ORBInitializer,
                        public virtual ::CORBA::LocalObject
{
public:
  ORB_Initializer () = default;

  void pre_init (ORBInitInfo_ptr) override
  {
  }

  void post_init (ORBInitInfo_ptr info) override
  {
    slot_id = info->allocate_slot_id ();
    ACE_DEBUG ((LM_DEBUG, "Allocated slot with id %d.\n", slot_id));

    ServerRequestInterceptor_var const interceptor (new ReplicaController ());
    info->add_server_request_interceptor (interceptor.in ());
  }

  ORB_Initializer (ORB_Initializer const &) = delete;
  ORB_Initializer (ORB_Initializer &&) = delete;
  ORB_Initializer & operator = (ORB_Initializer const &) = delete;
  ORB_Initializer & operator = (ORB_Initializer &&) = delete;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ORBInitializer_var const orb_initializer (new ORB_Initializer ());
      register_orb_initializer (orb_initializer.in ());

      ORB_var const orb (ORB_init (argc, argv));

      if (parse_args (argc, argv) != 0)
        return 1;

      Object_var const obj (orb->resolve_initial_references ("RootPOA"));

      POA_var const root_poa (POA::_narrow (obj.in ()));
      POAManager_var const poa_manager (root_poa->the_POAManager ());

      StateTransferImpl * const impl = new StateTransferImpl (orb.in ());
      // Transfer the initial servant reference to the owning var.
      ServantBase_var impl_var (impl);

      PortableServer::ObjectId_var const id_act =
        root_poa->activate_object (impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id_act.in ());

      StateTransfer_var const ref = StateTransfer::_narrow (object.in ());
      String_var const ior (orb->object_to_string (ref.in ()));

      poa_manager->activate ();

      // Dump the ior.
      //
      FILE * const output_file = ACE_OS::fopen (ior_output_file, "w");
      if (output_file == nullptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file <%s> for writing "
                             "IOR: %C",
                             ior_output_file,
                             ior.in ()),
                            1);
        }

      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      ACE_DEBUG ((LM_DEBUG, "Server is ready, IOR is in '%s'\n", ior_output_file));

      // Run the ORB event loop.
      //
      orb->run ();

      root_poa->destroy (true, true);
      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught unexpected CORBA exception:");

      return 1;
    }

  return 0;
}
