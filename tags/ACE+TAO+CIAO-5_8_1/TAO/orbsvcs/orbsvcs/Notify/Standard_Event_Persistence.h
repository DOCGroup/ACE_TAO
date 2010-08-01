// -*- C++ -*-

//=============================================================================
/**
 *  \file    Event_Persistence_Factory.h
 *
 *  $Id$
 *
 *  The standard implementation of Event_Persistence_Factory.
 *
 *  \author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef STANDARD_EVENT_PERSISTENCE_H
#define STANDARD_EVENT_PERSISTENCE_H
#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Event_Persistence_Strategy.h"
#include "orbsvcs/Notify/Event_Persistence_Factory.h"
#include "orbsvcs/Notify/Persistent_File_Allocator.h"
#include "orbsvcs/Notify/Routing_Slip_Persistence_Manager.h"
#include <ace/SString.h>


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  static const ACE_UINT32 ROUTING_SLIP_ROOT_BLOCK_NUMBER = 0;
  static const ACE_UINT64 ROUTING_SLIP_ROOT_SERIAL_NUMBER = 1;

  /// \brief Standard (i.e. default) implementation of Event_Persistence_Factory interface
  class TAO_Notify_Serv_Export Standard_Event_Persistence_Factory :
    public Event_Persistence_Factory
  {
  public:
    /// Constructor
    Standard_Event_Persistence_Factory ();
    /// Destructor
    virtual ~Standard_Event_Persistence_Factory();

    /// Open persistence file and initialize.
    /// /param filename the fully qualified path/name of the file to contain
    ///        persistent information.
    /// /param block_size the size of a physical block on the device containing
    ///        the file.
    bool open (const ACE_TCHAR* filename, ACE_UINT32 block_size = 512);

    //////////////////////////////////////////////////////
    // Implement Event_Persistence_Factory virtual methods.
    virtual Routing_Slip_Persistence_Manager*
      create_routing_slip_persistence_manager(Persistent_Callback* callback);

    virtual Routing_Slip_Persistence_Manager * first_reload_manager();

    /// reloading process complete.  Where should the chain continue?
    void done_reloading(
      Persistent_Storage_Block * next_psb,
      ACE_UINT64 current_serial_number);

    /// return current serial # and a psb containing current record#
    /// allocate new serial # and record number, and return those, too.
    void preallocate_next_record(
      ACE_UINT64& current_serial_number,
      Persistent_Storage_Block*& current_psb,
      ACE_UINT64& next_serial_number,
      ACE_UINT32& next_block_number);

    // return block pointer info for the
    void get_preallocated_pointer(
      ACE_UINT64& next_serial_number,
      ACE_UINT32& next_block_number);

    /// Accessor for file allocator.
    /// Intended for use only by the Routing Slip Persistence Manager
    Persistent_File_Allocator* allocator();

    /// Access root record.
    /// Intended for use only by the Routing Slip Persistence Manager
    Routing_Slip_Persistence_Manager & root();

  public:
    TAO_SYNCH_MUTEX lock;

  private:
    Persistent_File_Allocator allocator_;
    Routing_Slip_Persistence_Manager root_;
    Persistent_Storage_Block* psb_;
    ACE_UINT64 serial_number_;
    bool is_reloading_;
  };

  /// \brief The standard implementation of the
  /// Event_Persistence_Strategy interface.
  class TAO_Notify_Serv_Export Standard_Event_Persistence :
    public Event_Persistence_Strategy
  {
  public :
    /// Constructor.
    Standard_Event_Persistence ();
    /// Destructor.
    virtual ~Standard_Event_Persistence ();
    /////////////////////////////////////////////
    // Override Event_Persistent_Strategy methods
    // Parse arguments and initialize.
    virtual int init(int argc, ACE_TCHAR *argv[]);
    // Prepare for shutdown
    virtual int fini ();

    // get the current factory, creating it if necessary
    virtual Event_Persistence_Factory * get_factory ();

  private:
    // release the current factory so a new one can be created
    virtual void reset ();

    ACE_TString filename_;  // set via -file_path
    ACE_UINT32 block_size_; // set via -block_size
    Standard_Event_Persistence_Factory * factory_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_Notify_Serv, TAO_Notify_Standard_Event_Persistence)

#include /**/ "ace/post.h"
#endif /* STANDARD_EVENT_PERSISTENCE_H */
