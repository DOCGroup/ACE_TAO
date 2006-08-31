// $Id$

#include "StdAfx.h"
#include "Common.h"
#include "ComHelp.h"
#include "GMECOM.h"
#include "ComponentConfig.h"
#include "RawComponent.h"
#include "Utils/Utils.h"

//=============================================================================
/**
 * @class COM_Exception
 *
 * COM exception helper class.
 */
//=============================================================================

class COM_Exception
{
public:
  /**
   * Constructor.
   *
   * @param[in]     hr        COM result.
   */
  inline COM_Exception (HRESULT hr)
    : hr_ (hr) { }

  /// Destructor.
  inline ~COM_Exception (void) { }

private:
  /// The COM result.
  HRESULT hr_;
};

/// Macro that verify the success of a COM operation. If
/// the operation does not succeed it throws an exception.
#define VERIFY_RESULT(x) \
  do { \
    HRESULT hr = x; \
    if (FAILED (hr)) \
      throw COM_Exception (hr); \
  } while (0)

//
// picml_types_
//
std::set <std::wstring> RawComponent::picml_types_;

//
// RawComponent
//
RawComponent::RawComponent (void)
: importing_ (0)
{

}

//
// ~RawComponent
//
RawComponent::~RawComponent (void)
{

}

//
// Initaialize
//
STDMETHODIMP RawComponent::Initialize (struct IMgaProject * project)
{
  this->project_ = project;

  /// Create a collection of PICML types that contain a UUID
  /// attribute. This will allow the addon to manage it's UUID.
  RawComponent::picml_types_.insert (L"Component");
  RawComponent::picml_types_.insert (L"ComponentAssembly");
  RawComponent::picml_types_.insert (L"ComponentPackage");
  RawComponent::picml_types_.insert (L"ComponentImplementation");
  RawComponent::picml_types_.insert (L"DeploymentPlan");
  RawComponent::picml_types_.insert (L"Domain");
  RawComponent::picml_types_.insert (L"ImplementationArtifact");
  RawComponent::picml_types_.insert (L"MonolithicImplementation");
  RawComponent::picml_types_.insert (L"PackageConfiguration");
  return S_OK;
}

//
// Invoke [obsolete]
//
STDMETHODIMP RawComponent::Invoke (IMgaProject* gme,
                                   IMgaFCOs *models,
                                   long param)
{
  return E_MGA_NOT_SUPPORTED;
}

//
// InvokeEx
//
STDMETHODIMP RawComponent::InvokeEx (IMgaProject *project,
                                     IMgaFCO *currentobj,
                                     IMgaFCOs *selectedobjs,
                                     long param)
{
  return E_MGA_NOT_SUPPORTED;
}


//
// ObjectsInvokeEx [not implemented]
//
STDMETHODIMP RawComponent::ObjectsInvokeEx (IMgaProject *project,
                                            IMgaObject *currentobj,
                                            IMgaObjects *selectedobjs,
                                            long param)
{
  return E_MGA_NOT_SUPPORTED;
}

//
// get_ComponentParameter
//
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal)
{
  return E_MGA_NOT_SUPPORTED;
}

//
// put_ComponentParameter
//
STDMETHODIMP RawComponent::put_ComponentParameter (BSTR name, VARIANT newVal)
{
  return E_MGA_NOT_SUPPORTED;
}

//
// GlobalEvent
//
STDMETHODIMP RawComponent::GlobalEvent (globalevent_enum event)
{
  /*
   * Right now I am simplifying the UUIDManager and it no longer
   * processes GlobalEvent messages. Instead the UUID manager
   * only creates UUIDs and manages existing ones when and
   * ObjectEvent is sent.
   */

  //try
  //{
  //  switch (event)
  //  {
  //  case APPEVENT_XML_IMPORT_BEGIN:
  //    this->importing_ = 1;

  //  case GLOBALEVENT_OPEN_PROJECT:
  //    this->verify_all_uuids ();
  //    break;

  //  case APPEVENT_XML_IMPORT_END:
  //    this->importing_ = 0;
  //    this->verify_all_uuids ();
  //    break;

  //  case GLOBALEVENT_NOTIFICATION_READY:
  //    this->handle_pending ();
  //    break;

  //  default:
  //    /* do nothing */;
  //  }
  //}
  //catch (COM_Exception &)
  //{
  //  AfxMessageBox ("Caught a COM exception");
  //}
  //catch (...)
  //{
  //  AfxMessageBox ("Caught an unknown exception");
  //}

  return S_OK;
}

//
// ObjectEvent
//
STDMETHODIMP RawComponent::ObjectEvent (IMgaObject * obj,
                                        unsigned long eventmask,
                                        VARIANT v)
{
  //if (this->pending_.GetCount ())
  //  this->handle_pending ();

  //if (this->importing_)
  //  return S_OK;

  CComPtr <IMgaObject> object (obj);

  // Determine if the object is a library object before
  // continuing.
  try
  {
    VARIANT_BOOL lib_object;
    VERIFY_RESULT (object->get_IsLibObject (&lib_object));

    if (lib_object == VARIANT_FALSE)
    {
      CComPtr <IMgaFCO> fco;
      VERIFY_RESULT (object.QueryInterface (&fco));

      // Get the meta information for the object.
      CComPtr <IMgaMetaFCO> fco_meta;
      VERIFY_RESULT (fco->get_Meta (&fco_meta));

      // Get the "type" of the object.
      CComBSTR bstr;
      VERIFY_RESULT (fco_meta->get_Name (&bstr));

      if (RawComponent::picml_types_.find (bstr.m_str) !=
          RawComponent::picml_types_ .end ())
      {
        if ((eventmask & OBJEVENT_CREATED) != 0)
          this->create_uuid (fco);
        else if ((eventmask & OBJEVENT_ATTR) != 0)
          this->verify_uuid (fco);
      }
    }
  }
  catch (COM_Exception &)
  {
    /* what should we really do right here??? */
  }
  catch (...)
  {
    /* what should we really do right here??? */
  }

  return S_OK;
}

