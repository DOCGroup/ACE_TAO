// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_ORBInitializer.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef FTEC_ORBINITIALIZER__H_
#define FTEC_ORBINITIALIZER__H_
#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class FTEC_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual ::CORBA::LocalObject
{
public:
  FTEC_ORBInitializer () = default;

  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;

private:
  FTEC_ORBInitializer (FTEC_ORBInitializer const &) = delete;
  FTEC_ORBInitializer &operator= (FTEC_ORBInitializer const &) = delete;
  FTEC_ORBInitializer (FTEC_ORBInitializer &&) = delete;
  FTEC_ORBInitializer &operator= (FTEC_ORBInitializer &&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* FTEC_ORBINITIALIZER__H_ */
