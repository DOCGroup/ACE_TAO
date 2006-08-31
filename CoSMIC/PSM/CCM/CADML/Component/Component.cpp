/**
 * Since all the GME Meta interpreter and UDM generated files don't follow the ACE guideline
 * all the interpreter related file will stick with UDM and GME style instead of ACE/TAO style.
 */
#include "stdafx.h"

#include <stdio.h>
#include <afxdlgs.h>
#include <stdlib.h>
#include <windows.h>
//#include <ole2.h>

#include "Component.h"
#include "XSLTransformer.h"

using namespace ccm_assembly;

void CComponent::InvokeEx(CBuilder &builder,CBuilderObject *focus, CBuilderObjectList &selected, long param)
{
  gen_file_path_ = "";
  try
  {
    // First get into the current interaction model.

    CString kind_name = focus->GetKindName();

    if( kind_name.Compare("Assembly_Descriptor") )
    {
      AfxMessageBox("You must start from an Assembly_Descriptor Model!", MB_OK | MB_ICONSTOP);
    }

    else
    {
      {
        // Note: The Assembly_descriptor is only valid in the current scope.
        Assembly_Descriptor assembly_descriptor((CBuilderModel *)focus, *this);

        // Now travers the interaction folder.
        assembly_descriptor.traverse();
      }

      // Here deals with the hack problem.
      if(!gen_file_path_.empty())
      {
        XSLTransformer new_transformer( string(gen_file_path_ + ".tmp").c_str(),
                                        string(xslt_path_ + "\\UDM_CAD_TO_CCM.xsl").c_str(),
                                        gen_file_path_.c_str());
        new_transformer.executeTransform( );
      }

    }
  }
  catch (const udm_exception &e)
  {
    AfxMessageBox(e.what(), MB_OK | MB_ICONSTOP);
  }
}



/**
  * Now starts to generate the assembly descriptor.
  *
  * @Note: Here we only have to look into the interaction folder in the model. Because the assembly
  *        descriptior is almost only related to the interaction foler. I will have all model related
  *        constraints(like a facet-receptacle connection implies the same underlying interface etc.)
  *        enforced in GME constriant manager. Thus in the interpreter we only traverse the model and
  *        generate the desctiptor file.
  */

// Destructor which is reponsible of close all the datanetworks.
Assembly_Descriptor::~Assembly_Descriptor(void)
{
  map<string, UdmDom::DomDataNetwork *>::iterator iter;
  for (iter = data_network_map_.begin(); iter != data_network_map_.end(); iter++ )
  {
    (*iter).second->CloseWithUpdate();
    delete (*iter).second ;
  }
}

// Method that traverse the whole interaction folder.
void Assembly_Descriptor::traverse (void)
{
  // First ask the user to input the dtd file
  // test for the file dialog

  string dialog_description = "Please specify the path/name of the XSLT folder.";
  string xslt_path, file_path;

  // If the user dosent specify any folder then quit the program.
  if(!get_path(dialog_description, xslt_path))
    return;

  // store the path for later's hack method(xslt transform)
  component_.set_xslt_path(xslt_path);


  // Val will be used to store temp CStrings.
  CString val;

  // Start a new datanetwork.
  UdmDom::DomDataNetwork * new_datanetwork = new UdmDom::DomDataNetwork(ccm_assembly::diagram);

  // The default name of the generated XML descriptor file is same as the name of
  //the model with an extension of "cad".
  string descriptor_name = string(assembly_model_->GetName());

  // Now ask the user about the name/path of the generated file.
  dialog_description = "Please specify the path where the generated file should be.";
  if(!get_path(dialog_description, file_path))
    return;

  // Create the new dataNetwork.
  string file_end = file_path + "\\" + descriptor_name + ".cad";
  string dtd_end = xslt_path + "\\ccm_assembly.dtd";

  // store the path for later's hack method(xslt transform)
  component_.set_file_path(file_end);

  new_datanetwork->CreateNew(file_end + ".tmp",
                             dtd_end,
                             ccm_assembly::componentassembly::meta,
                             Udm::CHANGES_PERSIST_ALWAYS);

  // Insert the new datanetwork into the map
  data_network_map_.insert(DataNetwrok_Map::value_type(descriptor_name, new_datanetwork));

  // First create(find) a new componentassebmly(rootobject) of UDM for later use.
  // 1. Use the name of the model as id
  // Get the root object in the CCM dtd.
  componentassembly new_descriptor = componentassembly::Cast(new_datanetwork->GetRootObject());
  new_descriptor.id() = string(assembly_model_->GetName()).c_str();

  // 2. look at the description of the descriptor.
  // If not NULL then insert it into the xml file
  assembly_model_->GetAttribute("Assembly_Description", val);
  if(! val.IsEmpty())
  {
    description new_discriptor_description = description::Create(new_descriptor);
    new_discriptor_description.content() = string(val).c_str();
  }

  // Now pass the work to make_componentfile to generate the XML
  make_componentfiles(assembly_model_, new_descriptor);

  // Now pass the work to make_partition.
  make_partitioning(assembly_model_, new_descriptor);

  // Now pass the work to make_connections
  make_connections(assembly_model_, new_descriptor);

}


