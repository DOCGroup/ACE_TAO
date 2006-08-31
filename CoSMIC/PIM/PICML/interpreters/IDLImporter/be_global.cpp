// $Id$

// ============================================================================
//
//
// = LIBRARY
//    IDL_TO_PICML_BE_DLL
//
// = FILENAME
//    be_global.cpp
//
// = DESCRIPTION
//    Stores global data specific to the compiler back end.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#include "XercesString.h"
#include "XML_Error_Handler.h"
#include "EntityResolver.h"
#include "be_extern.h"
#include "be_global.h"
#include "ast_generator.h"
#include "ast_module.h"
#include "ast_sequence.h"
#include "utl_identifier.h"
#include "fe_extern.h"
#include "global_extern.h"
#include "nr_extern.h"
#include "idl_defines.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Env_Value_T.h"
#include "ace/streams.h"

// Some magic strings.
const char *VERSION = "1.0";
const char *ENCODING = "UTF-8";
const char *DOCTYPE = "project";
const char *SYS_ID = "mga.dtd";
const char *FILE_EXT = ".xme";

IDL_TO_PICML_BE_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData (void)
  : filename_ (0),
    output_dir_ (0),
    input_xme_ (0),
    models_seen_ (0UL),
    atoms_seen_ (0UL),
    refs_seen_ (0UL),
    conns_seen_ (0UL),
    folders_seen_ (0UL),
    npredefined_ (11UL),
    nfiles_ (0L),
    first_file_ (true),
    output_file_ ("PICML_default_xme_file"),
    do_removal_ (true),
    doc_ (0),
    writer_ (0),
    target_ (0),
    root_folder_ (0),
    component_types_folder_ (0),
    implementation_artifacts_folder_ (0),
    implementations_folder_ (0),
    interface_definitions_folder_ (0),
    current_idl_file_ (0),
    component_types_rel_id_ (1UL),
    implementation_artifacts_rel_id_ (1UL),
    implementations_rel_id_ (1UL),
    basic_seq_suffix_ ("Seq_from_IDL_include")
{
  ACE_Env_Value<const char *> path ("GME_ROOT", 0);

  // Need an extra step here because some C++ compilers can't
  // match ACE_CString's assignment from char* operator with
  // ACE_Env_Value's cast operator.
  const char *path_str = path;

  if (path_str != 0)
    {
      // GME versions from 5.9.21 no longer install mga.dtd
      // in $GME_ROOT/bin, but in $GME_ROOT.
      this->schema_path_ = path_str;
    }
}

BE_GlobalData::~BE_GlobalData (void)
{
}

const char *
BE_GlobalData::filename (void) const
{
  return this->filename_;
}

void
BE_GlobalData::filename (char *fname)
{
  this->filename_ = fname;
}

const char*
BE_GlobalData::output_dir (void) const
{
  return this->output_dir_;
}

void
BE_GlobalData::output_dir (const char* s)
{
  delete [] this->output_dir_;
  this->output_dir_ = ACE::strnew (s);
}

const char*
BE_GlobalData::input_xme (void) const
{
  return this->input_xme_;
}

void
BE_GlobalData::input_xme (const char* s)
{
  delete [] this->input_xme_;
  this->input_xme_ = ACE::strnew (s);
}

unsigned long
BE_GlobalData::models_seen (void) const
{
  return this->models_seen_;
}

unsigned long
BE_GlobalData::atoms_seen (void) const
{
  return this->atoms_seen_;
}

unsigned long
BE_GlobalData::refs_seen (void) const
{
  return this->refs_seen_;
}

unsigned long
BE_GlobalData::conns_seen (void) const
{
  return this->conns_seen_;
}

unsigned long
BE_GlobalData::folders_seen (void) const
{
  return this->folders_seen_;
}

void
BE_GlobalData::incr_models_seen (void)
{
  ++this->models_seen_;
}

void
BE_GlobalData::incr_atoms_seen (void)
{
  ++this->atoms_seen_;
}

void
BE_GlobalData::incr_refs_seen (void)
{
  ++this->refs_seen_;
}

void
BE_GlobalData::incr_conns_seen (void)
{
  ++this->conns_seen_;
}

void
BE_GlobalData::incr_folders_seen (void)
{
  ++this->folders_seen_;
}

unsigned long
BE_GlobalData::npredefined (void) const
{
  return this->npredefined_;
}

const char**
BE_GlobalData::pdt_names (void) const
{
  static const char *list[] = {
                                "TypeKind",
                                "TypeEncoding",
                                "String",
                                "Boolean",
                                "ShortInteger",
                                "RealNumber",
                                "LongInteger",
                                "GenericValueObject",
                                "GenericValue",
                                "GenericObject",
                                "Byte"
                              };
  return list;
}

long
BE_GlobalData::nfiles (void) const
{
  return this->nfiles_;
}

void
BE_GlobalData::nfiles (long val)
{
  this->nfiles_ = val;
}

bool
BE_GlobalData::first_file (void) const
{
  return this->first_file_;
}

void
BE_GlobalData::first_file (bool val)
{
  this->first_file_ = val;
}

ACE_CString
BE_GlobalData::output_file (void) const
{
  return this->output_file_;
}

