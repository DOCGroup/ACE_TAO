// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/XML_Loader.h"
#include "orbsvcs/Notify/Topology_Object.h"

#include "ACEXML/common/DefaultHandler.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"

#include "ace/Containers_T.h"
#include "tao/debug.h"
#include "ace/OS_NS_unistd.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

using namespace TAO_Notify;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify {
  extern const char TOPOLOGY_ID_NAME[];
}

TAO_END_VERSIONED_NAMESPACE_DECL

namespace {
  CORBA::Long makeNVPList (NVPList& nvp, ACEXML_Attributes* attrs)
  {
    CORBA::Long id = 0;
    for (size_t i = 0; i < attrs->getLength (); ++i)
    {
      ACE_CString name = ACE_TEXT_ALWAYS_CHAR(attrs->getQName (i));
      ACE_CString value = ACE_TEXT_ALWAYS_CHAR(attrs->getValue (i));
      if (ACE_OS::strcmp (name.c_str (),
                          TAO_VERSIONED_NAMESPACE_NAME::TAO_Notify::TOPOLOGY_ID_NAME) == 0)
      {
        id = ACE_OS::atoi (value.c_str ());
      }
      nvp.push_back (NVP (name.c_str (), value.c_str ()));
    }
    return id;
  }
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  XML_Loader::XML_Loader ()
    : live_ (false)
  {
  }

  XML_Loader::~XML_Loader ()
  {
  }

  bool
  XML_Loader::open (const ACE_TString & base_name)
  {
    bool result = false;

    // if *.xml exists, use it
    // if it does not exist then
    // use the previous one was renamed to *.000
    // If neither *.xml nor *.000 exist then something
    // "impossible" happened (or its a new system with no saved state).

    this->file_name_ = base_name;
    this->file_name_ += ACE_TEXT(".xml");

    // 4 is "read permission"
    result =  (0 == ACE_OS::access (this->file_name_.c_str (), 4));
    if (result)
    {
      this->live_ = false;
      auto_ptr<ACEXML_FileCharStream> fstm (new ACEXML_FileCharStream);
      // xml input source will take ownership

      if (fstm->open (this->file_name_.c_str ()) == 0)
      {
        // InputSource takes ownership
        ACEXML_InputSource input (fstm.get ());
        (void) fstm.release ();

        ACEXML_Parser parser;
        parser.setContentHandler (this);
        parser.setDTDHandler (this);
        parser.setErrorHandler (this);
        parser.setEntityResolver (this);

        try
        {
          parser.parse (&input);
        }
        catch (const ACEXML_Exception& ex)
        {
          // The only way to find out what it is, it to let it print itself, so...
          ORBSVCS_ERROR ((LM_ERROR, "Unable to load \"%s\".\n Will try backup file.\n", this->file_name_.c_str ()));
          ex.print ();
          result = false;
        }
      }
      else
      {
        ORBSVCS_DEBUG((LM_DEBUG, ACE_TEXT("Unable to open the XML input file: %s.\n Will try backup file.\n"), file_name_.c_str()));
        result = false;
      }
    }

    if (! result)
    {
      this->file_name_ = base_name;
      this->file_name_ += ACE_TEXT(".000");
      result = (0 == ACE_OS::access (this->file_name_.c_str (), 4));
    }
    return result;
  }

  //virtual
  void
  XML_Loader::load (Topology_Object *root)
  {
    ACE_ASSERT (root != 0);
    this->live_ = true;

    auto_ptr<ACEXML_FileCharStream> fstm (new ACEXML_FileCharStream);
    // xml input source will take ownership

    if (fstm->open (this->file_name_.c_str ()) == 0)
    {
      // InputSource takes ownership
      ACEXML_InputSource input (fstm.get ());
      (void) fstm.release ();

      ACEXML_Parser parser;
      parser.setContentHandler (this);
      parser.setDTDHandler (this);
      parser.setErrorHandler (this);
      parser.setEntityResolver (this);

      try
      {
        object_stack_.push (root);
        parser.parse (&input);
        ACE_ASSERT (object_stack_.size () == 1);
        Topology_Object* cur = 0;
        object_stack_.pop (cur);
      }
      catch (const ACEXML_Exception& ex)
      {
        // The only way to find out what it is, it to let it print itself, so...
        ORBSVCS_ERROR ((LM_ERROR, "Unable to load \"%s\".\n", this->file_name_.c_str ()));
        ex.print ();
        throw CORBA::INTERNAL();
      }
    }
    else
    {
      ORBSVCS_DEBUG((LM_DEBUG, ACE_TEXT("Unable to open the XML input file: %s.\n"), file_name_.c_str()));
      throw CORBA::INTERNAL();
    }
  }

  void
  XML_Loader::startElement (const ACEXML_Char*,
    const ACEXML_Char*,
    const ACEXML_Char* name,
    ACEXML_Attributes* xml_attrs)
  {
    ACE_ASSERT (name != 0);
    ACE_ASSERT (xml_attrs != 0);
    if (this->live_)
    {
      ACE_ASSERT (object_stack_.size () > 0);
      Topology_Object* cur = 0;
      if (object_stack_.top (cur) == 0)
      {
        try
        {
          NVPList attrs;
          CORBA::Long id = makeNVPList (attrs, xml_attrs);

          if (DEBUG_LEVEL > 5) ORBSVCS_DEBUG ((LM_INFO,
            ACE_TEXT("(%P|%t) XML_Loader: Element %s\n"),
            name
            ));

          ACE_CString cname (ACE_TEXT_ALWAYS_CHAR(name));
          Topology_Object* next = cur->load_child (cname, id, attrs);
          ACE_ASSERT(next != 0);
          object_stack_.push (next);
        }
        catch (const CORBA::Exception& ex)
        {
          throw ACEXML_SAXException (ACE_TEXT_CHAR_TO_TCHAR(ex._info ().c_str ()));
        }
      }
    }
  }

  void
  XML_Loader::endElement (const ACEXML_Char*,
    const ACEXML_Char*,
    const ACEXML_Char* name)
  {
    ACE_UNUSED_ARG (name);
    if (this->live_)
    {
      ACE_ASSERT (object_stack_.size () > 0);
      if (DEBUG_LEVEL > 5)
        {
          ORBSVCS_DEBUG ((LM_INFO,
                      ACE_TEXT("(%P|%t) XML_Loader: End Element %s\n"),
                      name));
        }
      Topology_Object* cur = 0;
      object_stack_.pop (cur);
    }
  }

} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL
