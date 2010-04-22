// $Id$

#include "Svcconf_Handler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"
#include "ace/Service_Types.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Gestalt.h"
#include "ace/DLL.h"
#include "ace/ARGV.h"
#include "ace/Module.h"
#include "ace/OS_NS_strings.h"
#include "ace/SString.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 0)

#if !defined (__ACEXML_INLINE__)
# include "Svcconf_Handler.inl"
#endif /* __ACEXML_INLINE__ */

ACEXML_Svcconf_Handler::ACEXML_Svcconf_Handler (void)
  : in_stream_def_ (0),
    in_module_ (0),
    stream_svc_type_ (0),
    stream_ (0)
{
  // no-op
}

ACEXML_Svcconf_Handler::~ACEXML_Svcconf_Handler (void)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::characters (const ACEXML_Char *,
                                    int,
                                    int ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::endDocument ( ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::endElement (const ACEXML_Char *,
                                    const ACEXML_Char *,
                                    const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
{
  if (ACE_OS::strcmp (qName, ACE_TEXT ("dynamic")) == 0)
    {
      ACE_Parsed_Info *active_info = (this->in_stream_def_ == 0 ?
                                      &this->parsed_info_ :
                                      &this->stream_info_);

      // We must allocate a string here to ensure that the
      // name is still available by the time the
      // ACE_Service_Type_Dynamic_Guard is destructed.
      ACE_TString name = active_info->name ();
      ACE_Service_Type_Dynamic_Guard dummy (
        *ACE_Service_Config::current ()->current_service_repository (),
        name.c_str ());
      ACE_DLL svc_dll;

      if (svc_dll.open (active_info->path ()) == -1)
        {
          // build the error message
          ACE_CString msg (ACE_TEXT ("Cannot locate DLL: '"));
          msg += ACE_CString (ACE_TEXT (active_info->path ()));
          msg += ACE_CString (ACE_TEXT ("' for entity '"));
          msg += ACE_CString (ACE_TEXT (active_info->name ()));
          msg += ACE_CString (ACE_TEXT ("'\n"));

          ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
        }

      void *(*func) (ACE_Service_Object_Exterminator *) = 0;
      ACE_Service_Object_Exterminator gobbler = 0;
      void *symbol = 0;

      long temp_ptr =
        reinterpret_cast<long> (svc_dll.symbol (active_info->init_func ()));
      func = reinterpret_cast<void *(*)(ACE_Service_Object_Exterminator *)> (temp_ptr);

      if (func == 0)
        {
          // build the error message
          ACE_CString msg (ACE_TEXT ("Cannot locate init function: '"));
          msg += ACE_CString (ACE_TEXT (active_info->init_func ()));
          msg += ACE_CString (ACE_TEXT ("' for entity '"));
          msg += ACE_CString (ACE_TEXT (active_info->name ()));
          msg += ACE_CString (ACE_TEXT ("'\n"));

          ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
        }
      symbol = (*func)(&gobbler); // target object created in the loaded DLL.

      ACE_Service_Type_Impl *stp = ACE_Service_Config::create_service_type_impl
        (active_info->name (),
         active_info->service_type (),
         symbol,
         ACE_Service_Type::DELETE_THIS |
         ACE_Service_Type::DELETE_OBJ,
         gobbler);

      if (this->in_stream_def_)
        {
//           ACE_DEBUG ((LM_INFO, ACE_TEXT ("Create dynamic %s for stream\n"),
//                       this->stream_info_.name ()));
          if (active_info->service_type () == ACE_Service_Type::STREAM)
            {
              this->stream_ = (ACE_Stream_Type *) stp;
            }
          else
            {
              // We will not retain this stream
              delete stp;

              // build the error message
              ACE_CString msg (ACE_TEXT ("Expecting Stream type in stream header"));
              msg += ACE_CString (ACE_TEXT (" for entity '"));
              msg += ACE_CString (ACE_TEXT (active_info->name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
            }

          this->stream_svc_type_ =
            ACE_Service_Config::create_service_type (this->stream_info_.name (),
                                                     this->stream_,
                                                     svc_dll,
                                                     this->stream_info_.active ());

        }
      else
        {
          if (this->in_module_)
            {
              ACE_ARGV args (active_info->init_params ());

              ACE_Module_Type *mt = (ACE_Module_Type *) stp;

              ACE_Module<ACE_SYNCH> *mp = (ACE_Module<ACE_SYNCH> *) mt->object ();

              if (ACE_OS::strcmp (mp->name (), active_info->name ()) != 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("warning: assigning Module_Type name %s to Module %s since names differ\n"),
                              active_info->name (),
                              mp->name ()));
                  mp->name (active_info->name ());
                }

              if (mt->init (args.argc (), args.argv ()) == -1
                  || this->stream_->push (mt) == -1)
                {
                  // build the error message
                  ACE_CString msg (ACE_TEXT ("Error initializing module"));
                  msg += ACE_CString (ACE_TEXT (" for entity '"));
                  msg += ACE_CString (ACE_TEXT (active_info->name ()));
                  msg += ACE_CString (ACE_TEXT ("'\n"));

                  ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
                }


//               ACE_DEBUG ((LM_INFO, ACE_TEXT ("Push dynamic %s into stream %s\n"),
//                           this->parsed_info_.name (),
//                           this->stream_info_.name ()));
            }
          else
            {
              ACE_Service_Type *stype =
                ACE_Service_Config::create_service_type (active_info->name (),
                                                         stp,
                                                         svc_dll,
                                                         active_info->active ());
              // @@ Check error here.

              //               ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply dynamic %s\n"),
//                           this->parsed_info_.name ()));
              if (ACE_Service_Config::initialize (stype,
                                                  active_info->init_params ()) == -1)
                {
                  // If it did not initialize correctly, the
                  // ACE_Service_Config doesn't own this object
                  delete stype;

                  // build the error message
                  ACE_CString msg (ACE_TEXT ("Failed to initialize dynamic service"));
                  msg += ACE_CString (ACE_TEXT (" for entity '"));
                  msg += ACE_CString (ACE_TEXT (active_info->name ()));
                  msg += ACE_CString (ACE_TEXT ("'\n"));

                  ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
                }
            }
          this->parsed_info_.reset ();
        }
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("static")) == 0)
    {
      if (this->in_stream_def_)
        {
          // @@ Couldn't make sense out of the original Svc_Conf.y.
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Create static %s for stream\n"),
                      this->stream_info_.name ()));
        }
      else
        {
          if (this->in_module_)
            {
              // @@ Couldn't make sense out of the original Svc_Conf.y.
              ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Push static %s into stream %s\n"),
                          this->parsed_info_.name (),
                          this->stream_info_.name ()));
            }
          else
            {
//               ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply static %s\n"),
//                           this->parsed_info_.name ()));
              if (ACE_Service_Config::initialize (this->parsed_info_.name (),
                                                  this->parsed_info_.init_params ()) == -1)
                {
                  // build the error message
                  ACE_CString msg (ACE_TEXT ("Failed to initialize static service"));
                  msg += ACE_CString (ACE_TEXT (" for entity '"));
                  msg += ACE_CString (ACE_TEXT (this->parsed_info_.name ()));
                  msg += ACE_CString (ACE_TEXT ("'\n"));

                  ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
                }
            }
          this->parsed_info_.reset ();
        }
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("module")) == 0)
    {
      this->in_module_ = 0;
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("streamdef")) == 0)
    {
//       ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply stream %s of type %s\n"),
//                   this->stream_info_.name (),
//                   this->stream_info_.name ()));
      ACE_Service_Config::initialize (this->stream_svc_type_,
                                      this->stream_info_.init_params ());

      this->stream_info_.reset ();
      this->stream_svc_type_ = 0;
      this->stream_ = 0;
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("stream")) == 0)
    {
//       ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply stream %s of type %s\n"),
//                   this->stream_info_.name (),
//                   this->stream_info_.name ()));
      this->stream_info_.reset ();
    }
  else
    {
    }
}

