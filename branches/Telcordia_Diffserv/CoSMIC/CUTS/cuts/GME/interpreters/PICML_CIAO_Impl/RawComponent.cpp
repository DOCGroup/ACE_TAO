///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ComHelp.h"
#include "GMECOM.h"
#include "ComponentConfig.h"
#include "UdmConfig.h"
#include "RawComponent.h"

// Udm includes
#include "UdmBase.h"
#include "Uml.h"
#include "UmlExt.h"

#ifdef _USE_DOM
  #include "UdmDOM.h"
#endif

#include "UdmGme.h"
#include "UdmStatic.h"
#include "UdmUtil.h"

#include "UdmApp.h"

// Global config object
_config config;

//
// Initialize
//
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *)
{
  return S_OK;
}


//
// Invoke
//
STDMETHODIMP RawComponent::Invoke (IMgaProject* gme,
                                   IMgaFCOs *models,
                                   long param)
{
#ifdef SUPPORT_OLD_INVOKE
  CComPtr<IMgaFCO> focus;
  CComVariant parval = param;
  return InvokeEx(gme, focus, selected, parvar);
#else
  if(interactive) {
    AfxMessageBox("This component does not support the obsolete invoke mechanism");
  }
  return E_MGA_NOT_SUPPORTED;
#endif
}

#ifdef _DYNAMIC_META
//
// Dummy function for UDM meta initialization
//
void dummy (void)
{

}
#endif

