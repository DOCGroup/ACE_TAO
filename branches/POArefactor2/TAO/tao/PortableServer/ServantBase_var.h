// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ServantBase_var.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SERVANTBASE_VAR_H
#define TAO_SERVANTBASE_VAR_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ServantBase;

/**
 * @class TAO_ServantBase_var
 *
 * @brief Auto pointer for reference counting servants.
 *
 * For the convenience of automatically managing servant
 * reference counts, the PortableServer namespace also provides
 * the ServantBase_var class. This class behaves similarly to
 * _var classes for object references (see Section 20.3.1).
 */
class TAO_PortableServer_Export TAO_ServantBase_var
{
public:
  TAO_ServantBase_var (void);

  TAO_ServantBase_var (TAO_ServantBase *p);

  TAO_ServantBase_var (const TAO_ServantBase_var &b);

  ~TAO_ServantBase_var (void);

  TAO_ServantBase_var &operator= (TAO_ServantBase *p);

  TAO_ServantBase_var &operator= (const TAO_ServantBase_var &b);

  TAO_ServantBase *operator-> () const;

  TAO_ServantBase *in (void) const;

  TAO_ServantBase *&inout (void);

  TAO_ServantBase *&out (void);

  TAO_ServantBase *_retn (void);

private:

  TAO_ServantBase *ptr_;
};

#include /**/ "ace/post.h"

#endif /* TAO_SERVANTBASE_VAR_H */
