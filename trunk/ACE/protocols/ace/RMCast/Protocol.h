// $Id$
// author    : Boris Kolpackov <boris@kolpackov.net>

#ifndef ACE_RMCAST_PROTOCOL_H
#define ACE_RMCAST_PROTOCOL_H

#include "ace/Auto_Ptr.h"
#include "ace/Bound_Ptr.h"

#include "ace/Vector_T.h"
#include "ace/Hash_Map_Manager.h"

#include "ace/CDR_Stream.h"
#include "ace/CDR_Size.h"

#include "ace/INET_Addr.h"
#include "ace/Null_Mutex.h"

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"

#include "Bits.h"

namespace ACE_RMCast
{
  // Basic types.
  //
  typedef ACE_CDR::UShort u16;
  typedef ACE_CDR::ULong u32;
  typedef ACE_CDR::ULongLong u64;

  // Protocol parameters
  //
  //
  unsigned short const max_service_size = 60;  // service profiles (Part, SN,
                                               // etc), sizes plus message size.

  //
  //
  //
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
  ACE_Strong_Bound_Ptr<Profile, Mutex>
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

    Profile_ptr
    clone ()
    {
      Profile_ptr p (clone_ ());
      return p;
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

    virtual Profile_ptr
    clone_ () = 0;

  private:
    Profile&
    operator= (Profile const&);

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
  ACE_Strong_Bound_Ptr<Message, Mutex>
  Message_ptr;

  class Message
  {
    typedef
    ACE_Hash_Map_Manager<u16, Profile_ptr, ACE_Null_Mutex>
    Profiles;

  public:
    Message ()
        : profiles_ (4)
    {
    }

    Message_ptr
    clone ()
    {
      Message_ptr cloned (new Message (*this));
      return cloned;
    }

  protected:
    Message (Message const& m)
        : profiles_ (4)
    {
      for (Profiles::const_iterator i (m.profiles_); !i.done (); i.advance ())
      {
        // Shallow copy of profiles. This implies that profiles are not
        // modified as they go up/down the stack.
        //
        profiles_.bind ((*i).ext_id_, (*i).int_id_);
      }
    }

  private:
    Message&
    operator= (Message const&);

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

    void
    replace (Profile_ptr p)
    {
      profiles_.rebind (p->id (), p);
    }

    void
    remove (u16 id)
    {
      profiles_.unbind (id);
    }

    Profile const*
    find (u16 id) const
    {
      Profiles::ENTRY* e = 0;

      if (profiles_.find (id, e) == -1) return 0;

      return e->int_id_.get ();
    }

    typedef
    Profiles::const_iterator
    ProfileIterator;

    ProfileIterator
    begin () const
    {
      return ProfileIterator (profiles_);
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

  private:
    Profiles profiles_;
  };

  typedef ACE_Vector<Message_ptr, ACE_VECTOR_DEFAULT_SIZE> Messages;

  //
  //
  //
  struct From;

  typedef
  ACE_Strong_Bound_Ptr<From, Mutex>
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

    From_ptr
    clone ()
    {
      return From_ptr (clone_ ());
    }

  protected:
    virtual Profile_ptr
    clone_ ()
    {
      Profile_ptr p (new From (*this));
      return p;
    }

    From (From const& from)
        : Profile (from),
          address_ (from.address_)
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
  ACE_Strong_Bound_Ptr<To, Mutex>
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

    To_ptr
    clone ()
    {
      return To_ptr (clone_ ());
    }

  protected:
    virtual Profile_ptr
    clone_ ()
    {
      Profile_ptr p (new To (*this));
      return p;
    }

    To (To const& to)
        : Profile (to),
          address_ (to.address_)
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
  ACE_Strong_Bound_Ptr<Data, Mutex>
  Data_ptr;

  struct Data : Profile
  {
    static u16 const id;

  public:
    virtual
    ~Data ()
    {
      if (buf_)
        operator delete (buf_);
    }

    Data (Header const& h, istream& is)
        : Profile (h),
          buf_ (0),
          size_ (h.size ()),
          capacity_ (size_)
    {
      if (size_)
      {
        buf_ = reinterpret_cast<char*> (operator new (capacity_));
        is.read_char_array (buf_, size_);
      }
    }

    Data (void const* buf, size_t s, size_t capacity = 0)
        : Profile (id),
          buf_ (0),
          size_ (s),
          capacity_ (capacity < size_ ? size_ : capacity)
    {
      if (size_)
      {
        buf_ = reinterpret_cast<char*> (operator new (capacity_));
        ACE_OS::memcpy (buf_, buf, size_);
      }

      Profile::size (calculate_size ());
    }

    Data_ptr
    clone ()
    {
      return Data_ptr (clone_ ());
    }

  protected:
    virtual Profile_ptr
    clone_ ()
    {
      Profile_ptr p (new Data (*this));
      return p;
    }

    Data (Data const& d)
        : Profile (d),
          buf_ (0),
          size_ (d.size_),
          capacity_ (d.capacity_)
    {
      if (size_)
      {
        buf_ = reinterpret_cast<char*> (operator new (capacity_));
        ACE_OS::memcpy (buf_, d.buf_, size_);
      }

      Profile::size (calculate_size ());
    }

  public:
    char const*
    buf () const
    {
      return buf_;
    }

    char*
    buf ()
    {
      return buf_;
    }

    size_t
    size () const
    {
      return size_;
    }

    void
    size (size_t s)
    {
      if (s > capacity_)
        ACE_OS::abort ();

      size_ = s;

      Profile::size (calculate_size ());
    }

    size_t
    capacity () const
    {
      return capacity_;
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
    size_t capacity_;
  };


  //
  //
  //
  struct SN;

  typedef
  ACE_Strong_Bound_Ptr<SN, Mutex>
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

    SN_ptr
    clone ()
    {
      return SN_ptr (clone_ ());
    }

  protected:
    virtual Profile_ptr
    clone_ ()
    {
      Profile_ptr p (new SN (*this));
      return p;
    }

    SN (SN const& sn)
        : Profile (sn),
          n_ (sn.n_)
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
  ACE_Strong_Bound_Ptr<NAK, Mutex>
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


      is >> addr;
      is >> port;

      // num_of_sns = (size - addr_size) / sn_size
      //
      for (unsigned long i (0); i < ((h.size () - addr_size) / sn_size); ++i)
      {
        is >> sn;
        sns_.push_back (sn);
      }


      address_ = Address (port, addr);
    }

    NAK (Address const& src)
        : Profile (id), address_ (src)
    {
      size (calculate_size ());
    }

    NAK_ptr
    clone ()
    {
      return NAK_ptr (clone_ ());
    }

  protected:
    virtual Profile_ptr
    clone_ ()
    {
      Profile_ptr p (new NAK (*this));
      return p;
    }

    NAK (NAK const& nak)
        : Profile (nak),
          address_ (nak.address_),
          sns_ (nak.sns_)
    {
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
    // Count max number of elements that will fit into NAK profile
    // with size <= max_size.
    //
    static u32
    max_count (u32 max_size)
    {
      u32 n (0);

      sstream ss;

      Profile::Header hdr (0, 0);
      ss << hdr;

      u32 addr (0);
      u16 port (0);
      ss << addr;
      ss << port;

      while (true)
      {
        u64 sn (0);
        ss << sn;

        if (ss.total_length () <= max_size)
          ++n;

        if (ss.total_length () >= max_size)
          break;
      }

      return n;
    }

  public:
    virtual void
    serialize_body (ostream& os) const
    {
      NAK& this_ = const_cast<NAK&> (*this); // Don't put in ROM.

      u32 addr (address_.get_ip_address ());
      u16 port (address_.get_port_number ());

      os << addr;
      os << port;

      // Stone age iteration.
      //
      for (iterator i (this_.begin ()); !i.done (); i.advance ())
      {
        u64* psn = 0;
        i.next (psn);
        os << *psn;
      }
    }

    virtual void
    serialize_body (sstream& ss) const
    {
      NAK& this_ = const_cast<NAK&> (*this); // Don't put in ROM.

      u32 addr (0);
      u16 port (0);

      ss << addr;
      ss << port;

      // Stone age iteration.
      //
      for (iterator i (this_.begin ()); !i.done (); i.advance ())
      {
        u64 sn (0);
        ss << sn;
      }
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
  ACE_Strong_Bound_Ptr<NRTM, Mutex>
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

    NRTM_ptr
    clone ()
    {
      return NRTM_ptr (clone_ ());
    }

  protected:
    virtual Profile_ptr
    clone_ ()
    {
      Profile_ptr p (new NRTM (*this));
      return p;
    }

    NRTM (NRTM const& nrtm)
        : Profile (nrtm)
    {
      for (Map::const_iterator i (nrtm.map_); !i.done (); i.advance ())
      {
        map_.bind ((*i).ext_id_, (*i).int_id_);
      }
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
    // Count max number of elements that will fit into NRTM profile
    // with size <= max_size.
    //
    static u32
    max_count (u32 max_size)
    {
      u32 n (0);

      sstream ss;

      Profile::Header hdr (0, 0);
      ss << hdr;

      while (true)
      {
        u32 addr (0);
        u16 port (0);
        u64 sn (0);

        ss << sn;
        ss << addr;
        ss << port;

        if (ss.total_length () <= max_size)
          ++n;

        if (ss.total_length () >= max_size)
          break;
      }

      return n;
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
  ACE_Strong_Bound_Ptr<NoData, Mutex>
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

    NoData_ptr
    clone ()
    {
      return NoData_ptr (clone_ ());
    }

  protected:
    virtual Profile_ptr
    clone_ ()
    {
      Profile_ptr p (new NoData (*this));
      return p;
    }

    NoData (NoData const& no_data)
        : Profile (no_data)
    {
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


  //
  //
  //
  struct Part;

  typedef
  ACE_Strong_Bound_Ptr<Part, Mutex>
  Part_ptr;

  struct Part : Profile
  {
    static u16 const id;

  public:
    Part (Header const& h, istream& is)
        : Profile (h)
    {
      is >> num_;
      is >> of_;
      is >> total_size_;
    }

    Part (u32 num, u32 of, u64 total_size)
        : Profile (id),
          num_ (num),
          of_ (of),
          total_size_ (total_size)
    {
      size (calculate_size ());
    }

    Part_ptr
    clone ()
    {
      return Part_ptr (clone_ ());
    }

  protected:
    virtual Profile_ptr
    clone_ ()
    {
      Profile_ptr p (new Part (*this));
      return p;
    }

    Part (Part const& part)
        : Profile (part),
          num_ (part.num_),
          of_ (part.of_),
          total_size_ (part.total_size_)
    {
    }

  public:
    u32
    num () const
    {
      return num_;
    }

    u32
    of () const
    {
      return of_;
    }

    u64
    total_size () const
    {
      return total_size_;
    }

  public:
    virtual void
    serialize_body (ostream& os) const
    {
      os << num_;
      os << of_;
      os << total_size_;
    }

    virtual void
    serialize_body (sstream& ss) const
    {
      ss << num_;
      ss << of_;
      ss << total_size_;
    }


  private:
    u32 num_;
    u32 of_;
    u64 total_size_;
  };

}

/*
inline
std::ostream&
operator<< (std::ostream& os, ACE_RMCast::Address const& a)
{
  char buf[64];
  a.addr_to_string (buf, 64, 1);
  return os << buf;
}
*/

#endif  // ACE_RMCAST_PROTOCOL_H
