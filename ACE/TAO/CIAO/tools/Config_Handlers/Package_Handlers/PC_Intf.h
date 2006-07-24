//==============================================================
/**
 *  @file  PC_Intf.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_STD_PC_INTF_H
#define CIAO_CONFIG_STD_PC_INTF_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#include "ace/Auto_Ptr.h"


namespace Deployment
{
  struct PackageConfiguration ;
}

namespace CIAO
{
  namespace Config_Handlers
  {
   /*
    * @class PC_Intf
    *
    * @brief Wrapper class for PackageConfiguration extraction
    *
    * This class defines wrapper functions for extracting
    * PackageConfiguration IDL structures from XSC structures.
    * It serves as the public interface used by application
    * developers.
    *
    */
    class Config_Handlers_Export STD_PC_Intf
    {
    public:
      STD_PC_Intf (const char *file);

      ::Deployment::PackageConfiguration const *get_PC (void) const;
      ::Deployment::PackageConfiguration *get_PC (void);

    protected:
      bool prepare_PC (const char *file);

    private:
      ACE_Auto_Ptr< ::Deployment::PackageConfiguration> idl_pc_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /*CIAO_CONFIG_STD_PC_INTF_H*/
