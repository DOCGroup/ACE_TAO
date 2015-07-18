#include "TestC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Task.h"
#include "ace/Thread.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/ORBInitializer_Registry.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int number_of_tries = 5;
int ofs = 100;
TAO_SYNCH_MUTEX ofs_lock;
PortableInterceptor::Current_var pic;
PortableInterceptor::SlotId slot_id;
const IOP::ServiceId service_id = 0xdeadbeef;

int get_offset (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, ofs_lock, 0);
  int r = ofs;
  ofs += 100;
  return r;
}

class Worker : public ACE_Task_Base
{
public:
  Messenger_var messenger;

  Worker (Messenger_ptr m)
    : messenger(Messenger::_duplicate (m)) {}

  int svc (void)
  {
    int offset = get_offset ();
    CORBA::String_var message = CORBA::string_dup( "Hello!" );
    char user [100];
    ACE_OS::snprintf(user, 100, "client %d", offset / 100);
    int try_count = 0;
    for (; try_count < number_of_tries; ++try_count)
      {
        CORBA::Any in;
        in <<= CORBA::ULong (try_count + offset);
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) set slot data %d\n"),
                    try_count + offset));
        pic->set_slot (slot_id, in);
        try
          {
            messenger->send_message( user, "Test 1", message.inout() );

            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT("(%P|%t) received response for ")
                        ACE_TEXT("message <%d> to server\n"),
                        try_count));
          }
        catch (const CORBA::Exception& ex)
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT("(%P|%t) ignoring %C\n"),
                        ex._name ()));
          }
      }
    return 0;
  }

};


class Client_Req_Int
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:
  virtual char * name (void)
  {
    return CORBA::string_dup ("Client_Req_Int");
  }

  virtual void destroy (void)
  {
  }

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr)
  {
  }

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr)
  {
  }

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr)
  {
  }

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr)
  {
  }

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
  {

    if (CORBA::is_nil (this->pic_.in()))
      {
        ACE_GUARD (TAO_SYNCH_MUTEX, g, this->lock_);
        if (CORBA::is_nil (this->pic_.in()))
          {
            int argc = 0;
            ACE_TCHAR **argv = 0;
            CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
            CORBA::Object_var obj = orb->resolve_initial_references ("PICurrent");
            this->pic_ = PortableInterceptor::Current::_narrow (obj.in ());
          }
      }

    CORBA::String_var op = ri->operation ();
    if (ACE_OS::strcmp (op.in (), "send_message") != 0)
      return;

    CORBA::ULong v (0);
    CORBA::Any_var out = this->pic_->get_slot (slot_id);
    if (out >>= v)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) send_request got %u\n"), v));
        IOP::ServiceContext sc;
        sc.context_id = ::service_id;

        ri->add_request_service_context (sc, 0);
      }
    else
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("(%P|%t) send_request failed to get slot data\n")));
        ACE_ASSERT (0);
      }
  }
private:
  TAO_SYNCH_MUTEX lock_;
  PortableInterceptor::Current_var pic_;

};

class ORB_Initializer : public virtual PortableInterceptor::ORBInitializer,
                        public virtual ::CORBA::LocalObject
{
public:
  virtual void
  pre_init (PortableInterceptor::ORBInitInfo_ptr)
  {
  }

  virtual void
  post_init (PortableInterceptor::ORBInitInfo_ptr info)
  {
    slot_id = info->allocate_slot_id ();
    ACE_DEBUG ((LM_DEBUG, "Allocated slot with id %d.\n", slot_id));
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
      PortableInterceptor::ClientRequestInterceptor::_nil ();

    // Install the Echo client request interceptor
    ACE_NEW_THROW_EX (interceptor,
                      Client_Req_Int (),
                      CORBA::NO_MEMORY ());

    PortableInterceptor::ClientRequestInterceptor_var
      client_interceptor = interceptor;

    info->add_client_request_interceptor (client_interceptor.in ());

  }
};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage:  %s ")
                           ACE_TEXT("-k <ior> ")
                           ACE_TEXT("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_var orb_initializer =
        new ORB_Initializer ();
      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj = orb->string_to_object( ior );
      Messenger_var messenger = Messenger::_narrow( obj.in() );

      obj = orb->resolve_initial_references ("PICurrent");
      pic = PortableInterceptor::Current::_narrow (obj.in ());

      Worker w (messenger.in ());
      w.activate (THR_JOINABLE | THR_NEW_LWP, 3);
      w.wait ();

      orb->destroy ();
    }
  catch(const CORBA::Exception& ex) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Client main() Caught CORBA::Exception: %C\n"),
                ex._name ()));
    return 1;
  }

  return 0;
}
