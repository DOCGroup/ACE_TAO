// author    : Boris Kolpackov <boris@kolpackov.net>
// $Id$

#include "ace/Time_Value.h"     // ACE_Time_Value
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"   // abort
#include "ace/OS_NS_sys_time.h" // gettimeofday

#include "Acknowledge.h"

namespace ACE_RMCast
{
  Acknowledge::
  Acknowledge (Parameters const& params)
      : params_ (params),
        hold_ (params.addr_map_size ()),
        cond_ (mutex_),
        nrtm_timer_ (params_.nrtm_timeout ()),
        stop_ (false)
  {
  }

  void Acknowledge::
  in_start (In_Element* in)
  {
    Element::in_start (in);
  }

  void Acknowledge::
  out_start (Out_Element* out)
  {
    Element::out_start (out);

    tracker_mgr_.spawn (track_thunk, this);
  }

  void Acknowledge::
  out_stop ()
  {
    {
      Lock l (mutex_);
      stop_ = true;
      cond_.signal ();
    }

    tracker_mgr_.wait ();

    Element::out_stop ();
  }

  void Acknowledge::
  collapse (Queue& q)
  {
    // I would normally use iterators in the logic below but ACE_Map_Manager
    // iterates over entries in no particular order so it is pretty much
    // unusable here. Instead we will do slow and cumbersome find's.
    //

    u64 sn (q.sn () + 1);

    for (;; ++sn)
    {
      Queue::ENTRY* e = 0;

      if (q.find (sn, e) == -1 || e->int_id_.lost ()) break;

      Message_ptr m (e->int_id_.msg ());
      q.unbind (sn);

      in_->recv (m);
    }

    q.sn (sn - 1);
  }

  void Acknowledge::
  track ()
  {
    while (true)
    {
      Messages msgs;

      {
        Lock l (mutex_);

        if (stop_)
          break;

        if (hold_.current_size () != 0)
        {
          for (Map::iterator i (hold_.begin ()), e (hold_.end ());
               i != e;
               ++i)
          {
            Queue& q = (*i).int_id_;

            if (q.current_size () == 0) continue;

            track_queue ((*i).ext_id_, q, msgs);
          }
        }

        if (--nrtm_timer_ == 0)
        {
          nrtm_timer_ = params_.nrtm_timeout ();

          // Send NRTM.
          //
          unsigned short max_payload_size (
            params_.max_packet_size () - max_service_size);

          u32 max_elem (NRTM::max_count (max_payload_size));

          Profile_ptr nrtm (create_nrtm (max_elem));

          if (!nrtm.null ())
          {
            Message_ptr m (new Message);
            m->add (nrtm);
            msgs.push_back (m);

          }
        }
      }

      // Send stuff off.
      //
      for (Messages::Iterator i (msgs); !i.done (); i.advance ())
      {
        Message_ptr* ppm;
        i.next (ppm);

        //FUZZ: disable check_for_lack_ACE_OS
        send (*ppm);
        //FUZZ: enable check_for_lack_ACE_OS
      }

      // Go to sleep but watch for "manual cancellation" request.
      //
      {
        //FUZZ: disable check_for_lack_ACE_OS
        ACE_Time_Value time (ACE_OS::gettimeofday ());
        //FUZZ: enable check_for_lack_ACE_OS

        time += params_.tick ();

        Lock l (mutex_);

        while (!stop_)
        {
          if (cond_.wait (&time) == -1)
          {
            if (errno != ETIME)
              ACE_OS::abort ();
            else
              break;
          }
        }

        if (stop_)
          break;
      }
    }
  }