// Method that scan the assembly_descriptor model and generate the XML.
// Note: This method is for the "componentfile" part.
// It also include the ID and description part of the compoentassembly.
void Assembly_Descriptor::make_componentfiles(CBuilderModel * model, ccm_assembly::componentassembly& new_descriptor)
{
  CString val;

  /**
   *  Note: Create the componetfiles part.
   *        Here need to create compoentfile tag for every compnent in the new_descriptor.
   */
  componentfiles new_compoentfiles = componentfiles::Create(new_descriptor);

  const CBuilderModelList* component_model_list = model->GetModels("Component_Type");
  POSITION component_pos = component_model_list->GetHeadPosition();

  // For every component create the compoentfile tag
  while(component_pos)
  {
    CBuilderModel * component_ptr = component_model_list->GetNext(component_pos);

    componentfile new_componentfile = componentfile::Create(new_compoentfiles);

    // The id of the compnent file is "com-" + the component name.
    new_componentfile.id() = (string("com-")+string(component_ptr->GetName())).c_str();

    // Now create the fileinarchive tag for every componentfile
    // Note: About filearchive we don't know about the source of the name of file yet.
    //       Now I assume the name of the file is "compnent_name.csd"

    fileinarchive new_fileinarchive = fileinarchive::Create(new_componentfile);
    new_fileinarchive.name() = (string(component_ptr->GetName())+string(".csd")).c_str();

    // Here we have to do some preparation for the later partitioning work.
    // initialize the component_info_map_.
    Component_Info new_component_info;
    new_component_info.component_ = component_ptr;
    new_component_info.placed_ = false;
    component_info_map_.insert
      (Component_Info_Map::value_type(string(component_ptr->GetName()), new_component_info));
  }
}

/**
 *  @Method: compose the partition part of the xml assembly descriptor.
 */
void Assembly_Descriptor::make_partitioning(CBuilderModel * model, ccm_assembly::componentassembly& new_descriptor)
{
  partitioning new_partitioning = partitioning::Create(new_descriptor);

  // 1. find all host_location on the partitioning level
  const CBuilderModelList * hostcollocation_list = model->GetModels("Host_Location");
  POSITION hostcollocation_pos = hostcollocation_list->GetHeadPosition();

  while(hostcollocation_pos)
  {
    CBuilderModel * hostcollocation_ptr = hostcollocation_list->GetNext(hostcollocation_pos);
    hostcollocation new_hostcollocation = hostcollocation::Create(new_partitioning);
    new_hostcollocation.id() = string(hostcollocation_ptr->GetName()).c_str();
    make_destination(hostcollocation_ptr, new_hostcollocation);

    //Now scan all possible process collocation insdie the current hostcollocation.
    const CBuilderAtomList * processcollocation_list = hostcollocation_ptr->GetAtoms("Process_Location");
    POSITION processcollocation_pos = processcollocation_list->GetHeadPosition();
    while(processcollocation_pos)
    {
      CBuilderAtom * processcollocation_ptr = processcollocation_list->GetNext(processcollocation_pos);

      make_processcollocation(processcollocation_ptr, new_hostcollocation);
    }

    // Here should deal with the case when a component is associate with a host_location directly.
    CBuilderObjectList component_list_con;
    hostcollocation_ptr->GetOutConnectedObjects("Host_Location_Association", component_list_con);
    POSITION component_con_pos = component_list_con.GetHeadPosition();

    while(component_con_pos)
    {
      CBuilderObject * component_ptr_con = component_list_con.GetNext(component_con_pos);
      make_homeplacement(component_ptr_con, new_hostcollocation);
    }

  }


  // 2. find all process_location on the partitioning level.
  const CBuilderAtomList * processcollocation_list_e = model->GetAtoms("Process_Location");
  POSITION processcollocation_pos_e = processcollocation_list_e->GetHeadPosition();

  while(processcollocation_pos_e)
  {
    CBuilderAtom * processcollocation_ptr_e = processcollocation_list_e->GetNext(processcollocation_pos_e);

    make_processcollocation(processcollocation_ptr_e, new_partitioning);
  }

  // 3. find all homeplacement on the partitioning level(when a component is not associated
  // with any host_location or process_location).
  Component_Info_Map::const_iterator map_iter = component_info_map_.begin();
  while(map_iter != component_info_map_.end())
  {
    if((*map_iter).second.placed_ == false)
      make_homeplacement((*map_iter).second.component_, new_partitioning);
    map_iter++;
  }

}


