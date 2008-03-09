/**
 * @file Configurator_Factory.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Nanbor Wang <>
 *
 * Factory for creating configurators, based on old NodeApplication_Options class
 */

#ifndef CIAO_CONFIGURATOR_FACTORY_H
#define CIAO_CONFIGURATOR_FACTORY_H
#include /**/ "ace/pre.h"

//#include "CIAO_Configurator_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
//#include "ace/DLL.h"

#include "CIAO_ComponentServer_svnt_export.h"

namespace CIAO
{
  namespace Deployment
  {
    class ComponentServer_Configurator;
    
    struct CIAO_ComponentServer_svnt_Export Configurator_Factory
    {
      ComponentServer_Configurator * operator () (int &argc, ACE_TCHAR **argv);
      
    private:
      void parse_args (int &argc, ACE_TCHAR **argv);
      
      bool rt_;
    };
  }
}
#endif
