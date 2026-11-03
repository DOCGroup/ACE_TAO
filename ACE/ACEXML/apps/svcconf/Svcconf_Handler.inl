// -*- C++ -*-

ACE_INLINE
ACE_Parsed_Info::ACE_Parsed_Info ()
  : name_ (nullptr),
    service_type_ (ACE_Service_Type::INVALID_TYPE),
    active_ (true),
    path_ (nullptr),
    init_func_ (nullptr),
    init_params_ (nullptr)
{
}

ACE_INLINE
ACE_Parsed_Info::~ACE_Parsed_Info ()
{
  delete[] this->name_;
  delete[] this->path_;
  delete[] this->init_func_;
  delete[] this->init_params_;
}

ACE_INLINE int
ACE_Parsed_Info::name (ACEXML_Char const *n)
{
  if (this->name_ == nullptr)
    {
      this->name_ = ACE::strnew (n);
      return 0;
    }
  return -1;
}

ACE_INLINE ACEXML_Char const *
ACE_Parsed_Info::name () const
{
  return this->name_;
}

ACE_INLINE int
ACE_Parsed_Info::service_type (int type)
{
  if (this->service_type_ == -1)
    {
      this->service_type_ = type;
      return 0;
    }
  return -1;
}

ACE_INLINE int
ACE_Parsed_Info::service_type () const
{
  return this->service_type_;
}

ACE_INLINE int
ACE_Parsed_Info::active (bool active)
{
  this->active_ = active;
  return 0;
}

ACE_INLINE bool
ACE_Parsed_Info::active () const
{
  return this->active_;
}

ACE_INLINE int
ACE_Parsed_Info::path (ACEXML_Char const *p)
{
  if (this->path_ == nullptr)
    {
      this->path_ = ACE::strnew (p);
      return 0;
    }
  return -1;
}

ACE_INLINE ACEXML_Char const *
ACE_Parsed_Info::path () const
{
  return this->path_;
}

ACE_INLINE int
ACE_Parsed_Info::init_func (ACEXML_Char const *n)
{
  if (this->init_func_ == nullptr)
    {
      this->init_func_ = ACE::strnew (n);
      return 0;
    }
  return -1;
}

ACE_INLINE ACEXML_Char const *
ACE_Parsed_Info::init_func () const
{
  return this->init_func_;
}

ACE_INLINE int
ACE_Parsed_Info::init_params (ACEXML_Char const *n)
{
  if (this->init_params_ == nullptr)
    {
      this->init_params_ = ACE::strnew (n);
      return 0;
    }
  return -1;
}

ACE_INLINE ACEXML_Char const *
ACE_Parsed_Info::init_params () const
{
  return this->init_params_;
}

ACE_INLINE void
ACE_Parsed_Info::reset ()
{
  delete[] this->name_;
  this->name_ = nullptr;
  this->service_type_ = -1;
  delete[] this->path_;
  this->path_ = nullptr;
  delete[] this->init_func_;
  this->init_func_ = nullptr;
  delete[] this->init_params_;
  this->init_params_ = nullptr;
}
