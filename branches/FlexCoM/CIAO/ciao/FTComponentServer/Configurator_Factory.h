// $Id$
/**
 * @file Configurator_Factory.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Nanbor Wang <>
 *
 * Factory for creating configurators, based on old NodeApplication_Options
 * class
 */

#ifndef CIAO_CONFIGURATOR_FACTORY_H
#define CIAO_CONFIGURATOR_FACTORY_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_FTComponentServer_svnt_export.h"

namespace CIAO
{
  namespace Deployment
  {
    class ComponentServer_Configurator;

    class CIAO_FTComponentServer_svnt_Export Configurator_Factory
    {
    public:
      ComponentServer_Configurator * operator () (int &argc, ACE_TCHAR **argv);

    private:
      bool parse_args (int &argc, ACE_TCHAR **argv);
    };
  }
}
#include /**/ "ace/post.h"

#endif
