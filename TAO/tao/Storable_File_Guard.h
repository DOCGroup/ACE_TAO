// -*- C++ -*-

//=============================================================================
/**
 *  @file   Storable_File_Guard.h
 *
 *  $Id$
 *
 *  @author Rich Seibel (seibelr@ociweb.com)
 *  @author Byron Harris (harrisb@ociweb.com)
 */
//=============================================================================

#ifndef TAO_STORABLE_FILE_GUARD_H
#define TAO_STORABLE_FILE_GUARD_H

#include "tao/Storable_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class Storable_File_Guard
   * @brief Base class to use with TAO_Storable_Base to synch object state
   * with a storable base.
   *
   * A guard for Storable_Base that opens a file
   * for read/write and sets a lock on it. It then checks if the file has
   * changed and re-reads it if it has.
   *
   */
  class TAO_Export Storable_File_Guard
  {
  public:

    Storable_File_Guard (bool redundant,
                         bool use_backup = Storable_Base::use_backup_default);

    virtual ~Storable_File_Guard ();

    /// Releases the lock, closes the file, and deletes the I/O stream.
    /// Destructors of derived classes should call this this will
    /// virtual functions are available.
    void release (void);

    /// Return when the object in memory has last changed
    virtual time_t get_object_last_changed () = 0;

    /// Get the underlying stream being used.
    TAO::Storable_Base & peer ();

    /// Indicate how the state of the object is being used.
    /// This is used for determine the mode for accessing
    /// the persistent store.
    enum Method_Type
    {

      /// Construction with persistent file already existing
      CREATE_WITH_FILE,

      /// Construction with persistent file not existing
      CREATE_WITHOUT_FILE,

      /// Getting object state
      ACCESSOR,

      /// Setting object state
      MUTATOR
    };

  protected:

    /// Should be called by constructors of derived classes
    /// since can't call the virtual functions below in constructor
    /// of this class.
    void init (Method_Type method_type);

    /// Check if the object is current with the last update.
    virtual bool object_obsolete (void);

    /// Mark the object as up to date
    virtual void mark_object_current (void);

    /// Indicate when the object in memory has last changed
    virtual void set_object_last_changed (const time_t & time) = 0;

    /// Load object from file to memory
    virtual void load_from_stream () = 0;

    /// Answer if object has been loaded from file
    virtual bool is_loaded_from_stream () = 0;

    virtual Storable_Base * create_stream (const ACE_TCHAR * mode) = 0;

    /// The pointer to the actual file I/O (bridge pattern)
    Storable_Base *fl_;

  private:

    void load ();

    bool redundant_;

    /// A flag to keep us from trying to close things more than once.
    int closed_;

    /// The flags that we were opened with
    int rwflags_;

    /// A flag indicating if backup/restore should be performed
    bool use_backup_;

    /// Symbolic values for the flags in the above
    enum { mode_write = 1, mode_read = 2, mode_create = 4 };

  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
