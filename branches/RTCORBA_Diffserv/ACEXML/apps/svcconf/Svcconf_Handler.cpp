// -*- C++ -*-  $Id$

#include "Svcconf_Handler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"
#include "ace/Service_Types.h"
#include "ace/Service_Repository.h"
#include "ace/DLL.h"
#include "ace/ARGV.h"
#include "ace/Module.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 0)

#if !defined (__ACEXML_INLINE__)
# include "Svcconf_Handler.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_Svcconf_Handler::ACEXML_Svcconf_Handler (void)
  : in_stream_def_ (0),
    in_module_ (0),
    stream_svc_type_ (0),
    stream_ (0),
    dll_handle_ (0)
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
                                    int,
                                    ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::endDocument (ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::endElement (const ACEXML_Char *,
                                    const ACEXML_Char *,
                                    const ACEXML_Char *qName,
                                    ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (ACE_OS_String::strcmp (qName, ACE_TEXT ("dynamic")) == 0)
    {
      ACE_Parsed_Info *active_info = (this->in_stream_def_ == 0 ?
                                      &this->parsed_info_ :
                                      &this->stream_info_);

      ACE_DLL svc_dll;

      if (svc_dll.open (active_info->path ()) == -1)
        {
          xmlenv.exception (new ACEXML_SAXException ("Cannot locate DLL\n"));
          return;
        }

      void *(*func) (ACE_Service_Object_Exterminator *) = 0;
      ACE_Service_Object_Exterminator gobbler = 0;
      void *symbol = 0;

      long temp_ptr =
        ACE_reinterpret_cast(long, svc_dll.symbol (active_info->init_func ()));
      func = ACE_reinterpret_cast(void *(*)(ACE_Service_Object_Exterminator *),
                                  temp_ptr);

      if (func == 0)
        {
          xmlenv.exception (new ACEXML_SAXException ("Cannot locator init function\n"));
          return;
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
              this->dll_handle_ = svc_dll.get_handle (1);
            }
          else
            {
              xmlenv.exception (new ACEXML_SAXException
                                ("Expecting Stream type in stream header\n"));
              return;
            }

          this->stream_svc_type_ =
            ACE_Service_Config::create_service_type (this->stream_info_.name (),
                                                     this->stream_,
                                                     this->dll_handle_,
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
                              ACE_LIB_TEXT ("warning: assigning Module_Type name %s to Module %s since names differ\n"),
                              active_info->name (),
                              mp->name ()));
                  mp->name (active_info->name ());
                }

              if (mt->init (args.argc (), args.argv ()) == -1
                  || this->stream_->push (mt) == -1)
                {
                  xmlenv.exception (new ACEXML_SAXException ("Error initializing module"));
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
                                                         svc_dll.get_handle (1),
                                                         active_info->active ());
              // @@ Check error here.

              //               ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply dynamic %s\n"),
//                           this->parsed_info_.name ()));
              if (ACE_Service_Config::initialize (stype,
                                                  active_info->init_params ()) == -1)
                {
                  xmlenv.exception (new ACEXML_SAXException
                                    ("Fail to initialize dynamic service\n"));
                  return;
                }
            }
          this->parsed_info_.reset ();
        }
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("static")) == 0)
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
                  xmlenv.exception (new ACEXML_SAXException
                                    (ACE_TEXT ("Fail initializing static service\n")));
                }
            }
          this->parsed_info_.reset ();
        }
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("module")) == 0)
    {
      this->in_module_ = 0;
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("streamdef")) == 0)
    {
//       ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply stream %s of type %s\n"),
//                   this->stream_info_.name (),
//                   this->stream_info_.name ()));
      ACE_Service_Config::initialize (this->stream_svc_type_,
                                      this->stream_info_.init_params ());

      this->stream_info_.reset ();
      this->stream_svc_type_ = 0;
      this->stream_ = 0;
      this->dll_handle_ = 0;
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("stream")) == 0)
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
ACEXML_Svcconf_Handler::endPrefixMapping (const ACEXML_Char *,
                                          ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::ignorableWhitespace (const ACEXML_Char *,
                                             int,
                                             int,
                                             ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::processingInstruction (const ACEXML_Char *,
                                               const ACEXML_Char *,
                                               ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::setDocumentLocator (ACEXML_Locator *,
                                            ACEXML_Env &)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::skippedEntity (const ACEXML_Char *,
                                       ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::startDocument (ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::startElement (const ACEXML_Char *,
                                      const ACEXML_Char *,
                                      const ACEXML_Char *qName,
                                      ACEXML_Attributes *alist,
                                      ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (ACE_OS_String::strcmp (qName, ACE_TEXT ("dynamic")) == 0)
    {
      this->get_dynamic_attrs (alist, xmlenv);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("initializer")) == 0)
    {
      this->get_initializer_attrs (alist, xmlenv);

    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("static")) == 0)
    {
      this->get_static_attrs (alist, xmlenv);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("stream")) == 0)
    {
      this->get_stream_id (alist, xmlenv);

      if (ACE_Service_Repository::instance()->find
          (this->stream_info_.name (),
           (const ACE_Service_Type **) &this->stream_svc_type_) == -1)
        {
          xmlenv.exception (new ACEXML_SAXException ("Can not find stream\n"));
          return;
        }
      this->stream_ =   this->stream_svc_type_ == 0
        ? 0
        : ACE_dynamic_cast (ACE_Stream_Type *,
                            ACE_const_cast (ACE_Service_Type_Impl *,
                                            this->stream_svc_type_->type ()));

//       ACE_DEBUG ((LM_INFO, ACE_TEXT ("Retrieve stream %s from repository\n"),
//                   this->stream_info_.name ()));
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("streamdef")) == 0)
    {
      this->in_stream_def_ = 1;
      // @@ Set up stream service object
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("module")) == 0)
    {
      this->in_stream_def_ = 0;
      this->in_module_ = 1;
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("resume")) == 0)
    {
      this->get_id (alist, xmlenv);
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
              xmlenv.exception (new ACEXML_SAXException ("Can't locate module\n"));
              return;
            }

          mt->resume ();
        }
      else
        {
//           ACE_DEBUG ((LM_INFO, ACE_TEXT ("Resume %s\n"),
//                       this->parsed_info_.name ()));
          if (ACE_Service_Config::resume (this->parsed_info_.name ()) == -1)
            {
              xmlenv.exception (new ACEXML_SAXException (ACE_TEXT ("Resume fail\n")));
            }
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("suspend")) == 0)
    {
      this->get_id (alist, xmlenv);
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
              xmlenv.exception (new ACEXML_SAXException ("Can't locate module\n"));
              return;
            }

          mt->suspend ();
        }
      else
        {
          //          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Suspend %s\n"),
          //                      this->parsed_info_.name ()));
          if (ACE_Service_Config::suspend (this->parsed_info_.name ()) == -1)
            {
              xmlenv.exception (new ACEXML_SAXException (ACE_TEXT ("Suspend fail\n")));
            }
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("remove")) == 0)
    {
      this->get_id (alist, xmlenv);
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
              xmlenv.exception (new ACEXML_SAXException ("Can't locate module\n"));
              return;
            }

          this->stream_->remove (mt);
        }
      else
        {
//           ACE_DEBUG ((LM_INFO, ACE_TEXT ("Remove %s\n"),
//                       this->parsed_info_.name ()));
          if (ACE_Service_Config::remove (this->parsed_info_.name ()) == -1)
            {
              xmlenv.exception (new ACEXML_SAXException (ACE_TEXT ("Remove fail\n")));
            }
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("ACE_Svc_Conf")) == 0)
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
                                            const ACEXML_Char *,
                                            ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// *** Methods inherit from ACEXML_DTDHandler.

