//=============================================================================
/**
 *  @file   Storable_File_Guard.cpp
 *
 *  $Id$
 *
 *  @author Rich Seibel (seibelr@ociweb.com)
 *  @author Byron Harris (harrisb@ociweb.com)
 */
//=============================================================================

#include "tao/Storable_File_Guard.h"
#include "tao/Storable_Base.h"

#include "tao/SystemException.h"

TAO::Storable_File_Guard::
Storable_File_Guard (bool redundant, bool use_backup)
  : fl_(0)
  , redundant_(redundant)
  , closed_(1)
  , rwflags_(0)
  , use_backup_(use_backup)
{
}

TAO::Storable_File_Guard::
~Storable_File_Guard ()
{
}

void
TAO::Storable_File_Guard::init(Method_Type method_type)
{

  ACE_CString mode;

  // If backup is used then always need to open with
  // write access since if the file is corrupt then
  // will overwrite with backup file.
  if (this->use_backup_)
    {
      switch (method_type)
        {
        case CREATE_WITH_FILE:
          mode = "rw";
          break;
        case CREATE_WITHOUT_FILE:
          mode = "rwc";
          break;
        case ACCESSOR:
          mode = "rw";
          break;
        case MUTATOR:
          mode = "rw";
          break;
        }
    }
  else
    {
      switch (method_type)
        {
        case CREATE_WITH_FILE:
          mode = "r";
          break;
        case CREATE_WITHOUT_FILE:
          mode = "wc";
          break;
        case ACCESSOR:
          mode = "r";
          break;
        case MUTATOR:
          mode = "rw";
          break;
        }
    }

  // We only accept a subset of mode argument, check it
  rwflags_ = 0;
  for( unsigned int i = 0; i < mode.length (); i++ )
    {
      switch (mode[i])
        {
        case 'r': rwflags_ |= mode_read;
          break;
        case 'w': rwflags_ |= mode_write;
          break;
        case 'c': rwflags_ |= mode_create;
          break;
        default: rwflags_ = -1;
        }
    }
  if( rwflags_ <= 0 )
    {
      errno = EINVAL;
      throw CORBA::PERSIST_STORE();
    }

  // Create the stream
  fl_ = this->create_stream(mode.c_str ());
  if (redundant_)
    {
      if (fl_->open() != 0)
        {
          delete fl_;
          throw CORBA::PERSIST_STORE();
        }

      // acquire a lock on it
      if (fl_ -> flock(0, 0, 0) != 0)
        {
          fl_->close();
          delete fl_;
          throw CORBA::INTERNAL();
        }

      // now that the file is successfully opened and locked it must be
      // unlocked/closed before we leave this class
      closed_ = 0;

      if ( ! (rwflags_ & mode_create) )
        {
          // Check if our copy is up to date
          if (this->object_obsolete ())
            {
              this->mark_object_current ();
              this->load ();
            }
        }
    }
  else if ( ! this->is_loaded_from_stream () || (rwflags_ & mode_write) )
    {
      bool file_has_data = fl_->exists();

      if (fl_->open() != 0)
        {
          delete fl_;
          throw CORBA::PERSIST_STORE();
        }

      // now that the file is successfully opened
      // unlocked/closed before we leave this class
      closed_ = 0;

      if (file_has_data && ! this->is_loaded_from_stream ())
        {
          this->load ();
        }
    }
  else
    {
      // Need to insure that fl_ gets deleted
      delete fl_;
    }
}

bool
TAO::Storable_File_Guard::object_obsolete (void)
{ // Default implementation uses time to determine
  // if obsolete.
  return (fl_->last_changed () > this->get_object_last_changed ());
}

void
TAO::Storable_File_Guard::mark_object_current (void)
{ // Default implementation is to set the last changed
  // time to that of the file lock.
  this->set_object_last_changed (fl_->last_changed ());
}

void
TAO::Storable_File_Guard::release (void)
{
  if ( ! closed_ )
    {

      if (this->use_backup_ )
        {
          fl_->create_backup ();
        }

      // If we updated the disk, save the time stamp
      if(redundant_)
        {
          if( rwflags_ & mode_write )
            {
              // This is a costly call, but necessary if
              // we are running redundant. It ensures that
              // the data is written to disk.
              fl_->sync ();

              this->mark_object_current ();
            }

          // Release the lock
          fl_->funlock(0, 0, 0);
        }
      fl_->close();
      delete fl_;
      closed_ = 1;
    }

}

TAO::Storable_Base &
TAO::Storable_File_Guard::peer ()
{
  return *fl_;
}

void
TAO::Storable_File_Guard::load ()
{
  if (!this->use_backup_)
    {
      this->load_from_stream ();
      return;
    }

  try
    {
      this->load_from_stream ();
    }
  catch (const Storable_Read_Exception &ex)
    {
      ACE_CString state_str = Storable_Base::state_as_string (ex.get_state());

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO: (%P|%t) ERROR: State %s ")
                  ACE_TEXT ("encountered reading persistent ")
                  ACE_TEXT ("state from file\n%s\n"),
                  state_str.c_str (), ex.get_file_name().c_str ()));

      int result = this->fl_->restore_backup ();

      // result of 0 means OK.
      if (!result)
        {

          ACE_ERROR ((LM_INFO,
                      ACE_TEXT ("TAO: (%P|%t) Attempting to restore ")
                      ACE_TEXT ("from backup\n")));

          try
            {
              this->load_from_stream ();
            }
          catch (const Storable_Read_Exception)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO: (%P|%t) ERROR: Unable to restore ")
                          ACE_TEXT ("the state from backup.\n")));
              throw;
            }
          ACE_ERROR ((LM_INFO,
                      ACE_TEXT ("TAO: (%P|%t) The state was restored ")
                      ACE_TEXT ("from backup.\n")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t) ERROR: Could not read ")
                      ACE_TEXT ("backup file\n")));
            throw;
        }

    }
}
