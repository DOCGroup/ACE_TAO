// $Id$

#include "Shared_Backing_Store.h"
#include "Server_Info.h"
#include "Activator_Info.h"
#include "utils.h"
#include "Locator_XMLHandler.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/XML_Util.h"

Shared_Backing_Store::Shared_Backing_Store(const ACE_CString& filename, bool start_clean)
: XML_Backing_Store(filename, false),
  listing_file_(filename + "imr_listing.xml")
{
  if (start_clean)
    {
      ACE_Vector<ACE_CString> filenames;
      get_listings(filenames);
      CORBA::ULong sz = filenames.size ();
      for (CORBA::ULong i = 0; i < sz; ++i)
        {
          if (this->debug_ > 9)
            {
              ACE_DEBUG((LM_INFO, "Removing %s\n", filenames[i].c_str()));
            }
          ACE_OS::unlink ( filenames[i].c_str () );
        }

      if (this->debug_ > 9)
        {
          ACE_DEBUG((LM_INFO, "Removing %s\n", this->listing_file_.c_str()));
        }
      ACE_OS::unlink ( this->listing_file_.c_str () );
    }
}

Shared_Backing_Store::~Shared_Backing_Store()
{
}

int
Shared_Backing_Store::persistent_remove (const ACE_CString& name, bool activator)
{
  const ACE_CString fname = make_filename(name, activator);
  ACE_OS::unlink ( fname.c_str () );
  return 0;
}

ACE_CString
Shared_Backing_Store::make_filename(const ACE_CString& name, const bool activator)
{
  const char* const prefix = (activator ? "a_" : "s_");
  return this->filename_ + prefix + name + ".xml";
}

int
Shared_Backing_Store::persistent_update(const Server_Info_Ptr& info)
{
  ACE_CString name = ACEXML_escape_string (info->name);

  const ACE_CString fname = make_filename(name, false);
  if (this->debug_ > 9)
    {
      ACE_DEBUG((LM_INFO, "Persisting to %s(%s)\n", fname.c_str(), info->name.c_str()));
    }
  FILE* fp = ACE_OS::fopen (fname.c_str (), "w");
  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR, "Couldn't write to file %C\n", fname.c_str()));
      return -1;
    }
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  persist(fp, *info, "");
  ACE_OS::fclose (fp);
  return 0;
}


int
Shared_Backing_Store::persistent_update(const Activator_Info_Ptr& info)
{
  ACE_CString name = ACEXML_escape_string (info->name);

  const ACE_CString fname = make_filename(name, true);
  if (this->debug_ > 9)
    {
      ACE_DEBUG((LM_INFO, "Persisting to %s(%s)\n", fname.c_str(), info->name.c_str()));
    }
  FILE* fp = ACE_OS::fopen (fname.c_str (), "w");
  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR, "Couldn't write to file %C\n", fname.c_str()));
      return -1;
    }
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  persist(fp, *info, "");
  ACE_OS::fclose (fp);
  return 0;
}

int
Shared_Backing_Store::persistent_load ()
{
  ACE_Vector<ACE_CString> filenames;
  get_listings(filenames);

  CORBA::ULong sz = filenames.size ();
  if (this->debug_ > 9)
    {
      ACE_DEBUG((LM_INFO, "persistent_load %d files\n", sz));
    }
  for (CORBA::ULong i = 0; i < sz; ++i)
    {
      ACEXML_FileCharStream* fstm = new ACEXML_FileCharStream; // xml input source will take ownership

      int err = fstm->open (filenames[i].c_str());
      if (err != 0)
        {
          delete fstm;
          return err;
        }

      if (this->debug_ > 9)
        {
          ACE_DEBUG((LM_INFO, "persistent_load %s\n", filenames[i].c_str()));
        }
      err = load(fstm, filenames[i]);
      if (err != 0)
        {
          return err;
        }
    }
  return 0;
}

struct LocatorListings_XMLHandler : public ACEXML_DefaultHandler
{
  LocatorListings_XMLHandler(const ACE_CString& dir, ACE_Vector<ACE_CString>& filenames)
    : dir_(dir), filenames_(filenames) {}

