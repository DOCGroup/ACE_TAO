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

/*
 *  Processes string values
 *  Returns true if the node is the string node with name 'name'
 *
 *  handler: Pointer to the Handler object
 *  name: The name of the node
 *  var: If returned successful keeps the value of the node
 */
inline bool
process_string (DOMNodeIterator* iter,
                const XStr& node_name, const char* name,
                TAO_String_Manager& var);

/*
 *  Processes string sequences
 *  Returns true if the node is the string node with name 'node_name'
 *
 *  node: The possible string node
 *  iter: Iterator traversing the document
 *  node_name: The name of the node
 *  name: The name to compare
 *  seq: If returned successful keeps the value of the sequence
 */
inline bool
process_string_seq(DOMNodeIterator* iter,
                   const XStr& node_name, const char* name,
                   CORBA::StringSeq& seq);
#include "Process_Basic_Type.i"

#endif // PROCESS_BASIC_TYPE_H
