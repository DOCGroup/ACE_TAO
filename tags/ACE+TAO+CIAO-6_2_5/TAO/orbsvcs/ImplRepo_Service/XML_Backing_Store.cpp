// $Id$

#include "orbsvcs/Log_Macros.h"
#include "XML_Backing_Store.h"
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

XML_Backing_Store::XML_Backing_Store(const Options& opts,
                                     CORBA::ORB_ptr orb,
                                     bool suppress_erase)
: Locator_Repository(opts, orb),
  filename_(opts.persist_file_name())
{
  if (opts.repository_erase() && !suppress_erase)
    {
      ACE_OS::unlink ( this->filename_.c_str () );
    }
}

XML_Backing_Store::~XML_Backing_Store()
{
}

int
XML_Backing_Store::persistent_remove (const ACE_CString& , bool )
{
  // one big XML file, need to persist everything
  return persist();
}

int
XML_Backing_Store::persistent_update(const Server_Info_Ptr& , bool )
{
  // one big XML file, need to persist everything
  return persist();
}

int
XML_Backing_Store::persistent_update(const Activator_Info_Ptr& , bool )
{
  // one big XML file, need to persist everything
  return persist();
}

int
XML_Backing_Store::persist ()
{
  FILE* fp = ACE_OS::fopen (this->filename_.c_str (), "w");
  if (fp == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %C\n"),
        this->filename_.c_str()));
      return -1;
    }
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  ACE_OS::fprintf (fp,"<%s>\n", Locator_XMLHandler::ROOT_TAG);

  // Save servers
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::ITERATOR siit (this->servers ());
  for (; siit.next (sientry); siit.advance() )
    {
      persist(fp, *sientry->int_id_, "\t");
    }

  // Save Activators
  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::ITERATOR aiit (this->activators ());
  for (; aiit.next (aientry); aiit.advance ())
    {
      ACE_CString aname = aientry->ext_id_;
      persist(fp, *aientry->int_id_, "\t");
    }

  ACE_OS::fprintf (fp,"</%s>\n", Locator_XMLHandler::ROOT_TAG);
  ACE_OS::fclose (fp);

  return 0;
}

void
XML_Backing_Store::persist (FILE* fp,
                            const Server_Info& info,
                            const char* tag_prepend,
                            const NameValues& name_values)
{
  ACE_CString server_id = ACEXML_escape_string (info.server_id);
  ACE_CString pname = ACEXML_escape_string (info.poa_name);
  ACE_CString keyname = ACEXML_escape_string (info.key_name_);
  ACE_CString altkey = "";
  if (!info.alt_info_.null())
    {
      altkey = ACEXML_escape_string (info.alt_info_->key_name_);
    }

  ACE_CString activator = ACEXML_escape_string (info.activator);
  ACE_CString cmdline = ACEXML_escape_string (info.cmdline);
  ACE_CString wdir = ACEXML_escape_string (info.dir);
  ACE_CString partial_ior = ACEXML_escape_string (info.partial_ior);
  ACE_CString ior = ACEXML_escape_string (info.ior);
  ACE_CString amodestr =
    ImR_Utils::activationModeToString (info.activation_mode_);

  ACE_OS::fprintf (fp,"%s<%s", tag_prepend,
    Locator_XMLHandler::SERVER_INFO_TAG);
  ACE_OS::fprintf (fp, " %s=\"%s\"", Locator_XMLHandler::SERVER_TAG, server_id.c_str ());
  ACE_OS::fprintf (fp, " %s=\"%s\"", Locator_XMLHandler::POANAME_TAG, pname.c_str ());
  ACE_OS::fprintf (fp, " %s=\"%s\"", Locator_XMLHandler::ACTNAME_TAG, activator.c_str ());
  ACE_OS::fprintf (fp, " %s=\"%s\"", Locator_XMLHandler::CMDLINE_TAG, cmdline.c_str ());
  ACE_OS::fprintf (fp, " %s=\"%s\"", Locator_XMLHandler::DIR_TAG, wdir.c_str ());
  ACE_OS::fprintf (fp, " %s=\"%s\"", Locator_XMLHandler::MODE_TAG, amodestr.c_str ());
  ACE_OS::fprintf (fp, " %s=\"%d\"", Locator_XMLHandler::LIMIT_TAG, info.start_limit_);
  ACE_OS::fprintf (fp, " %s=\"%s\"", Locator_XMLHandler::PARTIOR_TAG, partial_ior.c_str ());
  ACE_OS::fprintf (fp, " %s=\"%s\"", Locator_XMLHandler::IOR_TAG, ior.c_str ());
  ACE_OS::fprintf (fp, " %s=\"%d\"", Locator_XMLHandler::STARTED_TAG, !CORBA::is_nil(info.server.in()));
  ACE_OS::fprintf (fp, " %s=\"%d\"", Locator_XMLHandler::JACORB_TAG, info.is_jacorb);
  ACE_OS::fprintf (fp, " %s=\"%d\"", Locator_XMLHandler::PID_TAG, info.pid);
  ACE_OS::fprintf (fp, " %s= \"%s\"", Locator_XMLHandler::KEYNAME_TAG, keyname.c_str ());
  ACE_OS::fprintf (fp, " %s= \"%s\"", Locator_XMLHandler::ALTKEY_TAG, altkey.c_str ());

  NameValues::const_iterator name_value;
  for (name_value = name_values.begin(); name_value != name_values.end(); ++name_value)
    {
      ACE_OS::fprintf (fp," %s=\"%s\"",
                       name_value->first.c_str(), name_value->second.c_str());
    }

  CORBA::ULong elen = info.env_vars.length ();
  CORBA::ULong plen = info.peers.length ();

  if (elen + plen > 0)
    {
      ACE_OS::fprintf (fp,">\n");
      for (CORBA::ULong i = 0; i < elen; ++i)
        {
          ACE_OS::fprintf (fp,"%s\t<%s", tag_prepend,
            Locator_XMLHandler::ENVIRONMENT_TAG);
          ACE_OS::fprintf (fp," name=\"%s\"", info.env_vars[i].name.in ());
          ACE_CString val = ACEXML_escape_string(info.env_vars[i].value.in());
          ACE_OS::fprintf (fp," value=\"%s\"", val.c_str());
          ACE_OS::fprintf (fp,"/>\n");
        }
      for (CORBA::ULong i = 0; i < plen; ++i)
        {
          ACE_OS::fprintf (fp,"%s\t<%s", tag_prepend,
            Locator_XMLHandler::PEER_TAG);
          ACE_CString name = ACEXML_escape_string(info.peers[i].in());
          ACE_OS::fprintf (fp," name=\"%s\"", name.c_str());
          ACE_OS::fprintf (fp,"/>\n");
        }

      ACE_OS::fprintf (fp,"%s</%s>\n", tag_prepend, Locator_XMLHandler::SERVER_INFO_TAG);
    }
  else
    {
      ACE_OS::fprintf (fp,"/>\n");
    }
}

