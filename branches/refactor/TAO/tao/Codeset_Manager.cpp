// $Id$
#include "tao/Codeset_Manager.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "tao/TAO_Server_Request.h"
#include "tao/operation_details.h"
#include "tao/ORB_Core.h"
#include "tao/Exception.h"
#include "tao/corbafwd.h"
#include "tao/Codeset_Translator_Factory.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Tagged_Components.h"

#include "ace/Codeset_Registry.h"

ACE_RCSID(tao, Codeset_Manager, "$Id$")

/// NCS for char is defaulted to ISO 8859-1:1987; Latin Alphabet No. 1
CONV_FRAME::CodeSetId TAO_Codeset_Manager::default_char_codeset = 0x00010001;
/// NCS for wchar is not defaulted by the CORBA specification, but a default
/// may be set here if desired
CONV_FRAME::CodeSetId TAO_Codeset_Manager::default_wchar_codeset = 0;


TAO_Codeset_Manager::TAO_Codeset_Manager(CONV_FRAME::CodeSetComponentInfo *csi)
 :codeset_info_ (csi)
{
}

TAO_Codeset_Manager::~TAO_Codeset_Manager()
{
  delete codeset_info_;
}

void
TAO_Codeset_Manager::set_codeset (TAO_Tagged_Components& tc) const
{
  tc.set_code_sets (*this->codeset_info_);
}


void
TAO_Codeset_Manager::set_tcs(TAO_Profile &theProfile,
                             TAO_Transport &trans)
{
   /// If tcs is already set on the transport then donot process,
   /// use existing transport as CDR have translators set.
   if (trans.is_tcs_set())
     {
       if(TAO_debug_level > 2)
         ACE_DEBUG((LM_DEBUG,
		    ACE_LIB_TEXT("(%P|%t) Codeset_Manager::set_tcs ")
		    ACE_LIB_TEXT("transport already set\n")));
       return;
     }

   TAO_Tagged_Components& theTaggedComp = theProfile.tagged_components();

   CONV_FRAME::CodeSetComponentInfo remote;
   /// Get the codeset component
   if (theTaggedComp.get_code_sets(remote) == 0 )
   {
     if(TAO_debug_level > 2)
       ACE_DEBUG((LM_DEBUG,
		  ACE_LIB_TEXT("(%P|%t) Codeset_Manager::set_tcs ")
		  ACE_LIB_TEXT("No codeset componnet in profile\n")));
     remote.ForCharData.native_code_set =
       TAO_Codeset_Manager::default_char_codeset;
     remote.ForWcharData.native_code_set =
       TAO_Codeset_Manager::default_wchar_codeset;
   }

  CONV_FRAME::CodeSetId ncs =
     this->codeset_info_->ForCharData.native_code_set;
   CONV_FRAME::CodeSetId tcs = computeTCS (remote.ForCharData,
                                           this->codeset_info_->ForCharData);
   TAO_Resource_Factory *rf = trans.orb_core()->resource_factory();
   if (TAO_debug_level > 2)
     ACE_DEBUG ((LM_DEBUG,
		 ACE_LIB_TEXT("(%P|%t) Codeset_Manager::set_tcs ")
                 ACE_LIB_TEXT("setting char translator(%08x, %08x)\n"),
		ncs,tcs));
   trans.char_translator(rf->get_char_translator (ncs,tcs));

   ncs = this->codeset_info_->ForWcharData.native_code_set;
   tcs = computeTCS (remote.ForWcharData,
                     this->codeset_info_->ForWcharData);

   if (TAO_debug_level > 2)
     ACE_DEBUG ((LM_DEBUG,
		 ACE_LIB_TEXT("(%P|%t) Codeset_Manager::set_tcs ")
                 ACE_LIB_TEXT("setting wchar translator (%08x, %08x)\n"),
                 ncs,tcs));
   trans.wchar_translator(rf->get_wchar_translator (ncs,tcs));
   trans.wchar_allowed (ncs != 0 || tcs != 0);
}

void
TAO_Codeset_Manager::process_service_context (TAO_ServerRequest &request)
{
  if( request.transport()->is_tcs_set())
    {
      return;
    }

  // Get the service Context from an object of TAO_ServerRequest
  // and set the TCS values on the Transport
  TAO_Service_Context &service_cntx = request.request_service_context ();
  IOP::ServiceContext context;
  context.context_id = IOP::CodeSets;
  CONV_FRAME::CodeSetId tcs_c = TAO_Codeset_Manager::default_char_codeset;
  CONV_FRAME::CodeSetId tcs_w = TAO_Codeset_Manager::default_wchar_codeset;

  // Codeset Context is not available, no need to process service context
  if (service_cntx.get_context(context))
    {
      // Convert the Service Context to Codeset Context
      const char *buffer =
        ACE_reinterpret_cast(const char*,context.context_data.get_buffer ());

      TAO_InputCDR cdr (buffer,context.context_data.length (),
                        request.incoming().byte_order());
      cdr >> tcs_c;
      cdr >> tcs_w;
    }

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
		ACE_LIB_TEXT("(%P|%t) Codeset_Manager::process_service_context ")
		ACE_LIB_TEXT("using tcsc = %08x, tcsw = %08x\n"),
		tcs_c,tcs_w));

  CONV_FRAME::CodeSetId ncs =
    this->codeset_info_->ForCharData.native_code_set;

  TAO_Resource_Factory *rf =
    request.transport()->orb_core()->resource_factory();
  request.transport()->char_translator(rf->get_char_translator (ncs,tcs_c));

  ncs = this->codeset_info_->ForWcharData.native_code_set;
  request.transport()->wchar_translator(rf->get_wchar_translator (ncs,tcs_w));
  request.transport()->wchar_allowed (ncs != 0 || tcs_w != 0);
}

