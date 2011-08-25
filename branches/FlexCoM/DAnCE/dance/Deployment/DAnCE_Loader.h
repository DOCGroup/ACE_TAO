// $Id$

/** 
 * @file DAnCE_Loader.h
 * Abstract class for DAnCE object loaders. 
 */

#ifndef DANCE_LOADDER_H_
#define DANCE_LOADDER_H_

namespace DAnCE
{
  class DAnCE_Object_Loader : public TAO_Object_Loader
  {
  public:
    /// Deliver a usage statement about the object.
    virtual const char * usage (void) = 0;
    
    /// Parse args passed to it; is not destructive
    virtual bool parse_args (int argc, ACE_TCHAR *argv []) = 0;
  };
    

}

#endif
