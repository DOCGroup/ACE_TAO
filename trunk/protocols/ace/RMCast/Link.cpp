// file      : ace/RMCast/Link.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include <ace/Time_Value.h>       // ACE_Time_Value
#include <ace/OS_NS_sys_socket.h>

#include "Link.h"

namespace ACE_RMCast
{
  // Time period after which a manual cancellation request is
  // checked for.
  //
  ACE_Time_Value const timeout (0, 500);

  Link::
  Link (Address const& addr)
      : addr_ (addr),
        ssock_ (Address (static_cast<unsigned short> (0),
                         static_cast<ACE_UINT32> (INADDR_ANY)),
                AF_INET,
                IPPROTO_UDP,
                1),
        stop_ (false)

  {
    srand (time (0));


    rsock_.set_option (IP_MULTICAST_LOOP, 0);

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
      perror ("connect: ");
      abort ();
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

  void Link::
  send (Message_ptr m)
  {
    bool const sim = false;

    // Simulate message loss and reordering.
    //
    if (sim)
    {
      if ((rand () % 5) != 0)
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
          hold_ = m;

          // Make a copy in M so that the reliable loop below
          // won't add FROM and TO to HOLD_.
          //
          m = Message_ptr (new Message (*hold_));
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

    ssock_.send (os.buffer (), os.length (), addr_);

    /*
      if (m->find (nrtm::id))
      {
      write (1, os.buffer (), os.length ());
      exit (1);
      }
    */
  }

  void Link::
  recv ()
  {
    // I could have used ACE_Data_Block but it does not support
    // resizing...
    //
    size_t size (0), capacity (8192);
    char* data = reinterpret_cast<char*> (operator new (capacity));

    ACE_Auto_Ptr<char> holder (data); // This is wicked.

    while (true)
    {
      //@@ Should I lock here?
      //

      Address addr;

      //@@ CDR-specific.
      //
      // Block for up to timeout time waiting for an incomming message.
      //
      for (;;)
      {
        ACE_Time_Value t (timeout);
        ssize_t r = rsock_.recv (data, 4, addr, MSG_PEEK, &t);

        if (r == -1)
        {
          if (errno != ETIME)
            abort ();
        }
        else
        {
          size = static_cast<size_t> (r);
          break;
        }

        // Check for cancellation request.
        //
        {
          Lock l (mutex_);
          if (stop_)
            return;
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

      if (msg_size <= 4)
      {
        // Bad message.
        //
        rsock_.recv (data, 0, addr);
        continue;
      }

      if (capacity < msg_size)
      {
        capacity = msg_size;
        data = reinterpret_cast<char*> (operator new (capacity));
        holder.reset (data);
      }

      size = rsock_.recv (data, capacity, addr);

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
        else
          {
            //cerr << 0 << "unknown profile id " << hdr.id () << endl;
            abort ();
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

  void Link::
  recv (Message_ptr)
  {
    abort ();
  }
}