void
BE_GlobalData::output_file (const char *val)
{
  this->output_file_ = val;
}

bool
BE_GlobalData::do_removal (void) const
{
  return this->do_removal_;
}

void
BE_GlobalData::do_removal (bool val)
{
  this->do_removal_ = val;
}

DOMDocument *
BE_GlobalData::doc (void) const
{
  return this->doc_;
}

DOMElement *
BE_GlobalData::root_folder (void) const
{
  return this->root_folder_;
}

void
BE_GlobalData::root_folder (DOMElement *elem)
{
  this->root_folder_ = elem;
}

DOMElement *
BE_GlobalData::component_types_folder (void) const
{
  return this->component_types_folder_;
}

void
BE_GlobalData::component_types_folder (DOMElement *elem)
{
  this->component_types_folder_ = elem;
}

DOMElement *
BE_GlobalData::implementation_artifacts_folder (void) const
{
  return this->implementation_artifacts_folder_;
}

void
BE_GlobalData::implementation_artifacts_folder (DOMElement *elem)
{
  this->implementation_artifacts_folder_ = elem;
}

DOMElement *
BE_GlobalData::implementations_folder (void) const
{
  return this->implementations_folder_;
}

void
BE_GlobalData::implementations_folder (DOMElement *elem)
{
  this->implementations_folder_ = elem;
}

DOMElement *
BE_GlobalData::interface_definitions_folder (void) const
{
  return this->interface_definitions_folder_;
}

void
BE_GlobalData::interface_definitions_folder (DOMElement *elem)
{
  this->interface_definitions_folder_ = elem;
}

DOMElement *
BE_GlobalData::current_idl_file (void) const
{
  return this->current_idl_file_;
}

void
BE_GlobalData::current_idl_file (DOMElement *elem)
{
  this->current_idl_file_ = elem;
}

unsigned long
BE_GlobalData::component_types_rel_id (void) const
{
  return this->component_types_rel_id_;
}

void
BE_GlobalData::incr_component_types_rel_id (void)
{
  ++this->component_types_rel_id_;
}

unsigned long
BE_GlobalData::implementation_artifacts_rel_id (void) const
{
  return this->implementation_artifacts_rel_id_;
}

void
BE_GlobalData::incr_implementation_artifacts_rel_id (void)
{
  ++this->implementation_artifacts_rel_id_;
}

unsigned long
BE_GlobalData::implementations_rel_id (void) const
{
  return this->implementations_rel_id_;
}

void
BE_GlobalData::incr_implementations_rel_id (void)
{
  ++this->implementations_rel_id_;
}

BE_GlobalData::DECL_ID_TABLE &
BE_GlobalData::decl_id_table (void)
{
  return this->decl_id_table_;
}

BE_GlobalData::DECL_ELEM_TABLE &
BE_GlobalData::decl_elem_table (void)
{
  return this->decl_elem_table_;
}

BE_GlobalData::REF_DECL_TABLE &
BE_GlobalData::ref_decl_table (void)
{
  return this->ref_decl_table_;
}

BE_GlobalData::GME_ID_SET &
BE_GlobalData::gme_id_set (void)
{
  return this->gme_id_set_;
}

ACE_CString
BE_GlobalData::spawn_options (void)
{
  return idl_global->idl_flags ();
}

void
BE_GlobalData::parse_args (long &i, char **av)
{
  switch (av[i][1])
    {
      case 'x':
        if (av[i][2] == '\0')
          {
            be_global->output_file (av[i + 1]);
            ++i;
          }
        else
          {
            be_global->output_file (av[i] + 2);
          }
        break;
      // Directory where the generated file is to
      // be kept. Default is the current directory from which
      // <idl_to_picml> is called.
      case 'o':
        if (av[i][2] == '\0')
          {
            be_global->output_dir (av [i + 1]);
            i++;
          }
        else
          {
            be_global->output_dir (av[i] + 2);
          }
        break;
      case 'i':
        if (av[i][2] == '\0')
          {
            be_global->input_xme (av [i + 1]);
            i++;
          }
        else
          {
            be_global->input_xme (av[i] + 2);
          }
        break;
      case 'r':
        if (av[i][2] == '\0')
          {
            idl_global->recursion_start (av [i + 1]);
            i++;
          }
        else
          {
            idl_global->recursion_start (av[i] + 2);
          }
        break;
      case 'm':
        if (av[i][2] == '\0')
          {
            this->schema_path_ = av [i + 1];
            i++;
          }
        else
          {
            this->schema_path_ = av[i] + 2;
          }
        break;
      case 'S':
        if (av[i][2] == 'p')
          {
            this->do_removal_ = false;
          }
        else
          {
            ACE_ERROR ((
                LM_ERROR,
                ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                av[i]
              ));
          }
        break;
      default:
        ACE_ERROR ((
            LM_ERROR,
            ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
            av[i]
          ));

        idl_global->set_compile_flags (idl_global->compile_flags ()
                                       | IDL_CF_ONLY_USAGE);
        break;
    }
}

// Does nothing in this backend.
void
BE_GlobalData::prep_be_arg (char *)
{
}

// Does nothing in this backend.
void
BE_GlobalData::arg_post_proc (void)
{
}

