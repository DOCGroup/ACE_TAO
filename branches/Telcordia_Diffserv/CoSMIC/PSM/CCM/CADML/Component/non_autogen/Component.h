#ifndef GME_INTERPRETER_H
#define GME_INTERPRETER_H

#include "Builder.h"
#include "ccm_assembly.h"
#include "UdmDom.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define NEW_BON_INVOKE
//#define DEPRECATED_BON_INVOKE_IMPLEMENTED

/**
 * @Class: CComponent
 *
 *          Bone related class
 */
class CComponent {
public:
  CComponent() : focusfolder(NULL), interaction_(false) { }
  CBuilderFolder *focusfolder;
  CBuilderFolderList selectedfolders;

  void set_file_path(const string& filepath)
  { gen_file_path_ = filepath;}

  void set_xslt_path(const string& xsltpath)
  { xslt_path_ = xsltpath;}

  void InvokeEx(CBuilder &builder,CBuilderObject *focus, CBuilderObjectList &selected, long param);
private:
  bool interaction_;
  string gen_file_path_;
  string xslt_path_;
};


/**
 * @Class: Generate the assembly descriptor.
 * @Note:   it only look into the interaction_folder.
 *          This is the BON side program but it calls the UDM API
 *          Every interaction_folder in the GME project will have
 *          one and only one object of this class.
 */
class Assembly_Descriptor
{
public:
  // There could be multiple descriptors in one project.
  // this type defines the map to store all of them.
  // The key is the name of the assembly.
  typedef std::map<string, UdmDom::DomDataNetwork *> DataNetwrok_Map;

  // The following types are for the compnent info used in partioning part
  typedef struct
  {
    CBuilderModel * component_;
    bool placed_;
  } Component_Info;

  typedef std::map<string, Component_Info> Component_Info_Map;

  // Constructor which takes the pointer to the interaction folder.
  // The Ctr initialize the datanetwork.
  Assembly_Descriptor(CBuilderModel * model, CComponent& parent_component)
    : assembly_model_(model),
      component_(parent_component) { ; }

  // Destructor
  ~Assembly_Descriptor(void);

  // Method that traverses the whole interaction folder.
  void traverse(void);

  // Method that scan the assembly_descriptor model and generate the XML.
  // Note: This method is for the "componentfile" part.
  void make_componentfiles(CBuilderModel * model,  ccm_assembly::componentassembly& new_descriptor);

  /**
   *  @Method: compose the partition part of the xml assembly descriptor.
   */
  void make_partitioning(CBuilderModel * model, ccm_assembly::componentassembly& new_descriptor);

  // @Method: compose the processcollocation part of the xml assembly descriptor.
  void make_processcollocation(CBuilderObject * model, ccm_assembly::Object& parent_obj);

  // @Method: compose the homeplacement part of the xml assembly descriptor.
  void make_homeplacement(CBuilderObject * home, ccm_assembly::Object& parent_obj);

  // @Method: register the compoent/event_source/interface with naming/tradingservice&WriteToIOR.
  void registercompoent(CBuilderObject * curr_object, ccm_assembly::Object& parent_obj);

  // @Method: that scan the assembly_descriptor model and generate the XML.
  // Note: This method is for the "connections" part.
  void make_connections(CBuilderModel * model,  ccm_assembly::componentassembly& new_descriptor);

  // @Method: little help method in the connection part.Help to generate the componentinstantiationref.
  void make_componentinstantiationref(CBuilderObject * curr_obj_ptr, ccm_assembly::Object& parent_obj);

  // @Method: write the destination for host/process/home when its specified.
  void make_destination(CBuilderObject * curr_object, ccm_assembly::Object& parent_obj);

  // @Method: for event port to find out the associated event type, get the
  //  the name of the associated the refence of the event type(could be different from the name of the event type)
  string get_event_ref_name(CBuilderObject * event_port);

private:

  // The XML backend datanetwork.
  DataNetwrok_Map data_network_map_;

  // The pointer to the BON interaction_folder object in GME.
  CBuilderModel * assembly_model_;

  // To store the info of components for partioning.
  Component_Info_Map component_info_map_;

  // Pointer to the Compoent object which created this
  CComponent& component_;

  // Get the path of a foler.
  bool get_path (const string& description, string& path);

};

// Globle method to do string conversion.
void convert_string(string& file_name);

#endif // whole file