  virtual void startElement (const ACEXML_Char* namespaceURI,
                             const ACEXML_Char* localName,
                             const ACEXML_Char* qName,
                             ACEXML_Attributes* attrs)
    {
      const bool server = (ACE_OS::strcasecmp (qName, Locator_XMLHandler::SERVER_INFO_TAG) == 0);
      if (!server && (ACE_OS::strcasecmp (qName, Locator_XMLHandler::ACTIVATOR_INFO_TAG) != 0))
        {
          return;
        }

      if (attrs != 0 && attrs->getLength () == 1)
        {
          ACE_CString aname = ACE_TEXT_ALWAYS_CHAR(attrs->getValue ((size_t)0));
          filenames_.push_back(dir_ + aname);
        }
      else
        ACE_DEBUG((LM_INFO, "startElement ERROR too few attrs\n"));

    }

  virtual void endElement (const ACEXML_Char* ,
                           const ACEXML_Char* ,
                           const ACEXML_Char* )
    {
    }

  const ACE_CString& dir_;
  ACE_Vector<ACE_CString>& filenames_;
};

int
Shared_Backing_Store::get_listings(ACE_Vector<ACE_CString>& filenames) const
{
  LocatorListings_XMLHandler listings_handler(this->filename_, filenames);
  ACEXML_FileCharStream* fstm = new ACEXML_FileCharStream; // xml input source will take ownership

  if (fstm->open (this->listing_file_.c_str()) != 0)
    {
      // This is not a real error. The xml file may not exist yet.
      delete fstm;
      return 0;
    }

  ACEXML_Parser parser;

  // InputSource takes ownership
  ACEXML_InputSource input (fstm);

  parser.setContentHandler (&listings_handler);
  parser.setDTDHandler (&listings_handler);
  parser.setErrorHandler (&listings_handler);
  parser.setEntityResolver (&listings_handler);

  try
    {
      parser.parse (&input);
    }
  catch (const ACEXML_Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Error during read of ImR persistence xml listings file at %s.", this->listing_file_.c_str()));
      ex.print ();
      return -1;
    }

  return 0;
}

int
Shared_Backing_Store::sync_load (const ACE_CString& name, SyncOp sync_op, bool activator)
{
  if (this->debug_ > 9)
    {
      ACE_DEBUG((LM_INFO, "sync_load name=%s\n", name.c_str()));
    }
  FILE* list = ACE_OS::fopen (this->listing_file_.c_str (), "w");
  if (list == 0)
    {
      ACE_ERROR ((LM_ERROR, "Couldn't write to file %C\n", this->listing_file_.c_str()));
      return -1;
    }
  ACE_OS::fprintf (list,"<?xml version=\"1.0\"?>\n");
  ACE_OS::fprintf (list,"<ImRListing>\n");

  // Save servers
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::ITERATOR siit (this->servers ());
  for (; siit.next (sientry); siit.advance() )
    {
      Server_Info_Ptr& info = sientry->int_id_;
      if (sync_op == SYNC_REMOVE && !activator && name == info->name)
        {
          // skip the removed server
          continue;
        }
      ACE_CString listing_name = ACEXML_escape_string (info->name);

      const ACE_CString fname = make_filename(listing_name, false);
      ACE_OS::fprintf (list,"\t<%s", Locator_XMLHandler::SERVER_INFO_TAG);
      ACE_OS::fprintf (list," fname=\"%s\"/>\n", fname.c_str ());
    }

  if (sync_op == SYNC_ADD && !activator)
    {
      const ACE_CString fname = make_filename(name, false);
      ACE_OS::fprintf (list,"\t<%s", Locator_XMLHandler::SERVER_INFO_TAG);
      ACE_OS::fprintf (list," fname=\"%s\"/>\n", fname.c_str ());
    }

  // Save Activators
  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::ITERATOR aiit (this->activators ());
  for (; aiit.next (aientry); aiit.advance ())
    {
      Activator_Info_Ptr& info = aientry->int_id_;
      if (sync_op == SYNC_REMOVE && activator && lcase(name) == info->name)
        {
          // skip the removed activator
          continue;
        }
      ACE_CString aname = aientry->ext_id_;
      const ACE_CString fname = make_filename(aname, true);
      ACE_OS::fprintf (list,"\t<%s", Locator_XMLHandler::ACTIVATOR_INFO_TAG);
      ACE_OS::fprintf (list," fname=\"%s\"/>\n", fname.c_str ());
    }

  if (sync_op == SYNC_ADD && activator)
    {
      const ACE_CString fname = make_filename(name, false);
      ACE_OS::fprintf (list,"\t<%s", Locator_XMLHandler::ACTIVATOR_INFO_TAG);
      ACE_OS::fprintf (list," fname=\"%s\"/>\n", fname.c_str ());
    }

  ACE_OS::fprintf (list,"</ImRListing>\n");
  ACE_OS::fclose (list);
  return 0;
}