  void Acknowledge::
  track_queue (Address const& addr, Queue& q, Messages& msgs)
  {
    unsigned short max_payload_size (
      params_.max_packet_size () - max_service_size);

    u32 max_elem (NAK::max_count (max_payload_size));
    u32 count (0);

    Queue::iterator i (q.begin ()), e (q.end ());

    // Track existing losses.
    //
    while (i != e)
    {
      auto_ptr<NAK> nak (new NAK (addr));

      // Inner loop that fills NAK profile with up to max_elem elements.
      //
      for (; i != e && nak->count () < max_elem; ++i)
      {
        u64 sn ((*i).ext_id_);
        Descr& d = (*i).int_id_;

        if (d.lost ())
        {
          d.timer (d.timer () - 1);

          if (d.timer () == 0)
          {
            //@@ Need exp fallback.
            //
            d.nak_count (d.nak_count () + 1);
            d.timer ((d.nak_count () + 1) * params_.nak_timeout ());

            nak->add (sn);

            ++count;

            // cerr << 6 << "NAK # " << d.nak_count () << ": "
            // << addr << " " << sn << endl;
          }
        }
      }

      // Send this NAK.
      //
      if (nak->count ())
      {
        // cerr << 5 << "NAK: " << addr << " " << nak->count () << " sns"
        //     << endl;

        Message_ptr m (new Message);

        m->add (Profile_ptr (nak.release ()));

        msgs.push_back (m);
      }
    }

    // Detect and record new losses.
    //
    for (u64 sn (q.sn () + 1), end (q.max_sn ()); sn < end; ++sn)
    {
      if (q.find (sn) == -1)
      {
        q.bind (sn, Descr (1));
      }
    }
  }

  void Acknowledge::recv (Message_ptr m)
  {
    // Handle NRTM. There could be some nasty interaction with code
    // that handles data below (like missing message and NAK). This
    // is why I hold the lock at the beginning (which may be not very
    // efficient).
    //
    Lock l (mutex_);

    if (NRTM const* nrtm = static_cast<NRTM const*> (m->find (NRTM::id)))
    {
      for (Map::iterator i (hold_.begin ()), e (hold_.end ()); i != e; ++i)
      {
        u64 sn (nrtm->find ((*i).ext_id_));

        if (sn != 0)
        {
          Queue& q = (*i).int_id_;

          u64 old (q.max_sn ());

          if (old < sn)
          {
            // Mark as lost.
            //
            q.bind (sn, Descr (1));
          }
        }
      }
    }

    if (m->find (Data::id) || m->find (NoData::id))
    {
      Address from (
        static_cast<From const*> (m->find (From::id))->address ());

      u64 sn (static_cast<SN const*> (m->find (SN::id))->num ());

      Map::ENTRY* e;

      if (hold_.find (from, e) == -1)
      {
        // First message from this source.
        //
        hold_.bind (from, Queue (sn));
        in_->recv (m);
      }
      else
      {
        Queue& q = e->int_id_;

        if (sn <= q.sn ())
        {
          // Duplicate.
          //
          //cerr << 6 << "DUP " << from << " " << q.sn () << " >= " << sn
          //     << endl;
        }
        else if (sn == q.sn () + 1)
        {
          // Next message.
          //

          q.rebind (sn, Descr (m));
          collapse (q);
        }
        else
        {
          // Some messages are missing. Insert this one into the queue.
          //
          q.rebind (sn, Descr (m));
        }
      }
    }
    else
    {
      l.release ();

      // Just forward it up.
      //
      in_->recv (m);
    }
  }

  void Acknowledge::send (Message_ptr m)
  {
    if (Data const* data = static_cast<Data const*> (m->find (Data::id)))
    {
      size_t max_payload_size (
        params_.max_packet_size () - max_service_size);

      if (max_payload_size > data->size ())
      {
        u32 max_size (max_payload_size - data->size ());
        u32 max_elem (NRTM::max_count (max_size));

        if (max_elem > 0)
        {
          Lock l (mutex_);

          Profile_ptr nrtm (create_nrtm (max_elem));

          if (nrtm.get ())
            m->add (nrtm);
        }
      }

      nrtm_timer_ = params_.nrtm_timeout (); // Reset timer.
    }

    out_->send (m);
  }

  Profile_ptr Acknowledge::
  create_nrtm (u32 max_elem)
  {
    // Prepare NRTM.
    //
    auto_ptr<NRTM> nrtm (new NRTM ());

    // Gather the information.
    //
    {
      for (Map::iterator i (hold_.begin ()), e (hold_.end ()); i != e; ++i)
      {
        Address addr ((*i).ext_id_);
        Queue& q = (*i).int_id_;

        //@@ Should look for the highest known number.
        //
        nrtm->insert (addr, q.sn ());

        if (--max_elem == 0)
          break;
      }
    }

    if (nrtm->empty ())
      return Profile_ptr (0);
    else
      return Profile_ptr (nrtm.release ());
  }

  ACE_THR_FUNC_RETURN Acknowledge::
  track_thunk (void* obj)
  {
    reinterpret_cast<Acknowledge*> (obj)->track ();
    return 0;
  }
}
