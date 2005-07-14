// $Id$

#include "tao/TAO_Server_Request.h"
#include "tao/operation_details.h"
#include "tao/Transport.h"
#include "tao/Profile.h"
#include "tao/SystemException.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"
#include "tao/Resource_Factory.h"

#include "Codeset_Manager_i.h"
#include "Codeset_Translator_Factory.h"

#include "ace/Dynamic_Service.h"
#include "ace/Codeset_Registry.h"
#include "ace/OS_NS_string.h"
#include "ace/Service_Config.h"

ACE_RCSID (tao,
           Codeset_Manager_i,
           "$Id$")


// These numbers are assigned by the OpenGroup, a database is
// available at
//
// ftp://ftp.opengroup.org/pub/code_set_registry/
//
#define TAO_CODESET_ID_ISO8859_1 0x00010001U
#define TAO_CODESET_ID_UNICODE   0x00010109U
#define TAO_CODESET_ID_XOPEN_UTF_8 0x05010001U

// These are the default codesets that TAO declares, of course they
// will be different on each platform, once the complete support for
// character sets is implemented

#if (defined TAO_DEFAULT_CHAR_CODESET_ID)
# undef TAO_DEFAULT_CHAR_CODESET_ID
#endif /* defined TAO_DEFAULT_CHAR_CODESET_ID */

#if (defined TAO_DEFAULT_WCHAR_CODESET_ID)
# undef TAO_DEFAULT_WCHAR_CODESET_ID
#endif /* defined TAO_DEFAULT_WCHAR_CODESET_ID */

//#define TAO_DEFAULT_CHAR_CODESET_ID  TAO_CODESET_ID_XOPEN_UTF_8
#define TAO_DEFAULT_CHAR_CODESET_ID  TAO_CODESET_ID_ISO8859_1
#define TAO_DEFAULT_WCHAR_CODESET_ID TAO_CODESET_ID_UNICODE

// ****************************************************************

/// NCS for char is defaulted to ISO 8859-1:1987; Latin Alphabet No. 1
CONV_FRAME::CodeSetId
TAO_Codeset_Manager_i::default_char_codeset = TAO_DEFAULT_CHAR_CODESET_ID;
/// NCS for wchar is not defaulted by the CORBA specification, but a default
/// may be set here if desired
CONV_FRAME::CodeSetId
TAO_Codeset_Manager_i::default_wchar_codeset = TAO_DEFAULT_WCHAR_CODESET_ID;

TAO_Codeset_Manager_i::TAO_Codeset_Manager_i ()
  : codeset_info_ (),
    char_factories_ (),
    wchar_factories_ ()
{
  this->codeset_info_.ForCharData.native_code_set =
    TAO_Codeset_Manager_i::default_char_codeset;
  this->codeset_info_.ForWcharData.native_code_set =
    TAO_Codeset_Manager_i::default_wchar_codeset;

  this->add_char_translator ("UTF8_Latin1_Factory");
  this->add_wchar_translator ("UTF16_BOM_Factory");
}

TAO_Codeset_Manager_i::~TAO_Codeset_Manager_i ()
{
  // Cleanup the character map
  TAO_CodesetFactorySetItor cf_end = this->char_factories_.end ();
  TAO_CodesetFactorySetItor cf_iter = this->char_factories_.begin ();

  for (; cf_iter != cf_end; ++cf_iter)
    {
      delete *cf_iter;
    }

  this->char_factories_.reset ();

  // Cleanup the wide character map
  cf_end = this->wchar_factories_.end ();
  cf_iter = this->wchar_factories_.begin ();

  for (;cf_iter != cf_end; ++cf_iter)
    {
      delete *cf_iter;
    }

  this->wchar_factories_.reset ();
}

void
TAO_Codeset_Manager_i::set_codeset (TAO_Tagged_Components& tc) const
{
  tc.set_code_sets (this->codeset_info_);
}

