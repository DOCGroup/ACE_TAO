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
    ctx_ (c)
{
}

ACE_INLINE
CIAO_GLUE_HUDisplay::position_Servant::~position_Servant ()
{
}


// This is only a guideline...  we always relay the operation to underlying
// executor.

ACE_INLINE CORBA::Long
CIAO_GLUE_HUDisplay::position_Servant::pos (ACE_ENV_SINGLE_ARG_DECL)
{
  // Simply relay to executor.  May not need to return the result...
  return this->executor_->pos (ACE_ENV_SINGLE_ARG_PARAMETER);
}
