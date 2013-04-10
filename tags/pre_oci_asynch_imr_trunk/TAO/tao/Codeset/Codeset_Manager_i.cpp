// -*- C++ -*-
// $Id$

#include "tao/TAO_Server_Request.h"
#include "tao/operation_details.h"
#include "tao/Transport.h"
#include "tao/Profile.h"
#include "tao/SystemException.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"

#include "tao/Codeset/Codeset_Descriptor.h"
#include "tao/Codeset/Codeset_Manager_i.h"
#include "tao/Codeset/Codeset_Translator_Factory.h"
#include "tao/Codeset/Codeset.h"
#include "tao/Codeset/CodeSetContextC.h"
#include "tao/Codeset/Codeset_Service_Context_Handler.h"

#include "ace/Dynamic_Service.h"
#include "ace/Codeset_Registry.h"
#include "ace/OS_NS_string.h"
#include "ace/Service_Config.h"

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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// NCS for char is defaulted to ISO 8859-1:1987; Latin Alphabet No. 1
CONV_FRAME::CodeSetId
TAO_Codeset_Manager_i::default_char_codeset = TAO_DEFAULT_CHAR_CODESET_ID;
/// NCS for wchar is not defaulted by the CORBA specification, but a default
/// may be set here if desired
CONV_FRAME::CodeSetId
TAO_Codeset_Manager_i::default_wchar_codeset = TAO_DEFAULT_WCHAR_CODESET_ID;

TAO_Codeset_Manager_i::TAO_Codeset_Manager_i (void)
  : codeset_info_ (),
    char_descriptor_ (),
    wchar_descriptor_ ()
{
  char_descriptor_.ncs(TAO_Codeset_Manager_i::default_char_codeset);
  char_descriptor_.add_translator (ACE_TEXT ("UTF8_Latin1_Factory"));

  wchar_descriptor_.ncs(TAO_Codeset_Manager_i::default_wchar_codeset);
  wchar_descriptor_.add_translator (ACE_TEXT ("UTF16_BOM_Factory"));

}

TAO_Codeset_Manager_i::~TAO_Codeset_Manager_i (void)
{
}

TAO_Codeset_Descriptor_Base *
TAO_Codeset_Manager_i::char_codeset_descriptor (void)
{
  return &this->char_descriptor_;
}

TAO_Codeset_Descriptor_Base *
TAO_Codeset_Manager_i::wchar_codeset_descriptor (void)
{
  return &this->wchar_descriptor_;
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
  /// If tcs is already set on the transport then do not process,
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

  // These are the "fallback" codeset ids for use if no other codeset
  // can be computed based on our local set and those in the context
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
                    ACE_TEXT("TAO (%P|%t) - ")
                    ACE_TEXT("Codeset_Manager_i::process_service_context ")
                    ACE_TEXT("no codeset context in request, using defaults\n")
                    ));
      tcs_c = TAO_Codeset_Manager_i::default_char_codeset;
      tcs_w = TAO_Codeset_Manager_i::default_wchar_codeset;
    }
  if (TAO_debug_level > 2)
    {
      ACE_CString tcs_c_locale;
      ACE_CString tcs_w_locale;
      ACE_Codeset_Registry::registry_to_locale (tcs_c, tcs_c_locale, 0, 0);
      ACE_Codeset_Registry::registry_to_locale (tcs_w, tcs_w_locale, 0, 0);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                  ACE_TEXT ("process_service_context, ")
                  ACE_TEXT ("using tcsc <%C> (%08x), tcsw <%C> (%08x)\n"),
                  tcs_c_locale.c_str (), tcs_c, tcs_w_locale.c_str (), tcs_w));
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
      ACE_CString tcs_c_locale;
      ACE_CString tcs_w_locale;
      ACE_Codeset_Registry::registry_to_locale (codeset_cntx.char_data, tcs_c_locale, 0, 0);
      ACE_Codeset_Registry::registry_to_locale (codeset_cntx.wchar_data, tcs_w_locale, 0, 0);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                  ACE_TEXT ("generate_service_context, ")
                  ACE_TEXT ("using tcs_c <%C> (%08x), tcs_w <%C> (%08x)\n"),
                  tcs_c_locale.c_str (),
                  codeset_cntx.char_data,
                  tcs_w_locale.c_str (),
                  codeset_cntx.wchar_data));
    }

  TAO_OutputCDR codeset_cdr;
  if ((codeset_cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)) &&
      (codeset_cdr << codeset_cntx))
    {
      service_cntx.set_context (IOP::CodeSets,codeset_cdr);
    }
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
                                   CONV_FRAME::CodeSetComponent &local)
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
          throw ::CORBA::CODESET_INCOMPATIBLE ();
        }
    }

  return tcs;
}