void Assembly_Descriptor::make_processcollocation(CBuilderObject * curr_object, ccm_assembly::Object& parent_obj)
{
  processcollocation new_processcollocation = processcollocation::Create(parent_obj);
  new_processcollocation.id() = string(curr_object->GetName()).c_str();
  make_destination(curr_object, new_processcollocation);

  // can all componenthome/instantiation associated with the procss location
  CBuilderObjectList component_list_con;
  curr_object->GetOutConnectedObjects("Process_Location_Association", component_list_con);
  POSITION component_con_pos = component_list_con.GetHeadPosition();

  while(component_con_pos)
  {
    CBuilderObject * component_ptr_con = component_list_con.GetNext(component_con_pos);
    make_homeplacement(component_ptr_con, new_processcollocation);
  }
}

void Assembly_Descriptor::make_homeplacement(CBuilderObject * curr_object, ccm_assembly::Object& parent_obj)
{
  //homeplacement.id
  homeplacement new_homeplacement = homeplacement::Create(parent_obj);
  new_homeplacement.id() = (string("a_") + string(curr_object->GetName()) + string("Home")).c_str();
  make_destination(curr_object, new_homeplacement);

  //componentfileref
  componentfileref new_componentfileref = componentfileref::Create(new_homeplacement);
  new_componentfileref.idref() = (string("com-") + string(curr_object->GetName())).c_str();

  //componentinstantiation
  componentinstantiation new_componentinstantiation = componentinstantiation::Create(new_homeplacement);
  new_componentinstantiation.id() = (string("a_") + string(curr_object->GetName())).c_str();
  {
	//Now deal with registration issue.
	registercompoent(curr_object, new_componentinstantiation);
  }

  //now mark this component home/instantiaiton, so we know which one is left.
  component_info_map_[string(curr_object->GetName())].placed_ = true;
}


//Get the destination of the host/process/home if it is specified
void Assembly_Descriptor::make_destination(CBuilderObject * curr_object, ccm_assembly::Object& parent_obj)
{
  CString val;
  curr_object->GetAttribute("Destination",val);
  if(val.IsEmpty()) return;

  destination new_destination= destination::Create(parent_obj);
  new_destination.content() = string(val).c_str();
}

//Method that deals with Componentregistration tags in the dtd.
/**
 *Here deal with register component
 *1. Detect if there are any register point in the model
 *2. for every point detect if the connection is the kind of writetoior
 *3. do all the registration.
 *Note: naming&/trading service will be introduced in here in the future.
 *      Only component/eventsource/interface(facade) could be registered.
 *      This method could be called for component/event_source/interface.
 */
