// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Valuetype_Adapter.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_VALUETYPE_ADAPTER_H
#define TAO_VALUETYPE_ADAPTER_H
#include "ace/pre.h"

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"

/**
 * @class TAO_Valuetype_Adapter
 *
 * @brief TAO_Valuetype_Adapter.
 *
 * Class that adapts various functions involving the Valuetype,
 * ValueFactory and AbstractInterface classes. This is a base class for
 * the actual implementation in the TAO_Valuetype library.
 */
class TAO_Export TAO_Valuetype_Adapter : public ACE_Service_Object
{
public:
  virtual ~TAO_Valuetype_Adapter (void);

  virtual CORBA::Object_ptr abstractbase_to_object (
      CORBA::AbstractBase_ptr p
    ) = 0;

  virtual CORBA::ULong type_info_single (void) const = 0;

  virtual int vf_map_rebind (const char *,
                             CORBA::ValueFactory &) = 0;

  virtual int vf_map_unbind (const char *) = 0;


  virtual CORBA::ValueFactory vf_map_find (const char *) = 0;
};

#include "ace/post.h"
#endif /* TAO_VALUETYPE_ADAPTER_H */
