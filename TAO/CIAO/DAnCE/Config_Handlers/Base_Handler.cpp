// $Id$

#include "Base_Handler.h"


namespace CIAO
{
  namespace Config_Handlers
  {

    Base_Handler::Base_Handler (void)
    {
    }

    Base_Handler::~Base_Handler (void)
    {
    }
    
    /// The Deployment spec references elements by 
    /// their position within their parent sequence.
    /// These two methods allow an element's index
    /// to be stored/retrieved.
    void 
    Base_Handler::bind_ref (ACE_TString& id, size_t index)
    {
      idref_map_.bind (id,index); 
    }
    
    int 
    Base_Handler::get_ref (ACE_TString& id, size_t val)
    {
      return idref_map_.find (id,val);
    }

  }
}