void
TAO_Codeset_Manager_i::set_tcs (TAO_Profile &theProfile,
                              TAO_Transport &trans)
{
  /// If tcs is already set on the transport then donot process,
  /// use existing transport as CDR have translators set.
  TAO_Tagged_Components& theTaggedComp = theProfile.tagged_components ();

  CONV_FRAME::CodeSetComponentInfo remote;

  /// Get the codeset component
  if (theTaggedComp.get_code_sets(remote) == 0 )
    {
      if (trans.is_tcs_set ())
        {
          if(TAO_debug_level > 2)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::set_tcs, ")
                        ACE_TEXT ("transport already set\n")));
          return;
        }
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::set_tcs, ")
                    ACE_TEXT ("No codeset component in profile\n")));

  // These are the "fallback" codeset ids for use if no context is
  // available
       remote.ForCharData.native_code_set =
         TAO_CODESET_ID_XOPEN_UTF_8;
       remote.ForWcharData.native_code_set =
         TAO_CODESET_ID_UNICODE;

       trans.char_translator
         (this->get_char_trans
          (TAO_Codeset_Manager_i::default_char_codeset));
       trans.wchar_translator
         (this->get_wchar_trans
          (TAO_Codeset_Manager_i::default_wchar_codeset));
    }
   else
     {
       CONV_FRAME::CodeSetId tcs =
         computeTCS (remote.ForCharData,
                     this->codeset_info_.ForCharData);
       if (TAO_debug_level > 2)
         ACE_DEBUG ((LM_DEBUG,
                     ACE_TEXT("TAO (%P|%t) - Codeset_Manager_i::set_tcs, ")
                     ACE_TEXT("setting char translator (%08x)\n"),
                     tcs));
       trans.char_translator(this->get_char_trans (tcs));

       tcs = computeTCS (remote.ForWcharData,
                         this->codeset_info_.ForWcharData);

       if (TAO_debug_level > 2)
         ACE_DEBUG ((LM_DEBUG,
                     ACE_TEXT("TAO (%P|%t) - Codeset_Manager_i::set_tcs, ")
                     ACE_TEXT("setting wchar translator (%08x)\n"),
                     tcs));
       trans.wchar_translator(this->get_wchar_trans (tcs));
     }
}

void
TAO_Codeset_Manager_i::process_service_context (TAO_ServerRequest &request)
{
  // Get the service Context from an object of TAO_ServerRequest
  // and set the TCS values on the Transport
  TAO_Service_Context &service_cntx = request.request_service_context ();
  IOP::ServiceContext context;
  context.context_id = IOP::CodeSets;

  // These are the "fallback" codeset ids for use if no context is
  // available
  CONV_FRAME::CodeSetId tcs_c = TAO_CODESET_ID_XOPEN_UTF_8;
  CONV_FRAME::CodeSetId tcs_w = TAO_CODESET_ID_UNICODE;

  if (service_cntx.get_context(context))
    {
      // Convert the Service Context to Codeset Context
      const char *buffer =
        reinterpret_cast<const char*> (context.context_data.get_buffer ());

      TAO_InputCDR cdr (buffer,context.context_data.length ());
      CORBA::Boolean byte_order;

      if (cdr >> TAO_InputCDR::to_boolean (byte_order))
        {
          cdr.reset_byte_order (static_cast<int> (byte_order));
          cdr >> tcs_c;
          cdr >> tcs_w;
        }
    }
  else
    {
      if (request.transport()->is_tcs_set())
        return;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - Codeset_Manager_i::process_service_context ")
                    ACE_TEXT("no codeset context in request, inferring TAO backwards compatibility\n")));
    }
  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                  ACE_TEXT ("process_service_context, ")
                  ACE_TEXT ("using tcsc = %08x, tcsw = %08x\n"),
                  tcs_c,tcs_w));
    }

  request.transport()->char_translator(this->get_char_trans (tcs_c));
  request.transport()->wchar_translator(this->get_wchar_trans (tcs_w));
}

