// -*- C++ -*-

//=============================================================================
/**
 *  @file IOR_Table_Impl.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@uci.edu)
 */
//=============================================================================


#ifndef TAO_IOR_TABLE_IMPL_H
#define TAO_IOR_TABLE_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/IORTable/IORTable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"
#include "tao/LocalObject.h"
#include "ace/SString.h"
#include "tao/Objref_VarOut_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IORTable_Export TAO_IOR_Table_Impl;
typedef TAO_IOR_Table_Impl *TAO_IOR_Table_Impl_ptr;
typedef TAO_Objref_Var_T <TAO_IOR_Table_Impl> TAO_IOR_Table_Impl_var;
typedef TAO_Objref_Out_T <TAO_IOR_Table_Impl> TAO_IOR_Table_Impl_out;

class TAO_IORTable_Export TAO_IOR_Table_Impl
  : public virtual IORTable::Table,
    public virtual ::CORBA::LocalObject
{
public:
  static TAO_IOR_Table_Impl_ptr
  _nil (void)
  {
    return static_cast<TAO_IOR_Table_Impl_ptr> (0);
  }

  /// Constructor
  TAO_IOR_Table_Impl (void);

  virtual bool async_available (void) { return false; }

  /// Find the object, using the locator if it is not on the table.
  char *find (const char *object_key);

  /**
   * @name The IORTable::Table methods
   *
   * Please check the IORTable.pidl file for details.
   */
  //@{
  virtual void bind (const char * object_key, const char * IOR);

  virtual void rebind (const char * object_key, const char * IOR);

  virtual void unbind (const char * object_key);

  virtual void set_locator (IORTable::Locator_ptr the_locator);
  //@}

protected:
  typedef ACE_Hash_Map_Manager_Ex <ACE_CString, ACE_CString,
                                   ACE_Hash <ACE_CString>,
                                   ACE_Equal_To <ACE_CString>,
                                   ACE_Null_Mutex> Map;

  /// The map
  Map map_;

  /// The locator
  IORTable::Locator_var locator_;

  /// Synchronization
  TAO_SYNCH_MUTEX lock_;
};

// Traits specializations.
namespace TAO
{
  template<>
  struct TAO_IORTable_Export
  Objref_Traits <TAO_IOR_Table_Impl>
  {
    static TAO_IOR_Table_Impl_ptr duplicate (TAO_IOR_Table_Impl_ptr);

    static void release (TAO_IOR_Table_Impl_ptr);

    static TAO_IOR_Table_Impl_ptr nil (void);

    static ::CORBA::Boolean marshal (const TAO_IOR_Table_Impl_ptr p,
                                     TAO_OutputCDR &cdr);
  }
  ;
}


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_IOR_TABLE_IMPL */
