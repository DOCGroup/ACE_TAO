//==================================================================
/**
 *  @file  Basic_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay <emre.turkay@vanderbilt.edu>
 */
//=====================================================================

#ifndef BASIC_HANDLER_H
#define BASIC_HANDLER_H

#include "Deployment.h"

#include "ace/SString.h"
#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager.h"
#include "Config_Handler_export.h"

#include <xercesc/dom/DOM.hpp>

using xercesc::DOMDocument;
using xercesc::DOMNodeIterator;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNode;

BEGIN_DEPLOYMENT_NAMESPACE

/**
 * @class Basic_Handler
 *
 * @brief The base class for all the Handler classes
 *
 * This class provides the functionalities common to all handler classes.
 * Keeps variables for basic XML document handling,
 *       initialization/uninitialization,
 *       and required get/set methods.
 */

class Config_Handler_Export Basic_Handler {
public:
  typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
  typedef ACE_Hash_Map_Iterator<ACE_TString, int, ACE_Null_Mutex> REF_ITER;
  typedef ACE_Hash_Map_Manager<int, ACE_TString, ACE_Null_Mutex> IDREF_MAP;

  Basic_Handler (DOMDocument* doc, unsigned long filter_);

  Basic_Handler (DOMNodeIterator* iter, bool release = false);

  Basic_Handler (DOMDocument* doc, DOMNodeIterator* iter, 
                 bool release = false);

  ~Basic_Handler (void);

  REF_MAP& id_map()
  {
    return this->id_map_;
  }

  void set_iter (DOMNodeIterator* iter)
    {
      iter_ = iter;
    }

  void set_doc (DOMDocument* doc)
    {
      doc_ = doc;
    }

protected:
  DOMDocumentTraversal* traverse_;

  DOMDocument* doc_;

  DOMNode* root_;

  unsigned long filter_;

  DOMNodeIterator* iter_;

  bool release_;

  int index_;

  REF_MAP id_map_;

  IDREF_MAP idref_map_;
};

END_DEPLOYMENT_NAMESPACE

#endif // BASIC_HANDLER_H