void
TAO_Codeset_Manager_i::generate_service_context (TAO_Operation_Details &opd,
                                               TAO_Transport &trans)
{
  TAO_Service_Context &service_cntx = opd.request_service_context ();
  CONV_FRAME::CodeSetContext codeset_cntx;

  // Generating codeset context
  // Assuming the TCS values from Transport will be defaulted
  TAO_Codeset_Translator_Factory *tf =
    dynamic_cast<TAO_Codeset_Translator_Factory*>(trans.char_translator());

  codeset_cntx.char_data =
    tf ? tf->tcs () : this->codeset_info_.ForCharData.native_code_set;

  tf =
    dynamic_cast<TAO_Codeset_Translator_Factory*>(trans.wchar_translator());

  codeset_cntx.wchar_data =
    tf ? tf->tcs () : this->codeset_info_.ForWcharData.native_code_set;

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                  ACE_TEXT ("generate_service_context, ")
                  ACE_TEXT ("using tcs_c = %08x, tcs_w = %08x\n"),
                  codeset_cntx.char_data,
                  codeset_cntx.wchar_data));
    }

  TAO_OutputCDR codeset_cdr;
  codeset_cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  codeset_cdr << codeset_cntx;

  service_cntx.set_context (IOP::CodeSets,codeset_cdr);
}

/// Checks whether the NCS is a part of CCS
int
TAO_Codeset_Manager_i::isElementOf (CONV_FRAME::CodeSetId id,
                                  CONV_FRAME::CodeSetComponent &cs_comp)
{
  for (CORBA::ULong i = 0L;
       i < cs_comp.conversion_code_sets.length ();
       ++i )
    {
      if (id == cs_comp.conversion_code_sets[i])
        return 1;
    }

  return 0;
}

/// Find the Intersection of Client and Server CCS's
CONV_FRAME::CodeSetId
TAO_Codeset_Manager_i::intersectionOf (CONV_FRAME::CodeSetComponent &cs_comp1,
                                     CONV_FRAME::CodeSetComponent &cs_comp2)
{
  for(CORBA::ULong index = 0L;
       index < cs_comp1.conversion_code_sets.length();
       ++index )
    {
      if (this->isElementOf(cs_comp1.conversion_code_sets[index], cs_comp2))
        {
          return cs_comp1.conversion_code_sets[index];
        }
    }

  return 0;
}

int
TAO_Codeset_Manager_i::isCompatible(CONV_FRAME::CodeSetId cs1,
                                  CONV_FRAME::CodeSetId cs2 )
{
  // Call the is_compatible method of ACE_Codeset_Registry
  return ACE_Codeset_Registry::is_compatible(cs1,cs2);
}

/// returns the TCS for Char / Wchar
CONV_FRAME::CodeSetId
TAO_Codeset_Manager_i::computeTCS (CONV_FRAME::CodeSetComponent &remote,
                                 CONV_FRAME::CodeSetComponent &local )
{
  if (remote.native_code_set == local.native_code_set)
    {
      return local.native_code_set;
    }

  if (this->isElementOf (remote.native_code_set, local))
    {
      return remote.native_code_set;
    }

  if (this->isElementOf (local.native_code_set, remote))
    {
      return local.native_code_set;
    }

  CONV_FRAME::CodeSetId tcs;

  if ((tcs = this->intersectionOf (remote, local)) == 0)
    {
      if (isCompatible (local.native_code_set, remote.native_code_set))
        {
          return remote.native_code_set;
        }
      else
        {
          ACE_DECLARE_NEW_CORBA_ENV;
          ACE_THROW_RETURN(CORBA::CODESET_INCOMPATIBLE (), 0);
        }
    }

  return tcs;
}

void
TAO_Codeset_Manager_i::set_ncs_c (CONV_FRAME::CodeSetId ncs)
{
  this->codeset_info_.ForCharData.native_code_set = ncs;
}

void
TAO_Codeset_Manager_i::set_ncs_w (CONV_FRAME::CodeSetId ncs,int mb)
{
  this->codeset_info_.ForWcharData.native_code_set = ncs;
  ACE_OutputCDR::wchar_maxbytes(mb);
}

int
TAO_Codeset_Manager_i::add_char_translator (const char *name)
{
  TAO_Codeset_Item *item = 0;
  ACE_NEW_RETURN (item, TAO_Codeset_Item (name), -1);

  if (this->char_factories_.insert (item) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                         ACE_TEXT ("add_char_translator, ")
                         ACE_TEXT ("Unable to add Codeset ")
                         ACE_TEXT ("factories for %s: %m\n"),
                         ACE_TEXT_CHAR_TO_TCHAR (name)),
                        -1);
    }

  return 0;
}