void
ACEXML_Svcconf_Handler::endPrefixMapping (const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::ignorableWhitespace (const ACEXML_Char *,
                                             int,
                                             int ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::processingInstruction (const ACEXML_Char *,
                                               const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::setDocumentLocator (ACEXML_Locator* locator)
{
  this->locator_ = locator;
}

void
ACEXML_Svcconf_Handler::skippedEntity (const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::startDocument ( ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::startElement (const ACEXML_Char *,
                                      const ACEXML_Char *,
                                      const ACEXML_Char *qName,
                                      ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{
  if (ACE_OS::strcmp (qName, ACE_TEXT ("dynamic")) == 0)
    {
      this->get_dynamic_attrs (alist ACEXML_ENV_ARG_PARAMETER);
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("initializer")) == 0)
    {
      this->get_initializer_attrs (alist ACEXML_ENV_ARG_PARAMETER);

    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("static")) == 0)
    {
      this->get_static_attrs (alist ACEXML_ENV_ARG_PARAMETER);
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("stream")) == 0)
    {
      this->get_stream_id (alist ACEXML_ENV_ARG_PARAMETER);

      if (ACE_Service_Repository::instance()->find
          (this->stream_info_.name (),
           (const ACE_Service_Type **) &this->stream_svc_type_) == -1)
        {
          // build the error message
          ACE_CString msg (ACE_TEXT ("Cannot find stream '"));
          msg += ACE_CString (ACE_TEXT (this->stream_info_.name ()));
          msg += ACE_CString (ACE_TEXT ("'\n"));

          ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
        }
      this->stream_ =   this->stream_svc_type_ == 0
        ? 0
        : dynamic_cast<ACE_Stream_Type *> (const_cast<ACE_Service_Type_Impl *> (this->stream_svc_type_->type ()));

//       ACE_DEBUG ((LM_INFO, ACE_TEXT ("Retrieve stream %s from repository\n"),
//                   this->stream_info_.name ()));
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("streamdef")) == 0)
    {
      this->in_stream_def_ = 1;
      // @@ Set up stream service object
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("module")) == 0)
    {
      this->in_stream_def_ = 0;
      this->in_module_ = 1;
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("resume")) == 0)
    {
      this->get_id (alist ACEXML_ENV_ARG_PARAMETER);
      if (this->in_module_)
        {
//           ACE_DEBUG ((LM_INFO, ACE_TEXT ("Resume %s in stream %s\n"),
//                       this->parsed_info_.name (),
//                       this->stream_info_.name ()));
          ACE_Module_Type *mt = (this->stream_ == 0)
            ? 0
            : this->stream_->find (this->parsed_info_.name ());

          if (mt == 0)
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Can't locate module '"));
              msg += ACE_CString (ACE_TEXT (this->parsed_info_.name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
            }

          mt->resume ();
        }
      else
        {
//           ACE_DEBUG ((LM_INFO, ACE_TEXT ("Resume %s\n"),
//                       this->parsed_info_.name ()));
          if (ACE_Service_Config::resume (this->parsed_info_.name ()) == -1)
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Resume failed"));
              msg += ACE_CString (ACE_TEXT (" for entity '"));
              msg += ACE_CString (ACE_TEXT (this->parsed_info_.name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
            }
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("suspend")) == 0)
    {
      this->get_id (alist ACEXML_ENV_ARG_PARAMETER);
      if (this->in_module_)
        {
//           ACE_DEBUG ((LM_INFO, ACE_TEXT ("Suspend %s in stream %s\n"),
//                       this->parsed_info_.name (),
//                       this->stream_info_.name ()));
          ACE_Module_Type *mt = (this->stream_ == 0)
            ? 0
            : this->stream_->find (this->parsed_info_.name ());

          if (mt == 0)
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Can't locate module '"));
              msg += ACE_CString (ACE_TEXT (this->parsed_info_.name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
            }

          mt->suspend ();
        }
      else
        {
          //          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Suspend %s\n"),
          //                      this->parsed_info_.name ()));
          if (ACE_Service_Config::suspend (this->parsed_info_.name ()) == -1)
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Suspend failed"));
              msg += ACE_CString (ACE_TEXT (" for entity '"));
              msg += ACE_CString (ACE_TEXT (this->parsed_info_.name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
            }
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("remove")) == 0)
    {
      this->get_id (alist ACEXML_ENV_ARG_PARAMETER);
      if (this->in_module_)
        {
//           ACE_DEBUG ((LM_INFO, ACE_TEXT ("Remove %s in stream %s\n"),
//                       this->parsed_info_.name (),
//                       this->stream_info_.name ()));
          ACE_Module_Type *mt = (this->stream_ == 0)
            ? 0
            : this->stream_->find (this->parsed_info_.name ());

          if (mt == 0)
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Can't locate module '"));
              msg += ACE_CString (ACE_TEXT (this->parsed_info_.name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
            }

          this->stream_->remove (mt);
        }
      else
        {
//           ACE_DEBUG ((LM_INFO, ACE_TEXT ("Remove %s\n"),
//                       this->parsed_info_.name ()));
          if (ACE_Service_Config::remove (this->parsed_info_.name ()) == -1)
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Remove failed"));
              msg += ACE_CString (ACE_TEXT (" for entity '"));
              msg += ACE_CString (ACE_TEXT (this->parsed_info_.name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW (ACEXML_SAXException (msg.c_str ()));
            }
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS::strcmp (qName, ACE_TEXT ("ACE_Svc_Conf")) == 0)
    {
      // Main document tag. no-op.
//       ACE_DEBUG ((LM_INFO, ACE_TEXT ("ACE_Svc_Conf tag\n")));
    }
  else
    {
      // @@ Error.  Perhaps we should relay to user event handler here, if available.
    }

  return;
}

void
ACEXML_Svcconf_Handler::startPrefixMapping (const ACEXML_Char *,
                                            const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // No-op.
}

// *** Methods inherit from ACEXML_DTDHandler.

void
ACEXML_Svcconf_Handler::notationDecl (const ACEXML_Char *,
                                      const ACEXML_Char *,
                                      const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::unparsedEntityDecl (const ACEXML_Char *,
                                            const ACEXML_Char *,
                                            const ACEXML_Char *,
                                            const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // No-op.
}

// Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_Svcconf_Handler::resolveEntity (const ACEXML_Char *,
                                       const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
{
  // No-op.
  return 0;
}

// Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
void
ACEXML_Svcconf_Handler::error (ACEXML_SAXParseException& ex ACEXML_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line :%d col: %d ", this->locator_->getSystemId(),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_Svcconf_Handler::fatalError (ACEXML_SAXParseException& ex ACEXML_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line :%d col: %d ", this->locator_->getSystemId(),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_Svcconf_Handler::warning (ACEXML_SAXParseException& ex ACEXML_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_DEBUG, "%s: line :%d col: %d ", this->locator_->getSystemId(),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

int
ACEXML_Svcconf_Handler::get_stream_id (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{
  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
          {
            this->stream_info_.name (alist->getValue (i));
          }
        else
          {
            // build the error message
            ACE_CString msg (ACE_TEXT ("Invalid stream attribute '"));
            msg += ACE_CString (ACE_TEXT (alist->getQName (i)));
            msg += ACE_CString (ACE_TEXT ("' for entity '"));
            msg += ACE_CString (ACE_TEXT (this->stream_info_.name ()));
            msg += ACE_CString (ACE_TEXT ("'\n"));

            ACEXML_THROW_RETURN (ACEXML_SAXException (msg.c_str ()),
                                 -1);
          }
      }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_id (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{
  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
          {
            this->parsed_info_.name (alist->getValue (i));
          }
        else
          {
            // build the error message
            ACE_CString msg (ACE_TEXT ("Invalid attribute '"));
            msg += ACE_CString (ACE_TEXT (alist->getQName (i)));
            msg += ACE_CString (ACE_TEXT ("', expecting 'id'"));
            msg += ACE_CString (ACE_TEXT (" for entity '"));
            msg += ACE_CString (ACE_TEXT (this->parsed_info_.name ()));
            msg += ACE_CString (ACE_TEXT ("'\n"));

            ACEXML_THROW_RETURN (ACEXML_SAXException (msg.c_str ()),
                            -1);
          }
      }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_dynamic_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{
  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
            {
              info->name (alist->getValue (i));
            }
          else if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("status")) == 0)
            {
              if (ACE_OS::strcmp (alist->getValue (i), ACE_TEXT ("inactive")) == 0)
                {
                  info->active (0);
                }
              else if (ACE_OS::strcmp (alist->getValue (i), ACE_TEXT ("active")) == 0)
                {
                  info->active (1);
                }
              else
                {
                  // build the error message
                  ACE_CString msg (ACE_TEXT ("Invalid attribute value '"));
                  msg += ACE_CString (ACE_TEXT (alist->getQName (i)));
                  msg += ACE_CString (ACE_TEXT ("', expecting 'active' or 'inactive'"));
                  msg += ACE_CString (ACE_TEXT (" for entity '"));
                  msg += ACE_CString (ACE_TEXT (info->name ()));
                  msg += ACE_CString (ACE_TEXT ("'\n"));

                  ACEXML_THROW_RETURN (ACEXML_SAXException (msg.c_str ()),
                                       -1);
                }
            }
          else if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("type")) == 0)
            {
              if (ACE_OS::strcasecmp (alist->getValue (i), ACE_TEXT ("service_object")) == 0)
                {
                  info->service_type (ACE_Service_Type::SERVICE_OBJECT);
                }
              else if (ACE_OS::strcasecmp (alist->getValue (i), ACE_TEXT ("stream")) == 0)
                {
                  info->service_type (ACE_Service_Type::STREAM);
                }
              else if (ACE_OS::strcasecmp (alist->getValue (i), ACE_TEXT ("module")) == 0)
                {
                  info->service_type (ACE_Service_Type::MODULE);
                }
              else
                {
                  // build the error message
                  ACE_CString msg (ACE_TEXT ("Invalid Service_Object attribute value'"));
                  msg += ACE_CString (ACE_TEXT (alist->getQName (i)));
                  msg += ACE_CString (ACE_TEXT ("' for entity '"));
                  msg += ACE_CString (ACE_TEXT (info->name ()));
                  msg += ACE_CString (ACE_TEXT ("'\n"));

                  ACEXML_THROW_RETURN (ACEXML_SAXException (msg.c_str ()),
                                  -1);
                }
            }
          else
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Invalid attribute'"));
              msg += ACE_CString (ACE_TEXT (alist->getQName (i)));
              msg += ACE_CString (ACE_TEXT ("' for entity '"));
              msg += ACE_CString (ACE_TEXT (info->name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW_RETURN (ACEXML_SAXException (msg.c_str ()), -1);
            }
        }
    }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_initializer_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{
  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("init")) == 0)
            {
              info->init_func (alist->getValue (i));
            }
          else if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("path")) == 0)
            {
              info->path (alist->getValue (i));
            }
          else if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("params")) == 0)
            {
              info->init_params (alist->getValue (i));
            }
          else
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Invalid initializer attribute'"));
              msg += ACE_CString (ACE_TEXT (alist->getQName (i)));
              msg += ACE_CString (ACE_TEXT ("' for entity '"));
              msg += ACE_CString (ACE_TEXT (info->name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW_RETURN (ACEXML_SAXException (msg.c_str ()), -1);
            }
        }
    }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_static_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{
  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
            {
              info->name (alist->getValue (i));
            }
          else if (ACE_OS::strcmp (alist->getQName (i), ACE_TEXT ("params")) == 0)
            {
              info->init_params (alist->getValue (i));
            }
          else
            {
              // build the error message
              ACE_CString msg (ACE_TEXT ("Invalid static attribute '"));
              msg += ACE_CString (ACE_TEXT (alist->getQName (i)));
              msg += ACE_CString (ACE_TEXT ("' for entity '"));
              msg += ACE_CString (ACE_TEXT (info->name ()));
              msg += ACE_CString (ACE_TEXT ("'\n"));

              ACEXML_THROW_RETURN (ACEXML_SAXException (msg.c_str ()), -1);
            }
        }
    }
  return 0;
}

#endif /* ACE_USES_CLASSIC_SVC_CONF == 0 */
