
//=============================================================================
/**
 *  @file    IORManip_Filter.h
 *
 *  $Id$
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================


#ifndef TAO_IORMANIP_FILTER_H
#define TAO_IORMANIP_FILTER_H
#include /**/ "ace/pre.h"

#include "tao/GIOP_Message_State.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "tao/IORManipulation/ior_manip_export.h"
#include "tao/Environment.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Profile;
class TAO_MProfile;

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
}

class TAO_IORManip_Export TAO_IORManip_Filter
{
public:
  /// Constructor.
  TAO_IORManip_Filter (void);

  /// Destructor.
  virtual ~TAO_IORManip_Filter (void);

  /// Perform filtering using the profile passed in as a guide.
  /// If no profile is provided, filter using the profile_matches() method.
  CORBA::Object_ptr sanitize_profiles (CORBA::Object_ptr object,
                                       TAO_Profile* profile = 0);

protected:

  /// This will be the bulk of the filtering code.
  virtual void filter_and_add (TAO_Profile* profile,
                               TAO_MProfile& profiles,
                               TAO_Profile* guideline = 0) = 0;


private:

  /// The sanitize_profiles() methods call this to do the work.
  CORBA::Object_ptr sanitize (CORBA::Object_ptr object,
                              TAO_Profile* profile);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_IORMANIP_FILTER_H */