int
TAO_Codeset_Manager_i::add_wchar_translator (const char *name)
{
  TAO_Codeset_Item *item = 0;
  ACE_NEW_RETURN (item, TAO_Codeset_Item (name), -1);

  if (this->wchar_factories_.insert (item) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                         ACE_TEXT ("add_wchar_translator, ")
                         ACE_TEXT ("Unable to add Codeset ")
                         ACE_TEXT ("factories for %s: %m\n"),
                         ACE_TEXT_CHAR_TO_TCHAR (name)),
                        -1);
    }

  return 0;
}

void
TAO_Codeset_Manager_i::open(void)
{
  // These translators help comply with the CORBA 3.0.2 specifcation
  TAO_Codeset_Translator_Factory *fact =
    ACE_Dynamic_Service<TAO_Codeset_Translator_Factory>::
    instance ("UTF8_Latin1_Factory");
  if (fact == 0)
    ACE_Service_Config::process_directive
      (ACE_DYNAMIC_SERVICE_DIRECTIVE ("UTF8_Latin1_Factory",
                                      "TAO_Codeset",
                                      "_make_TAO_UTF8_Latin1_Factory",
                                      ""));
  else
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - Codeset_Manager_i::open skipping ")
                    ACE_TEXT("redundant load of UTF8_Latin1_Factory\n")
                    ));
    }

  fact = ACE_Dynamic_Service<TAO_Codeset_Translator_Factory>::
    instance ("UTF16_BOM_Factory");
  if (fact == 0)
    ACE_Service_Config::process_directive
      (ACE_DYNAMIC_SERVICE_DIRECTIVE ("UTF16_BOM_Factory",
                                      "TAO_Codeset",
                                      "_make_TAO_UTF16_BOM_Factory",
                                      ""));
  else
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - Codeset_Manager_i::open skipping ")
                    ACE_TEXT("redundant load of UTF16_BOM_Factory\n")
                    ));
    }

  if (init_codeset_factories_i (this->char_factories_,
                                this->codeset_info_.ForCharData) == -1)
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                    ACE_TEXT ("configure_codeset_factories, failed to init ")
                    ACE_TEXT ("char codeset factories\n")));
    }

  if (init_codeset_factories_i (this->wchar_factories_,
                                this->codeset_info_.ForWcharData) == -1)
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                    ACE_TEXT ("configure_codeset_factories, failed to init ")
                    ACE_TEXT ("wchar codeset factories\n")));
    }

  if (this->codeset_info_.ForWcharData.native_code_set == 0)
    {
      ACE_OutputCDR::wchar_maxbytes(0); // disallow wchar when no ncs_w set
    }
}

/// Initialise the specific type codeset factories
int
TAO_Codeset_Manager_i::init_codeset_factories_i (
    TAO_CodesetFactorySet& factset,
    CONV_FRAME::CodeSetComponent& cs_comp
  )
{
  TAO_CodesetFactorySetItor end = factset.end ();
  TAO_CodesetFactorySetItor iter = factset.begin ();

  CONV_FRAME::CodeSetId ncs = cs_comp.native_code_set;
  cs_comp.conversion_code_sets.length (
    static_cast<CORBA::ULong> (factset.size()));
  CORBA::ULong index;

  for (index = 0; iter != end; ++iter)
    {
      const char *name = (*iter)->codeset_name ();
      TAO_Codeset_Translator_Factory *fact =
        ACE_Dynamic_Service<TAO_Codeset_Translator_Factory>::instance (name);

      if (fact == 0)
        {
          if (TAO_debug_level)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                        ACE_TEXT ("init_codeset_factories_i, Unable to load ")
                        ACE_TEXT ("CodeSet <%s>, %m\n"),
                        ACE_TEXT_CHAR_TO_TCHAR(name)));
          continue;
        }

      if (fact->ncs() == ncs)
        {
          (*iter)->factory (fact);
          cs_comp.conversion_code_sets[index++] = fact->tcs();

          if (TAO_debug_level > 2)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                          ACE_TEXT ("init_codeset_factories_i, Loaded Codeset ")
                          ACE_TEXT ("<%s>, ncs = %08x tcs = %08x\n"),
                          ACE_TEXT_CHAR_TO_TCHAR(name),
                          fact->ncs(),fact->tcs()));
            }
        }
    }

  cs_comp.conversion_code_sets.length(index);
  return 0;
}


