// $Id$
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "Package_Handlers/CPD_Handler.h"
#include "Package_Handlers/CID_Handler.h"
#include "Package_Handlers/Comp_Intf_Descr_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_Packaging_DataC.h"
#include "Deployment.hpp"
#include "Utils/Exceptions.h"
#include "Property_Handler.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Packaging
    {
      struct Packaging_Handlers_Export PCI_Handler
      {
        static void handle_pci (
            const PackagedComponentImplementation &desc,
            ::Deployment::PackagedComponentImplementation &toconfig)
        {
          CIAO_TRACE ("PCI_Handler::get_pci");
          toconfig.name = desc.name ().c_str ();

          CID_Handler::component_impl_descr (desc.referencedImplementation (),
                                             toconfig.referencedImplementation);
        }

        static PackagedComponentImplementation
        get_pci (const ::Deployment::PackagedComponentImplementation &src)
        {
          CIAO_TRACE ("PCI_Handler::get_pci - reverse");
          return PackagedComponentImplementation (
                    src.name.in (),
                    CID_Handler::component_impl_descr (src.referencedImplementation));
        }
      };

      typedef Sequence_Handler <
        PackagedComponentImplementation,
        ::Deployment::PackagedComponentImplementations,
        ::Deployment::PackagedComponentImplementation,
        PCI_Handler::handle_pci > PCI_Functor;

      void
      CPD_Handler::handle_component_package_descr (
          const ComponentPackageDescription &desc,
          ::Deployment::ComponentPackageDescription &toconfig)
      {
        CIAO_TRACE ("CPD_Handler::component_package_descr");

        auto_ptr < ComponentPackageDescription > xsc_cpd;
        const ComponentPackageDescription *cpd = 0;

        if (desc.href_p ())
          {
            xsc_cpd.reset (CPD_Handler::resolve_cpd (desc.href ().c_str ()));
            cpd = xsc_cpd.get ();
          }
        else
          cpd = &desc;

        if (cpd->label_p ())
          toconfig.label = cpd->label ().c_str ();

        if (cpd->UUID_p ())
          toconfig.UUID = cpd->UUID ().c_str ();

        // CID
        if (cpd->realizes_p ())
          Comp_Intf_Descr_Handler::comp_intf_descr (cpd->realizes (),
                                                    toconfig.realizes);

        // Config Properties
        toconfig.configProperty.length (desc.count_configProperty ());
        std::for_each (cpd->begin_infoProperty (),
                       cpd->end_infoProperty (),
                       Property_Functor (toconfig.configProperty));

        //        ACE_ERROR ((LM_ERROR, "***** Count of PCIs is %i\n",
        //           cpd->count_implementation ()));

        // Packaged Component Implementations
        toconfig.implementation.length ( cpd->count_implementation ());
        SEQ_HAND_GCC_BUG_WORKAROUND (PCI_Handler::handle_pci,
                                     cpd->begin_implementation (),
                                     toconfig.implementation);
        std::for_each (cpd->begin_implementation (),
                       cpd->end_implementation (),
                       PCI_Functor (toconfig.implementation));

        // Info Properties
        toconfig.infoProperty.length (cpd->count_infoProperty ());
        std::for_each (cpd->begin_infoProperty (),
                       cpd->end_infoProperty (),
                       Property_Functor (toconfig.infoProperty));
      }

      ComponentPackageDescription
      CPD_Handler::component_package_descr (const Deployment::ComponentPackageDescription& src)
      {
        CIAO_TRACE ("CPD_Handler::component_package_descr - reverse");
        ComponentPackageDescription toconfig;

        if (src.label.in () != 0)
          toconfig.label (src.label.in ());

        if (src.UUID.in () != 0)
          toconfig.UUID (src.UUID.in ());

        {
          toconfig.realizes
            (Comp_Intf_Descr_Handler::comp_intf_descr (src.realizes));
        }

        for (size_t i = 0; i < src.configProperty.length (); ++i)
          {
            toconfig.add_configProperty (
              Property_Handler::get_property (src.configProperty[i]));
          }

        { // Packaged Component Implementations
          for (size_t i = 0; i < src.implementation.length (); ++i)
            toconfig.add_implementation (
              PCI_Handler::get_pci (src.implementation[i]));
        }

        for (size_t i = 0; i < src.infoProperty.length (); ++i)
          {
            toconfig.add_infoProperty (
              Property_Handler::get_property (src.infoProperty[i]));
          }

        return toconfig;
      }

      ComponentPackageDescription * CPD_Handler::resolve_cpd (const char *uri)
      {
        CIAO_TRACE ("CPD_Handler::resolve_cpd");
        if (!XML_HELPER->is_initialized ())
          return 0;

        xercesc::DOMDocument* dom =
          XML_HELPER->create_dom (uri);

        if (!dom)
          throw Parse_Error ("Unable to create DOM for component package description");

        try
        {
          //ACE_ERROR ((LM_ERROR, "Creating new CPD XSC Object\n"));
          return new ComponentPackageDescription (componentPackageDescription (dom));
        }
        catch (...)
        {
          throw Parse_Error ("Unable to create XSC structure for CID");
        }
      }
    }
  }
}
