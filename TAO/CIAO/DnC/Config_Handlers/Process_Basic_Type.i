//==================================================================
/**
 *  @file Process_Basic_Type.i
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

/*
 *  Process string values
 */

using xercesc::DOMNode;
using xercesc::XMLString;
using xercesc::DOMText;

inline bool
process_string (DOMNodeIterator* iter,
                const XStr& node_name, const char* name,
                TAO_String_Manager& var)
{
  bool result = (node_name == XStr(ACE_TEXT (name)));
  if (result == true)
    {
      DOMNode* node = iter->nextNode();
      const XMLCh* text = ACE_reinterpret_cast(DOMText*, node)->getNodeValue ();
      if (text)
        var = XMLString::transcode (text);
    }
  return result;
}

/*
 *  Process string sequences
 */

inline bool
process_string_seq (DOMNodeIterator* iter,
                    const XStr& node_name, const char* name,
                    CORBA::StringSeq& seq)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      DOMNode* node = iter->nextNode ();
      const XMLCh* text = ACE_reinterpret_cast 
                            (DOMText*, node)->getNodeValue ();

      if (text) 
        {
          CORBA::ULong i (seq.length ());
          seq.length (i + 1);
          seq[i] = XMLString::transcode (text);
        }
    }

  return result;
}

inline bool
process_ulong (DOMNodeIterator* iter,
               const XStr& node_name, const char* name,
               CORBA::ULong& var)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      DOMNode* node = iter->nextNode ();
      const XMLCh* text = ACE_reinterpret_cast 
                            (DOMText*, node)->getNodeValue ();

      if (text) 
        {
          CORBA::String_var temp = XMLString::transcode (text);
          var = ACE_static_cast (CORBA::ULong, ACE_OS::strtol (temp.in (), 
                                 0, 10));
        }
    }

  return result;
}

inline bool
process_boolean (DOMNodeIterator* iter,
                 const XStr& node_name, const char* name,
                 CORBA::Boolean& var)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      DOMNode* node = iter->nextNode ();
      const XMLCh* text = ACE_reinterpret_cast 
                            (DOMText*, node)->getNodeValue ();

      if (text) 
        {
          CORBA::String_var temp = XMLString::transcode (text);
          var = ACE_static_cast (CORBA::Boolean, ACE_OS::strtol (temp.in (), 
                                 0, 10));
        }
    }

  return result;
}
