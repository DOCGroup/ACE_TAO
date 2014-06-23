// -*- C++ -*-
//
// $Id$

#include "tao/SystemException.h"

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

ACE_INLINE void
TAO_OutputCDR::message_attributes (CORBA::ULong request_id,
                                   TAO_Stub * stub,
                                   TAO_Message_Semantics message_semantics,
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

ACE_INLINE TAO_Message_Semantics
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

ACE_INLINE TAO_OutputCDR::Repo_Id_Map_Handle&
TAO_OutputCDR::get_repo_id_map ()
{
  return this->repo_id_map_;
}

#ifdef TAO_HAS_VALUETYPE_CODEBASE
ACE_INLINE TAO_OutputCDR::Codebase_URL_Map_Handle&
TAO_OutputCDR::get_codebase_url_map ()
{
  return this->codebase_map_;
}
#endif

ACE_INLINE TAO_OutputCDR::Value_Map_Handle&
TAO_OutputCDR::get_value_map ()
{
  return this->value_map_;
}

ACE_INLINE void
TAO_OutputCDR::set_repo_id_map (TAO_OutputCDR::Repo_Id_Map_Handle& map)
{
  this->repo_id_map_ = map;
}

#ifdef TAO_HAS_VALUETYPE_CODEBASE
ACE_INLINE void
TAO_OutputCDR::set_codebase_url_map (TAO_OutputCDR::Codebase_URL_Map_Handle& map)
{
  this->codebase_map_ = map;
}
#endif

ACE_INLINE void
TAO_OutputCDR::set_value_map (TAO_OutputCDR::Value_Map_Handle& map)
{
  this->value_map_ = map;
}

ACE_INLINE void
TAO_OutputCDR::reset_vt_indirect_maps ()
{
  if (! this->repo_id_map_.is_nil () && this->repo_id_map_->get()->current_size () != 0)
  {
    this->repo_id_map_->get()->unbind_all ();
  }
#ifdef TAO_HAS_VALUETYPE_CODEBASE
  if (! this->codebase_map_.is_nil () && this->codebase_map_->get()->current_size () != 0)
  {
    this->codebase_map_->get()->unbind_all ();
  }
#endif
  if (! this->value_map_.is_nil () && this->value_map_->get()->current_size () != 0)
  {
    this->value_map_->get()->unbind_all ();
  }
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
    orb_core_ (orb_core)
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
    orb_core_ (orb_core)
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
    orb_core_ (orb_core)
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
    orb_core_ (orb_core)
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
    orb_core_ (orb_core)
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
    orb_core_ (orb_core)
{
}


ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size,
                            ACE_CDR::Long offset)
  : ACE_InputCDR (rhs,
                  size,
                  offset),
    orb_core_ (rhs.orb_core_)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs,
                            size_t size)
  : ACE_InputCDR (rhs,
                  size),
    orb_core_ (rhs.orb_core_)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (const TAO_InputCDR& rhs)
  : ACE_InputCDR (rhs),
    orb_core_ (rhs.orb_core_),
    repo_id_map_ (rhs.repo_id_map_),
    codebase_map_ (rhs.codebase_map_),
    value_map_ (rhs.value_map_)
{
}

ACE_INLINE
TAO_InputCDR::TAO_InputCDR (ACE_InputCDR::Transfer_Contents rhs,
                            TAO_ORB_Core* orb_core)
  : ACE_InputCDR (rhs),
    orb_core_ (orb_core)
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


ACE_INLINE TAO_InputCDR::Repo_Id_Map_Handle&
TAO_InputCDR::get_repo_id_map ()
{
  return this->repo_id_map_;
}

ACE_INLINE TAO_InputCDR::Codebase_URL_Map_Handle&
TAO_InputCDR::get_codebase_url_map ()
{
  return this->codebase_map_;
}

ACE_INLINE TAO_InputCDR::Value_Map_Handle&
TAO_InputCDR::get_value_map ()
{
  return this->value_map_;
}

ACE_INLINE void
TAO_InputCDR::set_repo_id_map (TAO_InputCDR::Repo_Id_Map_Handle& map)
{
  this->repo_id_map_ = map;
}