void
TAO_Codeset_Manager::generate_service_context (TAO_Operation_Details &opd,
                                               TAO_Transport &trans)
{
  // Take a reference to service context
  IOP::ServiceContextList &serv_cntxs = opd.request_service_info();

  CONV_FRAME::CodeSetContext codeset_cntx;

  // Generating codeset context
  // Assuming the TCS values from Transport will be defaulted
  TAO_Codeset_Translator_Factory *tf = trans.char_translator();
  codeset_cntx.char_data = tf ? tf->tcs() :
    this->codeset_info_->ForCharData.native_code_set;
  tf = trans.wchar_translator();
  codeset_cntx.wchar_data = tf ? tf->tcs() :
    this->codeset_info_->ForWcharData.native_code_set;
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
		ACE_LIB_TEXT("(%P|%t) Codeset_Manager::generate_service_context ")
		ACE_LIB_TEXT("using tcs_c = %08x, tcs_w = %08x\n"),
		codeset_cntx.char_data,
		codeset_cntx.wchar_data));

  TAO_OutputCDR codeset_cdr;
  codeset_cdr << codeset_cntx;

  // Convert the codeset_cntx to servicecontext and insert into the List
  IOP::ServiceContext cntx;
  cntx.context_id = IOP::CodeSets;

  CORBA::ULong length = codeset_cdr.total_length ();
  cntx.context_data.length (length);
  CORBA::Octet *buf = cntx.context_data.get_buffer ();

  for (const ACE_Message_Block *i = codeset_cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }

  /// Append it to IIOP::ServiceContextList
  length = serv_cntxs.length ();
  serv_cntxs.length (length + 1);

  CORBA::ULong max = cntx.context_data.maximum ();
  CORBA::ULong len = cntx.context_data.length ();
  buf = cntx.context_data.get_buffer (1);

  serv_cntxs[length].context_id = cntx.context_id;
  serv_cntxs[length].context_data.replace (max, len, buf, 1);
}

/// Checks whether the NCS is a part of CCS
CORBA::Boolean
TAO_Codeset_Manager::isElementOf (CONV_FRAME::CodeSetId id,
                                  CONV_FRAME::CodeSetComponent &cs_comp)
{
  for (CORBA::ULong i=0L;
       i < cs_comp.conversion_code_sets.length();
       ++i )
    {
      if (id == cs_comp.conversion_code_sets[i])
        return true;
    }
  return false;
}

/// Find the Intersection of Client and Server CCS's
CONV_FRAME::CodeSetId
TAO_Codeset_Manager::intersectionOf (CONV_FRAME::CodeSetComponent &cs_comp1,
                                     CONV_FRAME::CodeSetComponent &cs_comp2)
{
  for( CORBA::ULong index=0L;
       index < cs_comp1.conversion_code_sets.length();
       ++index )
    if(this->isElementOf(cs_comp1.conversion_code_sets[index], cs_comp2))
        return cs_comp1.conversion_code_sets[index];
  return 0;
}

CORBA::Boolean
TAO_Codeset_Manager::isCompatible(CONV_FRAME::CodeSetId cs1,
                                  CONV_FRAME::CodeSetId cs2 )
{
  // Call the is_compatible method of ACE_Codeset_Registry
  return ACE_Codeset_Registry::is_compatible(cs1,cs2);
}

/// returns the TCS for Char / Wchar
CONV_FRAME::CodeSetId
TAO_Codeset_Manager::computeTCS (CONV_FRAME::CodeSetComponent &remote,
                                 CONV_FRAME::CodeSetComponent &local )
{
  if (remote.native_code_set == local.native_code_set)
    return local.native_code_set;

  if (this->isElementOf (remote.native_code_set, local))
    return remote.native_code_set;

  if (this->isElementOf (local.native_code_set, remote))
    return local.native_code_set;

  CONV_FRAME::CodeSetId tcs;
  if ((tcs = this->intersectionOf (remote, local)) == 0)
    if (isCompatible (local.native_code_set, remote.native_code_set))
      return remote.native_code_set;
    else
      {
        ACE_DECLARE_NEW_CORBA_ENV;
        ACE_THROW_RETURN(CORBA::CODESET_INCOMPATIBLE(),0);
      }
  return tcs;
}
