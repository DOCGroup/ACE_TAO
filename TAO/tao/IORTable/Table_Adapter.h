// -*- C++ -*-

//=============================================================================
/**
 *  @file Table_Adapter.h
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
  ~TAO_Table_Adapter () override;

  // = The TAO_Adapter methods, please check tao/Adapter.h for the
  // documentation
  void open () override;
  void close (int wait_for_completion) override;
  void check_close (int wait_for_completion) override;
  int priority () const override;
  int dispatch (TAO::ObjectKey &key,
                TAO_ServerRequest &request,
                CORBA::Object_out foward_to) override;

  char const *name () const override;
  CORBA::Object_ptr root () override;
  CORBA::Object_ptr create_collocated_object (TAO_Stub *,
                                              TAO_MProfile const &) override;

  CORBA::Long initialize_collocated_object (TAO_Stub *stub) override;

protected:
  static ACE_Lock * create_lock (TAO_SYNCH_MUTEX &l);

  /// Helper method to find an object bound in the table.
  bool find_object (TAO::ObjectKey &key,
                    CORBA::Object_out forward_to);

  /// The ORB Core we belong to
  TAO_ORB_Core &orb_core_;

  /// The table implementation
  TAO_IOR_Table_Impl_var root_;

  bool closed_ {true};
  TAO_SYNCH_MUTEX thread_lock_;
  ACE_Lock *lock_;

private:
  TAO_Table_Adapter (TAO_Table_Adapter const &) = delete;
  TAO_Table_Adapter &operator= (TAO_Table_Adapter const &) = delete;
  TAO_Table_Adapter (TAO_Table_Adapter &&) = delete;
  TAO_Table_Adapter &operator= (TAO_Table_Adapter &&) = delete;
};

// ****************************************************************

class TAO_IORTable_Export TAO_Table_Adapter_Factory : public TAO_Adapter_Factory
{
public:
  /// Constructor
  TAO_Table_Adapter_Factory ();

  /// The TAO_Adapter_Factory methods, please read tao/Adapter.h for
  /// details.
  TAO_Adapter *create (TAO_ORB_Core *orb_core) override;

private:
  TAO_Table_Adapter_Factory (TAO_Table_Adapter_Factory const &) = delete;
  TAO_Table_Adapter_Factory &operator= (TAO_Table_Adapter_Factory const &) = delete;
  TAO_Table_Adapter_Factory (TAO_Table_Adapter_Factory &&) = delete;
  TAO_Table_Adapter_Factory &operator= (TAO_Table_Adapter_Factory &&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_Table_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_IORTable, TAO_Table_Adapter_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_TABLE_ADAPTER_H */