void
BE_GlobalData::usage (void) const
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -x <filename>\t\tName of the generated file.")
      ACE_TEXT (" Default is \"PICML_default_xme_file.xme\".\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -r <pathname>\t\tPath of the directory to iterate")
      ACE_TEXT (" over recursively, processing each IDL file found.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -o <dir>\t\tOutput directory for the generated file.")
      ACE_TEXT (" Default is current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -i <filepath>\t\tPath to XME input file.")
      ACE_TEXT (" Default is no XME input file\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -m <filepath>\t\tPath to GME's mga.dtd file.")
      ACE_TEXT (" Default is c:\\Program Files\\GME\\bin\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sp \t\t\tSuppress pruning of elements removed")
      ACE_TEXT (" in modified IDL.")
      ACE_TEXT (" Default is to do pruning\n")
    ));
}

AST_Generator *
BE_GlobalData::generator_init (void)
{
  AST_Generator *gen = 0;
  ACE_NEW_RETURN (gen,
                  AST_Generator,
                  0);
  return gen;
}

void
BE_GlobalData::xerces_init (void)
{
  // Initialize the Xerces run-time
  try
    {
      XMLPlatformUtils::Initialize ();
      XMLCh tempStr[100];
      XMLString::transcode ("LS", tempStr, 99);
      DOMImplementation *impl =
        DOMImplementationRegistry::getDOMImplementation (tempStr);

      if (0 == impl)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Null DOMImplementation returned\n")));
          BE_abort ();
        }

      this->writer_ =
        ((DOMImplementationLS *)impl)->createDOMWriter ();
      this->writer_->setNewLine (XStr ("\n"));
      bool can_pretty_print =
        this->writer_->canSetFeature (
            XMLUni::fgDOMWRTFormatPrettyPrint,
            true
          );

      if (can_pretty_print)
        {
          this->writer_->setFeature (
              XMLUni::fgDOMWRTFormatPrettyPrint,
              true
            );
        }

      const char *xme = this->input_xme ();

      // If we are inputting an existing XME file, use that to create
      // the DOM tree, else create an empty one.
      if (0 == xme)
        {
          DOMDocumentType *doc_type =
            impl->createDocumentType (X (DOCTYPE),
                                      0,
                                      X (SYS_ID));
          this->doc_ = impl->createDocument (0,
                                             X (DOCTYPE),
                                             doc_type);

          this->doc_->setEncoding (X (ENCODING));
          this->doc_->setVersion (X (VERSION));
        }
      else
        {
          DOMBuilder *parser =
            ((DOMImplementationLS*)impl)->createDOMBuilder (
                DOMImplementationLS::MODE_SYNCHRONOUS,
                0
              );

          if (parser->canSetFeature(XMLUni::fgDOMValidation, true))
            {
              parser->setFeature(XMLUni::fgDOMValidation, true);
            }

          if (parser->canSetFeature(XMLUni::fgDOMNamespaces, true))
            {
              parser->setFeature(XMLUni::fgDOMNamespaces, true);
            }

          if (parser->canSetFeature(XMLUni::fgDOMDatatypeNormalization, true))
            {
              parser->setFeature(XMLUni::fgDOMDatatypeNormalization, true);
            }

          parser->setFeature (XMLUni::fgDOMComments, false);
          parser->setFeature (XMLUni::fgDOMEntities, false);
          parser->setFeature (XMLUni::fgDOMNamespaces, true);
          parser->setFeature (XMLUni::fgDOMValidation, true);
          parser->setFeature (XMLUni::fgDOMWhitespaceInElementContent, false);
          parser->setFeature (XMLUni::fgXercesSchema, true);

          XML_Error_Handler handler;
          parser->setErrorHandler (&handler);

          if (this->schema_path_ == "" && this->input_xme_ != 0)
            {
              ACE_DEBUG ((LM_WARNING,
                          "No schema path found or supplied - schema or "
                          "DTD file must be in directory of execution\n"));
            }

          EntityResolver resolver (this->schema_path_.c_str ());
          parser->setEntityResolver (&resolver);

          this->doc_ = parser->parseURI (xme);

          if (handler.getErrors ())
            {
              // The error handler will output a message.
              BE_abort ();
            }
        }

      ACE_CString target_name;
      const char *path = be_global->output_dir ();

      if (path != 0)
        {
          target_name += path;
          target_name += "/";
        }

      target_name += be_global->output_file ();
      target_name += FILE_EXT;

      // Create a target for the output:

      // (@@@ JP) All of a sudden (2/7/05) there was an exception when
      // XMLCh* was passed, but not when char* was passed (the constructor
      // for LocalFileFormatTarget has two variants).
      XStr target_xstr (target_name.c_str ());
      const XMLCh * target_arg = target_xstr.begin ();

      this->target_ = new LocalFileFormatTarget (target_name.c_str ());
    }
  catch (const DOMException &e)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("xerces_init - ")
                  ACE_TEXT ("DOMException code is:  %d\n"),
                  e.code));
      BE_abort ();
    }
  catch (const XMLException &e)
    {
      char *message = XMLString::transcode (e.getMessage ());
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("xerces_init - ")
                  ACE_TEXT ("XMLException message is: %s\n"),
                  message));
      XMLString::release (&message);
      BE_abort ();
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Unknown exception in xerces_init\n")));
      BE_abort ();
    }
}

