// -*- C++ -*-

// file      : ace/RMCast/Protocol.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_PROTOCOL_H
#define ACE_RMCAST_PROTOCOL_H

#include "ace/Refcounted_Auto_Ptr.h"

#include "ace/Vector_T.h"
#include "ace/Hash_Map_Manager.h"

#include "ace/CDR_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/Null_Mutex.h"

#include "ace/OS_NS_string.h"

#include "Bits.h"


namespace ACE_RMCast
{
  // Basic types.
  //
  typedef ACE_CDR::UShort u16;
  typedef ACE_CDR::ULong u32;
  typedef ACE_CDR::ULongLong u64;

  typedef ACE_INET_Addr Address;

  struct AddressHasher
  {
    unsigned long
    operator() (Address const& a) const
    {
      unsigned long port (a.get_port_number ());
      unsigned long ip (a.get_ip_address ());

      port <<= sizeof  (sizeof (unsigned long) - sizeof (unsigned short));

      return port ^ ip;
    }
  };

  //@@ Provide stream<< (Address const&)
  //

  typedef ACE_OutputCDR ostream;
  typedef ACE_InputCDR istream;

  struct Profile;

  typedef
  ACE_Refcounted_Auto_Ptr<Profile, ACE_Null_Mutex>
  Profile_ptr;

  struct Profile
  {
  public:
    class Header
    {
    public:
      Header (u16 id, u16 size)
          : id_ (id), size_ (size)
      {
      }

      Header (istream& is)
      {
        (void) (is >> id_ >> size_);
      }

    public:
      u16
      id () const
      {
        return id_;
      }

      u16
      size () const
      {
        return size_;
      }

    protected:
      void
      size (u16 s)
      {
        size_ = s;
      }

      friend struct Profile;

    private:
      u16 id_;
      u16 size_;
    };

  public:
    virtual
    ~Profile ()
    {
    }

  protected:
    Profile (u16 id, u16 size, u16 boundary)
        : header_ (id, size), boundary_ (boundary)
    {
    }

    Profile (Header const& h, u16 boundary)
        : header_ (h), boundary_ (boundary)
    {
    }
  public:
    u16
    id () const
    {
      return header_.id ();
    }

    u16
    size () const
    {
      return header_.size ();
    }

    u16
    boundary () const
    {
      return boundary_;
    }

  protected:
    void
    size (u16 s)
    {
      header_.size (s);
    }

  public:
    virtual void
    serialize_body (ostream&) const = 0;

    friend
    ostream&
    operator<< (ostream& os, Profile const& p);

  private:
    Header header_;
    u16 boundary_;
  };

  inline
  ostream&
  operator<< (ostream& os, Profile::Header const& hdr)
  {
    os << hdr.id ();
    os << hdr.size ();

    return os;
  }

  inline
  ostream&
  operator<< (ostream& os, Profile const& p)
  {
    os << p.header_;
    p.serialize_body (os);

    return os;
  }

  //
  //
  //

  class Message;

  typedef
  ACE_Refcounted_Auto_Ptr<Message, ACE_Null_Mutex>
  Message_ptr;

  class Message
  {
  public:
    Message ()
        : profiles_ (4)
    {
    }

    Message (Message const& m)
        : profiles_ (4)
    {
      for (Profiles::const_iterator i (m.profiles_); !i.done (); i.advance ())
      {
        // Shallow copy of profiles.
        //
        profiles_.bind ((*i).ext_id_, (*i).int_id_);
      }
    }

  public:
    struct duplicate {};

    void
    add (Profile_ptr p)
    {
      u16 id (p->id ());

      if (profiles_.find (id) == 0)
      {
        throw duplicate ();
      }

      profiles_.bind (id, p);
    }

    Profile const*
    find (u16 id) const
    {
      Profiles::ENTRY* e;

      if (profiles_.find (id, e) == -1) return 0;

      return e->int_id_.get ();
    }

  public:
    size_t
    size () const
    {
      size_t s (4); // 4 is for size (u32)

      for (Profiles::const_iterator i (profiles_); !i.done (); i.advance ())
      {
        //@@ This is so broken: in CDR the padding depends on
        //   what comes after.
        //
        s += s % 2; // Padding to the boundary of 2.
        s += 4;     // Profile header: u16 + u16
        s += s % (*i).int_id_->boundary (); // Padding to the b. of profile body.
        s += (*i).int_id_->size (); // Profile body.
      }

      return s;
    }

