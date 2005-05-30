/** 
 * $Id$
 * @file IDREF_Base.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * Defines the IDREF_Base class, which is an abstract class that serves
 * as a base for any config handler that must map IDREFS.  
 */

#ifndef CIAO_CONFIG_HANDLERS_IDREF_BASE_H
#define CIAO_CONFIG_HANDLERS_IDREF_BASE_H

#include /**/ "ace/pre.h"
#include "Config_Handlers_Export.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/String_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Config_Handlers
  {
    
    /**
     * @class IDREF_Handler
     * @brief Base class for any handler that must process idrefs.
     *
     */
    class Config_Handlers_Export IDREF_Base
    {
    public:
      /// Bind an IDREF to an index
      bool
      bind_ref (ACE_CString& id, size_t index);
  
      /// Get the index associated with an IDREF
      bool
      find_ref (const ACE_CString& id, size_t& val);
      
      /// get the IDREF associated with an index
      bool
      find_ref (const size_t id, ACE_CString& val);
  
      /// Unbind all IDREFS
      bool
      unbind_refs (void);

      typedef ACE_Hash_Map_Manager<ACE_CString,
                                   size_t,
                                   ACE_Null_Mutex> IDREF_MAP;

          
      typedef ACE_Hash_Map_Manager<size_t,
                                   ACE_CString,
                                   ACE_Null_Mutex> POS_MAP;
    protected:
      /// The map used to store and look up the indexes of elements
      /// referenced by their IDREF.
      IDREF_MAP idref_map_;
      /// The map used to store and look up the IDREFS of elements
      /// referenced by their index.
      POS_MAP pos_map_;
    };
    
  }
  
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_IDREF_BASE_H */