void
BE_GlobalData::cache_files (char *files[], long nfiles)
{
  for (long i = 0; i < nfiles; ++i)
    {
      // Cache the file names in be_global.
      this->allfiles_[i] = files[i];

      // Store DOM element and GME id for each file now, so we
      // won't have to be concerned that a lookup failure later
      // might be due to a mistake or that the filename just
      // appears further down the command line.

      ACE_CString fname (files[i]);
      int fpos = fname.rfind ('/');
      int pos = (fpos == ACE_CString::npos ? fname.rfind ('\\') : fpos);
      ACE_CString lname =
        (pos == ACE_CString::npos ? fname : fname.substr (pos + 1));
      lname = lname.substr (0, lname.rfind ('.'));
      const char *lname_cstr = lname.c_str ();

      DOMElement *file = 0;

      if (0 != be_global->input_xme ())
        {
          file =
            this->imported_dom_element (this->interface_definitions_folder_,
                                        lname_cstr);
        }

      if (0 == file)
        {
          // Create a DOM element and store in the table under the
          // local filename. This may lead to name clashes, but it is
          // too much of a bottomless pit to deal with full pathnames
          // portably.
          file = this->doc_->createElement (X ("model"));

          // Create a new GME id and store it as well.
          ACE_CString new_id = this->make_gme_id (MODEL);
          file->setAttribute (X ("id"), X (new_id.c_str ()));
        }

      this->decl_elem_table_.bind (ACE::strnew (lname_cstr),
                                   file);

      this->decl_id_table_.bind (ACE::strnew (lname_cstr),
                                 file->getAttribute (X ("id")));
   }

  this->nfiles_ = nfiles;
}

void
BE_GlobalData::destroy (void)
{
  if (0 != this->writer_ && 0 != this->target_ && 0 != this->doc_)
    {
      // Write out the file last, just before cleanup.
      this->writer_->writeNode (this->target_, *this->doc_);
    }

  if (0 != this->writer_)
    {
      this->writer_->release ();
    }

  if (0 != this->doc_)
    {
      this->doc_->release ();
    }

  delete this->target_;
  this->target_ = 0;

  XMLPlatformUtils::Terminate ();

  DECL_ID_TABLE_ENTRY *id_entry = 0;
  for (DECL_ID_TABLE_ITERATOR id_iter (this->decl_id_table_);
       id_iter.next (id_entry) != 0;
       id_iter.advance ())
    {
      delete [] const_cast<char *> (id_entry->ext_id_);
    }

  DECL_ELEM_TABLE_ENTRY *fwd_entry = 0;
  for (DECL_ELEM_TABLE_ITERATOR fwd_iter (this->decl_elem_table_);
       fwd_iter.next (fwd_entry) != 0;
       fwd_iter.advance ())
    {
      delete [] const_cast<char *> (fwd_entry->ext_id_);
    }

  REF_DECL_TABLE_ENTRY *ref_entry = 0;
  for (REF_DECL_TABLE_ITERATOR ref_iter (this->ref_decl_table_);
       ref_iter.next (ref_entry) != 0;
       ref_iter.advance ())
    {
      delete [] const_cast<char *> (ref_entry->ext_id_);
    }

  this->gme_id_set_.reset ();
}

char *
BE_GlobalData::hex_string (unsigned long number)
{
  static char holder[9];
  ACE_OS::memset (holder,
                  '\0',
                  9);
  ACE_OS::sprintf (holder,
                   "0x%x",
                   number);
  return holder;
}

ACE_CString
BE_GlobalData::basic_seq_suffix (void) const
{
  return this->basic_seq_suffix_;
}

ACE_CString
BE_GlobalData::make_gme_id (kind_id kind)
{
  ACE_CString val ("id-");
  static char short_str[5];
  ACE_OS::sprintf (short_str, "%4.4x", kind);
  short_str[4] = '\0';
  val += short_str;
  val += "-";
  unsigned long tmp = 0;

  switch (kind)
    {
      case MODEL:
        tmp = ++this->models_seen_;
        break;
      case ATOM:
        tmp = ++this->atoms_seen_;
        break;
      case REF:
        tmp = ++this->refs_seen_;
        break;
      case CONN:
        tmp = ++this->conns_seen_;
        break;
      case FOLDER:
        tmp = ++this->folders_seen_;
        break;
      default:
        break;
    }

  static char long_str[9];
  ACE_OS::sprintf (long_str, "%8.8x", tmp);
  long_str[8] = '\0';
  val += long_str;
  return val;
}

