// author    : Boris Kolpackov <boris@kolpackov.net>
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"

#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"

#include "tao/LocalObject.h"
#include "tao/ORBInitializer_Registry.h"

#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include <cstring>

#include "InvokeMeS.h"

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


namespace
{
  PortableInterceptor::SlotId slot_id;

  CORBA::Long getMySlot (
    PortableInterceptor::ServerRequestInfo_ptr ri,
    PortableInterceptor::Current_ptr pi_current,
    char const *location,
    CORBA::Long const correctTSCvalue,
    CORBA::Long const correctRSCvalue)
  {
    CORBA::Long number {};
    CORBA::String_var const op = ri->operation ();

    bool const correctOperation =
      (0 == std::strcmp (op.in (), "invoke_me"));

    if (correctOperation)
    {
      ACE_DEBUG ((LM_DEBUG, "SERVER %C -> get_slot(TSC) -> is ", location));
      CORBA::Any_var data = pi_current->get_slot (slot_id);
      CORBA::TypeCode_var const tc = data->type ();
      if (tc->kind () == CORBA::tk_null)
      {
        ACE_DEBUG ((LM_DEBUG, "EMPTY -> "));
        if (correctTSCvalue != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "WRONG\n"));
          throw INTERNAL ();
        }
        ACE_DEBUG ((LM_DEBUG, "OK\n"));
      }
      else if (data.in () >>= number)
      {
        ACE_DEBUG ((LM_DEBUG, "long (%d) -> ", number));
        if ((correctTSCvalue != 0) && (correctTSCvalue != number))
        {
          ACE_DEBUG ((LM_DEBUG, "WRONG\n"));
          throw INTERNAL ();
        }
        ACE_DEBUG ((LM_DEBUG, "OK\n"));
      }
      else
      {
        ACE_DEBUG ((LM_DEBUG, "UNKNOWN TYPE\n"));
        throw INTERNAL ();
      }

      ACE_DEBUG ((LM_DEBUG, "SERVER %C -> get_slot(RSC) -> is ", location));
      CORBA::Any_var request_data = ri->get_slot (slot_id);
      CORBA::TypeCode_var const request_tc = request_data->type ();
      if (request_tc->kind () == CORBA::tk_null)
      {
        ACE_DEBUG ((LM_DEBUG, "EMPTY -> "));
        if (correctRSCvalue != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "WRONG\n"));
          throw INTERNAL ();
        }
        ACE_DEBUG ((LM_DEBUG, "OK\n"));
#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
        number= 61;
#else
        number= 62;
#endif /* TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1 */
      }
      else if (request_data.in () >>= number)
      {
        ACE_DEBUG ((LM_DEBUG, "long (%d) -> ", number));
        if ((correctRSCvalue != 0) && (correctRSCvalue != number))
        {
          ACE_DEBUG ((LM_DEBUG, "WRONG\n"));
          throw INTERNAL ();
        }
        ACE_DEBUG ((LM_DEBUG, "OK\n"));
        ++number;
      }
      else
      {
        ACE_DEBUG ((LM_DEBUG, "WRONG TYPE\n"));
        throw INTERNAL ();
      }
    }

    return number;
  }

  void getAndSetMySlot (
    PortableInterceptor::ServerRequestInfo_ptr ri,
    PortableInterceptor::Current_ptr pi_current,
    char const *location,
    CORBA::Long const correctTSCvalue,
    CORBA::Long const correctRSCvalue)
  {
    CORBA::Long const number =
      getMySlot (
        ri,
        pi_current,
        location,
        correctTSCvalue,
        correctRSCvalue);

    if (number && (0 != std::strcmp (location, "receive_request")))
    {
      // Insert data into the RSC (request scope current).
      CORBA::Any data;
      data <<= number;

      ACE_DEBUG ((LM_DEBUG, "SERVER %C -> set_slot(RSC)", location));
      ri->set_slot (slot_id, data);

      ACE_DEBUG ((LM_DEBUG, " -> long (%d)\n", number));

      getMySlot (
          ri,
          pi_current,
          location,
          correctTSCvalue,
          number);
    }
  }
}

