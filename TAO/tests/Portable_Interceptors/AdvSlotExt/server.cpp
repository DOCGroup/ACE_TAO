// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "tao/LocalObject.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"

#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

#include "StateTransferS.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

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
  number ()
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

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void
  shutdown ()
  {
    ACE_DEBUG ((LM_DEBUG, "Server is shutting down.\n"));
    this->orb_->shutdown (0);
  }
  //FUZZ: enable check_for_lack_ACE_OS

private:
  ORB_var orb_;
};


//
//
class ReplicaController: public virtual ServerRequestInterceptor,
                         public virtual ::CORBA::LocalObject
{
public:
  ReplicaController (PortableInterceptor::Current_ptr pi_current)
   : pi_current_ (PortableInterceptor::Current::_duplicate (pi_current))
  {
  }

  virtual char*
  name ()
  {
    return string_dup ("ReplicaController");
  }

  virtual void
  destroy ()
  {
  }

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
  virtual void
  tao_ft_interception_point (ServerRequestInfo_ptr, OctetSeq_out)
  {
  }
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

  virtual void
  receive_request_service_contexts (ServerRequestInfo_ptr)
  {
  }

  virtual void
  receive_request (ServerRequestInfo_ptr ri)
  {
    Any state;
    CORBA::Long number = 5;
    state <<= number;

    // After we have set things to TSC, change the value in RSC, at this
    // moment no copy should be done anymore.
    pi_current_->set_slot (slot_id, state);
    CORBA::Long number6 = 6;
    state <<= number6;
    ri->set_slot (slot_id, state);

    Any_var state_get (pi_current_->get_slot (slot_id));
    CORBA::Long n (0);
    state_get >>= n;

    if (n == 5)
      ACE_DEBUG ((LM_DEBUG, "State value is correctly %d.\n", n));
    else
      ACE_ERROR ((LM_ERROR, "ERROR: State value is incorrectly %d.\n", n));
  }

  virtual void
  send_reply (ServerRequestInfo_ptr ri)
  {
    CORBA::String_var op = ri->operation ();

    if (ACE_OS::strcmp (op.in (), "number") != 0)
      return; // Don't mess with PICurrent if not invoking test method.

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
  {
  }

  virtual void
  send_other (ServerRequestInfo_ptr)
  {
  }
  PortableInterceptor::Current_var pi_current_;
};


//
//
class ORB_Initializer : public virtual ORBInitializer,
                        public virtual ::CORBA::LocalObject
{
public:
  virtual void
  pre_init (ORBInitInfo_ptr)
  {
  }

  virtual void
  post_init (ORBInitInfo_ptr info)
  {
    slot_id = info->allocate_slot_id ();
    ACE_DEBUG ((LM_DEBUG, "Allocated slot with id %d.\n", slot_id));

    CORBA::Object_var obj =
      info->resolve_initial_references ("PICurrent");

    PortableInterceptor::Current_var pi_current =
      PortableInterceptor::Current::_narrow (obj.in ());

    if (CORBA::is_nil (pi_current.in ()))
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) ERROR: Could not resolve PICurrent object.\n"));

        throw CORBA::INTERNAL ();
      }

    ServerRequestInterceptor_var interceptor (new ReplicaController (pi_current.in ()));
    info->add_server_request_interceptor (interceptor.in ());
  }
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ORBInitializer_var orb_initializer (new ORB_Initializer ());
      register_orb_initializer (orb_initializer.in ());

      ORB_var orb (ORB_init (argc, argv));

      if (parse_args (argc, argv) != 0)
        return 1;

      Object_var obj (orb->resolve_initial_references ("RootPOA"));

      POA_var root_poa (POA::_narrow (obj.in ()));
      POAManager_var poa_manager (root_poa->the_POAManager ());

      StateTransferImpl* impl = new StateTransferImpl (orb.in ());
      ServantBase_var impl_var (impl);

      PortableServer::ObjectId_var id_act =
        root_poa->activate_object (impl);

      CORBA::Object_var object = root_poa->id_to_reference (id_act.in ());

      StateTransfer_var ref = StateTransfer::_narrow (object.in ());
      String_var ior (orb->object_to_string (ref.in ()));

      poa_manager->activate ();


      // Dump the ior.
      //
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot open output file <%s> for writing "
                          "IOR: %s",
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

      root_poa->destroy (1, 1);
      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
