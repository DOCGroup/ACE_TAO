// -*- C++ -*-

//=============================================================================
/**
 *  @file    XML_Topology_Factory.h
 *
 *  $Id$
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef XML_TOPOLOGY_FACTORY_H
#define XML_TOPOLOGY_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/Topology_Factory.h"
#include "orbsvcs/Notify/notify_persist_export.h"

#include "tao/corba.h"
#include "ace/SString.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  extern const char TOPOLOGY_ID_NAME[];

  /**
   * \brief Create XML topology savers and loaders
   *
   * Loaded by a svc.conf line like:
   * dynamic Topology_Factory Service_Object* TAO_CosNotificationd:_make_XML_Topology_Factory() "[arguments]"
   *  where arguments are:
   *    -base_path        Base path (directory and filename) for both saving and loading.
   *                      .xml will be appended to the base path
   *                      Default is ./Notification_Service_Topology
   *    -save_base_path   Base path for saving.
   *    -load_base_path   Base path for loading.
   *    -file_count       How many backup copies to keep.  Default is 1
   *    -no_timestamp     Disable timestamping (makes files diffable)
   *        Note: you can set both saving and storing to the same file using -base_path
   *              Or you can set them independently using -save_base_path
   *              and -load_base_path
   *
   */
  class TAO_Notify_Persist_Export XML_Topology_Factory : public Topology_Factory
  {
  public:
    /// The constructor.
    XML_Topology_Factory ();
    virtual ~XML_Topology_Factory ();

    ////////////////////////////////////
    // Override Topology_Factory methods
    // documented in Topology_Factory.h
    virtual Topology_Saver* create_saver ();
    virtual Topology_Loader* create_loader();

    ////////////////////////////////
    // Override Share_Object methods
    // documented in ace/Shared_Object.h
    virtual int init (int argc, ACE_TCHAR *argv[]);
    virtual int fini ();

   private:
    ACE_TString save_base_path_;
    ACE_TString load_base_path_;
    size_t backup_count_;
    bool timestamp_;
  };

} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_Notify_Persist, TAO_Notify_XML_Topology_Factory)

#include /**/ "ace/post.h"
#endif /* XML_TOPOLOGY_FACTORY_H */