//
class InvokeMeImpl : public virtual POA_InvokeMe
{
public:
  explicit InvokeMeImpl (ORB_ptr orb)
      : orb_ (ORB::_duplicate (orb))
  {
  }

  void invoke_me () override
  {
    ACE_DEBUG ((LM_DEBUG, "invoke_me\n"));
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override
  {
    ACE_DEBUG ((LM_DEBUG, "\nServer is shutting down.\n"));
    this->orb_->shutdown (false);
  }
  //FUZZ: enable check_for_lack_ACE_OS

private:
  InvokeMeImpl (InvokeMeImpl const &) = delete;
  InvokeMeImpl (InvokeMeImpl &&) = delete;
  InvokeMeImpl & operator = (InvokeMeImpl const &) = delete;
  InvokeMeImpl & operator = (InvokeMeImpl &&) = delete;

  ORB_var orb_;
};


//
class ReplicaController : public virtual ServerRequestInterceptor,
                          public virtual ::CORBA::LocalObject
{
public:
  explicit ReplicaController (PortableInterceptor::Current_ptr pi_current)
    : pi_current_ (PortableInterceptor::Current::_duplicate (pi_current))
  {
  }

  char * name () override
  {
    return string_dup ("ReplicaController");
  }

  void destroy () override
  {
  }

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
  void tao_ft_interception_point (ServerRequestInfo_ptr ri, OctetSeq_out) override
  {
    getAndSetMySlot (ri, this->pi_current_.in (), "tao_ft_interception_point", 0, 0);
  }
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

  void receive_request_service_contexts (ServerRequestInfo_ptr ri) override
  {
#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
    getAndSetMySlot (ri, this->pi_current_.in (), "receive_request_service_contexts", 0, 61);
#else
    getAndSetMySlot (ri, this->pi_current_.in (), "receive_request_service_contexts", 0, 0);
#endif /* TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1 */
  }

  void receive_request (ServerRequestInfo_ptr ri) override
  {
    getAndSetMySlot (ri, this->pi_current_.in (), "receive_request", 62, 62);
  }

  void send_reply (ServerRequestInfo_ptr ri) override
  {
    getAndSetMySlot (ri, this->pi_current_.in (), "send_reply", 62, 62);
  }

  void send_exception (ServerRequestInfo_ptr ri) override
  {
    getAndSetMySlot (ri, this->pi_current_.in (), "send_exception", 62, 62);
  }

  void send_other (ServerRequestInfo_ptr ri) override
  {
    getAndSetMySlot (ri, this->pi_current_.in (), "send_other", 62, 62);
  }

  ReplicaController (ReplicaController const &) = delete;
  ReplicaController (ReplicaController &&) = delete;
  ReplicaController & operator = (ReplicaController const &) = delete;
  ReplicaController & operator = (ReplicaController &&) = delete;

private:
  PortableInterceptor::Current_var pi_current_;
};

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

  ORB_Initializer (ORB_Initializer const &) = delete;
  ORB_Initializer (ORB_Initializer &&) = delete;
  ORB_Initializer & operator = (ORB_Initializer const &) = delete;
  ORB_Initializer & operator = (ORB_Initializer &&) = delete;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ORBInitializer_var const orb_initializer (new ORB_Initializer ());
  register_orb_initializer (orb_initializer.in ());

  ORB_var const orb (ORB_init (argc, argv));

  if (parse_args (argc, argv) != 0)
    return 1;

  Object_var const obj (orb->resolve_initial_references ("RootPOA"));

  POA_var const root_poa (POA::_narrow (obj.in ()));
  POAManager_var const poa_manager (root_poa->the_POAManager ());

  InvokeMeImpl * const impl = new InvokeMeImpl (orb.in ());
  // Transfer the initial servant reference to the owning var.
  ServantBase_var impl_var (impl);

  PortableServer::ObjectId_var const id_act =
    root_poa->activate_object (impl);

  CORBA::Object_var const object = root_poa->id_to_reference (id_act.in ());

  InvokeMe_var const ref = InvokeMe::_narrow (object.in ());
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

  return 0;
}
