// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Private_Transport_Descriptor.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PRIVATE_CONNECTION_DESCRIPTOR_H
#define TAO_PRIVATE_CONNECTION_DESCRIPTOR_H
#include "ace/pre.h"

#include "tao/orbconf.h"
#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport_Descriptor_Interface.h"

/**
 * @class TAO_Private_Transport_Descriptor
 *
 * @brief Concrete connection descriptor.
 *
 * Holds info necessary to identify private connections and
 * store/look them up in the Transport Cache.  (For description
 * of private connections see RTCORBA::PrivateTransportPolicy.)
 */
class TAO_RTCORBA_Export TAO_Private_Transport_Descriptor:
  public TAO_Transport_Descriptor_Interface
{
public:

  // = Initialization and termination methods.

  TAO_Private_Transport_Descriptor (TAO_Endpoint *endpoint,
                                    long object_id,
                                    CORBA::Boolean flag = 0);

  virtual ~TAO_Private_Transport_Descriptor (void);


  // = Abstract TAO_Transport_Descriptor methods to be implemented by
  // subclasses.

  virtual  TAO_Transport_Descriptor_Interface *duplicate (void);

  virtual CORBA::Boolean is_equivalent (
      const TAO_Transport_Descriptor_Interface *other_prop);

  virtual u_long hash (void) const;

private:

  // Descriptor should not be copied, except using the <duplicate> method.
  TAO_Private_Transport_Descriptor (const TAO_Private_Transport_Descriptor&);
  void operator= (const TAO_Private_Transport_Descriptor&);

private:

  /**
   * Unique identifier of the object to which private connection
   * identified with this descriptor belongs.  The value of
   * @c object_id_ is the @c TAO_Stub* of the object.
   */
  long object_id_;

};

#if defined (__ACE_INLINE__)
# include "Private_Transport_Descriptor.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_PRIVATE_CONNECTION_DESCRIPTOR_H */
