// $Id$
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Synch.h"
#include "ace/Bound_Ptr.h"

#include "Protocol.hpp"
#include "Messaging.hpp"

#include <typeinfo>

namespace ACE_TMCast
{

  // Messages
  //
  //
  class Send : public virtual Message
  {
  public:
    Send (void const* msg, size_t size)
        : size_ (size)
    {
      ACE_OS::memcpy (payload_, msg, size_);
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
    size_t size_;
    char payload_[Protocol::MAX_PAYLOAD_SIZE];
  };

  typedef
  ACE_Strong_Bound_Ptr<Send, ACE_SYNCH_MUTEX>
  SendPtr;


  class Recv : public virtual Message
  {
  public:
    Recv (void const* msg, size_t size)
        : size_ (size)
    {
      ACE_OS::memcpy (payload_, msg, size_);
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
    size_t size_;
    char payload_[Protocol::MAX_PAYLOAD_SIZE];
  };

  typedef
  ACE_Strong_Bound_Ptr<Recv, ACE_SYNCH_MUTEX>
  RecvPtr;

  class Aborted : public virtual Message {};

  class Commited : public virtual Message {};


  //
  //
  //
  class TransactionController
  {
  public:
    TransactionController (MessageQueue& in,
                           MessageQueue& send_out,
                           MessageQueue& recv_out)
        : trace_ (false),
          voting_duration_ (0),
          separation_duration_ (0),
          in_ (in),
          send_out_ (send_out),
          recv_out_ (recv_out)
    {
      current_.id = 0;
      current_.status = Protocol::TS_COMMITED;
    }

  public:
    class Failure {};


    void
    outsync (Protocol::Transaction& c, void* payload, size_t& size)
    {
      if (current_.status == Protocol::TS_COMMIT ||
          current_.status == Protocol::TS_ABORT)
      {
        if (++voting_duration_ >= Protocol::VOTING_FRAME)
        {
          // end of voting frame

          if (current_.status == Protocol::TS_COMMIT)
          {
            {
              if (initiated_)
              {
                MessageQueueAutoLock lock (send_out_);
                send_out_.push (MessagePtr (new Commited));
              }
              else // joined transaction
              {
                MessageQueueAutoLock lock (recv_out_);
                recv_out_.push (MessagePtr (recv_));
                recv_ = RecvPtr ();
              }
            }

            current_.status = Protocol::TS_COMMITED;

            // if (trace_) cerr << "commited transaction with id "
            //                  << current_.id << endl;
          }
          else // TS_ABORT
          {
            if (initiated_)
            {
              MessageQueueAutoLock lock (send_out_);
              send_out_.push (MessagePtr (new Aborted));
            }
            else
            {
              // free revc_ buffer if necessary
              //
              if (recv_.get ()) recv_ = RecvPtr ();
            }


            current_.status = Protocol::TS_ABORTED;

            // if (trace_) cerr << "aborted transaction with id "
            //                  << current_.id << endl;
          }

          // start transaction separation frame (counts down)
          // +1 because it will be decremented on this iteration
          separation_duration_ = Protocol::SEPARATION_FRAME + 1;
        }
      }

      // Set current outsync info

      c.id = current_.id;
      c.status = current_.status;


      // Do some post-processing

      switch (current_.status)
      {
      case Protocol::TS_COMMITED:
      case Protocol::TS_ABORTED:
        {
          if (separation_duration_ > 0) --separation_duration_;
          break;
        }
      case Protocol::TS_BEGIN:
        {
          // transfer payload

          size = send_->size ();
          ACE_OS::memcpy (payload, send_->payload (), size);

          send_ = SendPtr ();

          // get redy to vote for 'commit'

          current_.status = Protocol::TS_COMMIT;
          voting_duration_ = 0;
        }
      }
    }