void Assembly_Descriptor::registercompoent(CBuilderObject * curr_object, ccm_assembly::Object& parent_obj)
{
  const CBuilderConnectionList * registration_con_list = curr_object->GetOutConnections("Registration_Association");
  if (registration_con_list->GetCount() > 0 )
  {
    POSITION registration_con_pos = registration_con_list->GetHeadPosition();
    while(registration_con_pos)
    {
      CBuilderConnection * registration_con_ptr = registration_con_list->GetNext(registration_con_pos);
      //Now look at the connection to see if it's of type "writetoior".
      CString val;
      registration_con_ptr->GetAttribute("Registration_Method", val);

      if(!val.Compare("writetoior"))
      {
        // This line should be outside of current scope. Its here for the reason that
        // we are not considering the case of naming/trading service.
        registercomponent new_registercomponent = registercomponent::Create(parent_obj);

        // Get the registraiton_point.
        CBuilderObject * registration_point_ptr = registration_con_ptr->GetDestination();
        registration_point_ptr->GetAttribute("Registration_Description",val);
        writeiortofile new_writeiortofile = writeiortofile::Create(new_registercomponent);
        new_writeiortofile.name() = string(val).c_str();
      }

      else { /*Deals with naming/trading service */}

    }
  }

  // If no registration found for the component,
  // But still we need to look inside the component to see if there are any registration for event_source/interface.
  else
  {
    //scan inside of the component for registration.
  }
}

// Method that scan the assembly_descriptor model and generate the XML.
// Note: This method is for the "componentfile" part.
void Assembly_Descriptor::make_connections(CBuilderModel * model,  ccm_assembly::componentassembly& new_descriptor)
{
  connections new_connections = connections::Create(new_descriptor);

  // 1. For all event connections.
  const CBuilderConnectionList * event_con_list = model->GetConnections("Event_Connection");
  POSITION event_con_pos = event_con_list->GetHeadPosition();
  while(event_con_pos)
  {
    CBuilderConnection * event_con_ptr = event_con_list->GetNext(event_con_pos);
    connectevent new_connectevent = connectevent::Create(new_connections);

    // Get the destination of the connection.
    {
      CBuilderObject * event_sink_ptr = event_con_ptr->GetDestination();
      consumesport new_consumesport = consumesport::Create(new_connectevent);

      // Construct the consumesidentifier
      consumesidentifier new_consumesidentifier = consumesidentifier::Create(new_consumesport);
      ////new_consumesidentifier.content() = get_event_ref_name(event_sink_ptr).c_str();
      new_consumesidentifier.content() = string(event_sink_ptr->GetName());

      // Construct the componentinstantiationref
      make_componentinstantiationref(event_sink_ptr, new_consumesport);
    }

    // Get the source of the connection.
    {
      CBuilderObject * event_source_ptr = event_con_ptr->GetSource();

      // For event source need to differenciate between publisher and emitter.
      CString val;
      event_source_ptr->GetAttribute("Event_Source_Type", val);
      if(!val.Compare("Publisher"))
      {
        publishesport new_publishesport = publishesport::Create(new_connectevent);

        // Construct the publishesidentifier
        publishesidentifier new_publishesidentifier = publishesidentifier::Create(new_publishesport);
        ////new_publishesidentifier.content() = get_event_ref_name(event_source_ptr).c_str();
        new_publishesidentifier.content() = string(event_source_ptr->GetName());

        // Construct the componentinstantiationref
        make_componentinstantiationref(event_source_ptr, new_publishesport);
      }
      else
      {
        emitsport new_emitsport = emitsport::Create(new_connectevent);

        // Construct the emitsidentifier
        emitsidentifier new_emitsidentifier = emitsidentifier::Create(new_emitsport);
        ////new_emitsidentifier.content() = get_event_ref_name(event_source_ptr).c_str();
        new_emitsidentifier.content() = string(event_source_ptr->GetName());

        // Construct the componentinstantiationref
        make_componentinstantiationref(event_source_ptr, new_emitsport);

      }
    }
  }


  // 2. For all interface connections.
  const CBuilderConnectionList * interface_con_list = model->GetConnections("Facet_Receptacle_Connection");
  POSITION interface_con_pos = interface_con_list->GetHeadPosition();
  while(interface_con_pos)
  {
    CBuilderConnection * interface_con_ptr = interface_con_list->GetNext(interface_con_pos);
    connectinterface new_connectinterface = connectinterface::Create(new_connections);

    // Get the destination of the connection.
    {
      CBuilderObject * facet_ptr = interface_con_ptr->GetDestination();
      providesport new_providesport = providesport::Create(new_connectinterface);

      // Construct the providesideidentifier
      providesidentifier new_providesidentifier = providesidentifier::Create(new_providesport);
      new_providesidentifier.content() = string(facet_ptr->GetName()).c_str();

      // Construct the componentinstantiationref
      make_componentinstantiationref(facet_ptr, new_providesport);
    }

    // Get the source of the connection.
    {
      CBuilderObject * receptacle_ptr = interface_con_ptr->GetSource();
      usesport new_usesport = usesport::Create(new_connectinterface);

      // Construct the usesidendifier
      usesidentifier new_usesidentifier = usesidentifier::Create(new_usesport);
      new_usesidentifier.content() = string(receptacle_ptr->GetName()).c_str();

      // Construct the componentinstantiationref
      make_componentinstantiationref(receptacle_ptr, new_usesport);
    }
  }

}

