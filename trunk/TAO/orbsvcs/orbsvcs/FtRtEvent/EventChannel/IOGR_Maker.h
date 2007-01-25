// -*- C++ -*-

//=============================================================================
/**
 *  @file   IOGR_Maker.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef IOGR_MAKER_H
#define IOGR_MAKER_H

#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/IORManipulation/IORManipulation.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class IOGR_Maker
{
public:
  IOGR_Maker();

  void init(CORBA::ORB_ptr orb);

  /// instance() will only return a valid object after an
  /// instance is explicitly created by client and init() is called.
  static IOGR_Maker* instance();

   /// Create a new object reference by merging the profiles lists in the
   /// supplied list of one or more object references.
  CORBA::Object_ptr merge_iors(const TAO_IOP::TAO_IOR_Manipulation::IORList&);

   /// Create a new IOGR (with FT_PRIMARY and FT_GROUP components) by merging
   /// the profiles lists in the supplied list of one or more object references.
  CORBA::Object_ptr make_iogr(const TAO_IOP::TAO_IOR_Manipulation::IORList&,
                              CORBA::ULong object_group_ref_version);

  /// Make an new IOGR with obj as primary.
  CORBA::Object_ptr forge_iogr(CORBA::Object_ptr obj);

  CORBA::Object_ptr ior_replace_key(CORBA::Object_ptr obj,
                                    const TAO::ObjectKey& key);

  bool copy_ft_group_component(CORBA::Object_ptr obj);

  void set_ft_domain_id(const char*);
  void set_group_id(CORBA::ULongLong);
  void set_ref_version(CORBA::ULong);
  CORBA::ULong increment_ref_version();
  CORBA::ULong get_ref_version() const;

private:
  void set_tag_components(CORBA::Object_ptr merged,
                          CORBA::Object_ptr primary,
                          FT::TagFTGroupTaggedComponent& ft_tag_component);
  TAO_IOP::TAO_IOR_Manipulation_var iorm_;
  FT::TagFTGroupTaggedComponent ft_tag_component_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
