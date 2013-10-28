// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Operation_Details::TAO_Operation_Details (const char *name,
                                              CORBA::ULong len,
                                              TAO::Argument **args,
                                              CORBA::ULong num,
                                              CORBA::Boolean has_in_args,
                                              TAO::Exception_Data *data,
                                              CORBA::ULong count)
  : opname_ (name)
    , opname_len_ (len)
    , request_id_ (0)
    , response_flags_ (0)
    , addressing_mode_ (TAO_Target_Specification::Key_Addr)
    , args_ (args)
    , num_args_ (num)
    , has_in_args_ (has_in_args)
    , ex_data_ (data)
    , ex_count_ (count)
    , use_stub_args_ (args ? true : false)
#if TAO_HAS_INTERCEPTORS == 1
    , ft_expiration_time_ (0)
    , ft_retention_id_ (0)
#endif /*TAO_HAS_INTERCEPTORS == 1*/
    , cac_ (0)
    , reply_dispatcher_ (0)
{
}

ACE_INLINE const char*
TAO_Operation_Details::opname (void) const
{
  return this->opname_;
}

ACE_INLINE CORBA::ULong
TAO_Operation_Details::opname_len (void) const
{
  return this->opname_len_;
}

ACE_INLINE CORBA::Boolean
TAO_Operation_Details::argument_flag (void) const
{
  return (this->num_args_ > 1);
}

#if 1
ACE_INLINE CORBA::Boolean
TAO_Operation_Details::in_argument_flag (void) const
{
  return this->has_in_args_  && this->num_args_ > 1;
}
#else
ACE_INLINE CORBA::Boolean
TAO_Operation_Details::in_argument_flag (void) const
{
  for (CORBA::ULong i = 1; i < this->num_args_ && !this->has_in_args_; i++)
    {
      if (dynamic_cast<TAO::InArgument *>(args_[i]) != 0 ||
          dynamic_cast<TAO::InoutArgument *>(args_[i]))
        {
          return true;
        }
    }
  return false;
}
#endif // TAO_IGNORE_IN_ARGS

ACE_INLINE TAO_Service_Context &
TAO_Operation_Details::request_service_context (void)
{
  return this->request_service_info_;
}

ACE_INLINE const TAO_Service_Context &
TAO_Operation_Details::request_service_context (void) const
{
  return this->request_service_info_;
}

ACE_INLINE IOP::ServiceContextList &
TAO_Operation_Details::request_service_info (void)
{
  return this->request_service_context ().service_info ();
}

ACE_INLINE const IOP::ServiceContextList &
TAO_Operation_Details::request_service_info (void) const
{
  return this->request_service_context ().service_info ();
}

ACE_INLINE TAO_Service_Context &
TAO_Operation_Details::reply_service_context (void)
{
  return this->reply_service_info_;
}

ACE_INLINE const TAO_Service_Context &
TAO_Operation_Details::reply_service_context (void) const
{
  return this->reply_service_info_;
}

ACE_INLINE IOP::ServiceContextList &
TAO_Operation_Details::reply_service_info (void)
{
  return this->reply_service_context ().service_info ();
}

ACE_INLINE const IOP::ServiceContextList &
TAO_Operation_Details::reply_service_info (void) const
{
  return this->reply_service_context ().service_info ();
}

ACE_INLINE void
TAO_Operation_Details::reset_request_service_info (void)
{
  this->request_service_context ().service_info ().length (0);
}
ACE_INLINE void
TAO_Operation_Details::reset_reply_service_info (void)
{
  this->reply_service_context ().service_info ().length (0);
}

ACE_INLINE void
TAO_Operation_Details::request_id (CORBA::ULong id)
{
  this->request_id_ = id;
}

ACE_INLINE CORBA::ULong
TAO_Operation_Details::request_id (void) const
{
  return this->request_id_;
}

ACE_INLINE void
TAO_Operation_Details::response_flags (CORBA::Octet flags)
{
  this->response_flags_ = flags;
}

ACE_INLINE CORBA::ULong
TAO_Operation_Details::ex_count (void) const
{
  return this->ex_count_;
}

ACE_INLINE TAO::Exception_Data const *
TAO_Operation_Details::ex_data (void) const
{
  return this->ex_data_;
}

ACE_INLINE CORBA::Octet
TAO_Operation_Details::response_flags (void) const
{
  return this->response_flags_;
}

ACE_INLINE TAO_Target_Specification::TAO_Target_Address
TAO_Operation_Details::addressing_mode (void) const
{
  return this->addressing_mode_;
}

ACE_INLINE void
TAO_Operation_Details::addressing_mode (CORBA::Short mode)
{
  if (mode == 0)
    this->addressing_mode_ = TAO_Target_Specification::Key_Addr;
  else if (mode == 1)
    this->addressing_mode_ = TAO_Target_Specification::Profile_Addr;
  else if (mode == 2)
    this->addressing_mode_ = TAO_Target_Specification::Reference_Addr;
}

ACE_INLINE TAO::Argument **
TAO_Operation_Details::args (void) const
{
  return this->args_;
}

ACE_INLINE CORBA::ULong
TAO_Operation_Details::args_num (void) const
{
  return this->num_args_;
}

ACE_INLINE CORBA::Boolean
TAO_Operation_Details::use_stub_args (void) const
{
  return this->use_stub_args_;
}

ACE_INLINE void
TAO_Operation_Details::use_stub_args (CORBA::Boolean use_stub_args)
{
  this->use_stub_args_ = use_stub_args;
}

#if TAO_HAS_INTERCEPTORS == 1
ACE_INLINE void
TAO_Operation_Details::ft_expiration_time (TimeBase::TimeT time)
{
  this->ft_expiration_time_ = time;
}

ACE_INLINE TimeBase::TimeT
TAO_Operation_Details::ft_expiration_time (void) const
{
  return this->ft_expiration_time_;
}

ACE_INLINE void
TAO_Operation_Details::ft_retention_id (CORBA::Long request_id)
{
  this->ft_retention_id_ = request_id;
}

ACE_INLINE CORBA::Long
TAO_Operation_Details::ft_retention_id (void) const
{
  return this->ft_retention_id_;
}
#endif /*TAO_HAS_INTERCEPTORS == 1*/

ACE_INLINE TAO::Collocated_Arguments_Converter *
TAO_Operation_Details::cac (void) const
{
  return this->cac_;
}

ACE_INLINE void
TAO_Operation_Details::cac (TAO::Collocated_Arguments_Converter *cac)
{
  this->cac_ = cac;
}

ACE_INLINE TAO_Reply_Dispatcher *
TAO_Operation_Details::reply_dispatcher (void) const
{
  return this->reply_dispatcher_;
}

ACE_INLINE void
TAO_Operation_Details::reply_dispatcher (TAO_Reply_Dispatcher *rd)
{
  this->reply_dispatcher_ = rd;
}


TAO_END_VERSIONED_NAMESPACE_DECL