TAO_Codeset_Translator_Base *
TAO_Codeset_Manager_i::get_char_trans (CONV_FRAME::CodeSetId tcs)
{
  if (this->codeset_info_.ForCharData.native_code_set == tcs)
    return 0;
  return this->get_translator_i (this->char_factories_,tcs);
}

TAO_Codeset_Translator_Base *
TAO_Codeset_Manager_i::get_wchar_trans (CONV_FRAME::CodeSetId tcs)
{
  if (tcs == this->codeset_info_.ForWcharData.native_code_set &&
      tcs != ACE_CODESET_ID_ISO_UTF_16)
    return 0;
  return this->get_translator_i (this->wchar_factories_,tcs);
}

TAO_Codeset_Translator_Base *
TAO_Codeset_Manager_i::get_translator_i (TAO_CodesetFactorySet& factset,
                                       CONV_FRAME::CodeSetId tcs)
{
  const TAO_CodesetFactorySetItor end = factset.end ();

  for (TAO_CodesetFactorySetItor iter = factset.begin (); iter != end; ++iter)
    {
      TAO_Codeset_Translator_Factory *fact = (*iter)->factory ();

      if (fact && tcs == fact->tcs ())
        {
          return fact;
        }
    }

  return 0;
}


//---------------------------------------------------------------------
// Codeset Item
TAO_Codeset_Item::TAO_Codeset_Item (const char *name)
  :   name_ (0),
      factory_ (0)
{
  if (name)
    {
      name_ = ACE_OS::strdup(name);
    }
}

TAO_Codeset_Item::~TAO_Codeset_Item (void)
{
  ACE_OS::free (this->name_);
}

const char *
TAO_Codeset_Item::codeset_name (void)
{
  return this->name_;
}

TAO_Codeset_Translator_Factory *
TAO_Codeset_Item::factory (void)
{
  return this->factory_;
}

void
TAO_Codeset_Item::factory (TAO_Codeset_Translator_Factory *factory)
{
  this->factory_ = factory;
}

// End of Codeset Item Class

TAO_Codeset_Manager_Factory::~TAO_Codeset_Manager_Factory ()
{
}

TAO_Codeset_Manager *
TAO_Codeset_Manager_Factory::create (TAO_ORB_Core *oc)
{
  TAO_Codeset_Manager_i *csm = 0;
  ACE_NEW_RETURN (csm, TAO_Codeset_Manager_i, 0);

  const TAO_Codeset_Descriptor *cd =
    oc->resource_factory()->get_codeset_descriptor(0); // char
  if (cd->ncs_set())
    csm->set_ncs_c(cd->ncs());
  const TAO_Codeset_Descriptor::Translator_Node *tlist =
    cd->translators();
  while (tlist != 0)
    {
      csm->add_char_translator (tlist->name_);
      tlist = tlist->next_;
    }

  cd = oc->resource_factory()->get_codeset_descriptor(1); // wchar
  if (cd->ncs_set())
    csm->set_ncs_w(cd->ncs(), cd->max_bytes());
  tlist = cd->translators();
  while (tlist != 0)
    {
      csm->add_wchar_translator (tlist->name_);
      tlist = tlist->next_;
    }

  return csm;
}

ACE_FACTORY_DEFINE (TAO_Codeset, TAO_Codeset_Manager_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Table_Adapter_Factory,
                       ACE_TEXT ("TAO_Codeset"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Codeset_Manager_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Codeset_Translator_Factory>;

template class ACE_Node<TAO_Codeset_Item*>;
template class ACE_Unbounded_Set<TAO_Codeset_Item*>;
template class ACE_Unbounded_Set_Iterator<TAO_Codeset_Item*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Codeset_Translator_Factory>
#pragma instantiate ACE_Node<TAO_Codeset_Item*>
///
#pragma instantiate ACE_Unbounded_Set<TAO_Codeset_Item*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Codeset_Item*>

#  endif
