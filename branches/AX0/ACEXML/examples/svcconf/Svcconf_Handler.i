// -*- C++ -*-  $Id$

ACE_INLINE
ACE_Parsed_Info::ACE_Parsed_Info ()
  : name_ (0),
    service_type_ (INVALID_TYPE),
    active_ (1),
    path_ (0),
    init_func_ (0),
    init_params_ (0)
{
}

ACE_INLINE
ACE_Parsed_Info::~ACE_Parsed_Info ()
{
  delete this->name_;
  delete this->path_;
  delete this->init_func_;
  delete this->init_params_;
}

ACE_INLINE int
ACE_Parsed_Info::name (const ACEXML_Char *n)
{
  if (this->name_ == 0)
    {
      this->name_ = ACE::strnew (n);
      return 0;
    }
  return -1;
}

ACE_INLINE const ACEXML_Char *
ACE_Parsed_Info::name (void)
{
  return this->name_;
}

ACE_INLINE int
ACE_Parsed_Info::service_type (Service_Type type)
{
  if (this->service_type_ == INVALID_TYPE)
    {
      this->service_type_ = type;
      return 0;
    }
  return -1;
}

ACE_INLINE ACE_Parsed_Info::Service_Type
ACE_Parsed_Info::service_type (void)
{
  return this->service_type_;
}

ACE_INLINE int
ACE_Parsed_Info::active (int a)
{
  this->active_ = a;
  return 0;
}

ACE_INLINE int
ACE_Parsed_Info::active (void)
{
  return this->active_;
}

ACE_INLINE int
ACE_Parsed_Info::path (const ACEXML_Char *p)
{
  if (this->path_ == 0)
    {
      this->path_ = ACE::strnew (p);
      return 0;
    }
  return -1;
}

ACE_INLINE const ACEXML_Char *
ACE_Parsed_Info::path (void)
{
  return this->path_;
}

ACE_INLINE int
ACE_Parsed_Info::init_func (const ACEXML_Char *n)
{
  if (this->init_func_ == 0)
    {
      this->init_func_ = ACE::strnew (n);
      return 0;
    }
  return -1;
}

ACE_INLINE const ACEXML_Char *
ACE_Parsed_Info::init_func (void)
{
  return this->init_func_;
}

ACE_INLINE int
ACE_Parsed_Info::init_params (const ACEXML_Char *n)
{
  if (this->init_params_ == 0)
    {
      this->init_params_ = ACE::strnew (n);
      return 0;
    }
  return -1;
}

ACE_INLINE const ACEXML_Char *
ACE_Parsed_Info::init_params (void)
{
  return this->init_params_;
}

ACE_INLINE void
ACE_Parsed_Info::reset (void)
{
  delete this->name_;
  this->name_ = 0;
  this->service_type_ = INVALID_TYPE;
  delete this->path_;
  this->path_ = 0;
  delete this->init_func_;
  this->init_func_ = 0;
  delete this->init_params_;
  this->init_params_ = 0;
}

ACE_INLINE
ACE_Stream_Parsed_Info::ACE_Stream_Parsed_Info ()
  : ACE_Parsed_Info (),
    stream_name_ (0)
{
}

ACE_INLINE
ACE_Stream_Parsed_Info::~ACE_Stream_Parsed_Info ()
{
  delete this->stream_name_;
}

ACE_INLINE int
ACE_Stream_Parsed_Info::stream_name (const ACEXML_Char *n)
{
  if (this->stream_name_ == 0)
    {
      this->stream_name_ = ACE::strnew (n);
      return 0;
    }
  return -1;
}

ACE_INLINE const ACEXML_Char *
ACE_Stream_Parsed_Info::stream_name (void)
{
  return this->stream_name_;
}
