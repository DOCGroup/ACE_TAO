// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_OutputCDR::~TAO_OutputCDR (void)
{
}

ACE_INLINE bool
TAO_OutputCDR::more_fragments (void) const
{
  return this->more_fragments_;
}

ACE_INLINE void
TAO_OutputCDR::more_fragments (bool more)
{
  this->more_fragments_ = more;
}

ACE_INLINE bool
TAO_OutputCDR::compressed (void) const
{
  return this->compressed_;
}

ACE_INLINE void
TAO_OutputCDR::compressed (bool compressed)
{
  this->compressed_ = compressed;
}

ACE_INLINE void
TAO_OutputCDR::message_attributes (CORBA::ULong request_id,
                                   TAO_Stub * stub,
                                   TAO_Transport::TAO_Message_Semantics message_semantics,
                                   ACE_Time_Value * timeout)
{
  this->request_id_        = request_id;
  this->stub_              = stub;
  this->message_semantics_ = message_semantics;
  this->timeout_           = timeout;
}

ACE_INLINE CORBA::ULong
TAO_OutputCDR::request_id (void) const
{
  return this->request_id_;
}

ACE_INLINE TAO_Stub *
TAO_OutputCDR::stub (void) const
{
  return this->stub_;
}

ACE_INLINE TAO_Transport::TAO_Message_Semantics
TAO_OutputCDR::message_semantics (void) const
{
  return this->message_semantics_;
}

ACE_INLINE ACE_Time_Value *
TAO_OutputCDR::timeout (void) const
{
  return this->timeout_;
}

ACE_INLINE void
TAO_OutputCDR::get_version (TAO_GIOP_Message_Version& giop_version)
{
  giop_version.major = this->major_version_;
  giop_version.minor = this->minor_version_;
}
// -------------------------------------------------------------------

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const char *buf,
                            size_t bufsiz,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (buf,
                  bufsiz,
                  byte_order,
                  major_version,
                  minor_version),
    orb_core_ (orb_core),
    compressed_ (false)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (size_t bufsiz,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (bufsiz,
                  byte_order,
                  major_version,
                  minor_version),
    orb_core_ (orb_core),
    compressed_ (false)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const ACE_Message_Block *data,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (data,
                  byte_order,
                  major_version,
                  minor_version),
    orb_core_ (orb_core),
    compressed_ (false)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const ACE_Message_Block *data,
                            ACE_Lock* lock,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (data,
                  byte_order,
                  major_version,
                  minor_version,
                  lock),
    orb_core_ (orb_core),
    compressed_ (false)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (ACE_Data_Block *data,
                            ACE_Message_Block::Message_Flags flag,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (data,
                  flag,
                  byte_order,
                  major_version,
                  minor_version),
    orb_core_ (orb_core),
    compressed_ (false)
{
}


ACE_INLINE
TAO_InputCDR::TAO_InputCDR (ACE_Data_Block *data,
                            ACE_Message_Block::Message_Flags flag,
                            size_t rd_pos,
                            size_t wr_pos,
                            int byte_order,
                            ACE_CDR::Octet major_version,
                            ACE_CDR::Octet minor_version,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (data,
                  flag,
                  rd_pos,
                  wr_pos,
                  byte_order,
                  major_version,
                  minor_version),
    orb_core_ (orb_core),
    compressed_ (false)
{
}


ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size,
                            ACE_CDR::Long offset)
  : ACE_InputCDR (rhs,
                  size,
                  offset),
    orb_core_ (rhs.orb_core_),
    compressed_ (rhs.compressed_)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size)
  : ACE_InputCDR (rhs,
                  size),
    orb_core_ (rhs.orb_core_),
    compressed_ (rhs.compressed_)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs)
  : ACE_InputCDR (rhs),
    orb_core_ (rhs.orb_core_),
    compressed_ (rhs.compressed_)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (ACE_InputCDR::Transfer_Contents rhs,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (rhs),
    orb_core_ (orb_core),
    compressed_ (false)
{
}

ACE_INLINE
TAO_InputCDR::~TAO_InputCDR (void)
{
}

ACE_INLINE TAO_ORB_Core*
TAO_InputCDR::orb_core (void) const
{
  return this->orb_core_;
}

// ****************************************************************

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::Short x)
{
  return
    os.fragment_stream (ACE_CDR::SHORT_ALIGN,
                        sizeof (CORBA::Short))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::UShort x)
{
  return
    os.fragment_stream (ACE_CDR::SHORT_ALIGN,
                        sizeof (CORBA::UShort))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::Long x)
{
  return
    os.fragment_stream (ACE_CDR::LONG_ALIGN,
                        sizeof (CORBA::Long))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::ULong x)
{
  return
    os.fragment_stream (ACE_CDR::LONG_ALIGN,
                        sizeof (CORBA::ULong))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::LongLong x)
{
  return
    os.fragment_stream (ACE_CDR::LONGLONG_ALIGN,
                        sizeof (CORBA::LongLong))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::ULongLong x)
{
  return
    os.fragment_stream (ACE_CDR::LONGLONG_ALIGN,
                        sizeof (CORBA::ULongLong))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR& os,
                                      CORBA::LongDouble x)
{
  return
    os.fragment_stream (ACE_CDR::LONGDOUBLE_ALIGN,
                        sizeof (CORBA::LongDouble))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::Float x)
{
  return
    os.fragment_stream (ACE_CDR::LONG_ALIGN,
                        sizeof (CORBA::Float))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      CORBA::Double x)
{
  return
    os.fragment_stream (ACE_CDR::LONGLONG_ALIGN,
                        sizeof (CORBA::Double))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      const char * x)
{
  return
    os.fragment_stream (ACE_CDR::OCTET_ALIGN,
                        sizeof (char))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      const CORBA::WChar * x)
{
  return
    os.fragment_stream ((sizeof (CORBA::WChar) == 2
                         ? ACE_CDR::SHORT_ALIGN
                         : ACE_CDR::LONG_ALIGN),
                        sizeof (CORBA::WChar))
    && static_cast<ACE_OutputCDR &> (os) << x;
}

// ****************************************************************

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Short &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::UShort &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Long &x)
{
  return static_cast<ACE_InputCDR&> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::ULong &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::LongLong &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::ULongLong &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::LongDouble &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Float &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Double &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::Char* &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      CORBA::WChar* &x)
{
  return static_cast<ACE_InputCDR &> (is) >> x;
}

TAO_END_VERSIONED_NAMESPACE_DECL
