// $Id$
#include "Service_Type_Exporter.h"

TAO_Service_Type_Exporter::
TAO_Service_Type_Exporter (CosTradingRepos::ServiceTypeRepository_ptr str)
  : repos_ (str)
{
  this->create_types ();
}

void
TAO_Service_Type_Exporter::remove_all_types (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType, 
		   CosTrading::UnknownServiceType, 
		   SERVICE_TYPE_REPOS::HasSubTypes))
{
  ACE_DEBUG ((LM_DEBUG, "removing all types from the Repository.\n"));

  for (int i = NUM_TYPES - 1; i >= 0; i--)
    {
      TAO_TRY
	{
	  this->repos_->remove_type (TT_Info::INTERFACE_NAMES[i], TAO_TRY_ENV);
	  TAO_CHECK_ENV;  
	}
      TAO_CATCH (CosTrading::UnknownServiceType, excp)
	{
	}
      TAO_CATCHANY
	{
	  TAO_TRY_ENV.print_exception ("TAO_Service_Type_Exporter::remove_all_types");
	  continue;
	  //	  TAO_RETHROW;
	}
      TAO_ENDTRY;
    }
  
}

void
TAO_Service_Type_Exporter::add_all_types (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType, 
		   SERVICE_TYPE_REPOS::ServiceTypeExists, 
		   SERVICE_TYPE_REPOS::InterfaceTypeMismatch, 
		   CosTrading::IllegalPropertyName, 
		   CosTrading::DuplicatePropertyName, 
		   SERVICE_TYPE_REPOS::ValueTypeRedefinition, 
		   CosTrading::UnknownServiceType, 
		   SERVICE_TYPE_REPOS::DuplicateServiceTypeName))
{
  ACE_DEBUG ((LM_DEBUG, "adding all types to the Repository.\n"));
  
  for (int i = 0; i < NUM_TYPES; i++)
    {
      TAO_TRY
	{
	  this->repos_->add_type (TT_Info::INTERFACE_NAMES[i],
				  this->type_structs_[i].if_name,
				  this->type_structs_[i].props,
				  this->type_structs_[i].super_types,
				  TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	}
      TAO_CATCH (SERVICE_TYPE_REPOS::ServiceTypeExists, ste)
	{
	  TAO_TRY_ENV.print_exception ("TAO_Service_Type_Exporter::add_all_types");

	  if (ste.name.in () != 0)	
	    ACE_DEBUG ((LM_DEBUG, "Invalid name: %s\n", ste.name.in ()));

	  goto add_type_label;
	}
      TAO_CATCH (CosTrading::IllegalPropertyName, excp)
	{
	  TAO_TRY_ENV.print_exception ("TAO_Service_Type_Exporter::add_all_types");
	  
	  if (excp.name.in () != 0)	
	    ACE_DEBUG ((LM_DEBUG, "Invalid name: %s\n", excp.name.in ()));
	}
      TAO_CATCH (SERVICE_TYPE_REPOS::ValueTypeRedefinition, vtr)
	{
	  TAO_TRY_ENV.print_exception ("TAO_Service_Type_Exporter::add_all_types");
	  
	  if (vtr.type_1.in () != 0)	
	    ACE_DEBUG ((LM_DEBUG, "Type One: %s\n", vtr.type_2.in ()));
	  if (vtr.type_2.in () != 0)	
	    ACE_DEBUG ((LM_DEBUG, "Type Two: %s\n", vtr.type_2.in ()));
	}
      TAO_CATCHANY
	{
	  TAO_TRY_ENV.print_exception ("TAO_Service_Type_Exporter::add_all_types");
	  TAO_RETHROW;
	}
      TAO_ENDTRY;

    add_type_label: ;
    }
}

void
TAO_Service_Type_Exporter::list_all_types (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  TAO_TRY
    {
      SERVICE_TYPE_REPOS::SpecifiedServiceTypes sst;
      ACE_DEBUG ((LM_DEBUG, "listing all types in the Repository.\n"));
      
      sst._d (SERVICE_TYPE_REPOS::all);
      SERVICE_TYPE_REPOS::ServiceTypeNameSeq_var type_names =
	this->repos_->list_types (sst, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      for (int length = type_names->length (), i = 0; i < length; i++)
	{
	  ACE_DEBUG ((LM_DEBUG, "type name: %s\n", (const char *)type_names[i]));
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Service_Type_Exporter::list_all_types");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
TAO_Service_Type_Exporter::describe_all_types (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType, 
		   CosTrading::UnknownServiceType))
{
  TAO_TRY
    {      
      ACE_DEBUG ((LM_DEBUG, "describing all types in the Repository.\n"));

      for (int i = 0; i < NUM_TYPES; i++)
	{
	  SERVICE_TYPE_REPOS::TypeStruct_var type_struct = 
	    this->repos_->describe_type (TT_Info::INTERFACE_NAMES[i],
					 TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  this->dump_typestruct (TT_Info::INTERFACE_NAMES[i], type_struct.in ());
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Service_Type_Exporter::describe_all_types");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
TAO_Service_Type_Exporter::fully_describe_all_types (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType, 
		   CosTrading::UnknownServiceType))
{
  TAO_TRY
    {      
      ACE_DEBUG ((LM_DEBUG, "fully describing all types in the Repository.\n"));

      for (int i = 0; i < NUM_TYPES; i++)
	{
	  SERVICE_TYPE_REPOS::TypeStruct_var type_struct = 
	    this->repos_->fully_describe_type (TT_Info::INTERFACE_NAMES[i],
					       TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  this->dump_typestruct (TT_Info::INTERFACE_NAMES[i], type_struct.in ());
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Service_Type_Exporter::fully_describe_all_types");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
TAO_Service_Type_Exporter::
dump_typestruct (const char* type_name,
		 const SERVICE_TYPE_REPOS::TypeStruct& type_struct) const
{
  const char* mode_str[] = 
    {
      "Normal",
      "Read only",
      "Mandatory",
      "Mandatory and Readonly"
    };
  
  CORBA::Environment env;
  ACE_DEBUG ((LM_DEBUG, "Type Name: %s\n", type_name));
  ACE_DEBUG ((LM_DEBUG, "Interface Name: %s\n", type_struct.if_name.in ()));

  for (int length = type_struct.super_types.length (), i = 0; i < length; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "Super Type: %s\n",
		  (const char *) type_struct.super_types[i]));
    }

  for (length = type_struct.props.length (), i = 0; i < length; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "Property: %-20s  Mode: %-24s\n",
		  type_struct.props[i].name.in (),
		  mode_str[type_struct.props[i].mode])); 
    }
}

void
TAO_Service_Type_Exporter::create_types (void)
{
  TT_Info::Remote_Output ro;
  this->type_structs_[TT_Info::REMOTE_IO].props.length (3);
  this->type_structs_[TT_Info::REMOTE_IO].props[0].name       =
    TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::NAME];
  this->type_structs_[TT_Info::REMOTE_IO].props[0].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::REMOTE_IO].props[0].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY_READONLY;
  this->type_structs_[TT_Info::REMOTE_IO].props[1].name       =
    TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::LOCATION];
  this->type_structs_[TT_Info::REMOTE_IO].props[1].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::REMOTE_IO].props[1].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::REMOTE_IO].props[2].name       =
    TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::DESCRIPTION];
  this->type_structs_[TT_Info::REMOTE_IO].props[2].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::REMOTE_IO].props[2].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
  this->type_structs_[TT_Info::REMOTE_IO].if_name =
    ro._interface_repository_id ();

  TT_Info::Plotter pl;
  this->type_structs_[TT_Info::PLOTTER].props.length (6);
  this->type_structs_[TT_Info::PLOTTER].super_types.length (1);
  this->type_structs_[TT_Info::PLOTTER].super_types[0] =
    TT_Info::INTERFACE_NAMES[TT_Info::REMOTE_IO]; 
  this->type_structs_[TT_Info::PLOTTER].props[0].name       =
    TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_NUM_COLORS];
  this->type_structs_[TT_Info::PLOTTER].props[0].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_long);
  this->type_structs_[TT_Info::PLOTTER].props[0].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PLOTTER].props[1].name       =
    TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_AUTO_LOADING];
  this->type_structs_[TT_Info::PLOTTER].props[1].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);
  this->type_structs_[TT_Info::PLOTTER].props[1].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_READONLY;
  this->type_structs_[TT_Info::PLOTTER].props[2].name       =
    TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_COST_PER_PAGE];
  this->type_structs_[TT_Info::PLOTTER].props[2].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_float);
  this->type_structs_[TT_Info::PLOTTER].props[2].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PLOTTER].props[3].name       =
    TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_MODEL_NUMBER];
  this->type_structs_[TT_Info::PLOTTER].props[3].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::PLOTTER].props[3].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_READONLY;
  CORBA::Any string_holder, ulong_holder;
  TAO_Sequences::ULongSeq ulong_seq (4);
  TAO_Sequences::StringSeq string_seq (4);

  ulong_holder <<= &ulong_seq;
  string_holder <<= &string_seq;  
  this->type_structs_[TT_Info::PLOTTER].props[4].name       =
    TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_USER_QUEUE];
  this->type_structs_[TT_Info::PLOTTER].props[4].value_type =
    CORBA::TypeCode::_duplicate (string_holder.type ());
  this->type_structs_[TT_Info::PLOTTER].props[4].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PLOTTER].props[5].name       =
    TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_FILE_SIZES_PENDING];
  this->type_structs_[TT_Info::PLOTTER].props[5].value_type =
    CORBA::TypeCode::_duplicate (ulong_holder.type ());
  this->type_structs_[TT_Info::PLOTTER].props[5].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PLOTTER].if_name =
    pl._interface_repository_id ();

  TT_Info::Printer pr;
  this->type_structs_[TT_Info::PRINTER].props.length (7);
  this->type_structs_[TT_Info::PRINTER].super_types.length (1);
  this->type_structs_[TT_Info::PRINTER].super_types[0] = TT_Info::INTERFACE_NAMES[TT_Info::REMOTE_IO];
  this->type_structs_[TT_Info::PRINTER].props[0].name       =
    TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_COLOR];
  this->type_structs_[TT_Info::PRINTER].props[0].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);
  this->type_structs_[TT_Info::PRINTER].props[0].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY_READONLY;
  this->type_structs_[TT_Info::PRINTER].props[1].name       =
    TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_DOUBLE_SIDED];
  this->type_structs_[TT_Info::PRINTER].props[1].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_boolean);
  this->type_structs_[TT_Info::PRINTER].props[1].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_READONLY;
  this->type_structs_[TT_Info::PRINTER].props[2].name       =
    TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_COST_PER_PAGE];
  this->type_structs_[TT_Info::PRINTER].props[2].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_float);
  this->type_structs_[TT_Info::PRINTER].props[2].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PRINTER].props[3].name       =
    TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_MODEL_NUMBER];
  this->type_structs_[TT_Info::PRINTER].props[3].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::PRINTER].props[3].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_READONLY;
  this->type_structs_[TT_Info::PRINTER].props[4].name       =
    TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_PAGES_PER_SEC];
  this->type_structs_[TT_Info::PRINTER].props[4].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);
  this->type_structs_[TT_Info::PRINTER].props[4].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_READONLY;
  this->type_structs_[TT_Info::PRINTER].props[5].name       =
    TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_USER_QUEUE];
  this->type_structs_[TT_Info::PRINTER].props[5].value_type =
    //    CORBA::TypeCode::_duplicate (TAO_Sequences::_tc_StringSeq);
    CORBA::TypeCode::_duplicate (string_holder.type ());
  this->type_structs_[TT_Info::PRINTER].props[5].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PRINTER].props[6].name       =
    TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_FILE_SIZES_PENDING];
  this->type_structs_[TT_Info::PRINTER].props[6].value_type =
    //    CORBA::TypeCode::_duplicate (TAO_Sequences::_tc_ULongSeq);
    CORBA::TypeCode::_duplicate (ulong_holder.type ());
  this->type_structs_[TT_Info::PRINTER].props[6].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PRINTER].if_name =
    pr._interface_repository_id ();

  TT_Info::File_System fs;
  this->type_structs_[TT_Info::FILESYSTEM].props.length (3);
  this->type_structs_[TT_Info::FILESYSTEM].super_types.length (1);
  this->type_structs_[TT_Info::FILESYSTEM].super_types[0] = TT_Info::INTERFACE_NAMES[TT_Info::REMOTE_IO];
  this->type_structs_[TT_Info::FILESYSTEM].props[0].name       =
    TT_Info::FILESYSTEM_PROPERTY_NAMES[TT_Info::DISK_SIZE];
  this->type_structs_[TT_Info::FILESYSTEM].props[0].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);
  this->type_structs_[TT_Info::FILESYSTEM].props[0].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY_READONLY;
  this->type_structs_[TT_Info::FILESYSTEM].props[1].name       =
    TT_Info::FILESYSTEM_PROPERTY_NAMES[TT_Info::SPACE_REMAINING];
  this->type_structs_[TT_Info::FILESYSTEM].props[1].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_ulong);
  this->type_structs_[TT_Info::FILESYSTEM].props[1].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::FILESYSTEM].props[2].name       =
    TT_Info::FILESYSTEM_PROPERTY_NAMES[TT_Info::PERMISSION_LEVEL];
  this->type_structs_[TT_Info::FILESYSTEM].props[2].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);
  this->type_structs_[TT_Info::FILESYSTEM].props[2].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::FILESYSTEM].if_name =
    fs._interface_repository_id ();  

  TT_Info::PostScript_Printer ps;
  this->type_structs_[TT_Info::PS_PRINTER].props.length (1);
  this->type_structs_[TT_Info::PS_PRINTER].super_types.length (1);
  this->type_structs_[TT_Info::PS_PRINTER].super_types[0] =
    TT_Info::INTERFACE_NAMES[TT_Info::PRINTER];
  this->type_structs_[TT_Info::PS_PRINTER].props[0].name       =
    TT_Info::PS_PRINTER_PROPERTY_NAMES[TT_Info::VERSION];
  this->type_structs_[TT_Info::PS_PRINTER].props[0].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_ushort);
  this->type_structs_[TT_Info::PS_PRINTER].props[0].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY_READONLY;
  this->type_structs_[TT_Info::PS_PRINTER].if_name =
    ps._interface_repository_id ();  
}


