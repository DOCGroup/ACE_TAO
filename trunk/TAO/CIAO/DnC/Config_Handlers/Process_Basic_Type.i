//==================================================================
/**
 *  @file Process_Basic_Type.i
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

// -*- C++ -*-
/*
 *  Process string values
 */

#include <iostream>

inline bool
process_string (DOMNodeIterator* iter,
                const XStr& node_name, const char* name,
                TAO_String_Manager& var)
{
  bool result = (node_name == XStr(ACE_TEXT (name)));
  if (result == true)
    {
      DOMNode* node = iter->nextNode();
      const XMLCh* text = ACE_reinterpret_cast(DOMText*, node)->getNodeValue();
      if (text)
        var = XMLString::transcode (text);
    }
  return result;
}

/*
 *  Process string sequences
 */

inline bool
process_string_seq(DOMNodeIterator* iter,
                   const XStr& node_name, const char* name,
                   CORBA::StringSeq& seq)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      DOMNode* node = iter->nextNode ();
      const XMLCh* text = ACE_reinterpret_cast (DOMText*, node)->getNodeValue();

      if (text) {
        CORBA::ULong i (seq.length ());
        seq.length (i + 1);
        seq[i] = XMLString::transcode (text);
      }
    }

  return result;
}

