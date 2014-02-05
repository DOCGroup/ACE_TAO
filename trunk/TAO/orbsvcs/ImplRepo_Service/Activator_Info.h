// $Id$
#ifndef ACTIVATOR_INFO_H
#define ACTIVATOR_INFO_H

#include "ace/Bound_Ptr.h"
#include "ace/SString.h"

#include "ImR_ActivatorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
* @brief Information about IMR registered Activators.
*/
struct Activator_Info
{
  Activator_Info (void);
  Activator_Info (const Activator_Info& other);

  Activator_Info (const ACE_CString& aname,
    CORBA::Long atoken,
    const ACE_CString& aior,
    ImplementationRepository::Activator_ptr act =
    ImplementationRepository::Activator::_nil ());

  void clear (void);

  /// Reset the connection portion
  void reset_runtime(void);

  ACE_CString name;
  CORBA::Long token;
  ACE_CString ior;
  ImplementationRepository::Activator_var activator;
};

typedef ACE_Strong_Bound_Ptr<Activator_Info, ACE_Null_Mutex> Activator_Info_Ptr;

#endif /* ACTIVATOR_INFO_H */
