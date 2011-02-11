// -*- C++ -*-

//=============================================================================
/**
 *  @file Table_Adapter.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@uci.edu)
 */
//=============================================================================


#ifndef TAO_TABLE_ADAPTER_H
#define TAO_TABLE_ADAPTER_H
#include /**/ "ace/pre.h"

#include "tao/IORTable/iortable_export.h"
#include "tao/IORTable/IORTable.h"
#include "tao/IORTable/IOR_Table_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Adapter.h"
#include "tao/Adapter_Factory.h"
#include "ace/Service_Config.h"
#include "ace/Lock.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IORTable_Export TAO_Table_Adapter : public TAO_Adapter
{
public:

  /// Constructor
  TAO_Table_Adapter (TAO_ORB_Core &orb_core);

  /// Destructor
  virtual ~TAO_Table_Adapter (void);

  // = The TAO_Adapter methods, please check tao/Adapter.h for the
  // documentation
  virtual void open (void);
  virtual void close (int wait_for_completion);
  virtual void check_close (int wait_for_completion);
  virtual int priority (void) const;
  virtual int dispatch (TAO::ObjectKey &key,
                        TAO_ServerRequest &request,
                        CORBA::Object_out foward_to);

  virtual const char *name (void) const;
  virtual CORBA::Object_ptr root (void);
  virtual CORBA::Object_ptr create_collocated_object (TAO_Stub *,
                                                      const TAO_MProfile &);

  virtual CORBA::Long initialize_collocated_object (TAO_Stub *stub);

private:
  static ACE_Lock * create_lock (TAO_SYNCH_MUTEX &l);

  /// Helper method to find an object bound in the table.
  bool find_object (TAO::ObjectKey &key,
                    CORBA::Object_out forward_to);

  /// The ORB Core we belong to
  TAO_ORB_Core &orb_core_;

  /// The table implementation
  TAO_IOR_Table_Impl_var root_;

  bool closed_;
  TAO_SYNCH_MUTEX thread_lock_;
  ACE_Lock *lock_;
};

// ****************************************************************

class TAO_IORTable_Export TAO_Table_Adapter_Factory : public TAO_Adapter_Factory
{
public:
  /// Constructor
  TAO_Table_Adapter_Factory (void);

  /// The TAO_Adapter_Factory methods, please read tao/Adapter.h for
  /// details.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_Table_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_IORTable, TAO_Table_Adapter_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_TABLE_ADAPTER_H */