    friend
    ostream&
    operator<< (ostream& os, Message const& m)
    {
      u32 s (m.size ());

      os << s;

      for (Profiles::const_iterator i (m.profiles_); !i.done (); i.advance ())
      {
        os << *((*i).int_id_);
      }

      return os;
    }

    typedef
    ACE_Hash_Map_Manager<u16, Profile_ptr, ACE_Null_Mutex>
    Profiles;

    Profiles profiles_;
  };

#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x570)
  // Borland C++ Builder 6 and earlier don't handle default template
  // arguments correctly.  Provide an explicit template argument.
  typedef ACE_Vector<Message_ptr, ACE_VECTOR_DEFAULT_SIZE> Messages;
#else
  typedef ACE_Vector<Message_ptr> Messages;
#endif  /* __BORLANDC__ <= 0x570 */


  //
  //
  //
  struct From;

  typedef
  ACE_Refcounted_Auto_Ptr<From, ACE_Null_Mutex>
  From_ptr;

  struct From : Profile
  {
    static u16 const id;

  public:
    From (Header const& h, istream& is)
        : Profile (h, 4)
    {
      u32 addr;
      u16 port;

      is >> addr;
      is >> port;

      address_ = Address (port, addr);
    }

    // 6 is CDR-specific.
    //
    From (Address const& addr)
        : Profile (id, 6, 4), address_ (addr)
    {
    }

  public:
    Address const&
    address () const
    {
      return address_;
    }

  public:
    virtual void
    serialize_body (ostream& os) const
    {
      u32 addr (address_.get_ip_address ());
      u16 port (address_.get_port_number ());

      os << addr;
      os << port;
    }

  private:
    Address address_;
  };


  //
  //
  //
  struct To;

  typedef
  ACE_Refcounted_Auto_Ptr<To, ACE_Null_Mutex>
  To_ptr;

  struct To : Profile
  {
    static u16 const id;

  public:
    To (Header const& h, istream& is)
        : Profile (h, 4)
    {
      u32 addr;
      u16 port;

      is >> addr;
      is >> port;

      address_ = Address (port, addr);
    }

    // 6 is CDR-specific.
    //
    To (Address const& addr)
        : Profile (id, 6, 4), address_ (addr)
    {
    }

  public:
    Address const&
    address () const
    {
      return address_;
    }

  public:
    virtual void
    serialize_body (ostream& os) const
    {
      u32 addr (address_.get_ip_address ());
      u16 port (address_.get_port_number ());

      os << addr;
      os << port;
    }

  private:
    Address address_;
  };

  //
  //
  //
  struct Data;

  typedef
  ACE_Refcounted_Auto_Ptr<Data, ACE_Null_Mutex>
  Data_ptr;

  struct Data : Profile
  {
    static u16 const id;

  public:
    Data (Header const& h, istream& is)
        : Profile (h, 1), buf_ (0), size_ (h.size ())
    {
      if (size_)
      {
        buf_ = reinterpret_cast<char*> (operator new (size_));
        is.read_char_array (buf_, size_);
      }

    }

    Data (void const* buf, size_t s)
        : Profile (id, s, 1), buf_ (0), size_ (s)
    {
      if (size_)
      {
        buf_ = reinterpret_cast<char*> (operator new (size_));
        ACE_OS::memcpy (buf_, buf, size_);
      }
    }

  public:
    char const*
    buf () const
    {
      return buf_;
    }

    size_t
    size () const
    {
      return size_;
    }

  public:
    virtual void
    serialize_body (ostream& os) const
    {
      os.write_char_array (buf_, size_);
    }

  private:
    char* buf_;
    size_t size_;
  };

  //
  //
  //
  struct SN;

  typedef
  ACE_Refcounted_Auto_Ptr<SN, ACE_Null_Mutex>
  SN_ptr;

  struct SN : Profile
  {
    static u16 const id;

  public:
    SN (Header const& h, istream& is)
        : Profile (h, 8)
    {
      is >> n_;
    }

    // 8 is CDR-specific.
    //
    SN (u64 n)
        : Profile (id, 8, 8), n_ (n)
    {
    }

  public:
    u64
    num () const
    {
      return n_;
    }

  public:
    virtual void
    serialize_body (ostream& os) const
    {
      os << n_;
    }

  private:
    u64 n_;
  };


  //
  //
  //
  struct NAK;