//
// create_uuid
//
void RawComponent::create_uuid (IMgaFCO * _fco)
{
  CComPtr <IMgaFCO> fco (_fco);
  CComPtr <IMgaAttribute> uuid_attr;
  long status;

  if (this->get_uuid_i (fco, &uuid_attr, status))
  {
    if (status == ATTSTATUS_METADEFAULT ||
        status == ATTSTATUS_UNDEFINED ||
        status > 0)
    {
      // This will force the generation of an <UUID> for
      // any new component/assembly inserted, as well as
      // instances and subtypes.
      try
      {
        VERIFY_RESULT (uuid_attr->put_StringValue (
                       CComBSTR (Utils::CreateUuid ().c_str ())));
      }
      catch (...)
      {
        this->pending_.AddTail (fco);
      }
    }
  }
}

//
// verify_uuid
//
void RawComponent::verify_uuid (IMgaFCO * _fco)
{
  CComPtr <IMgaFCO> fco (_fco);
  CComPtr <IMgaAttribute> uuid_attr;
  long status;

  if (!this->get_uuid_i (fco, &uuid_attr, status))
    return;

  // This will validate <UUID> of existing components.
  // This is mainly useful during the "import" phase.
  CComBSTR uuid_bstr;
  VERIFY_RESULT (uuid_attr->get_StringValue (&uuid_bstr));

  CW2A uuid_str (uuid_bstr);
  if (Utils::ValidUuid (std::string (uuid_str)))
    return;

  try
  {
    VERIFY_RESULT (uuid_attr->put_StringValue (
                   CComBSTR (Utils::CreateUuid ().c_str ())));
  }
  catch (COM_Exception & ex)
  {
    this->pending_.AddTail (fco);
    ex;
  }
}

//
// get_uuid_i
//
bool RawComponent::get_uuid_i (IMgaFCO * _fco,
                               IMgaAttribute ** attr,
                               long & status)
{
  CComPtr <IMgaFCO> fco (_fco);

  // Get the <IMgaAttributes> of the <fco_object>.
  CComPtr <IMgaAttributes> attrs;
  VERIFY_RESULT (fco->get_Attributes (&attrs));

  long count;
  VERIFY_RESULT (attrs->get_Count (&count));

  CComPtr <IMgaAttribute> uuid_attr;
  CComPtr <IMgaMetaAttribute> meta_attr;
  CComBSTR attr_name;

  // We have to enumerate thru all the attributes b/c the method
  // that gets the attribute directly is *broken*.
  for (long i = 1; i <= count; i ++)
  {
    // Get the next <IMgaAttribute>.
    VERIFY_RESULT (attrs->get_Item (i, &uuid_attr));

    // Determine if this is the UUID <attribute>.
    VERIFY_RESULT (uuid_attr->get_Status (&status));
    VERIFY_RESULT (uuid_attr->get_Meta (&meta_attr));
    VERIFY_RESULT (meta_attr->get_Name (&attr_name));

    if (attr_name == L"UUID")
    {
      *attr = uuid_attr.Detach ();
      return true;
    }

    // Apparently the implementation of CComPtr and CComBSTR
    // do not allow usage of "initialized" objects for holding
    // data. Therefore we have to "manually" release everything
    // which is really STUPID!!!!
    uuid_attr.Release ();
    meta_attr.Release ();
    attr_name.Empty ();
  }

  return false;
}

//
// verify_all_uuids
//
void RawComponent::verify_all_uuids (void)
{
  //// Get the <project> hosting the add-on.
  //CComPtr <IMgaProject> project;
  //VERIFY_RESULT (this->addon->get_Project (&project));

  // Get all the component (component assembly) objects.
  CComPtr <IMgaFilter> filter;
  this->project_->CreateFilter (&filter);

  long status;
  this->project_->get_ProjectStatus (&status);

  filter->put_Kind (L"Component ComponentAssembly");
  filter->put_Level (L"0-");
  filter->put_ObjType (L"OBJTYPE_MODEL");

  CComPtr <IMgaFCOs> fcos;
  VERIFY_RESULT (this->project_->AllFCOs (filter, &fcos));

  long count;
  fcos->get_Count (&count);

  for (long i = 1; i <= count; i ++)
  {
    CComPtr <IMgaFCO> fco;
    fcos->get_Item (i, &fco);

    VARIANT_BOOL lib_object;
    fco->get_IsLibObject (&lib_object);

    if (lib_object == VARIANT_FALSE)
      this->verify_uuid (fco);
  }
}

//
// handle_pending
//
void RawComponent::handle_pending (void)
{
  long status;
  this->project_->get_ProjectStatus (&status);

  size_t count = this->pending_.GetCount ();

  while (count -- > 0)
  {
    CComPtr <IMgaFCO> fco = this->pending_.RemoveHead ();
    this->verify_uuid (fco);
  }
}
