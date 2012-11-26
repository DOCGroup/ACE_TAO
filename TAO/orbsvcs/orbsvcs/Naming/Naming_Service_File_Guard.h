// -*- C++ -*-

//=============================================================================
/**
 *  @file   Naming_Service_File_Guard.h
 *
 *  $Id$
 *
 *  @author Rich Seibel (seibelr@ociweb.com)
 *  @author Byron Harris (harrisb@ociweb.com)
 */
//=============================================================================

#ifndef TAO_NAMING_SERVICE_FILE_GUARD_H
#define TAO_NAMING_SERVICE_FILE_GUARD_H

#include "tao/orbconf.h"
#include "orbsvcs/Naming/naming_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Storable_Base;

/**
 * @class Naming_Service_File_Guard
 * @brief Bridge abstract class for TAO_Storable_Naming_Context that performs locking.
 *
 * A guard for TAO_Storable_Naming_Context that opens a file
 * for read/write and sets a lock on it. It then checks if the file has
 * changed and re-reads it if it has.
 *
 * The destructor releases the lock.
 */
class TAO_Naming_Serv_Export Naming_Service_File_Guard
{
public:

  Naming_Service_File_Guard (bool redundant);

  virtual ~Naming_Service_File_Guard ();

  /// Releases the lock, closes the file, and deletes the I/O stream.
  void release (void);

  /// Returns the stream to read/write on
  TAO_Storable_Base & peer (void);

protected:

  /// Should be called by constructors of derived classes
  /// since can't call virtual functions below in constructor
  /// of this class.
  void init (const char * mode);

  virtual void set_parent_last_changed (const time_t & time) = 0;

  virtual time_t get_parent_last_changed () = 0;

  virtual void create_child () = 0;

  virtual bool is_child_created () = 0;

  virtual TAO_Storable_Base * create_stream (const char * mode) = 0;

  /// The pointer to the actual file I/O (bridge pattern)
  TAO_Storable_Base *fl_;

private:

  bool redundant_;

  /// Default constructor
  Naming_Service_File_Guard ();

  /// A flag to keep us from trying to close things more than once.
  int closed_;

  /// The flags that we were opened with
  int rwflags_;

  /// Symbolic values for the flags in the above
  enum { mode_write = 1, mode_read = 2, mode_create = 4 };

};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
