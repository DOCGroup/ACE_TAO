/* -*- C++ -*- */

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
#include "ace/pre.h"

#include "IORTable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_IORTable_Export TAO_IOR_Table_Impl : public virtual IORTable::Table, public virtual TAO_Local_RefCounted_Object
{
public:
  /// Constructor
  TAO_IOR_Table_Impl (void);

  /// Find the object, using the locator if it is not on the table.
  char *find (
      const char *object_key
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      IORTable::NotFound
    ));

  // = The IORTable::Table methods, check the IORTable.pidl file for
  // details.
    virtual void bind (
        const char * object_key,
        const char * IOR
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)

      ACE_THROW_SPEC ((
        CORBA::SystemException,
        IORTable::AlreadyBound
      ));

    virtual void rebind (
        const char * object_key,
        const char * IOR
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)

      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    virtual void unbind (
        const char * object_key
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)

      ACE_THROW_SPEC ((
        CORBA::SystemException,
        IORTable::NotFound
      ));

    virtual void set_locator (
        IORTable::Locator_ptr the_locator
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)

      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

private:
  typedef ACE_Hash_Map_Manager<ACE_CString,ACE_CString,ACE_Null_Mutex> Map;

  /// The map
  Map map_;

  /// The locator
  IORTable::Locator_var locator_;

  /// Synchronization
  TAO_SYNCH_MUTEX lock_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_IOR_TABLE_IMPL */
