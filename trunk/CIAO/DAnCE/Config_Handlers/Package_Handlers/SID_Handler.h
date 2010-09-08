/**
 * @file SID_Handler.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */


#ifndef CIAO_PACKAGING_SID_HANDLER_H
#define CIAO_PACKAGING_SID_HANDLER_H

#include /**/ "ace/pre.h"

#include "XML_Typedefs.h"
#include "Utils/Functors.h"
#include "IDREF_Base.h"
#include "Package_Handlers/Packaging_Handlers_Export.h"

namespace Deployment
{
  struct SubcomponentInstantiationDescription;
  class SubcomponentInstantiationDescriptions;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class SubcomponentInstantiationDescription;

    namespace Packaging
    {
      /**
       * @class SID_Handler
       * @brief Handler class for SubcomponentInstantiationDescription
       */
      class Packaging_Handlers_Export SID_Handler
      {
        SID_Handler (XML_Helper *xml_helper)
          : xml_helper_ (xml_helper)
        {

        }

      public:
        static void handle_sub_comp_inst_descr (const SubcomponentInstantiationDescription &desc,
                                         ::Deployment::SubcomponentInstantiationDescription &toconfig);

        static SubcomponentInstantiationDescription
        sub_comp_inst_descr (const ::Deployment::SubcomponentInstantiationDescription &src);

        static IDREF_Base< CORBA::ULong > IDREF;

      private:
        static SubcomponentInstantiationDescription *resolve_sid (const char *uri);

        XML_Helper *xml_helper_;

      };

      typedef Sequence_Handler < SubcomponentInstantiationDescription,
                                 ::Deployment::SubcomponentInstantiationDescriptions,
                                 ::Deployment::SubcomponentInstantiationDescription,
    SID_Handler::handle_sub_comp_inst_descr > SID_Functor;

    }
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_PACKAGING_SID_HANDLER_H */
