// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Private_Connection_Descriptor.h
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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/Connection_Descriptor_Interface.h"

class TAO_Export TAO_Private_Connection_Descriptor:
  public TAO_Connection_Descriptor_Interface
{
  // = TITLE
  //   Concrete connection descriptor.
  //
  // = DESCRIPTION
  //
  //   Holds info necessary to identify private connections and
  //   store/look them up in the Connection Cache.  (For description
  //   of private connections see RTCORBA::PrivateConnectionPolicy.)
  //
public:

  // = Initialization and termination methods.

  TAO_Private_Connection_Descriptor (TAO_Endpoint *endpoint,
                                     long object_id,
                                     CORBA::Boolean flag = 0);

  virtual ~TAO_Private_Connection_Descriptor (void);


  // = Abstract TAO_Connection_Descriptor methods to be implemented by
  // subclasses.

  virtual  TAO_Connection_Descriptor_Interface *duplicate (void);

  virtual CORBA::Boolean is_equivalent (
      const TAO_Connection_Descriptor_Interface *other_prop);

  virtual u_long hash (void) const;

private:

  // Descriptor should not be copied, except using the <duplicate> method.
  ACE_UNIMPLEMENTED_FUNC (TAO_Private_Connection_Descriptor (const TAO_Private_Connection_Descriptor&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Private_Connection_Descriptor&))

  /**
   * Unique identifier of the object to which private connection
   * identified with this descriptor belongs.  The value of
   * <object_id_> is the TAO_Stub* of the object.
   */
  long object_id_;
};

#if defined (__ACE_INLINE__)
# include "tao/Private_Connection_Descriptor.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif  /* TAO_PRIVATE_CONNECTION_DESCRIPTOR_H */
