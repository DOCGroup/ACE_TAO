// -*- C++ -*-

//=============================================================================
/**
 *  @file    Routing_Slip_Persistence_Manager.h
 *
 *  $Id$
 *
 *  A Routing_Slip_Persistence manager controls the actual allocation of
 *  blocks through a Persistent_Storage_Allocator and can persist an
 *  event and its routing slip.
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef ROUTING_SLIP_PERSISTENCE_MANAGER_H
#define ROUTING_SLIP_PERSISTENCE_MANAGER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "tao/orbconf.h"
#include "ace/Message_Block.h"
#include "ace/Containers_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
// Some forward declarations.
class Standard_Event_Persistence_Factory;
class Persistent_File_Allocator;
class Persistent_Storage_Block;
class Persistent_Callback;

/**
 * \brief Manage interaction between Routing_Slip and persistent storage.
 *
 * todo: to complete the strategization of event persistent storage this
 * should become an interface that is implemented differently by different
 * strategies.  For now it interacts with Standard_Event_Persistence.
 */
class TAO_Notify_Serv_Export Routing_Slip_Persistence_Manager
{
public:
  /// A unique identifier for logical blocks in persistent storage.
  typedef ACE_UINT64 Block_Serial_Number;
  /// The physical address of a block in persistent storage.
  typedef ACE_UINT32 Block_Number;
  /// The size of a block in persistent storage.
  typedef ACE_UINT16 Block_Size;
  /// A code to indicate the type of block in persistent storage.
  typedef ACE_UINT16 Block_Type;

  /// The constructor.
  Routing_Slip_Persistence_Manager(Standard_Event_Persistence_Factory* factory);

  /// The destructor.
  ~Routing_Slip_Persistence_Manager();

  /// Set up callbacks
  void set_callback(Persistent_Callback* callback);

  /// Store an event + routing slip.
  bool store(const ACE_Message_Block& event,
    const ACE_Message_Block& routing_slip);

  /// \brief Update the routing slip.
  ///
  /// We must always overwrite the first block
  /// last, and it may not chance.  Other blocks should be freed and
  /// reallocated.
  bool update(const ACE_Message_Block& routing_slip);

  /// \brief Remove our associated event and routing slip from the
  /// Persistent_File_Allocator.
  bool remove();

  /////////////////////////////////////////
  // Methods to be used during reload only.

  /// \brief Call this method to recover data during event reload.
  ///
  /// It should not fail under normal circumstances.
  /// Caller owns the resulting message blocks and is responsible
  /// for deleting them.
  /// Reload the event and routing_slip from the Persistent_File_Allocator.
  bool reload(ACE_Message_Block*& event, ACE_Message_Block*&routing_slip);

  /// \brief Get next RSPM during reload.
  ///
  /// After using the data from the reload method, call this
  /// method to get the next RSPM.  It returns a null pointer
  /// when all persistent events have been reloaded.
  Routing_Slip_Persistence_Manager * load_next ();

  /////////////////////////
  // Implementation methods.
  // Should not be called by Routing_Slip

  /// \brief Commit root data to disk, which should only be done for a root node.
  bool store_root();

  /// \brief Reload data into this RSPM from the given block/serial#
  ///
  /// \return false if the reload is not successful.
  bool load(Block_Number block_number, Block_Serial_Number expected_serial_number);

  /// \brief Is this RSPM attached to the root block?
  bool is_root () const;

  /// \brief During cleanup for shut down, release all chained RSPMs.
  void release_all ();

private:
  /**
   * \brief private: Storage for header information of all persistent block.
   */
  class Block_Header
  {
  public:
    enum Header_Type {
      BT_Routing_Slip,
      BT_Event,
      BT_Overflow
      };

    Block_Header(Header_Type type);
    virtual ~Block_Header (void);
    virtual size_t extract_header(Persistent_Storage_Block& psb,
      size_t offset = 0);
    virtual size_t put_header(Persistent_Storage_Block& psb,
      size_t offset = 0);