void
ACEXML_Svcconf_Handler::notationDecl (const ACEXML_Char *,
                                      const ACEXML_Char *,
                                      const ACEXML_Char *,
                                      ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::unparsedEntityDecl (const ACEXML_Char *,
                                            const ACEXML_Char *,
                                            const ACEXML_Char *,
                                            const ACEXML_Char *,
                                            ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_Svcconf_Handler::resolveEntity (const ACEXML_Char *,
                                       const ACEXML_Char *,
                                       ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
  return 0;
}

// Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
void
ACEXML_Svcconf_Handler::error (ACEXML_SAXParseException &,
                               ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::fatalError (ACEXML_SAXParseException &,
                                    ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::warning (ACEXML_SAXParseException &,
                                 ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

int
ACEXML_Svcconf_Handler::get_stream_id (ACEXML_Attributes *alist,
                                       ACEXML_Env &xmlenv)
{
  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
          {
            this->stream_info_.name (alist->getValue (i));
          }
        else
          {
            xmlenv.exception (new ACEXML_SAXException ("Invalid stream attribute\n"));
            return -1;
          }
      }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_id (ACEXML_Attributes *alist,
                                ACEXML_Env &xmlenv)
{
  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
          {
            this->parsed_info_.name (alist->getValue (i));
          }
        else
          {
            xmlenv.exception (new ACEXML_SAXException ("Invalid attribute, expecting 'id'\n"));
            return -1;
          }
      }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_dynamic_attrs (ACEXML_Attributes *alist,
                                           ACEXML_Env &xmlenv)
{
  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
            {
              info->name (alist->getValue (i));
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("status")) == 0)
            {
              if (ACE_OS_String::strcmp (alist->getValue (i), ACE_TEXT ("inactive")) == 0)
                {
                  info->active (0);
                }
              else if (ACE_OS_String::strcmp (alist->getValue (i), ACE_TEXT ("active")) == 0)
                {
                  info->active (1);
                }
              else
                {
                  xmlenv.exception
                    (new ACEXML_SAXException ("Invalid attribute value, expecting 'active' or 'inactive'\n"));
                  return -1;
                }
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("type")) == 0)
            {
              if (ACE_OS_String::strcasecmp (alist->getValue (i), ACE_TEXT ("service_object")) == 0)
                {
                  info->service_type (ACE_Service_Type::SERVICE_OBJECT);
                }
              else if (ACE_OS_String::strcasecmp (alist->getValue (i), ACE_TEXT ("stream")) == 0)
                {
                  info->service_type (ACE_Service_Type::STREAM);
                }
              else if (ACE_OS_String::strcasecmp (alist->getValue (i), ACE_TEXT ("module")) == 0)
                {
                  info->service_type (ACE_Service_Type::MODULE);
                }
              else
                {
                  xmlenv.exception
                    (new ACEXML_SAXException ("Invalid Service_Object attribute value\n"));
                  return -1;
                }
            }
          else
            {
              xmlenv.exception (new ACEXML_SAXException ("Invalid attribute\n"));
              return -1;
            }
        }
    }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_initializer_attrs (ACEXML_Attributes *alist,
                                               ACEXML_Env &xmlenv)
{
  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("init")) == 0)
            {
              info->init_func (alist->getValue (i));
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("path")) == 0)
            {
              info->path (alist->getValue (i));
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("params")) == 0)
            {
              info->init_params (alist->getValue (i));
            }
          else
            {
              xmlenv.exception (new ACEXML_SAXException ("Invalid initializer attribute.\n"));
              return -1;
            }
        }
    }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_static_attrs (ACEXML_Attributes *alist,
                                          ACEXML_Env &xmlenv)
{
  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
            {
              info->name (alist->getValue (i));
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("params")) == 0)
            {
              info->init_params (alist->getValue (i));
            }
          else
            {
              xmlenv.exception (new ACEXML_SAXException ("Invalid static attribute.\n"));
              return -1;
            }
        }
    }
  return 0;
}

#endif /* ACE_USES_CLASSIC_SVC_CONF == 0 */
