// $Id$

/**
 * @file Active_Objref_Map.h
 *
 * A CIAO implementation of object reference map which enables
 * fast insertion/deletion/indexing of object reference thru
 * map-generated keys.
 *
 * @sa ACE_Active_Map_Manager
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_ACTIVE_OBJREF_MAP_H
#define CIAO_ACTIVE_OBJREF_MAP_H
#include /**/ "ace/pre.h"

#include "ace/Active_Map_manager_T.h"
#include "ciao/CIAO_Container_Export.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  typedef ACE_Active_Map_Manager<CORBA::Object_var> Active_Objref_Map;

  CIAO_CONTAINER_SINGLETON_DECLARATION (ACE_Active_Map_Manager<CORBA::Object_var>);
}

#include /**/ "ace/post.h"
#endif /* CIAO_ACTIVE_OBJREF_MAP_H */