//
// InvokeEx
//
STDMETHODIMP RawComponent::InvokeEx (IMgaProject *project,
                                     IMgaFCO *currentobj,
                                     IMgaFCOs *selectedobjs,
                                     long param)
{
  // @@ INSERT PREPROCESSOR HOOK HERE

  // Calling the user's initialization function
  if (CUdmApp::Initialize())
  {
    return S_FALSE;
  }

  CComPtr <IMgaProject> ccpProject (project);

  try
  {
    if (interactive)
    {
      CComBSTR projname;
      CComBSTR focusname = "<nothing>";

      CComPtr <IMgaTerritory> terr;

      COMTHROW (ccpProject->CreateTerritory (NULL, &terr));

    // Setting up Udm
#ifdef _DYNAMIC_META

#ifdef _DYNAMIC_META_DOM
      // Loading the meta for the project
      UdmDom::DomDataNetwork  ddnMeta(Uml::diagram);
      Uml::Diagram theUmlDiagram;

      // Opening the XML meta of the project
      ddnMeta.OpenExisting (config.metaPath,
                            "uml.xsd",
                            Udm::CHANGES_LOST_DEFAULT);

      // Casting the DataNetwork to diagram
      theUmlDiagram = Uml::Diagram::Cast (ddnMeta.GetRootObject());

      // Creating the UDM diagram
      Udm::UdmDiagram udmDataDiagram;
      udmDataDiagram.dgr = &theUmlDiagram;
      udmDataDiagram.init = dummy;

#elif defined _DYNAMIC_META_STATIC
      // Loading the meta for the project
      UdmStatic::StaticDataNetwork  dnsMeta(Uml::diagram);
      Uml::Diagram theUmlDiagram;

      // Opening the static meta of the project
      dnsMeta.OpenExisting (config.metaPath,
                            "",
                            Udm::CHANGES_LOST_DEFAULT);

      // Casting the DataNetwork to diagram
      theUmlDiagram = Uml::Diagram::Cast (dnsMeta.GetRootObject ());

      // Creating the UDM diagram
      Udm::UdmDiagram udmDataDiagram;
      udmDataDiagram.dgr = &theUmlDiagram;
      udmDataDiagram.init = dummy;
#else
      ASSERT ((0,
               "Nor _DYNAMIC_META_DOM either _DYNAMIC_META_STATIC ""
               "defined for dynamic loading"));
#endif
      // Loading the project
      UdmGme::GmeDataNetwork dngBackend(udmDataDiagram);

#else
      using namespace META_NAMESPACE;

      // Loading the project
      UdmGme::GmeDataNetwork dngBackend(META_NAMESPACE::diagram);

#endif
      try
      {
        // Opening backend
        dngBackend.OpenExisting(ccpProject);


        CComPtr<IMgaFCO> ccpFocus(currentobj);
        Udm::Object currentObject;
        if(ccpFocus)
        {
          currentObject=dngBackend.Gme2Udm(ccpFocus);
        }

        set<Udm::Object> selectedObjects;

        CComPtr<IMgaFCOs> ccpSelObject(selectedobjs);

        MGACOLL_ITERATE (IMgaFCO,ccpSelObject)
        {
          Udm::Object currObj;
          if(MGACOLL_ITER)
          {
            currObj=dngBackend.Gme2Udm(MGACOLL_ITER);
          }

          selectedObjects.insert(currObj);
        }
        MGACOLL_ITERATE_END;

#ifdef _ACCESS_MEMORY
        // Creating Cache
#ifdef _DYNAMIC_META
        UdmStatic::StaticDataNetwork dnsCacheBackend(udmDataDiagram);
#else
        UdmStatic::StaticDataNetwork dnsCacheBackend(META_NAMESPACE::diagram);
#endif

        const Uml::Class & safeType =
          Uml::SafeTypeContainer::GetSafeType (
          dngBackend.GetRootObject ().type ());

        dnsCacheBackend.CreateNew("","",safeType, Udm::CHANGES_LOST_DEFAULT);

        Udm::Object nullObject(&Udm::__null);
        UdmUtil::copy_assoc_map copyAssocMap;
        copyAssocMap[currentObject]=nullObject; // currentObject may be null object

        for (set <Udm::Object>::iterator p_CurrSelObject = selectedObjects.begin ();
            p_CurrSelObject != selectedObjects.end ();
            p_CurrSelObject ++)
        {
          pair <Udm::Object const, Udm::Object> item (*p_CurrSelObject, nullObject);

          pair <UdmUtil::copy_assoc_map::iterator,
                bool> insRes = copyAssocMap.insert(item);

          if (!insRes.second)
          {
            ASSERT(NULL);
          }
        }

        // Copying from GME to memory
        UdmUtil::CopyObjectHierarchy(dngBackend.GetRootObject().__impl(),
                                    dnsCacheBackend.GetRootObject().__impl(),
                                    &dnsCacheBackend,
                                    copyAssocMap);

        // Searching for focus object
        Udm::Object currentObjectCache;
        UdmUtil::copy_assoc_map::iterator currObject =
          copyAssocMap.find(currentObject);

        if (currObject != copyAssocMap.end ()) // It is in the map
        {
          currentObjectCache=currObject->second;
        }

        // Searching for selected objects
        set<Udm::Object> selectedObjectsCache;

        for (p_CurrSelObject=selectedObjects.begin ();
             p_CurrSelObject!=selectedObjects.end ();
             p_CurrSelObject ++)
        {
          Udm::Object object;

          UdmUtil::copy_assoc_map::iterator currSelObjectIt =
            copyAssocMap.find (*p_CurrSelObject);

          if (currSelObjectIt != copyAssocMap.end ()) // It is in the map
          {
            object=currSelObjectIt->second;
            selectedObjectsCache.insert(object);
          }
        }

        // Closing GME backend
        dngBackend.CloseNoUpdate();

        // Calling the main entry point
        // @@ We need to find some way to "strategize" this line of
        // @@ code so we can create a library that links with the
        // @@ various types of interpreters
        CUdmApp::UdmMain (&dnsCacheBackend,
                          currentObjectCache,
                          selectedObjectsCache,
                          param);

        // Close cache backend
        dnsCacheBackend.CloseNoUpdate();

#else
        // Calling the main entry point
        CUdmApp::UdmMain (&dngBackend,
                          currentObject,
                          selectedObjects,
                          param);

        // Closing backend
        dngBackend.CloseWithUpdate();
#endif
      }
      catch(udm_exception &exc)
      {
#ifdef _META_ACCESS_MEMORY
        dnCacheBackend.CloseNoUpdate();
#endif
        // Close GME Backend (we may close it twice, but GmeDataNetwork handles it)
        dngBackend.CloseNoUpdate ();

        AfxMessageBox (exc.what (), MB_ICONERROR | MB_OK);
        return S_FALSE;
      }
    }
  }
  catch(udm_exception &exc)
  {
    AfxMessageBox(exc.what());
    return S_FALSE;
  }
  catch(...)
  {
    ccpProject->AbortTransaction();
    AfxMessageBox("An unexpected error has occured during the interpretation process.");
    return E_UNEXPECTED;
  }

  // @@ INSERT POSTPROCESSOR HOOK HERE

  return S_OK;
}

//
// ObjectsInvokeEx
//
STDMETHODIMP RawComponent::ObjectsInvokeEx (IMgaProject *project,
                                            IMgaObject *currentobj,
                                            IMgaObjects *selectedobjs,
                                            long param)
{
  if (interactive)
  {
    AfxMessageBox("Tho ObjectsInvoke method is not implemented");
  }
  return E_MGA_NOT_SUPPORTED;
}

//
// get_ComponentParameter
//
STDMETHODIMP RawComponent::get_ComponentParameter (BSTR name,
                                                   VARIANT *pVal)
{
  return S_OK;
}

//
// get_ComponentParameter
//
STDMETHODIMP RawComponent::put_ComponentParameter (BSTR name,
                                                   VARIANT newVal)
{
  return S_OK;
}
