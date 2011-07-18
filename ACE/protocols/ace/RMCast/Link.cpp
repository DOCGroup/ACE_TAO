// author    : Boris Kolpackov <boris@kolpackov.net>
// $Id$

#include "ace/Time_Value.h"        // ACE_Time_Value
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_sys_socket.h"

#include "Link.h"

namespace ACE_RMCast
{
  Link::
  ~Link ()
  {
    ssock_.close ();
    rsock_.close ();
  }

  Link::
  Link (Address const& addr, Parameters const& params)
      : params_ (params),
        addr_ (addr),
        ssock_ (Address (static_cast<unsigned short> (0),
                         static_cast<ACE_UINT32> (INADDR_ANY)),
                AF_INET,
                IPPROTO_UDP,
                1),
        stop_ (false)

  {
    ACE_OS::srand ((unsigned int) ACE_OS::time (0));


    rsock_.set_option (IP_MULTICAST_LOOP, 0);
    // rsock_.set_option (IP_MULTICAST_TTL, 0);

    // Set recv/send buffers.
    //
    {
      int r (131070);
      int s (sizeof (r));

      static_cast<ACE_SOCK&> (rsock_).set_option (
        SOL_SOCKET, SO_RCVBUF, &r, s);

      static_cast<ACE_SOCK&> (ssock_).set_option (
        SOL_SOCKET, SO_RCVBUF, &r, s);

      rsock_.get_option (SOL_SOCKET, SO_RCVBUF, &r, &s);
      //cerr << 5 << "recv buffer size: " << r << endl;

      ssock_.get_option (SOL_SOCKET, SO_RCVBUF, &r, &s);
      //cerr << 5 << "send buffer size: " << r << endl;

    }

    // Bind address and port.
    //
    if (ACE_OS::connect (ssock_.get_handle (),
                         reinterpret_cast<sockaddr*> (addr_.get_addr ()),
                         addr_.get_addr_size ()) == -1)
    {
      ACE_OS::perror ("connect: ");
      ACE_OS::abort ();
    }


    ssock_.get_local_addr (self_);

    //cerr << 5 << "self: " << self_ << endl;
  }

  void Link::
  in_start (In_Element* in)
  {
    Element::in_start (in);

    rsock_.join (addr_);

    // Start receiving thread.
    //
    recv_mgr_.spawn (recv_thunk, this);
  }

  void Link::
  out_start (Out_Element* out)
  {
    Element::out_start (out);
  }

  void Link::
  in_stop ()
  {
    // Stop receiving thread.
    //
    {
      Lock l (mutex_);
      stop_ = true;
    }
    recv_mgr_.wait ();

    Element::in_stop ();
  }

  void Link::send (Message_ptr m)
  {
    // Simulate message loss and reordering.
    //
    if (params_.simulator ())
    {
      if ((ACE_OS::rand () % 17) != 0)
      {
        Lock l (mutex_);

        if (hold_.get ())
        {
          send_ (m);
          send_ (hold_);
          hold_ = Message_ptr (0);
        }
        else
        {
          if ((ACE_OS::rand () % 17) != 0)
          {
            send_ (m);
          }
          else
          {
            hold_ = m;

            // Make a copy in M so that the reliable loop below
            // won't add FROM and TO to HOLD_.
            //
            m = hold_->clone ();
          }
        }
      }
    }
    else
      send_ (m);

    // Reliable loop.
    //
    m->add (Profile_ptr (new From (self_)));
    m->add (Profile_ptr (new To (self_)));

    in_->recv (m);
  }

  void Link::
  send_ (Message_ptr m)
  {
    ostream os (m->size (), 1); // Always little-endian.

    os << *m;

    if (os.length () > size_t (params_.max_packet_size ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "packet length (%d) exceeds max_poacket_size (%d)\n",
                  os.length (), params_.max_packet_size ()));

      for (Message::ProfileIterator i (m->begin ()); !i.done (); i.advance ())
      {
        ACE_ERROR ((LM_ERROR,
                    "profile id: %d; size: %d\n",
                    (*i).ext_id_, (*i).int_id_->size ()));
      }

      ACE_OS::abort ();
    }

    ssock_.send (os.buffer (), os.length (), addr_);

    /*
      if (m->find (nrtm::id))
      {
        ACE_OS::write (1, os.buffer (), os.length ());
        ACE_OS::exit (1);
      }
    */
  }

  void Link::recv ()
  {
    size_t max_packet_size (params_.max_packet_size ());

    ACE_Auto_Array_Ptr<char> holder (new char[max_packet_size + ACE_CDR::MAX_ALIGNMENT]);

    char* data = ACE_ptr_align_binary (holder.get (), ACE_CDR::MAX_ALIGNMENT);

    size_t size (0);

    while (true)
    {
      //@@ Should I lock here?
      //

      Address addr;

      // Block for up to one tick waiting for an incomming message.
      //
      for (;;)
      {
        ACE_Time_Value t (params_.tick ());
        ssize_t r = rsock_.recv (data, 4, addr, MSG_PEEK, &t);


        // Check for cancellation request.
        //
        {
          Lock l (mutex_);
          if (stop_)
            return;
        }

        if (r == -1)
        {
          if (errno != ETIME)
            ACE_OS::abort ();
        }
        else
        {
          size = static_cast<size_t> (r);
          break;
        }
      }


      if (size != 4 || addr == self_)
      {
        // Discard bad messages and ones from ourselvs since
        // we are using reliable loopback.
        //
        rsock_.recv (data, 0, addr);
        continue;
      }

      u32 msg_size;
      {
        istream is (data, size, 1); // Always little-endian.
        is >> msg_size;
      }

      if (msg_size <= 4 || msg_size > max_packet_size)
      {
        // Bad message.
        //
        rsock_.recv (data, 0, addr);
        continue;
      }

      size = rsock_.recv (data, max_packet_size, addr);

      if (msg_size != size)
      {
        // Bad message.
        //
        continue;
      }

      //cerr << 6 << "from: " << addr << endl;

      Message_ptr m (new Message ());

      m->add (Profile_ptr (new From (addr)));
      m->add (Profile_ptr (new To (self_)));

      istream is (data, size, 1); // Always little-endian.

      is >> msg_size;

      while (true)
      {
        u16 id, size;

        if (!((is >> id) && (is >> size))) break;

        //cerr << 6 << "reading profile with id " << id << " "
        //     << size << " bytes long" << endl;

        Profile::Header hdr (id, size);

        if (id == SN::id)
          {
            m->add (Profile_ptr (new SN (hdr, is)));
          }
        else if (id == Data::id)
          {
            m->add (Profile_ptr (new Data (hdr, is)));
          }
        else if (id == NAK::id)
          {
            m->add (Profile_ptr (new NAK (hdr, is)));
          }
        else if (id == NRTM::id)
          {
            m->add (Profile_ptr (new NRTM (hdr, is)));
          }
        else if (id == NoData::id)
          {
            m->add (Profile_ptr (new NoData (hdr, is)));
          }
        else if (id == Part::id)
          {
            m->add (Profile_ptr (new Part (hdr, is)));
          }
        else
          {
            //cerr << 0 << "unknown profile id " << hdr.id () << endl;
            ACE_OS::abort ();
          }
      }

      in_->recv (m);
    }
  }

  ACE_THR_FUNC_RETURN Link::
  recv_thunk (void* obj)
  {
    reinterpret_cast<Link*> (obj)->recv ();
    return 0;
  }

  void Link::recv (Message_ptr)
  {
    ACE_OS::abort ();
  }
}
