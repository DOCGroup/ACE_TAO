//=============================================================================
/**
 *  @file   Storable_Base.cpp
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com>
 *  @author Chanaka Liyanaarachchi <chanaka@ociweb.com>
 *  @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#include "tao/Storable_Base.h"

#if !defined (__ACE_INLINE__)
#include "tao/Storable_Base.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool TAO::Storable_Base::use_backup_default = false;

void
TAO::Storable_Base::remove (void)
{
  if (this->use_backup_)
    {
      this->remove_backup ();
    }
  this->do_remove ();
}

bool
TAO::Storable_Base::use_backup ()
{
  return this->use_backup_;
}

ACE_CString
TAO::Storable_Base::state_as_string (Storable_State state)
{
  ACE_CString state_string;
  if (state == goodbit)
    state_string = "goodbit";
  else
    {
      if (state & badbit)
        state_string = "badbit ";
      if (state & eofbit)
        state_string += "eofbit ";
      if (state & failbit)
        state_string += "failbit";
    }
  return state_string;
}

TAO::Storable_Exception::
Storable_Exception (const ACE_CString & file_name)
  : file_name_ (file_name)
{
}

TAO::Storable_Exception::
~Storable_Exception ()
{
}

const ACE_CString &
TAO::Storable_Exception::get_file_name () const
{
  return file_name_;
}

TAO::Storable_Read_Exception::
Storable_Read_Exception (Storable_Base::Storable_State state,
                         const ACE_CString & file_name)
  : Storable_Exception (file_name)
  , storable_state_ (state)
{
}

TAO::Storable_Base::Storable_State
TAO::Storable_Read_Exception::get_state () const
{
  return storable_state_;
}


TAO::Storable_Write_Exception::
Storable_Write_Exception (Storable_Base::Storable_State state,
                          const ACE_CString & file_name)
  : Storable_Exception (file_name)
  , storable_state_ (state)
{
}

TAO::Storable_Base::Storable_State
TAO::Storable_Write_Exception::get_state () const
{
  return storable_state_;
}


TAO_END_VERSIONED_NAMESPACE_DECL
