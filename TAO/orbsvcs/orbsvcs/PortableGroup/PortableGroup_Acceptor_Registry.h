// -*- C++ -*-

//=============================================================================
/**
 *  @file     PortableGroup_Acceptor_Registry.h
 *
 *  $Id$
 *
 *  Registry for dynamically created acceptors.
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PORTABLEGROUP_ACCEPTOR_REGISTRY_H
#define TAO_PORTABLEGROUP_ACCEPTOR_REGISTRY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "portablegroup_export.h"

#include "tao/Transport_Acceptor.h"
#include "ace/Unbounded_Queue.h"
#include "tao/Resource_Factory.h"

class TAO_Profile;

/**
 * @class TAO_PortableGroup_Acceptor_Registry
 *
 * @brief .
 *
 * There is one TAO_PortableGroup_Acceptor_Registry per ORB_Core.
 */
class TAO_PortableGroup_Export TAO_PortableGroup_Acceptor_Registry
{
public:

  /**
   * @struct Entry
   *
   * @brief Value field of the portable group acceptor registry.
   */
  struct Entry
  {
    /// Pointer to the acceptor for this GroupId.
    TAO_Acceptor *acceptor;

    /// Cached endpoint that the acceptor is listening on.
    TAO_Endpoint *endpoint;

    /// Count of times that this GroupId has been opened.
    int cnt;
  };


  // = Initialization and termination methods.
  ///  Default constructor.
  TAO_PortableGroup_Acceptor_Registry (void);

  ///  Default destructor.
  ~TAO_PortableGroup_Acceptor_Registry (void);

  /// Open an acceptor based on a tagged profile.
  void open (const TAO_Profile* profile,
             TAO_ORB_Core &orb_core
             ACE_ENV_ARG_DECL);

  /// Close all open acceptors.
  int close_all (void);

  typedef ACE_Unbounded_Queue<Entry> Acceptor_Registry;
  typedef ACE_Unbounded_Queue_Iterator<Entry> Acceptor_Registry_Iterator;

protected:

  /// Helper function to open a group acceptor.
  void open_i (const TAO_Profile* profile,
               TAO_ORB_Core &orb_core,
               TAO_ProtocolFactorySetItor &factory
               ACE_ENV_ARG_DECL);

  /// Find an acceptor by using a profile.
  int find (const TAO_Profile* profile,
            Entry *&entry);

private:
  // The acceptor registry should not be copied.
  ACE_UNIMPLEMENTED_FUNC (TAO_PortableGroup_Acceptor_Registry (const TAO_PortableGroup_Acceptor_Registry&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_PortableGroup_Acceptor_Registry&))

private:
  /// List of acceptors that are currently open.
  Acceptor_Registry registry_;
};

//#if defined(__ACE_INLINE__)
//#include "tao/PortableGroup_Acceptor_Registry.i"
//#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_PORTABLEGROUP_ACCEPTOR_REGISTRY_H */