ACE_INLINE void
TAO_InputCDR::set_codebase_url_map (TAO_InputCDR::Codebase_URL_Map_Handle& map)
{
  this->codebase_map_ = map;
}

ACE_INLINE void
TAO_InputCDR::set_value_map (TAO_InputCDR::Value_Map_Handle& map)
{
  this->value_map_ = map;
}

ACE_INLINE void
TAO_InputCDR::reset_vt_indirect_maps ()
{
  if (! this->repo_id_map_.is_nil () && this->repo_id_map_->get()->current_size () != 0)
  {
    this->repo_id_map_->get()->unbind_all ();
  }
  if (! this->codebase_map_.is_nil () && this->codebase_map_->get()->current_size () != 0)
  {
    this->codebase_map_->get()->unbind_all ();
  }
  if (! this->value_map_.is_nil () && this->value_map_->get()->current_size () != 0)
  {
    this->value_map_->get()->unbind_all ();
  }
}

ACE_INLINE
TAO_InputCDR::to_std_string::to_std_string (std::string &s,
                                            ACE_CDR::ULong b)
  : val_ (s),
    bound_ (b)
{
}

#if !defined(ACE_LACKS_STD_WSTRING)
ACE_INLINE
TAO_InputCDR::to_std_wstring::to_std_wstring (std::wstring &s,
                                              ACE_CDR::ULong b)
  : val_ (s),
    bound_ (b)
{
}
#endif /* ACE_LACKS_STD_WSTRING */

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

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      ACE_OutputCDR::from_string x)
{
  if (x.bound_ != 0 && x.val_ != 0 &&
      ACE_OS::strlen (x.val_) > x.bound_)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return os << x.val_;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      ACE_OutputCDR::from_wstring x)
{
  if (x.bound_ != 0 && x.val_ != 0 &&
      ACE_OS::strlen (x.val_) > x.bound_)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return os << x.val_;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      const std::string &x)
{
  return os << x.c_str ();
}

#if !defined(ACE_LACKS_STD_WSTRING)
ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &os,
                                      const std::wstring &x)
{
  return os << x.c_str ();
}
#endif /* ACE_LACKS_STD_WSTRING */

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

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      ACE_InputCDR::to_string x)
{
  CORBA::Boolean const marshal_flag =
    is >> const_cast<ACE_CDR::Char *&> (x.val_);
  if (marshal_flag && x.bound_ != 0 && x.val_ != 0 &&
      ACE_OS::strlen (x.val_) > x.bound_)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return marshal_flag;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      ACE_InputCDR::to_wstring x)
{
  CORBA::Boolean const marshal_flag =
    is >> const_cast<ACE_CDR::WChar *&> (x.val_);
  if (marshal_flag && x.bound_ != 0 && x.val_ != 0 &&
      ACE_OS::strlen (x.val_) > x.bound_)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return marshal_flag;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      std::string &x)
{
  char *buf = 0;
  CORBA::Boolean const marshal_flag = is >> buf;
  x.assign (buf);
  ACE::strdelete (buf);
  return marshal_flag;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      TAO_InputCDR::to_std_string x)
{
  CORBA::Boolean const marshal_flag = is >> x.val_;
  if (marshal_flag && x.bound_ != 0 && x.val_.size () > x.bound_)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return marshal_flag;
}

#if !defined(ACE_LACKS_STD_WSTRING)
ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      std::wstring &x)
{
  CORBA::WChar *buf = 0;
  CORBA::Boolean const marshal_flag = is >> buf;
  x.assign (buf);
  ACE::strdelete (buf);
  return marshal_flag;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &is,
                                      TAO_InputCDR::to_std_wstring x)
{
  CORBA::Boolean const marshal_flag = is >> x.val_;
  if (marshal_flag && x.bound_ != 0 && x.val_.size () > x.bound_)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return marshal_flag;
}
#endif /* ACE_LACKS_STD_WSTRING */

TAO_END_VERSIONED_NAMESPACE_DECL
