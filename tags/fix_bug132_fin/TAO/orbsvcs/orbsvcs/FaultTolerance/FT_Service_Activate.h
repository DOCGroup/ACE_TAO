// $Id$
//
// ============================================================================
//
// = LIBRARY
//    FT
//
// = FILENAME
//    FT_Callbacks.h
//
// = DESCRIPTION
//   A concrete implementation of a service callback
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef TAO_FT_ACTIVATE_H
#define TAO_FT_ACTIVATE_H
#include "ace/pre.h"

#include "fault_tol_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Services_Activate.h"
#include "ace/Service_Config.h"

class TAO_Service_Callbacks;

// Forward declarations
class TAO_FT_Export TAO_FT_Service_Activate : public TAO_Services_Activate
{
  // = TITLE
  //   A class to dynamically load the FT callback implementations in
  //   to the ORB.
  //
  // = DESCRIPTION
  //   An implementation of the service callbacks that allows the
  //   loading of the FT ORB level library in to the ORB

public:
  TAO_FT_Service_Activate (void);
  // Constructor

  virtual ~TAO_FT_Service_Activate (void);
  // The destructor

  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initializes object when dynamic linking occurs.

  virtual TAO_Service_Callbacks *activate_services (TAO_ORB_Core *orb)
    ACE_THROW_SPEC (());
  // Create and activate the service callbacks into the orb.
  // This method cannot throw any exception, but it can return a nil
  // object to indicate an error condition.

  static int Initializer (void);
  // Used to force the initialization.
};

ACE_STATIC_SVC_DECLARE (TAO_FT_Service_Activate)
ACE_FACTORY_DECLARE (TAO_FT, TAO_FT_Service_Activate)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_FT_Requires_Service_Activate = &TAO_FT_Service_Activate::Initializer;

#else

static int
TAO_FT_Requires_Service_Activate = TAO_FT_Service_Activate::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#define TAO_FT_SERVICE_SAFE_INCLUDE
#include "FT_IOGR_Property.h"
#undef TAO_FT_SERVICE_SAFE_INCLUDE

#include "ace/post.h"
#endif /*TAO_FT_ACTIVATE_H*/