  public:
    /// Our serial number
    Block_Serial_Number serial_number;
    /// Address of the overflow record (if any)
    Block_Number next_overflow;
    /// How much extra header data is in this block (not including this header)
    Block_Type header_type;
    /// How much actual data is in this block? (not including headers)
    Block_Size data_size;
  };

  /**
   * \brief private: Storage for header information for Routing_Slip blocks.
   */
  class Routing_Slip_Header : public Block_Header
  {
  public:
    Routing_Slip_Header();
    virtual size_t extract_header(Persistent_Storage_Block& psb,
      size_t offset = 0);
    virtual size_t put_header(Persistent_Storage_Block& psb,
      size_t offset = 0);

  public:
    /// The next event in the system
    Block_Number next_routing_slip_block;
    /// The next expected serial number
    Block_Serial_Number next_serial_number;
    Block_Number event_block;
  };

  /// \brief An Event block header.
  ///
  /// is just a Block_Header with no extra data
  class Event_Header : public Block_Header
  {
  public:
    Event_Header ();
  };

  /// \brief An overflow block header.
  ///
  /// is just a Block_Header with no extra data
  /// The same record type is used for both Routing_Slip
  /// and Event overflows.
  class Overflow_Header : public Block_Header
  {
  public:
    Overflow_Header ();
  };

  bool store_i(const ACE_Message_Block& event,
    const ACE_Message_Block& routing_slip);

  bool update_i(const ACE_Message_Block& routing_slip);

  bool store_event(const ACE_Message_Block& event);

  /// Fill in a block with data, and return the number of bytes
  /// of data remaining to be written.
  size_t fill_block(Persistent_Storage_Block& psb,
    size_t offset_into_block, const ACE_Message_Block* data,
    size_t offset_into_msg);
  size_t fill_block(Persistent_Storage_Block& psb,
    size_t offset_into_block, unsigned char* data,
    size_t data_size);

  /// Build a chain of Persistent_Storage_Blocks
  bool build_chain(
    Persistent_Storage_Block* first_block,
    Block_Header& first_header,
    ACE_Unbounded_Stack<size_t>& allocated_blocks,
    const ACE_Message_Block& data);

  /// Reload a chain from persistent store.
  bool reload_chain(Persistent_Storage_Block* first_block,
    Block_Header& first_header,
    ACE_Unbounded_Stack<size_t>& allocated_blocks,
    ACE_Message_Block* amb,
    ACE_UINT64 expected_serial_number);

  /// Locked method to do the work of setting the next_manager_.
  bool update_next_manager(Routing_Slip_Persistence_Manager* next);

  /// Have we been persisted yet?
  bool persisted();

  /// Write out our first event block.
  size_t write_first_routing_slip_block(bool prepare_only = false);

  /// Insert ourselves into a linked list of Routing_Slip_Persistnce_Managers
  void dllist_push_back();

  void insert_before (Routing_Slip_Persistence_Manager * node);

  /// Remove ourselves from a linked list of Routing_Slip_Persistence_Managers
  void remove_from_dllist();

private:
  TAO_SYNCH_MUTEX lock_;
  bool removed_;
  ACE_UINT64 serial_number_;
  Persistent_File_Allocator* allocator_;
  Standard_Event_Persistence_Factory* factory_;
  Event_Header event_header_;
  Routing_Slip_Header routing_slip_header_;
  Persistent_Storage_Block* first_event_block_;
  Persistent_Storage_Block* first_routing_slip_block_;
  /// We are part of a doubly-linked list
  Routing_Slip_Persistence_Manager* prev_manager_;
  Routing_Slip_Persistence_Manager* next_manager_;
  ACE_Unbounded_Stack<size_t> allocated_event_blocks_;
  ACE_Unbounded_Stack<size_t> allocated_routing_slip_blocks_;
  Persistent_Callback* callback_;

  /// If these are non-zero we own 'em
  ACE_Message_Block * event_mb_;
  ACE_Message_Block * routing_slip_mb_;
};

} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ROUTING_SLIP_PERSISTENCE_MANAGER_H */