// @Method: little help method in the connection part.
//          help to generate the componentinstantiationref.
// @Procedure: 1. create the new XML end object
//             2. get the parent object in GME(should be the compoent_type)
//             3. get the name of the (event sink/source)/(facede/receptacle)
void Assembly_Descriptor::make_componentinstantiationref(CBuilderObject * curr_obj_ptr, ccm_assembly::Object& parent_obj)
{
  componentinstantiationref new_componentinstantiationref = componentinstantiationref::Create(parent_obj);
  const CBuilderModel* component_ptr = curr_obj_ptr->GetParent();
  new_componentinstantiationref.idref() =
    (string("a_") + string(component_ptr->GetName())).c_str();
}

// @Method: for event port to find out the associated event type, get the
// the name of the associated the refence of the event type(could be different from the name of the event type)
string Assembly_Descriptor::get_event_ref_name(CBuilderObject * event_port)
{

  AfxMessageBox(event_port->GetName());
  // Even though we have to use list here, only one event possible.
  CBuilderObjectList event_type_ref_list;
  event_port->GetOutConnectedObjects("Event_Typing", event_type_ref_list);
  int i = event_type_ref_list.GetCount();
  AfxMessageBox(i);
  const CBuilderObject * event_ref = event_type_ref_list.GetHead();
  AfxMessageBox("4");
  AfxMessageBox(event_ref->GetName());

  return string(event_ref->GetName());
}

// Globle method to do string conversion.
void convert_string(string& file_name)
{
  using namespace std;
  bool done = false;
  static const basic_string <char>::size_type npos = -1;
  size_t index = 0;

  while(!done)
  {
    index = file_name.find("\\", index);
    if(index == npos) done = true;
    else
    { file_name.replace(index, 1,"/");
      index += 1;
    }
  }
}

// This method prompt a dialog to allow the user to specify a folder
bool Assembly_Descriptor::get_path (const string& description, string& path)
{
  // Initalize the com library
  //WINOLEAPI com_lib_return = OleInitialize(NULL);

  // Dialog instruction
  char display_buffer[MAX_PATH];
  BROWSEINFO folder_browsinfo;
  memset ( &folder_browsinfo, 0, sizeof (folder_browsinfo));

  // Set GME as the owner of the dialog
  folder_browsinfo.hwndOwner = GetForegroundWindow();
  // Start the brows from desktop
  folder_browsinfo.pidlRoot = NULL;
  // Pointer to the folder name display buffer
  folder_browsinfo.pszDisplayName = &display_buffer[0];
  // Diaglog instruction string
  folder_browsinfo.lpszTitle = description.c_str();
  // Use new GUI style and allow edit plus file view
  folder_browsinfo.ulFlags = //BIF_USENEWUI
                           //|
                           BIF_BROWSEINCLUDEFILES
                           | BIF_RETURNONLYFSDIRS;
  // No callback function
  folder_browsinfo.lpfn = NULL;
  // No parameter passing into the dialog
  folder_browsinfo.lParam = 0;


  LPITEMIDLIST folder_pidl;
  folder_pidl = SHBrowseForFolder(&folder_browsinfo);

  if(folder_pidl == NULL)
    return false;
  else
  {
	  TCHAR FolderNameBuffer[MAX_PATH];

		// Convert the selection into a path
		if (SHGetPathFromIDList (folder_pidl, FolderNameBuffer))
		{
	    path = FolderNameBuffer;
      //AfxMessageBox(path.c_str());
		}

    // Free the ItemIDList object returned from the call to
	  // SHBrowseForFolder using Gawp utility function
    IMalloc * imalloc = 0;
    if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
    {
      imalloc->Free ( folder_pidl );
      imalloc->Release ( );
    }
  }
  return true;
}