void
XML_Backing_Store::persist (FILE* fp,
                            const Activator_Info& info,
                            const char* tag_prepend,
                            const NameValues& name_values)
{
  ACE_OS::fprintf (fp,"%s<%s", tag_prepend,
    Locator_XMLHandler::ACTIVATOR_INFO_TAG);
  ACE_OS::fprintf( fp," name=\"%s\"", info.name.c_str ());
  ACE_OS::fprintf (fp," token=\"%d\"", info.token);
  ACE_OS::fprintf (fp," ior=\"%s\"", info.ior.c_str ());

  NameValues::const_iterator name_value;
  for (name_value = name_values.begin(); name_value != name_values.end(); ++name_value)
    {
      ACE_OS::fprintf (fp," %s=\"%s\"",
                       name_value->first.c_str(), name_value->second.c_str());
    }

  ACE_OS::fprintf (fp,"/>\n");
}

int
XML_Backing_Store::init_repo(PortableServer::POA_ptr )
{
  // ignore load return since files don't have to exist
  load(this->filename_);
  return 0;
}

int
XML_Backing_Store::load (const ACE_TString& filename, FILE* open_file)
{
  Locator_XMLHandler xml_handler (*this);
  return load(filename, xml_handler, this->opts_.debug(), open_file);
}

int
XML_Backing_Store::load (const ACE_TString& filename,
                         ACEXML_DefaultHandler& xml_handler,
                         unsigned int debug,
                         FILE* open_file)
{
  // xml input source will take ownership
  ACEXML_FileCharStream* fstm;
  ACE_NEW_RETURN (fstm,
                  ACEXML_FileCharStream,
                  -1);

  int err;
  // use the open_file stream if it is provided
  if (open_file != 0)
    err = fstm->use_stream(open_file,
                           filename.c_str());
  else
    err = fstm->open (filename.c_str());

  if (debug > 9)
    {
      ORBSVCS_DEBUG ((LM_INFO, ACE_TEXT ("load %s%C\n"), filename.c_str(),
                 ((err == 0) ? ACE_TEXT ("")
                  : ACE_TEXT (" (file doesn't exist)"))));
    }

  if (err != 0)
    {
      delete fstm;
      return err;
    }

  ACEXML_Parser parser;

  // InputSource takes ownership
  ACEXML_InputSource input (fstm);

  parser.setContentHandler (&xml_handler);
  parser.setDTDHandler (&xml_handler);
  parser.setErrorHandler (&xml_handler);
  parser.setEntityResolver (&xml_handler);

  try
    {
      parser.parse (&input);
    }
    catch ( const ACEXML_SAXParseException* sax_ex)
    {
      ORBSVCS_ERROR ((LM_ERROR,
        ACE_TEXT ("Error during load of ImR persistence xml file (%s)."),
        filename.c_str()));
      sax_ex->print ();
      return -1;
    }
    catch (const ACEXML_Exception& ex)
    {
      ORBSVCS_ERROR ((LM_ERROR,
        ACE_TEXT ("Error during load of ImR persistence xml file (%s)."),
        filename.c_str()));
      ex.print ();
      return -1;
    }



  return 0;
}

const ACE_TCHAR*
XML_Backing_Store::repo_mode() const
{
  return this->filename_.c_str();
}

void
XML_Backing_Store::load_server (Server_Info *info,
                                bool server_started,
                                const NameValues& )
{
  Server_Info_Ptr si (info);

  this->servers().rebind (info->key_name_, si);

  create_server (server_started, si);
}

void
XML_Backing_Store::create_server (bool server_started,
                                  const Server_Info_Ptr& si)
{
  if (!server_started || si->ior.is_empty())
    {
      return;
    }

  CORBA::Object_var obj = this->orb_->string_to_object(si->ior.c_str());
  if (!CORBA::is_nil(obj.in()))
    {
      si->server =
        ImplementationRepository::ServerObject::_unchecked_narrow (obj.in());
      si->last_ping = ACE_Time_Value::zero;
    }
}

void
XML_Backing_Store::load_activator (const ACE_CString& activator_name,
                                   long token,
                                   const ACE_CString& ior,
                                   const NameValues& )
{
  Activator_Info *ai;
  ACE_NEW (ai,
           Activator_Info (activator_name, token, ior));

  Activator_Info_Ptr info (ai);
  this->activators().rebind(Locator_Repository::lcase (activator_name), info);
}
