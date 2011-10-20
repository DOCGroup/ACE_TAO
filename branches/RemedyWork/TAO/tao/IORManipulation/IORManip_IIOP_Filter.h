
//=============================================================================
/**
 *  @file    IORManip_IIOP_Filter.h
 *
 *  $Id$
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================


#ifndef TAO_IORMANIP_IIOP_FILTER_H
#define TAO_IORMANIP_IIOP_FILTER_H
#include /**/ "ace/pre.h"

#include "IORManip_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/IIOP_EndpointsC.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IIOP_Profile;
namespace TAO
{
  class IIOPEndpointSequence;
}

class TAO_IORManip_Export TAO_IORManip_IIOP_Filter: public TAO_IORManip_Filter
{
public:
  struct Profile_Info
  {
    ACE_CString host_name_;
    TAO_GIOP_Message_Version version_;
    CORBA::UShort port_;
  };

  /// Constructor.
  TAO_IORManip_IIOP_Filter (void);

  /// Destructor.
  virtual ~TAO_IORManip_IIOP_Filter (void);

  /// Compares the profile to the profile info.
  virtual CORBA::Boolean compare_profile_info (
                         const TAO_IORManip_IIOP_Filter::Profile_Info& left,
                         const TAO_IORManip_IIOP_Filter::Profile_Info& right);

  /// Empty virtual method to match on the profile info.
  /// Users must provide an implementation to use the first
  /// form of sanitize_profiles().
  virtual CORBA::Boolean profile_info_matches (
                         const TAO_IORManip_IIOP_Filter::Profile_Info& pinfo);

protected:

  /// This is the bulk of the filtering code.
  virtual void filter_and_add (TAO_Profile* profile,
                               TAO_MProfile& profiles,
                               TAO_Profile* guideline = 0);

private:

  /// Fill in the Profile_Info with information from the profile.
  int fill_profile_info (TAO_Profile* profile,
                         TAO_IORManip_IIOP_Filter::Profile_Info& pinfo);

  /// Get the endpoint sequence from the profile.
  int get_endpoints (TAO_Profile* profile,
                     TAO::IIOPEndpointSequence& endpoints);

  /// Allocate a new IIOP Profile based on the profile passed in.
  TAO_IIOP_Profile* create_profile (TAO_Profile* profile);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_IORMANIP_IIOP_FILTER_H */
