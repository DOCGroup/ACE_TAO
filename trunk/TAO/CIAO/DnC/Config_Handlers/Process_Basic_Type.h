//==================================================================
/**
 *  @file Process_Basic_Type.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef PROCESS_BASIC_TYPE_H
#define PROCESS_BASIC_TYPE_H

#include <xercesc/dom/DOM.hpp>
#include "XercesString.h"
#include "../DeploymentC.h"

using xercesc::DOMNodeIterator;
using Config_Handler::XStr;

inline bool
process_string (DOMNodeIterator* iter,
                const XStr& node_name, const char* name,
                TAO_String_Manager& var);

inline bool
process_string_seq(DOMNodeIterator* iter,
                   const XStr& node_name, const char* name,
                   CORBA::StringSeq& seq);

inline bool
process_boolean(DOMNodeIterator* iter,
                const XStr& node_name, const char* name,
                CORBA::Boolean& var);

#include "Process_Basic_Type.i"

#endif // PROCESS_BASIC_TYPE_H
