/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    IORTable
//
// = FILENAME
//    Object_Adapter.h
//
// = AUTHOR
//    Carlos O'Ryan (coryan@uci.edu)
//
// ============================================================================

#ifndef TAO_TABLE_ADAPTER_H
#define TAO_TABLE_ADAPTER_H
#include "ace/pre.h"

#include "iortable_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Adapter.h"
#include "ace/Service_Config.h"

class TAO_IOR_Table_Impl;

class TAO_IORTable_Export TAO_Table_Adapter : public TAO_Adapter
{
public:
  TAO_Table_Adapter (TAO_ORB_Core *orb_core);
  // Constructor

  virtual ~TAO_Table_Adapter (void);
  // Destructor

  // = The TAO_Adapter methods, please check tao/Adapter.h for the
  // documentation
  virtual void open (CORBA::Environment &ACE_TRY_ENV);
  virtual void close (int wait_for_completion,
                      CORBA::Environment &ACE_TRY_ENV);
  virtual void check_close (int wait_for_completion,
                            CORBA::Environment &ACE_TRY_ENV);
  virtual int priority (void) const;
  virtual int dispatch (TAO_ObjectKey &key,
                        TAO_ServerRequest &request,
                        void *context, /* unused? */
                        CORBA::Object_out foward_to,
                        CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual const char *name (void) const;
  virtual CORBA::Object_ptr root (void);
  virtual CORBA::Object_ptr create_collocated_object (TAO_Stub *,
                                                      const TAO_MProfile &);

private:
  TAO_ORB_Core *orb_core_;
  // The ORB Core we belong to

  TAO_IOR_Table_Impl *root_;
  // The table implementation
};

// ****************************************************************

class TAO_IORTable_Export TAO_Table_Adapter_Factory : public TAO_Adapter_Factory
{
public:
  TAO_Table_Adapter_Factory (void);
  // Constructor

  // = The TAO_Adapter_Factory methods, please read tao/Adapter.h for
  // details.
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core);
};

ACE_STATIC_SVC_DECLARE (TAO_Table_Adapter_Factory)
ACE_FACTORY_DECLARE (TAO_IORTable, TAO_Table_Adapter_Factory)

#include "ace/post.h"
#endif /* TAO_TABLE_ADAPTER_H */