DOMElement *
BE_GlobalData::imported_dom_element (DOMElement *sub_tree,
                                     const char *local_name,
                                     kind_id elem_kind,
                                     bool by_referred,
                                     bool in_file_iteration)
{
  // Can't already be in the DOM tree if we didn't read in an XME file.
  if (0 == this->input_xme_)
    {
      return 0;
    }

  // At global scope, we may be visiting an included file, so we
  // may have to look in all File elements to find an XML import.
  bool importing = idl_global->import ();
  bool in_main = idl_global->in_main_file ();
  if (!in_file_iteration && importing && !in_main)
    {
      if (X ("File") == sub_tree->getAttribute (X ("kind")))
        {
          // All the top level elements in the InterfaceDefinnitions
          // folder are Files.
          DOMNodeList *files =
            this->interface_definitions_folder_->getChildNodes ();

          DOMElement *file = 0;
          DOMElement *retval = 0;

          // Iterate over the list of Files.
          for (XMLSize_t i = 0; i < files->getLength (); ++i)
            {
              file = (DOMElement *) files->item (i);

              // Skip any other children, like <name>.
              if (X ("model") == file->getTagName ())
                {
                  retval = this->imported_dom_element (file,
                                                       local_name,
                                                       elem_kind,
                                                       by_referred,
                                                       true);

                  if (retval != 0)
                    {
                      return retval;
                    }
                }
            }

          // Not found in any file.
          return 0;
        }
    }

  DOMNodeList *children = sub_tree->getChildNodes ();
  XStr tag;

  switch (elem_kind)
    {
      default:
        tag = "model";
        break;
      case REF:
        tag = "reference";
        break;
      case ATOM:
        tag = "atom";
        break;
      case CONN:
        tag = "connection";
        break;
    }

  for (XMLSize_t index = 0; index < children->getLength (); ++index)
    {
      DOMElement *child =
        (DOMElement *) children->item (index);

      if (tag != X (child->getTagName ()))
        {
          continue;
        }

      DOMElement *compared_elem = child;

      // Elements of an exception list have no names, so we have to
      // look up the referenced exception by name and return its
      // DOMElement, if any, to see if this local_name represents a
      // new member of the exception list.
      if (by_referred)
        {
          const XMLCh *referred = child->getAttribute (X ("referred"));

          // Compare the 'referred' but return the 'reference'.
          compared_elem = this->doc_->getElementById (referred);
        }

      // There should be only one "name" node.
      DOMNodeList *namelist =
        compared_elem->getElementsByTagName (X ("name"));

      DOMNode *name_elem = namelist->item (0);

      DOMNode *name_item = name_elem->getFirstChild ();
      DOMText *name = (DOMText *) name_item;
      const XMLCh *text = name->getData ();

      if (X (local_name) == text)
        {
          return child;
        }
    }

  return 0;
}

DOMElement *
BE_GlobalData::imported_module_dom_elem (DOMElement *sub_tree,
                                         AST_Module *node)
{
  // Can't already be in the DOM tree if we didn't read in an XME file.
  if (0 == this->input_xme_)
    {
      return 0;
    }

  DOMNodeList *models =
    this->interface_definitions_folder_->getElementsByTagName (X ("model"));

  for (XMLSize_t i = 0; i < models->getLength (); ++i)
    {
      DOMElement *model = (DOMElement *) models->item (i);
      const XMLCh *kind = model->getAttribute (X ("kind"));

      if (X ("Package") != X (kind))
        {
          continue;
        }

      if (this->match_module_opening (model, node))
        {
          return model;
        }
    }

  return 0;
}

void
BE_GlobalData::set_working_folders (void)
{
  // Get root folder and interface definitions folder from the
  // input and cache them.
  if (0 == this->input_xme_)
    {
      return;
    }

  DOMElement *root = doc_->getDocumentElement ();

  DOMNodeList *folders =
    root->getElementsByTagName (X ("folder"));

  DOMElement *root_folder = (DOMElement *) folders->item (0);
  this->root_folder_ = root_folder;

  this->interface_definitions_folder_ =
    this->lookup_by_tag_and_kind (root_folder,
                                  "folder",
                                  "InterfaceDefinitions");
}

DOMElement *
BE_GlobalData::lookup_by_tag_and_kind (DOMElement *scope,
                                       const char *tag_name,
                                       const char *kind_name)
{
  DOMNodeList *children = scope->getElementsByTagName (X (tag_name));

  for (XMLSize_t i = 0; i < children->getLength (); ++i)
    {
      DOMElement *child = (DOMElement *) children->item (i);
      const XMLCh *kind = child->getAttribute (X ("kind"));

      if (X (kind_name) == kind)
        {
          return child;
        }
    }

  return 0;
}

void
BE_GlobalData::init_ids (DOMElement *sub_tree)
{
  static const XStr model ("model");
  static const XStr reference ("reference");
  static const XStr atom ("atom");
  static const XStr connection ("connection");
  static const XStr folder ("folder");

  // Element must have an id attribute to convert to an integer.
  if (sub_tree == 0 || !sub_tree->hasAttribute (X ("id")))
    {
      return;
    }

  const XMLCh *tag = sub_tree->getTagName ();
  const XMLCh *id = sub_tree->getAttribute (X ("id"));
  char *id_string = XMLString::transcode (id);
  unsigned long val = ACE_OS::strtoul (id_string + 8, 0, 16);
  XMLString::release (&id_string);

  if (model == tag)
    {
      this->models_seen_ =
        ace_max<unsigned long> (this->models_seen_, val);
    }
  else if (reference == tag)
    {
      this->refs_seen_ =
        ace_max<unsigned long> (this->refs_seen_, val);
    }
  else if (connection == tag)
    {
      this->conns_seen_ =
        ace_max<unsigned long> (this->conns_seen_, val);
    }
  else if (atom == tag)
    {
      this->atoms_seen_ =
        ace_max<unsigned long> (this->atoms_seen_, val);
    }
  else if (folder == tag)
    {
      this->folders_seen_ =
        ace_max<unsigned long> (this->folders_seen_, val);
    }

  DOMNodeList *children = sub_tree->getChildNodes ();

  for (XMLSize_t index = 0; index < children->getLength (); ++index)
    {
      this->init_ids ((DOMElement *) children->item (index));
    }
}