  typedef
  ACE_Refcounted_Auto_Ptr<NAK, ACE_Null_Mutex>
  NAK_ptr;

  struct NAK : Profile
  {
    static u16 const id;


#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x570)
  // Borland C++ Builder 6 and earlier don't handle default template
  // arguments correctly.  Provide an explicit template argument.
    typedef ACE_Vector<u64, ACE_VECTOR_DEFAULT_SIZE> SerialNumbers;
#else
    typedef ACE_Vector<u64> SerialNumbers;
#endif  /* __BORLANDC__ <= 0x570 */

    typedef SerialNumbers::Iterator iterator;

  public:
    NAK (Header const& h, istream& is)
        : Profile (h, 8)
    {
      //@@ All the numbers are CDR-specific.
      //
      //   8 = u32 + u16 + 2(padding to u64)
      //
      for (long i (0); i < ((h.size () - 8) / 8); ++i)
      {
        u64 sn;
        is >> sn;
        sns_.push_back (sn);
      }

      u32 addr;
      u16 port;

      is >> port;
      is >> addr;

      address_ = Address (port, addr);
    }

    // 8 is CDR-specific.
    //
    NAK (Address const& src)
        : Profile (id, 8, 8), address_ (src)
    {
    }

  public:
    void
    add (u64 sn)
    {
      sns_.push_back (sn);
      size (size () + 8); //@@ 8 is CDR-specific
    }

  public:
    Address const&
    address () const
    {
      return address_;
    }


    iterator
    begin () /* const */
    {
      return iterator (sns_);
    }

    /*
    iterator
    end ()  const
    {
      return sns_.end ();
    }
    */

    size_t
    count () const
    {
      return sns_.size ();
    }

  public:
    virtual void
    serialize_body (ostream& os) const
    {
      NAK& this_ (const_cast<NAK&> (*this)); // Don't put in ROM.

      // Stone age iteration.
      //
      for (iterator i (this_.begin ()); !i.done (); i.advance ())
      {
        u64* psn;
        i.next (psn);
        os << *psn;
      }


      u32 addr (address_.get_ip_address ());
      u16 port (address_.get_port_number ());

      os << port;
      os << addr;
    }

  private:
    Address address_;
    SerialNumbers sns_;
  };

  //
  //
  //
  struct NRTM;

  typedef
  ACE_Refcounted_Auto_Ptr<NRTM, ACE_Null_Mutex>
  NRTM_ptr;

  struct NRTM : Profile
  {
    static u16 const id;

  public:
    NRTM (Header const& h, istream& is)
        : Profile (h, 8), map_ (10)
    {
      //@@ 16 is CDR-specific.
      //
      //   16 = u32 + u16 + 2(padding to u64) + u64
      for (u16 i (0); i < (h.size () / 16); ++i)
      {
        u32 addr;
        u16 port;
        u64 sn;

        is >> sn;
        is >> port;
        is >> addr;

        map_.bind (Address (port, addr), sn);
      }
    }

    NRTM ()
        : Profile (id, 0, 8), map_ (10)
    {
    }

  public:
    void
    insert (Address const& addr, u64 sn)
    {
      map_.bind (addr, sn);

      size (size () + 16); //@@ 16 is CDR-specific.
    }

    u64
    find (Address const& addr) const
    {
      u64 sn;

      if (map_.find (addr, sn) == -1) return 0;

      return sn;
    }

    bool
    empty () const
    {
      return map_.current_size () == 0;
    }

  public:
    virtual void
    serialize_body (ostream& os) const
    {
      for (Map::const_iterator i (map_), e (map_, 1); i != e; ++i)
      {
        u32 addr ((*i).ext_id_.get_ip_address ());
        u16 port ((*i).ext_id_.get_port_number ());
        u64 sn ((*i).int_id_);

        os << sn;
        os << port;
        os << addr;
      }
    }

  private:
    typedef
    ACE_Hash_Map_Manager_Ex<Address,
                            u64,
                            AddressHasher,
                            ACE_Equal_To<Address>,
                            ACE_Null_Mutex>
    Map;

    Map map_;
  };

}

/*
inline
std::ostream&
operator<< (std::ostream& os, RMCast::Address const& a)
{
  char buf[64];
  a.addr_to_string (buf, 64, 1);
  return os << buf;
}
*/


#endif  // ACE_RMCAST_PROTOCOL_H
