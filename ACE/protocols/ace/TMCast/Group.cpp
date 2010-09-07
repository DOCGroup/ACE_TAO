// $Id$
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

#include "Group.hpp"

#include <typeinfo>

// OS primitives
#include <ace/OS.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/Synch.h>
#include <ace/Time_Value.h>
#include <ace/SOCK_Dgram_Mcast.h>

#include "Messaging.hpp"

#include "Protocol.hpp"

// Components

#include "LinkListener.hpp"
#include "FaultDetector.hpp"
#include "TransactionController.hpp"

namespace ACE_TMCast
{
  bool
  operator== (std::type_info const* pa, std::type_info const& b)
  {
    return *pa == b;
  }

  //
  //
  //
  class Terminate : public virtual Message {};


  //
  //
  //
  class Failure : public virtual Message {};


  //
  //
  //
  class Scheduler
  {
  public:
    Scheduler (ACE_INET_Addr const& addr,
               char const* id,
               MessageQueue& out_send_data,
               MessageQueue& out_recv_data,
               MessageQueue& out_control)

        : cond_ (mutex_),

          addr_ (addr),
          sock_ (),

          out_control_ (out_control),

          in_data_ (mutex_),
          in_link_data_(mutex_),
          in_control_ (mutex_),

          sync_schedule (ACE_OS::gettimeofday ()),

          transaction_controller_ (in_data_, out_send_data, out_recv_data)
    {
      ACE_OS::strncpy (id_, id, Protocol::MEMBER_ID_LENGTH);
      id_[Protocol::MEMBER_ID_LENGTH - 1] = '\0';

      sock_.set_option (IP_MULTICAST_TTL, 32); // @@ ttl is hardcoded

      in_data_.subscribe (cond_);
      in_link_data_.subscribe (cond_);
      in_control_.subscribe (cond_);

      ACE_thread_t unused;
      if (ACE_OS::thr_create (&thread_thunk,
                              this,
                              THR_JOINABLE,
                              &unused,
                              &thread_) != 0) ACE_OS::abort ();
    }

    virtual ~Scheduler ()
    {
      {
        MessageQueueAutoLock lock (in_control_);

        in_control_.push (MessagePtr (new Terminate));
      }

      if (ACE_OS::thr_join (thread_, 0) != 0) ACE_OS::abort ();

      // cerr << "Scheduler is down." << endl;
    }

  public:
    MessageQueue&
    in_data ()
    {
      return in_data_;
    }

  private:
    static ACE_THR_FUNC_RETURN
    thread_thunk (void* arg)
    {
      Scheduler* obj = reinterpret_cast<Scheduler*> (arg);
      obj->execute ();
      return 0;
    }

    void
    execute ()
    {
      try
      {
        sock_.join (addr_);
        auto_ptr<LinkListener> ll (new LinkListener (sock_, in_link_data_));

        {
          AutoLock lock (mutex_);

          // Loop
          //
          //

          while (true)
          {
            cond_.wait (&sync_schedule);

            // "Loop of Fairness"

            bool done = false;

            do
            {
              // control message
              //
              //
              if (!in_control_.empty ())
              {
                done = true;
                break;
              }

              // outsync
              //
              //
              if (sync_schedule < ACE_OS::gettimeofday ())
              {
                // OUTSYNC

                outsync ();

                // schedule next outsync
                sync_schedule =
                  ACE_OS::gettimeofday () +
                  ACE_Time_Value (0, Protocol::SYNC_PERIOD);
              }

              // link message
              //
              //
              if (!in_link_data_.empty ())
              {
                MessagePtr m (in_link_data_.front ());
                in_link_data_.pop ();

                std::type_info const* exp = &typeid (*m);

                if (exp == typeid (LinkFailure))
                {
                  // cerr << "link failure" << endl;
                  throw false;
                }
                else if (exp == typeid (LinkData))
                {

                  LinkData* data = dynamic_cast<LinkData*> (m.get ());

                  // INSYNC, TL, CT

                  // Filter out loopback.
                  //
                  if (ACE_OS::strcmp (data->header().member_id.id, id_) != 0)
                  {
                    insync ();
                    transaction_list ();
                    current_transaction (data->header().current,
                                         data->payload (),
                                         data->size ());
                  }
                }
                else
                {
                  // cerr << "unknown message type from link listener: "
                  //      << typeid (*m).name () << endl;
                  ACE_OS::abort ();
                }
              }

              // api message
              //
              //
              if (!in_data_.empty ())
              {
                // API

                api ();
              }

            } while (!in_link_data_.empty() ||
                     sync_schedule < ACE_OS::gettimeofday ());

            if (done) break;
          }
        }
      }
      catch (...)
      {
        // cerr << "Exception in scheduler loop." << endl;
        MessageQueueAutoLock lock (out_control_);
        out_control_.push (MessagePtr (new Failure));
      }
    }

    // Events
    //
    // Order:
    //
    // INSYNC, TSL, VOTE, BEGIN
    // API
    // OUTSYNC
    //

    void
    insync ()
    {
      fault_detector_.insync ();
    }

    void
    outsync ()
    {
      char buf[Protocol::MAX_MESSAGE_SIZE];

      Protocol::MessageHeader* hdr =
        reinterpret_cast<Protocol::MessageHeader*> (buf);

      void* data = buf + sizeof (Protocol::MessageHeader);

      hdr->length = sizeof (Protocol::MessageHeader);
      hdr->check_sum = 0;

      ACE_OS::strcpy (hdr->member_id.id, id_);

      size_t size (0);

      transaction_controller_.outsync (hdr->current, data, size);

      hdr->length += size;

      fault_detector_.outsync ();

      // sock_.send (buf, hdr->length, addr_);
      sock_.send (buf, hdr->length);
    }

