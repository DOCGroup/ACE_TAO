//=============================================================================
/**
 *  @file   Storable_File_Guard.cpp
 *
 *  @author Rich Seibel (seibelr@ociweb.com)
 *  @author Byron Harris (harrisb@ociweb.com)
 */
//=============================================================================

#include "tao/Storable_File_Guard.h"
#include "tao/Storable_Base.h"

#include "tao/SystemException.h"
#include "tao/debug.h"

TAO::Storable_File_Guard::
Storable_File_Guard (bool redundant, bool use_backup)
  : fl_(nullptr)
  , redundant_(redundant)
  , closed_(1)
  , rwflags_(0)
  , use_backup_(use_backup)
{
}

TAO::Storable_File_Guard::
~Storable_File_Guard () noexcept(false)
{
  delete fl_;
}

void
TAO::Storable_File_Guard::init_no_load(Method_Type method_type)
{
  ACE_CString mode;
  this->rwflags_ = 0;
  switch (method_type)
    {
    case ACCESSOR:
    case CREATE_WITH_FILE:
      mode = "r";
      this->rwflags_ = mode_read;
      break;
    case CREATE_WITHOUT_FILE:
      mode = "rwc";
      this->rwflags_ = mode_write | mode_create;
      break;
    case MUTATOR:
      mode = "rw";
      this->rwflags_ = mode_read | mode_write;
      break;
    }

  if( rwflags_ <= 0 )
    {
      errno = EINVAL;
      if (TAO_debug_level > 0)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                         ACE_TEXT ("(%P|%t) Storable_File_Guard:Invalid ")
                         ACE_TEXT ("flags\n")));
        }
      throw CORBA::PERSIST_STORE();
    }

  // Create the stream
  fl_ = this->create_stream(mode.c_str ());
}

void
TAO::Storable_File_Guard::reload ()
{
  if (redundant_)
    {
      if (fl_->open () != 0)
        {
          if (TAO_debug_level > 0)
            {
              TAOLIB_DEBUG ((LM_DEBUG,
                             ACE_TEXT ("(%P|%t) Storable_File_Guard::Open ")
                             ACE_TEXT ("failed in redundant\n")));
            }
          throw CORBA::PERSIST_STORE();
        }

      // acquire a lock on it
      if (fl_->flock (0, 0, 0) != 0)
        {
          fl_->close ();
          if (TAO_debug_level > 0)
            {
              TAOLIB_DEBUG ((LM_DEBUG,
                             ACE_TEXT ("(%P|%t) Storable_File_Guard:flock ")
                             ACE_TEXT ("failed in redundant\n")));
            }
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
      bool file_has_data = fl_->exists ();

      if (fl_->open () != 0)
        {
          if (TAO_debug_level > 0)
            {
              TAOLIB_DEBUG ((LM_DEBUG,
                             ACE_TEXT ("(%P|%t) Storable_File_Guard:Open ")
                             ACE_TEXT ("failed in non-redundant\n")));
            }
          throw CORBA::PERSIST_STORE ();
        }

      // now that the file is successfully opened
      // unlocked/closed before we leave this class
      closed_ = 0;

      if (file_has_data && ! this->is_loaded_from_stream ())
        {
          this->load ();
        }
    }
}

void
TAO::Storable_File_Guard::init(Method_Type method_type)
{
  this->init_no_load (method_type);
  this->reload ();
}

bool
TAO::Storable_File_Guard::object_obsolete ()
{ // Default implementation uses time to determine
  // if obsolete.
  return (fl_->last_changed () > this->get_object_last_changed ());
}

void
TAO::Storable_File_Guard::mark_object_current ()
{ // Default implementation is to set the last changed
  // time to that of the file lock.
  this->set_object_last_changed (fl_->last_changed ());
}

void
TAO::Storable_File_Guard::release ()
{
  if ( ! closed_ )
    {
      if (this->use_backup_ &&
          (rwflags_ & mode_write) != 0)
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
          fl_->funlock (0, 0, 0);
        }
      fl_->close ();
      closed_ = 1;
    }

}

TAO::Storable_Base &
TAO::Storable_File_Guard::peer ()
{
  return *fl_;
}

int
TAO::Storable_File_Guard::load ()
{
  int result = 0;
  if (!this->use_backup_)
    { // Not using a backup file. Just attempt to
      // load from the stream and return.
      return this->load_from_stream ();
    }

  // We are using a backup.
  try
    {
      // Attempt to load from the stream from the primary.
      result = this->load_from_stream ();
    }
  catch (const Storable_Read_Exception &ex)
    { // Failed to load from primary.
      ACE_CString state_str = Storable_Base::state_as_string (ex.get_state());

      TAOLIB_ERROR ((LM_ERROR,
                     ACE_TEXT ("TAO: (%P|%t) ERROR: State %s ")
                     ACE_TEXT ("encountered reading persistent ")
                     ACE_TEXT ("state from file\n%s\n"),
                     state_str.c_str (), ex.get_file_name().c_str ()));

      // The following opens the backup file, and copies it
      // to the primary file location.
      result = this->fl_->restore_backup ();

      // result of 0 means OK. We should now have a good primary file.
      if (!result)
        {
          TAOLIB_ERROR ((LM_INFO,
                         ACE_TEXT ("TAO: (%P|%t) Attempting to restore ")
                         ACE_TEXT ("from backup\n")));

          try
            { // Load the data from the newly restored primary.
              result = this->load_from_stream ();
            }
          catch (const Storable_Read_Exception&)
            { // Still having trouble reading from the file. Time to bail.
              TAOLIB_ERROR ((LM_ERROR,
                             ACE_TEXT ("TAO: (%P|%t) ERROR: Unable to restore ")
                             ACE_TEXT ("the state from backup.\n")));
              throw;
            }
          TAOLIB_ERROR ((LM_INFO,
                         ACE_TEXT ("TAO: (%P|%t) The state was restored ")
                         ACE_TEXT ("from backup.\n")));
        }
      else
        {
          TAOLIB_ERROR ((LM_ERROR,
                         ACE_TEXT ("TAO: (%P|%t) ERROR: Could not read ")
                         ACE_TEXT ("backup file\n")));
          throw;
        }

    }

  // Return a result value for the load operation.
  return result;
}