    void
    current_transaction (Protocol::Transaction const& t,
                         void const* payload,
                         size_t size)
    {
      Protocol::TransactionId& id = current_.id;
      Protocol::TransactionStatus& s = current_.status;

      if (id == 0 && t.id != 0) // catch up
      {
        switch (t.status)
        {
        case Protocol::TS_BEGIN:
        case Protocol::TS_COMMIT:
        case Protocol::TS_ABORT:
          {
            id = t.id - 1;
            s = Protocol::TS_COMMITED;
            break;
          }
        case Protocol::TS_ABORTED:
        case Protocol::TS_COMMITED:
          {
            id = t.id;
            s = t.status;
            break;
          }
        }

        // if (trace_) cerr << "caught up with id " << id << endl;
      }

      bool stable (s == Protocol::TS_COMMITED || s == Protocol::TS_ABORTED);

      switch (t.status)
      {
      case Protocol::TS_BEGIN:
        {
          if (!stable || t.id != id + 1)
          {
            // Transaction is in progress or hole in transaction id's

            // cerr << "unexpected request to join " << t
            //     << " while on " << current_ << endl;

            // if (!stable) cerr << "voting progress is " << voting_duration_
            //                  << "/" << Protocol::VOTING_FRAME << endl;

            if (t.id == id) // collision
            {
              if (!stable && s != Protocol::TS_ABORT)
              {
                // abort both
                // cerr << "aborting both transactions" << endl;

                s = Protocol::TS_ABORT;
                voting_duration_ = 0; //@@ reset voting frame
              }
            }
            else
            {
              // @@ delicate case. need to think more

              // cerr << "Declaring node failed." << endl;
              throw Failure ();
            }
          }
          else
          {
            // join the transaction

            initiated_ = false;

            recv_ = RecvPtr (new Recv (payload, size));

            id = t.id;
            s = Protocol::TS_COMMIT;
            voting_duration_ = 0;

            // if (trace_) cerr << "joining-for-commit transaction with id "
            //                  << id << endl;
          }
          break;
        }
      case Protocol::TS_COMMIT:
        {
          if (stable && id == t.id - 1)
          {
            // not begin and and we haven't joined

            // join for abort

            initiated_ = false;

            current_.id = t.id;
            current_.status = Protocol::TS_ABORT;
            voting_duration_ = 0;

            // if (trace_) cerr << "joining-for-abort transaction with id "
            //                  << current_.id << endl;
          }
          break;
        }
      case Protocol::TS_ABORT:
        {
          if ((!stable && id == t.id && s == Protocol::TS_COMMIT) ||
              (stable && id == t.id - 1)) // abort current || new transaction
          {
            // if (trace_) cerr << "voting-for-abort on transaction with id "
            //                  << current_.id << endl;

            id = t.id;
            s = Protocol::TS_ABORT;

            voting_duration_ = 0; //@@ reseting voting_duration_
          }
          else
          {
          }

          break;
        }
      case Protocol::TS_ABORTED:
      case Protocol::TS_COMMITED:
        {
          // nothing for now
          break;
        }
      }
    }

    void
    api ()
    {
      if ((current_.status == Protocol::TS_COMMITED ||
           current_.status == Protocol::TS_ABORTED) &&
          separation_duration_ == 0) // no transaction in progress
      {
        // start new transaction

        // Note that in_ is already locked by Scheduler

        MessagePtr m (in_.front ());
        in_.pop ();

        if (typeid (*m) == typeid (Send))
        {
          send_ = SendPtr (m);
        }
        else
        {
          // cerr << "Expecting Send but received " << typeid (*m).name ()
          //      << endl;

          ACE_OS::abort ();
        }

        current_.id++;
        current_.status = Protocol::TS_BEGIN;

        initiated_ = true;

        // if (trace_) cerr << "starting transaction with id " << current_.id
        //                  << endl;
      }
    }

  private:
    // FUZZ: disable check_for_ACE_Guard
    typedef ACE_Guard<ACE_Thread_Mutex> AutoLock;
    // FUZZ: enable check_for_ACE_Guard

    bool trace_;

    Protocol::Transaction current_;

    bool initiated_;

    unsigned short voting_duration_;
    unsigned short separation_duration_;

    MessageQueue& in_;
    MessageQueue& send_out_;
    MessageQueue& recv_out_;

    SendPtr send_;
    RecvPtr recv_;
  };
}