void
TAO_Codeset_Manager_i::open(TAO_ORB_Core& core)
{
#if 0
  // These translators help comply with the CORBA 3.0.2 specifcation
  TAO_Codeset_Translator_Factory *fact =
    ACE_Dynamic_Service<TAO_Codeset_Translator_Factory>::
    instance ("UTF8_Latin1_Factory");
  if (fact == 0)
    ACE_Service_Config::process_directive
      (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE ("UTF8_Latin1_Factory",
                                      "TAO_Codeset",
                                      TAO_VERSION,
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
      (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE ("UTF16_BOM_Factory",
                                      "TAO_Codeset",
                                      TAO_VERSION,
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
#endif
  //
  TAO_Codeset_Service_Context_Handler* h = 0;
  ACE_NEW (h,
           TAO_Codeset_Service_Context_Handler());
  core.service_context_registry ().bind (IOP::CodeSets, h);

  // add in from the service configurator
  this->codeset_info_.ForCharData.native_code_set =
    this->char_descriptor_.ncs();
  this->codeset_info_.ForWcharData.native_code_set =
    this->wchar_descriptor_.ncs();
  ACE_OutputCDR::wchar_maxbytes(this->wchar_descriptor_.max_bytes());

  if (init_ccs (this->char_descriptor_,
                this->codeset_info_.ForCharData) == -1)
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                    ACE_TEXT ("configure_codeset_factories, failed to init ")
                    ACE_TEXT ("char codeset factories\n")));
    }

  if (init_ccs (this->wchar_descriptor_,
                this->codeset_info_.ForWcharData) == -1)
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                    ACE_TEXT ("configure_codeset_factories, failed to init ")
                    ACE_TEXT ("wchar codeset factories\n")));
    }
}

// Initialise the specific type codeset factories
int
TAO_Codeset_Manager_i::init_ccs (TAO_Codeset_Descriptor& cd,
                                 CONV_FRAME::CodeSetComponent& cs_comp)
{
  cs_comp.conversion_code_sets.length
    (static_cast<CORBA::ULong> (cd.num_translators()));

  CORBA::ULong index;
  TAO_Codeset_Descriptor::Translator_Node *tlist = cd.translators();

  for (index = 0; tlist; tlist = tlist->next_)
    {
      tlist->translator_factory_ =
        ACE_Dynamic_Service<TAO_Codeset_Translator_Factory>::instance
        (tlist->name_);

      if (tlist->translator_factory_ == 0)
        {
          if (TAO_debug_level)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                        ACE_TEXT ("init_ccs, Unable to load ")
                        ACE_TEXT ("code set translator <%s>, %m\n"),
                        tlist->name_));
          continue;
        }

      if (tlist->translator_factory_->ncs() != cs_comp.native_code_set)
        {
          if (TAO_debug_level)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                        ACE_TEXT ("init_ccs, codeset translator <%s> ")
                        ACE_TEXT ("has wrong ncs (%d), %m\n"),
                        tlist->name_,
                        tlist->translator_factory_->ncs()));
          tlist->translator_factory_ = 0;
          continue;
        }

      // this is a special case for the utf16 bom translator.
      if (tlist->translator_factory_->tcs() ==  cs_comp.native_code_set)
        continue;

      cs_comp.conversion_code_sets[index++] =
        tlist->translator_factory_->tcs();
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Codeset_Manager_i::")
                      ACE_TEXT ("init_ccs, Loaded Codeset translator ")
                      ACE_TEXT ("<%s>, ncs = %08x tcs = %08x\n"),
                      tlist->name_,
                      tlist->translator_factory_->ncs(),
                      tlist->translator_factory_->tcs()
                      ));
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
  return this->get_translator_i (this->char_descriptor_,tcs);
}

TAO_Codeset_Translator_Base *
TAO_Codeset_Manager_i::get_wchar_trans (CONV_FRAME::CodeSetId tcs)
{
  if (tcs == this->codeset_info_.ForWcharData.native_code_set &&
      tcs != ACE_CODESET_ID_ISO_UTF_16)
    return 0;
  return this->get_translator_i (this->wchar_descriptor_,tcs);
}

TAO_Codeset_Translator_Base *
TAO_Codeset_Manager_i::get_translator_i (TAO_Codeset_Descriptor& cd,
                                         CONV_FRAME::CodeSetId tcs)
{
  for (TAO_Codeset_Descriptor::Translator_Node *tlist = cd.translators();
       tlist; tlist = tlist->next_)
    {
      TAO_Codeset_Translator_Factory *fact = tlist->translator_factory_;
      if (fact && tcs == fact->tcs ())
        return fact;
    }
  return 0;
}

void
TAO_Codeset_Manager_i::get_ncs (CONV_FRAME::CodeSetId &ncsc,
                                CONV_FRAME::CodeSetId &ncsw)
{
   ncsc = this->char_descriptor_.ncs();
   ncsw = this->wchar_descriptor_.ncs();
}

TAO_END_VERSIONED_NAMESPACE_DECL
