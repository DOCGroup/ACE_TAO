//==============================================================
/**
 *  @file  Base_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_BASE_HANDLER_H
#define CIAO_CONFIG_HANDLERS_BASE_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_export.h"
#include "Basic_Deployment_Data.hpp"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{

  namespace Config_Handlers
  {  
   
   /*
    * @class Base_Handler
    *
    * @brief base class for Type handlers.
    *
    * 
    *
    */
    
    class Config_Handlers_Export Base_Handler{
     
      public:
      
        typedef ACE_Hash_Map_Manager<ACE_TString, size_t, ACE_Null_Mutex> IDREF_MAP;
       
        Base_Handler (void);
        virtual ~Base_Handler (void);
        
        /// The Deployment spec references elements by 
        /// their position within their parent sequence.
        /// These two methods allow an element's index
        /// to be stored/retrieved.
        
        /// Map the index <index> of an element to its IDREF <id>.
        void bind_ref (ACE_TString& id, size_t index);
        
        /// Retrieve the index of an element with its IDREF <id>.
        /// Returns 0 if the <id> was found.
        int get_ref (ACE_TString& id, size_t val);         

      private:
      
        /// The map used to store and look up the indexes
        /// of elements referenced by their index.
        IDREF_MAP idref_map_;  

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_BASE_HANDLER_H*/
