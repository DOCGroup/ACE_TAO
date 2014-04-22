// -*- C++ -*-

//=============================================================================
/**
 *  @file Async_Table_Adapter.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_ASYNC_TABLE_ADAPTER_H
#define TAO_ASYNC_TABLE_ADAPTER_H
#include /**/ "ace/pre.h"

#include "tao/IORTable/async_iortable_export.h"
#include "tao/IORTable/Async_IORTable.h"
#include "tao/IORTable/Async_IOR_Table_Impl.h"

#include "tao/IORTable/Table_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Async_IORTable_Export TAO_Async_Table_Adapter : public TAO_Table_Adapter
{
public:

  /// Constructor
  TAO_Async_Table_Adapter (TAO_ORB_Core &orb_core);

  /// Destructor
  virtual ~TAO_Async_Table_Adapter (void);

  // = The TAO_Adapter methods, please check tao/Adapter.h for the
  // documentation
  virtual void open (void);

  virtual int dispatch (TAO::ObjectKey &key,
                        TAO_ServerRequest &request,
                        CORBA::Object_out foward_to);

protected:
  /// Helper method to find an object bound in the table.
  void find_object (::IORTable::Locate_ResponseHandler rh,
                    TAO::ObjectKey &key);

};

// ****************************************************************

class TAO_Async_IORTable_Export TAO_Async_Table_Adapter_Factory :
  public TAO_Table_Adapter_Factory
{
public:
  /// Constructor
  TAO_Async_Table_Adapter_Factory (void);

  /// The TAO_Adapter_Factory methods, please read tao/Adapter.h for
  /// details.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_Async_Table_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_Async_IORTable, TAO_Async_Table_Adapter_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_ASYNC_TABLE_ADAPTER_H */
