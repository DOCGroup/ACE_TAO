// $Id$ -*- C++ -*-

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.i        GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

// Constructor and destructor.
ACE_INLINE
CIAO_GLUE_HUDisplay::position_Servant::position_Servant (HUDisplay::CCM_position_ptr executor,
                                                         Components::CCMContext_ptr c)
  : executor_ (HUDisplay::CCM_position::_duplicate (executor)),
    ctx_ (Components::CCMContext::_duplicate (c))
{
}

ACE_INLINE
CIAO_GLUE_HUDisplay::position_Servant::~position_Servant ()
{
}


// This is only a guideline...  we always relay the operation to underlying
// executor.

ACE_INLINE CORBA::Long
CIAO_GLUE_HUDisplay::position_Servant::posx (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Simply relay to executor.  May not need to return the result...
  return this->executor_->posx (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE CORBA::Long
CIAO_GLUE_HUDisplay::position_Servant::posy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Simply relay to executor.  May not need to return the result...
  return this->executor_->posy (ACE_ENV_SINGLE_ARG_PARAMETER);
}