    void
    transaction_list ()
    {
    }

    void
    current_transaction (Protocol::Transaction const& t,
                         void const* payload,
                         size_t size)
    {
      transaction_controller_.current_transaction (t, payload, size);
    }

    void
    api ()
    {
      transaction_controller_.api ();
    }

  private:
    ACE_hthread_t thread_;

    ACE_Thread_Mutex mutex_;
    ACE_Condition<ACE_Thread_Mutex> cond_;

// FUZZ: disable check_for_ACE_Guard
    typedef ACE_Guard<ACE_Thread_Mutex> AutoLock;
// FUZZ: enable check_for_ACE_Guard

    char id_[Protocol::MEMBER_ID_LENGTH];

    ACE_INET_Addr addr_;
    ACE_SOCK_Dgram_Mcast sock_;

    MessageQueue& out_control_;

    MessageQueue  in_data_;
    MessageQueue  in_link_data_;
    MessageQueue  in_control_;

    // Protocol state
    //
    //

    ACE_Time_Value sync_schedule;

    FaultDetector fault_detector_;
    TransactionController transaction_controller_;
  };


  //
  //
  //
  class Group::GroupImpl
  {
  public:
    virtual ~GroupImpl ()
    {
    }

    GroupImpl (ACE_INET_Addr const& addr, char const* id)
        : send_cond_ (mutex_),
          recv_cond_ (mutex_),
          failed_ (false),
          in_send_data_ (mutex_),
          in_recv_data_ (mutex_),
          in_control_ (mutex_),
          scheduler_ (new  Scheduler (addr,
                                      id,
                                      in_send_data_,
                                      in_recv_data_,
                                      in_control_)),
          out_data_ (scheduler_->in_data ())
    {
      in_send_data_.subscribe (send_cond_);
      in_recv_data_.subscribe (recv_cond_);

      in_control_.subscribe (send_cond_);
      in_control_.subscribe (recv_cond_);
    }

    //FUZZ: disable check_for_lack_ACE_OS
    void send (void const* msg, size_t size)
    {
    //FUZZ: enable check_for_lack_ACE_OS

      if (size > Protocol::MAX_PAYLOAD_SIZE) throw InvalidArg ();

      // Note the potential deadlock if I lock mutex_ and out_data_ in
      // reverse order.

      MessageQueueAutoLock l1 (out_data_);
      AutoLock l2 (mutex_);

      throw_if_failed ();

      out_data_.push (MessagePtr (new Send (msg, size)));

      l1.unlock (); // no need to keep it locked

      while (true)
      {
        throw_if_failed ();

        if (!in_send_data_.empty ())
        {
          MessagePtr m (in_send_data_.front ());
          in_send_data_.pop ();

          std::type_info const* exp = &typeid (*m);

          if (exp == typeid (ACE_TMCast::Aborted))
          {
            throw Group::Aborted ();
          }
          else if (exp == typeid (Commited))
          {
            return;
          }
          else
          {
            // cerr << "send: group-scheduler messaging protocol violation; "
            //     << "unexpected message " << typeid (*m).name ()
            //     << " " << typeid (Aborted).name () << endl;

            ACE_OS::abort ();
          }
        }

        // cerr << "send: waiting on condition" << endl;
        send_cond_.wait ();
        // cerr << "send: wokeup on condition" << endl;
      }
    }

    //FUZZ: disable check_for_lack_ACE_OS
    size_t recv (void* msg, size_t size)
    {
    //FUZZ: enable check_for_lack_ACE_OS

      AutoLock lock (mutex_);

      while (true)
      {
        throw_if_failed ();

        if (!in_recv_data_.empty ())
        {
          MessagePtr m (in_recv_data_.front ());
          in_recv_data_.pop ();

          std::type_info const* exp = &typeid (*m);

          if (exp == typeid (Recv))
          {
            Recv* data = dynamic_cast<Recv*> (m.get ());

            if (size < data->size ()) 
              throw Group::InsufficienSpace ();

            ACE_OS::memcpy (msg, data->payload (), data->size ());

            return data->size ();
          }
          else
          {
            // cerr << "recv: group-scheduler messaging protocol violation. "
            //     << "unexpected message " << typeid (*m).name () << endl;

            ACE_OS::abort ();
          }
        }

        recv_cond_.wait ();
      }
    }

  private:
    void
    throw_if_failed ()
    {
      if (!failed_ && !in_control_.empty ()) failed_ = true;

      if (failed_) throw Group::Failed ();
    }

  private:
    ACE_Thread_Mutex mutex_;
    ACE_Condition<ACE_Thread_Mutex> send_cond_;
    ACE_Condition<ACE_Thread_Mutex> recv_cond_;

    // FUZZ: disable check_for_ACE_Guard
    typedef ACE_Guard<ACE_Thread_Mutex> AutoLock;
    // FUZZ: enable check_for_ACE_Guard

    bool failed_;

    MessageQueue  in_send_data_;
    MessageQueue  in_recv_data_;
    MessageQueue  in_control_;

    auto_ptr<Scheduler> scheduler_;

    MessageQueue& out_data_;
  };


  // Group
  //
  //
  Group::
  Group (ACE_INET_Addr const& addr, char const* id)
      : pimpl_ (new GroupImpl (addr, id))
  {
  }

  Group::
  ~Group ()
  {
  }

  void
  Group::send (void const* msg, size_t size)
  {
    pimpl_->send (msg, size);
  }

  size_t
  Group::recv (void* msg, size_t size)
  {
    return pimpl_->recv (msg, size);
  }
}

