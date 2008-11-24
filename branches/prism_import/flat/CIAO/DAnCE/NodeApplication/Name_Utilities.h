/** 
 * @file Name_Utilities.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * A set of utility methods to manages references. 
 * 
 */

#ifndef NAME_UTILITIES_H
#define NAME_UTILITIES_H

#include "orbsvcs/CosNamingC.h"

namespace DAnCE
{
  class Name_Utilities
  {
  public:
    /// Write IOR to named file.  Will overwrite file if present. 
    static bool write_ior (const ACE_TCHAR *file, const ACE_TCHAR *ior);
    
    /// Binds object to provided name, which may be formatted with / to indicate
    /// naming contexts, e.g. A/B/C. 
    static bool bind_object (const ACE_TCHAR *name, 
                             CORBA::Object_ptr obj, 
                             CosNaming::NamingContext_ptr);
    
    /// Will unbind the provided name.  
    static bool unbind_object (const ACE_TCHAR *name, 
                               CosNaming::NamingContext_ptr);
    
  private:
    static void bind_context (CosNaming::Name &nm,
                              CosNaming::NamingContext_ptr);
    
    static void build_name (const ACE_TCHAR *nm,
                            CosNaming::Name &);
    
  };
}

#endif /* NAME_UTILITIES_H */
