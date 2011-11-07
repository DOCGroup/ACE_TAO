// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T>
ACE_INLINE
TAO_EC_Auto_Command<T>::TAO_EC_Auto_Command (void)
  : command_ ()
  , allow_command_ (0)
{
}

template <class T>
ACE_INLINE
TAO_EC_Auto_Command<T>::TAO_EC_Auto_Command (const T & command)
  : command_ (command)
  , allow_command_ (1)
{
}

template <class T>
ACE_INLINE
TAO_EC_Auto_Command<T>::~TAO_EC_Auto_Command (void)
{
  this->execute ();
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::set_command (const T & command)
{
  this->command_ = command;
  this->allow_command_ = 1;
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::set_command (TAO_EC_Auto_Command<T> & auto_command)
{
  if (this == &auto_command)
    return;

  this->command_ = auto_command.command_;
  this->allow_command_ = auto_command.allow_command_;
  auto_command.allow_command_ = 0;
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::execute (void)
{
  if (this->allow_command_)
    {
      this->allow_command_ = 0;

      try
        {
          this->command_.execute ();
        }
      catch (const CORBA::Exception&)
        {
          // ignore.
        }
    }
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::allow_command (void)
{
  this->allow_command_ = 1;
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::disallow_command (void)
{
  this->allow_command_ = 0;
}


//***************************************************************************

template <class T>
ACE_INLINE
TAO_EC_Shutdown_Command<T>::TAO_EC_Shutdown_Command (void)
  : target_ ()
{
}

template <class T>
ACE_INLINE
TAO_EC_Shutdown_Command<T>::TAO_EC_Shutdown_Command (T target)
  : target_ (target)
{
}

template <class T>
ACE_INLINE void
TAO_EC_Shutdown_Command<T>::execute (void)
{
  if (this->target_.in ())
    {
      this->target_->shutdown ();
    }
}

//***************************************************************************

TAO_END_VERSIONED_NAMESPACE_DECL