void
BE_GlobalData::release_node (DOMElement *node)
{
  this->emit_diagnostic (node, REMOVING);
  DOMNode *parent = node->getParentNode ();
  DOMNode *releasable = parent->removeChild (node);
  releasable->release ();
}

void
BE_GlobalData::emit_diagnostic (DOMElement *node, diagnostic_type dt)
{
  // This is redundant for remove calls, but it will save a check
  // for each adding call.
  if (0 == this->input_xme_)
    {
      return;
    }

  const XMLCh *kindX = node->getAttribute (X ("kind"));
  char *kind = XMLString::transcode (kindX);
  const XMLCh *tagX = node->getTagName ();
  char *tag = XMLString::transcode (tagX);
  char *name = this->get_name (node);

  ACE_CString ref_name ("<no name>");
  bool no_name = (ACE_OS::strcmp (name, kind) == 0);

  // If the model element is tagged 'reference' and contains no
  // 'name' element, use the named of the referenced type.
  if (no_name && ACE_OS::strcmp (tag, "reference") == 0)
    {
      DOMElement *referred =
        this->doc_->getElementById (node->getAttribute (X ("referred")));
      const XMLCh *id = referred->getAttribute (X ("id"));
      char *r_name = this->get_name (referred);

      char *r_kind = 0;
      DOMElement *parent = (DOMElement *) referred->getParentNode ();
      const XMLCh *parent_kind = parent->getAttribute (X ("kind"));
      ACE_CString r_kind_str;

      if (X ("PredefinedTypes") == parent_kind)
        {
          // Here, the name and kind are the same, so we use the
          // parent kind, minus the final letter, see below.
          r_kind = XMLString::transcode (parent_kind);

          // Anally remove the final 's' from 'PredefinedTypes'.
          r_kind_str =
            ACE_CString (r_kind).substr (0, ACE_OS::strlen (r_kind) - 1);
        }
      else
        {
          r_kind =
            XMLString::transcode (referred->getAttribute (X ("kind")));
          r_kind_str = r_kind;
        }

      ref_name = "(referencing ";
      ref_name += r_kind_str.c_str ();
      ref_name += " ";
      ref_name += r_name;
      ref_name += ")";
      XMLString::release (&r_kind);
      XMLString::release (&r_name);
    }

  DOMElement *parent = (DOMElement *) node->getParentNode ();
  kindX = parent->getAttribute (X ("kind"));
  char *p_kind = XMLString::transcode (kindX);
  tagX = parent->getTagName ();
  char *p_tag = XMLString::transcode (tagX);
  char *p_name = this->get_name (parent);

  cout << (ADDING == dt ? "Added " : "Removed ") << kind << " "
       << (no_name ? ref_name.c_str () : name)
       << " in " << p_kind << " "
       << (ACE_OS::strcmp (p_name, p_kind) == 0 ? "<no name>" : p_name)
       << endl;

  XMLString::release (&name);
  XMLString::release (&p_name);
  XMLString::release (&kind);
  XMLString::release (&p_kind);
  XMLString::release (&tag);
  XMLString::release (&p_tag);
 }

void
BE_GlobalData::emit_attribute_diagnostic (DOMElement *node,
                                          const char *name,
                                          const char *new_value,
                                          DOMText *old_value)
{
  // We are here only if we have imported an XML file.

  // Default working value of "" for non-existent text nodes.
  ACE_CString old_val ("");
  ACE_CString new_val (new_value);

  if (0 != old_value)
    {
      char *tmp = XMLString::transcode (old_value->getData ());
      old_val = tmp;
      XMLString::release (&tmp);
    }

  // No output unless the value has changed, possibly to or from the
  // empty string.
  if (old_val != new_val)
    {
      const XMLCh *kindX = node->getAttribute (X ("kind"));
      char *kind = XMLString::transcode (kindX);
      const XMLCh *tagX = node->getTagName ();
      char *tag = XMLString::transcode (tagX);
      char *node_name = this->get_name (node);

      cout << "Changed value of GME attribute \"" << name << "\" of "
           << kind << " " << node_name << " from \""
           << old_val.c_str () << "\" to \"" << new_val.c_str () << "\""
           << endl;

      XMLString::release (&kind);
      XMLString::release (&tag);
      XMLString::release (&node_name);
    }
}

char *
BE_GlobalData::get_name (DOMElement *node)
{
  DOMNodeList *list = node->getElementsByTagName (X ("name"));
  char *retval = 0;

  if (list->getLength () > 0)
    {
      DOMText *text_node =
        (DOMText *) list->item (0)->getFirstChild ();
      retval = XMLString::transcode (text_node->getData ());
    }

  // Caller must release with XMLString::release.
  return retval;
}

