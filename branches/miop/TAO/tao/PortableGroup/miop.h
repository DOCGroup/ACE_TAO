// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    miop.h
//
// = DESCRIPTION
//    This class implements MIOP proprietary interface to the ORB
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_MIOP_H
#define TAO_MIOP_H
#include "ace/pre.h"

#include "PortableGroup_Loader.h"
#include "tao/LocalObject.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "miopC.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_PortableGroup_Export TAO_MOGF_impl
  : public MIOP::MulticastObjectGroupFactory,
    public TAO_Local_RefCounted_Object
{
  // = TITLE
  //     Multicast Object Group Factory class
  //
  // = DESCRIPTION
  //
  //
public:

  // constructor
  TAO_MOGF_impl (TAO_ORB_Core *orb_core,
                 const char *domain_id);

  // destructor
  ~TAO_MOGF_impl (void);

  CORBA::Object_ptr create_group (
          const char * type_id,
          const MIOP::MulticastObjectGroupFactory::ipaddr class_d_address,
          CORBA::UShort port,
          CORBA::Environment &ACE_TRY_ENV =
            TAO_default_environment ()
        )
        ACE_THROW_SPEC ((
          CORBA::SystemException
        ));

protected:
  // Create and return a new group ID.
  PortableGroup::ObjectGroupId next_group_id (void);


protected:
  TAO_ORB_Core *orb_core_;

  // Domain ID used for all Group ID's created by this class.
  ACE_CString domain_id_;

  // The current Group ID.
  PortableGroup::ObjectGroupId group_id_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_IOR_MANIPULATION_H */
