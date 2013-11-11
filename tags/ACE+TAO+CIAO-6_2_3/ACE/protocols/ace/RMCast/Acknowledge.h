// $Id$
// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_ACKNOWLEDGE_H
#define ACE_RMCAST_ACKNOWLEDGE_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Thread_Manager.h"

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"
#include "Parameters.h"

#if !defined (ACE_RMCAST_DEFAULT_MAP_SIZE)
#define ACE_RMCAST_DEFAULT_MAP_SIZE 10
#endif /* ACE_RMCAST_DEFAULT_MAP_SIZE */

#if !defined (ACE_RMCAST_DEFAULT_QUEUE_SIZE)
#define ACE_RMCAST_DEFAULT_QUEUE_SIZE 10
#endif /* ACE_RMCAST_DEFAULT_QUEUE_SIZE */

namespace ACE_RMCast
{
  class Acknowledge : public Element
  {
  public:
    Acknowledge (Parameters const& params);

    virtual void
    in_start (In_Element* in);

    virtual void
    out_start (Out_Element* out);

    virtual void
    out_stop ();

  public:
    virtual void
    recv (Message_ptr m);

    virtual void
    send (Message_ptr m);

  // Sun C++ 5.4 can't handle private here.
  //
  // private:
  public:
    struct Descr
    {
      //@@ There should be no default c-tor.
      //
      Descr ()
          : nak_count_ (0), timer_ (1)
      {
      }

      Descr (unsigned long timer)
          : nak_count_ (0), timer_ (timer)
      {
      }

      Descr (Message_ptr m)
          : m_ (m)
      {
      }

    public:
      bool
      lost () const
      {
        return m_.get () == 0;
      }

    public:
      Message_ptr
      msg ()
      {
        return m_;
      }

      void
      msg (Message_ptr m)
      {
        m_ = m;
      }

    public:
      unsigned long
      nak_count () const
      {
        return nak_count_;
      }

      void
      nak_count (unsigned long v)
      {
        nak_count_ = v;
      }

      unsigned long
      timer () const
      {
        return timer_;
      }

      void
      timer (unsigned long v)
      {
        timer_ = v;
      }

    private:
      Message_ptr m_;

      unsigned long nak_count_;
      unsigned long timer_;
    };

  private:
    struct Queue : ACE_Hash_Map_Manager<u64, Descr, ACE_Null_Mutex>
    {
      typedef ACE_Hash_Map_Manager<u64, Descr, ACE_Null_Mutex> Base;

      // Should never be here but required by ACE_Hash_Blah_Blah.
      //
      Queue ()
          : Base (ACE_RMCAST_DEFAULT_MAP_SIZE), sn_ (0), max_sn_ (0)
      {
      }

      Queue (u64 sn)
        : Base (ACE_RMCAST_DEFAULT_MAP_SIZE), sn_ (sn), max_sn_ (sn)
      {
      }

      Queue (Queue const& q)
        : Base (ACE_RMCAST_DEFAULT_MAP_SIZE), sn_ (q.sn_), max_sn_ (sn_)
      {
        for (Queue::const_iterator i (q), e (q, 1); i != e; ++i)
        {
          bind ((*i).ext_id_, (*i).int_id_);
        }
      }

    public:
      int
      bind (u64 sn, Descr const& d)
      {
        int r (Base::bind (sn, d));

        if (r == 0 && sn > max_sn_) max_sn_ = sn;

        return r;
      }

      int
      rebind (u64 sn, Descr const& d)
      {
        int r (Base::rebind (sn, d));

        if (r == 0 && sn > max_sn_) max_sn_ = sn;

        return r;
      }

      int
      unbind (u64 sn)
      {
        int r (Base::unbind (sn));

        if (r == 0 && sn == max_sn_)
        {
          for (--max_sn_; max_sn_ >= sn_; --max_sn_)
          {
            if (find (max_sn_) == 0) break;
          }
        }

        return r;
      }

    public:
      u64
      sn () const
      {
        return sn_;
      }

      void
      sn (u64 sn)
      {
        sn_ = sn;
      }

      u64
      max_sn () const
      {
        if (current_size () == 0) return sn_;

        return max_sn_;
      }

    private:
      u64 sn_, max_sn_;
    };

    typedef
    ACE_Hash_Map_Manager_Ex<Address,
                            Queue,
                            AddressHasher,
                            ACE_Equal_To<Address>,
                            ACE_Null_Mutex>
    Map;

  private:
    void
    collapse (Queue& q);

    void
    track ();

    void
    track_queue (Address const& addr, Queue& q, Messages& msgs);

    Profile_ptr
    create_nrtm (u32 max_elem);

    static ACE_THR_FUNC_RETURN
    track_thunk (void* obj);

  private:
    Parameters const& params_;

    Map hold_;
    Mutex mutex_;
    Condition cond_;

    unsigned long nrtm_timer_;

    bool stop_;
    ACE_Thread_Manager tracker_mgr_;
  };
}

#endif  // ACE_RMCAST_ACKNOWLEDGE_H