const XMLCh *
BE_GlobalData::lookup_id (AST_Decl *d)
{
  ACE_CString ext_id = d->repoID ();

  // One or the other of these, but not both, may replace ext_id.
  this->check_for_basic_type (d, ext_id);
  this->check_for_basic_seq (d, ext_id);

  const XMLCh *retval = 0;
  int result = this->decl_id_table_.find (ext_id.c_str (), retval);

  if (result != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_global::lookup_id - "
                         "lookup of id %s failed\n",
                         ext_id.c_str ()),
                        0);
    }

  return retval;
}

void
BE_GlobalData::check_for_basic_seq (AST_Decl *d, ACE_CString &str)
{
  if (d->node_type () != AST_Decl::NT_typedef)
    {
      return;
    }

  AST_Decl *p = ScopeAsDecl (d->defined_in ());
  if (ACE_OS::strcmp (p->local_name ()->get_string (), "CORBA") != 0)
    {
      return;
    }

  AST_Type *bt = AST_Typedef::narrow_from_decl (d)->base_type ();
  if (bt->node_type () != AST_Decl::NT_sequence)
    {
      return;
    }

  bt = AST_Sequence::narrow_from_decl (bt)->base_type ();
  AST_Decl::NodeType nt = bt->node_type ();
  this->check_for_basic_type (bt, str);

  // If the previous call modified the string, append the sequence suffix.
  if (nt == AST_Decl::NT_string
      || nt == AST_Decl::NT_wstring
      || nt == AST_Decl::NT_pre_defined)
    {
      str += be_global->basic_seq_suffix ();
    }
}

void
BE_GlobalData::check_for_basic_type (AST_Decl *d, ACE_CString &str)
{
  const char **namelist = be_global->pdt_names ();
  AST_Decl::NodeType nt = d->node_type ();

  if (nt == AST_Decl::NT_string || nt == AST_Decl::NT_wstring)
    {
      str = namelist[2UL];
    }
  else if (d->node_type () == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType *pdt = AST_PredefinedType::narrow_from_decl (d);

      switch (pdt->pt ())
        {
          case AST_PredefinedType::PT_long:
          case AST_PredefinedType::PT_ulong:
          case AST_PredefinedType::PT_longlong:
          case AST_PredefinedType::PT_ulonglong:
            str = namelist[6UL];
            break;
          case AST_PredefinedType::PT_short:
          case AST_PredefinedType::PT_ushort:
            str = namelist[4UL];
            break;
          case AST_PredefinedType::PT_float:
          case AST_PredefinedType::PT_double:
          case AST_PredefinedType::PT_longdouble:
            str = namelist[5UL];
            break;
          case AST_PredefinedType::PT_char:
          case AST_PredefinedType::PT_octet:
          case AST_PredefinedType::PT_wchar:
            str = namelist[10UL];
            break;
          case AST_PredefinedType::PT_boolean:
            str = namelist[3UL];
            break;
          case AST_PredefinedType::PT_any:
            str = namelist[8UL];
            break;
          case AST_PredefinedType::PT_object:
            str = namelist[9UL];
            break;
          case AST_PredefinedType::PT_value:
            str = namelist[7UL];
            break;
          case AST_PredefinedType::PT_pseudo:
            {
              const char *pseudo_name = d->local_name ()->get_string ();

              if (0 == ACE_OS::strcmp (pseudo_name, "TypeCode"))
                {
                  str = namelist[1UL];
                }
              else if (0 == ACE_OS::strcmp (pseudo_name, "TCKind"))
                {
                  str = namelist[0UL];
                }
            }
            break;
          default:
            break;
        }
    }
}

void
BE_GlobalData::type_change_diagnostic (DOMElement *node,
                                       const XMLCh *new_ref)
{
  if (0 == be_global->input_xme ())
    {
      return;
    }

  // If it hasn't been set yet, it can't have been changed.
  if (!node->hasAttribute (X ("referred")))
    {
      return;
    }

  const XMLCh *old_ref = node->getAttribute (X ("referred"));

  if (X (new_ref) == old_ref)
    {
      return;
    }

  DOMElement *old_type = this->doc_->getElementById (old_ref);
  DOMElement *new_type = this->doc_->getElementById (new_ref);
  DOMElement *parent = (DOMElement *) node->getParentNode ();

  char *old_type_name = this->get_name (old_type);
  char *new_type_name = this->get_name (new_type);
  char *child_name = this->get_name (node);
  char *parent_name = this->get_name (parent);
  char *changed_kind =
    XMLString::transcode (node->getAttribute (X ("kind")));
  char *parent_kind =
    XMLString::transcode (parent->getAttribute (X ("kind")));

  cout << "type of " << changed_kind << " "
       << (0 == ACE_OS::strcmp (child_name, changed_kind)
             ? "<no name>"
             : child_name)
       << " in " << parent_kind << " " << parent_name
       << " changed from " << old_type_name << " to "
       << new_type_name << endl;

  XMLString::release (&old_type_name);
  XMLString::release (&new_type_name);
  XMLString::release (&parent_name);
  XMLString::release (&child_name);
  XMLString::release (&changed_kind);
  XMLString::release (&parent_kind);
}

