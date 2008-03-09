/**
 * @file CIAO_Utils.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Some common utility methods for CIAO.
 */

#ifndef CIAO_UTILS_H
#define CIAO_UTILS_H

#include /**/ "ace/pre.h"

#include "ciao/CIAO_Utils_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  typedef ACE_Hash_Map_Manager_Ex<ACE_Cstring,
    CORBA::Any,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> CONFIGVALUE_MAP;
  
  void CIAO_Utils_Export build_config_values_map (CONFIGVALUE_MAP &map, const ::Components::ConfigValues &config);
  
  void CIAO_Utils_Export build_config_values_sequence (::Components::ConfigValues &config, const CONFIGVALUE_MAP &map);
}


#include /**/ "ace/post.h"
#endif
