/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Acceptor_Filter.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@cs.wustl.edu)
 *  @author Carlos O'Ryan (coryan@uci.edu)
 */
//=============================================================================


#ifndef TAO_ACCEPTOR_FILTER_H
#define TAO_ACCEPTOR_FILTER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_MProfile;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Acceptor;

/**
 * @class TAO_Acceptor_Filter
 *
 * @brief Interface for mprofile creation strategies.
 *
 * Concrete Acceptor_Filter encapsulates a strategy for populating
 * mprofile with profiles: it decides which endpoints are included
 * into mprofile and which ones are not.
 */
class TAO_Export TAO_Acceptor_Filter
{
public:
  virtual ~TAO_Acceptor_Filter (void);

  /**
   * Populate <mprofile> with profiles. Profiles are created for
   * selected acceptors (endpoints) from the array delimited by
   * <acceptors_begin> and <acceptors_end>.
   */
  virtual int fill_mprofile (const TAO_ObjectKey &object_key,
                             TAO_MProfile &mprofile,
                             TAO_Acceptor **acceptors_begin,
                             TAO_Acceptor **acceptors_end) = 0;
};

#if defined (__ACE_INLINE__)
# include "Acceptor_Filter.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_ACCEPTOR_FILTER_H */
