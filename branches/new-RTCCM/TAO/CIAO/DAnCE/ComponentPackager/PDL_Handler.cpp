// $Id$

#include "PDL_Handler.h"
#include "Config_Handlers/XML_Helper.h"
#include "xercesc/dom/DOM.hpp"

namespace CIAO
{
  namespace Component_Packager
  {
  using namespace CIAO::Config_Handlers;
  using xercesc::DOMDocument;

  PDL_Handler::PDL_Handler ()
  {
  }

  PDL_Handler::~PDL_Handler()
  {
  }


  int 
  PDL_Handler::process_pdl (const char *pdl_url,
                   DESC_LIST &desc_list)
  {
    //@@ retrieve location information for standard descriptors
    XML_Helper pdl_helper;

    if (!pdl_helper.is_initialized ())
      return -1;

    xercesc::DOMDocument* pdl_dom =
      pdl_helper.create_dom (pdl_url);

    if (!pdl_dom)
      return -1;

    Package::PackageDescriptorsList list = 
      Package::packageDescriptorsList (pdl_dom);

    this->get_DescriptorsList (list,
                               desc_list);
    return 0;
  }

  void 
  PDL_Handler::get_DescriptorsList (Package::PackageDescriptorsList &list,
                                    DESC_LIST &desc_list)
  {
    for (Package::PackageDescriptorsList::desc_const_iterator iter = list.begin_desc ();
         iter != list.end_desc ();
         iter++)
    {
      ACE_TString value((*iter).c_str ());
      if (value.length () != 0)
        desc_list.push_back (value);
    }
    return;    
  }

  }
}
