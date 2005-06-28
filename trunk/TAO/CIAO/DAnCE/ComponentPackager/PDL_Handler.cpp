// $Id$

#include "PDL_Handler.h"
#include "XercesString.h"

namespace CIAO
{
  namespace Component_Packager
  {
  PDL_Handler::PDL_Handler (DOMDocument* pdl_doc,
                            unsigned long filter)

    :traverse_ (pdl_doc),
     pdl_doc_ (pdl_doc),
     root_ (this->pdl_doc_->getDocumentElement()),
     filter_ (filter),
     iter_ (traverse_->createNodeIterator (this->root_,
                                           this->filter_,
                                           0,
                                           true))
  {}

  void
  PDL_Handler::process_pdl (DESC_LIST &desc_list)
  {
    for (DOMNode* node = this->iter_->nextNode();
        node != 0;
        node = this->iter_->nextNode())
    {

      XStr node_name (node->getNodeName());
      if (node_name == XStr (ACE_TEXT ("Deployment:PackageDescriptorsList")))
      {
      }
      else if (node_name == XStr (ACE_TEXT ("desc")))
      {
        ACE_TString value;
        this->get_node_value (this->iter_,
                              value);
        desc_list.push_back (value);
      }

    }
    return;
  }

  void
  PDL_Handler::get_node_value (DOMNodeIterator *iter,
                               ACE_TString &value)
  {
    DOMNode* node = iter->nextNode();
    const XMLCh* text = reinterpret_cast<DOMText*> (node)->getNodeValue ();
    if (text)
      value = XMLString::transcode (text);
    return;
  }

  }
}
