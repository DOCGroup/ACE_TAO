// file      : ace/RMCast/Protocol.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_PROTOCOL_H
#define ACE_RMCAST_PROTOCOL_H

#include "ace/Refcounted_Auto_Ptr.h"

#include "ace/Vector_T.h"
#include "ace/Hash_Map_Manager.h"

#include "ace/CDR_Stream.h"
#include "ace/CDR_Size.h"

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

      port <<= sizeof (unsigned long) - sizeof (unsigned short);

      return port ^ ip;
    }
  };

  //@@ Provide stream<< (Address const&)
  //

  typedef ACE_OutputCDR ostream;
  typedef ACE_SizeCDR sstream;
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
    Profile (u16 id)
        : header_ (id, 0)
    {
    }

    Profile (Header const& h)
        : header_ (h)
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

  protected:
    void
    size (u16 s)
    {
      header_.size (s);
    }

    u16
    calculate_size ()
    {
      sstream ss;

      serialize_body (ss);

      return static_cast<u16> (ss.total_length ());
    }

  public:
    virtual void
    serialize_body (ostream&) const = 0;

    virtual void
    serialize_body (sstream&) const = 0;

    friend
    ostream&
    operator<< (ostream& os, Profile const& p);

    friend
    sstream&
    operator<< (sstream& ss, Profile const& p);

  private:
    Header header_;
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
  sstream&
  operator<< (sstream& ss, Profile::Header const& hdr)
  {
    ss << hdr.id ();
    ss << hdr.size ();

    return ss;
  }

  inline
  ostream&
  operator<< (ostream& os, Profile const& p)
  {
    os << p.header_;
    p.serialize_body (os);

    return os;
  }

  inline
  sstream&
  operator<< (sstream& ss, Profile const& p)
  {
    ss << p.header_;
    p.serialize_body (ss);

    return ss;
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
    bool
    add (Profile_ptr p)
    {
      u16 id (p->id ());

      if (profiles_.find (id) == 0)
      {
        return false;
      }

      profiles_.bind (id, p);

      return true;
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
      sstream ss;

      u32 s (0);

      ss << s;

      for (Profiles::const_iterator i (profiles_); !i.done (); i.advance ())
      {
        ss << *((*i).int_id_);
      }

      return ss.total_length ();
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

  typedef ACE_Vector<Message_ptr, ACE_VECTOR_DEFAULT_SIZE> Messages;

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
        : Profile (h)
    {
      u32 addr;
      u16 port;

      is >> addr;
      is >> port;

      address_ = Address (port, addr);
    }

    From (Address const& addr)
        : Profile (id), address_ (addr)
    {
      size (calculate_size ());
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

    virtual void
    serialize_body (sstream& ss) const
    {
      u32 addr (0);
      u16 port (0);

      ss << addr;
      ss << port;
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
        : Profile (h)
    {
      u32 addr;
      u16 port;

      is >> addr;
      is >> port;

      address_ = Address (port, addr);
    }

    To (Address const& addr)
        : Profile (id), address_ (addr)
    {
      size (calculate_size ());
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

    virtual void
    serialize_body (sstream& ss) const
    {
      u32 addr (0);
      u16 port (0);

      ss << addr;
      ss << port;
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
        : Profile (h), buf_ (0), size_ (h.size ())
    {
      if (size_)
      {
        buf_ = reinterpret_cast<char*> (operator new (size_));
        is.read_char_array (buf_, size_);
      }
    }

    Data (void const* buf, size_t s)
        : Profile (id), buf_ (0), size_ (s)
    {
      if (size_)
      {
        buf_ = reinterpret_cast<char*> (operator new (size_));
        ACE_OS::memcpy (buf_, buf, size_);
      }

      Profile::size (calculate_size ());
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

    virtual void
    serialize_body (sstream& ss) const
    {
      ss.write_char_array (buf_, size_);
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
        : Profile (h)
    {
      is >> n_;
    }

    SN (u64 n)
        : Profile (id), n_ (n)
    {
      size (calculate_size ());
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

    virtual void
    serialize_body (sstream& ss) const
    {
      ss << n_;
    }

  private:
    u64 n_;
  };


  //
  //
  //
  class NAK;

  typedef
  ACE_Refcounted_Auto_Ptr<NAK, ACE_Null_Mutex>
  NAK_ptr;

  class NAK : public Profile
  {
  public:

    static u16 const id;

    typedef ACE_Vector<u64, ACE_VECTOR_DEFAULT_SIZE> SerialNumbers;
    typedef SerialNumbers::Iterator iterator;

    NAK (Header const& h, istream& is)
        : Profile (h)
    {
      u64 sn (0);
      u32 addr (0);
      u16 port (0);

      sstream ss;

      ss << sn;
      size_t sn_size (ss.total_length ());

      ss.reset ();

      ss << addr;
      ss << port;

      size_t addr_size (ss.total_length ());

      // num_of_sns = (size - addr_size) / sn_size
      //
      for (unsigned long i (0); i < ((h.size () - addr_size) / sn_size); ++i)
      {
        is >> sn;
        sns_.push_back (sn);
      }

      is >> addr;
      is >> port;


      address_ = Address (port, addr);
    }

    NAK (Address const& src)
        : Profile (id), address_ (src)
    {
      size (calculate_size ());
    }

  public:
    void
    add (u64 sn)
    {
      sns_.push_back (sn);
      size (calculate_size ());
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
      NAK& this_ = const_cast<NAK&> (*this); // Don't put in ROM.

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

      os << addr;
      os << port;
    }

    virtual void
    serialize_body (sstream& ss) const
    {
      NAK& this_ = const_cast<NAK&> (*this); // Don't put in ROM.

      // Stone age iteration.
      //
      for (iterator i (this_.begin ()); !i.done (); i.advance ())
      {
        u64 sn (0);
        ss << sn;
      }


      u32 addr (0);
      u16 port (0);

      ss << addr;
      ss << port;
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
        : Profile (h), map_ (10)
    {
      u32 addr (0);
      u16 port (0);
      u64 sn (0);

      sstream ss;

      ss << sn;
      ss << addr;
      ss << port;

      size_t block_size (ss.total_length ());


      // num_of_blocks = size / block_size
      //
      for (size_t i (0); i < (h.size () / block_size); ++i)
      {
        is >> sn;
        is >> addr;
        is >> port;


        map_.bind (Address (port, addr), sn);
      }
    }

    NRTM ()
        : Profile (id), map_ (10)
    {
      size (calculate_size ());
    }

  public:
    void
    insert (Address const& addr, u64 sn)
    {
      map_.bind (addr, sn);

      size (calculate_size ());
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
        os << addr;
        os << port;
      }
    }

    virtual void
    serialize_body (sstream& ss) const
    {
      for (Map::const_iterator i (map_), e (map_, 1); i != e; ++i)
      {
        u32 addr (0);
        u16 port (0);
        u64 sn (0);

        ss << sn;
        ss << addr;
        ss << port;
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


  //
  //
  //
  struct NoData;

  typedef
  ACE_Refcounted_Auto_Ptr<NoData, ACE_Null_Mutex>
  NoData_ptr;

  struct NoData : Profile
  {
    static u16 const id;

  public:
    NoData (Header const& h, istream&)
        : Profile (h)
    {
    }

    NoData ()
        : Profile (id)
    {
      Profile::size (0);
    }

  public:
    virtual void
    serialize_body (ostream&) const
    {
    }

    virtual void
    serialize_body (sstream&) const
    {
    }
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