void
BE_GlobalData::base_component_diagnostic (DOMElement *elem,
                                          AST_Component *node,
                                          AST_Component *base,
                                          bool was_derived,
                                          const XMLCh *base_id_from_idl)
{
  if (0 == this->input_xme_)
    {
      return;
    }

  const char *node_name = node->local_name ()->get_string ();
  DOMElement *parent = (DOMElement *) elem->getParentNode ();
  char *parent_name = be_global->get_name (parent);
  const XMLCh *parent_kindX = parent->getAttribute (X ("kind"));
  char *parent_kind = XMLString::transcode (parent_kindX);

  const XMLCh *base_id = 0;
  DOMElement *base_elem = 0;
  char *base_name = 0;
  DOMElement *base_parent = 0;
  char *base_parent_name = 0;
  const XMLCh *base_parent_kindX = 0;
  char *base_parent_kind = 0;

  if (was_derived)
    {
      base_id = elem->getAttribute (X ("derivedfrom"));
      char *tmp = XMLString::transcode (base_id);
      base_elem = this->doc_->getElementById (base_id);
      base_name = this->get_name (base_elem);
      base_parent = (DOMElement *) base_elem->getParentNode ();
      base_parent_name = this->get_name (base_parent);
      base_parent_kindX = base_parent->getAttribute (X ("kind"));
      base_parent_kind = XMLString::transcode (base_parent_kindX);
    }

  if (0 == base && was_derived)
    {
      cout << "Component " << node_name << " in "
           << parent_kind << " " << parent_name
           << " is no longer a subtype of " << base_name
           << " in " << base_parent_kind << " "
           << base_parent_name << endl;

      XMLString::release (&base_name);
      XMLString::release (&base_parent_name);
      XMLString::release (&base_parent_kind);
      return;
    }

  DOMElement *new_base =
    this->doc_->getElementById (base_id_from_idl);
  DOMElement *new_base_parent =
    (DOMElement *) new_base->getParentNode ();
  const XMLCh *new_base_parent_kindX =
    new_base_parent->getAttribute (X ("kind"));
  char *new_base_parent_kind = XMLString::transcode (new_base_parent_kindX);
  char *new_base_parent_name = this->get_name (new_base_parent);
  char *new_base_name = this->get_name (new_base);

  if (was_derived)
    {
      if (X (base_id) != X (base_id_from_idl))
        {
          cout << "base type of Component " << node_name << " in "
               << parent_kind << " " << parent_name
               << " has changed from " << base_name  << " in "
               << base_parent_kind << " " << base_parent_name
               << " to " << new_base_name << " in "
               << new_base_parent_kind << " " << new_base_parent_name
               << endl;
        }
    }
  else
    {
      cout << "Component " << node_name << " in " << parent_kind
           << " " << parent_name << " is now a subtype of "
           << new_base_name << " in " << new_base_parent_kind
           << " " << new_base_parent_name << endl;
    }

  XMLString::release (&parent_name);
  XMLString::release (&parent_kind);
  XMLString::release (&base_name);
  XMLString::release (&base_parent_kind);
  XMLString::release (&base_parent_name);
  XMLString::release (&new_base_name);
}

void
BE_GlobalData::included_file_diagnostic (DOMElement *fileref,
                                         DOMElement *file,
                                         const char *fileref_name)
{
  if (0 == this->input_xme_)
    {
      return;
    }

  char *file_name = this->get_name (file);

  cout << "Added FileRef " << fileref_name << " in File "
       << file_name << endl;

  XMLString::release (&file_name);
}

DOMElement *
BE_GlobalData::get_first_picml_element (DOMElement *scope)
{
  DOMNodeList *children = scope->getChildNodes ();

  for (XMLSize_t i = 0; i < children->getLength (); ++i)
    {
      DOMElement *child = (DOMElement *) children->item (i);

      if (0 == child)
        {
          continue;
        }

      const XMLCh *tag = child->getTagName ();

      if (X ("model") == tag || X ("reference") == tag)
        {
          return child;
        }
    }

  return 0;
}

bool
BE_GlobalData::match_module_opening (DOMElement *elem, AST_Decl *node)
{
  DOMElement *elem_child = this->get_first_picml_element (elem);

  if (0 == elem_child)
    {
      // We're in a module that was just added, so we can't match
      // the node anyway.
      return false;
    }

  char *elem_child_name = this->get_name (elem_child);
  UTL_ScopeActiveIterator iter (DeclAsScope (node), UTL_Scope::IK_decls);
  AST_Decl *node_child = iter.item ();
  char *node_child_name = node_child->local_name ()->get_string ();
  int match = ACE_OS::strcmp (node_child_name, elem_child_name);
  XMLString::release (&elem_child_name);

  if (0 != match)
    {
      return false;
    }

  AST_Decl::NodeType nt = node_child->node_type ();
  const XMLCh *kind = elem_child->getAttribute (X ("kind"));

  if (X ("Package") == kind && AST_Decl::NT_module == nt)
    {
      return this->match_module_opening (elem_child, node_child);
    }
  else
    {
      return true;
    }
}