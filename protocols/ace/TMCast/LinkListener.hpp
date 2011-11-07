// $Id$
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

// OS primitives
#include <ace/OS_NS_string.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/Synch.h>
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/Refcounted_Auto_Ptr.h>


#include "Messaging.hpp"
#include "Protocol.hpp"

namespace ACE_TMCast
{
  class LinkFailure : public virtual Message {};

  class LinkData : public virtual Message
  {
  public:
    LinkData (Protocol::MessageHeader const* header,
              void* payload,
              size_t size)
        : size_ (size)
    {
      ACE_OS::memcpy (&header_, header, sizeof (Protocol::MessageHeader));
      ACE_OS::memcpy (payload_, payload, size_);
    }

    Protocol::MessageHeader const&
    header () const
    {
      return header_;
    }

    void const*
    payload () const
    {
      return payload_;
    }

    size_t
    size () const
    {
      return size_;
    }

  private:
    Protocol::MessageHeader header_;
    char payload_[Protocol::MAX_MESSAGE_SIZE];
    size_t size_;
  };

  typedef
  ACE_Refcounted_Auto_Ptr<LinkData, ACE_Null_Mutex>
  LinkDataPtr;

  class LinkListener
  {
  private:
    class Terminate : public virtual Message {};

  public:
    LinkListener (ACE_SOCK_Dgram_Mcast& sock, MessageQueue& out)
        : sock_(sock), out_ (out)
    {
      ACE_thread_t unused;
      if (ACE_OS::thr_create (&thread_thunk,
                              this,
                              THR_JOINABLE,
                              &unused,
                              &thread_) != 0) ACE_OS::abort ();
    }

    ~LinkListener ()
    {
      {
        MessageQueueAutoLock lock (control_);

        control_.push (MessagePtr (new Terminate));
      }

      if (ACE_OS::thr_join (thread_, 0) != 0) ACE_OS::abort ();

      // cerr << "Link listener is down." << endl;
    }


    static ACE_THR_FUNC_RETURN
    thread_thunk (void* arg)
    {
      LinkListener* obj = reinterpret_cast<LinkListener*> (arg);

      obj->execute ();
      return 0;
    }

    void
    execute ()
    {
      char msg[Protocol::MAX_MESSAGE_SIZE];

      ssize_t header_size = sizeof (Protocol::MessageHeader);

      // OS::Time timeout (1000000); // one millisecond

      ACE_Time_Value timeout (0, 1000); // one millisecond

      try
      {
        while (true)
        {
          // Check control message queue

          {
            MessageQueueAutoLock lock (control_);

            if (!control_.empty ()) break;
          }

          ACE_INET_Addr junk;
          ssize_t n = sock_.recv (msg,
                                  Protocol::MAX_MESSAGE_SIZE,
                                  junk,
                                  0,
                                  &timeout);

          if (n != -1)
          {
            if (n < header_size) throw false;

            Protocol::MessageHeader* header =
              reinterpret_cast<Protocol::MessageHeader*> (msg);

            MessageQueueAutoLock lock (out_);

            out_.push (MessagePtr (new LinkData (header,
                                                 msg + header_size,
                                                 n - header_size)));
          }
        }
      }
      catch (...)
      {
        MessageQueueAutoLock lock (out_);

        out_.push (MessagePtr (new LinkFailure));
      }
    }

  private:
    // FUZZ: disable check_for_ACE_Guard
    typedef ACE_Guard<ACE_Thread_Mutex> AutoLock;
    // FUZZ: enable check_for_ACE_Guard

    ACE_hthread_t thread_;
    ACE_SOCK_Dgram_Mcast& sock_;
    MessageQueue& out_;
    MessageQueue  control_;
  };
}
