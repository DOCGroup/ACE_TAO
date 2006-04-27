// $Id$

#include "Service_Type_Exporter.h"

ACE_RCSID(Trading, Service_Type_Exporter, "$Id$")

TAO_Service_Type_Exporter::
TAO_Service_Type_Exporter (CosTrading::Lookup_ptr lookup_if,
                           CORBA::Boolean verbose
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  : verbose_ (verbose),
    lookup_ (lookup_if)
{
  // Obtain the Service Type Repository.
  CosTrading::TypeRepository_var obj = lookup_if->type_repos (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Narrow the Service Type Repository.
  this->repos_ = CosTradingRepos::ServiceTypeRepository::_narrow (obj.in ()
                                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Build the service type descriptions.
  this->create_types ();
}

void
TAO_Service_Type_Exporter::remove_all_types (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::IllegalServiceType,
                   CosTrading::UnknownServiceType,
                   CosTradingRepos::ServiceTypeRepository::HasSubTypes))
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Service_Type_Exporter::removing all"
              " types from the Repository.\n"));

  for (int i = NUM_TYPES - 1; i >= 0; i--)
    {
      ACE_TRY
        {
          this->repos_->remove_type (TT_Info::INTERFACE_NAMES[i] ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CosTrading::UnknownServiceType, excp)
        {
          if (this->verbose_)
            {
              if (excp.type.in () != 0)
                ACE_DEBUG ((LM_DEBUG, "Service type not yet registered: %s\n", excp.type.in ()));
            }

        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "TAO_Service_Type_Exporter::remove_all_types");
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

void
TAO_Service_Type_Exporter::add_all_types (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::IllegalServiceType,
                   CosTradingRepos::ServiceTypeRepository::ServiceTypeExists,
                   CosTradingRepos::ServiceTypeRepository::InterfaceTypeMismatch,
                   CosTrading::IllegalPropertyName,
                   CosTrading::DuplicatePropertyName,
                   CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition,
                   CosTrading::UnknownServiceType,
                   CosTradingRepos::ServiceTypeRepository::DuplicateServiceTypeName))

{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Service_Type_Exporter::"
              "adding all types to the Repository.\n"));
  this->add_all_types_to (this->repos_.in () ACE_ENV_ARG_PARAMETER);
  // ACE_CHECK;
}

void
TAO_Service_Type_Exporter::add_all_types_to_all (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::IllegalServiceType,
                   CosTradingRepos::ServiceTypeRepository::ServiceTypeExists,
                   CosTradingRepos::ServiceTypeRepository::InterfaceTypeMismatch,
                   CosTrading::IllegalPropertyName,
                   CosTrading::DuplicatePropertyName,
                   CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition,
                   CosTrading::UnknownServiceType,
                   CosTradingRepos::ServiceTypeRepository::DuplicateServiceTypeName))
{
  ACE_DEBUG ((LM_DEBUG, "*** TAO_Service_Type_Exporter::"
              "add all types to all repositories.\n"));

  ACE_DEBUG ((LM_DEBUG, "Obtaining link interface.\n"));
  CosTrading::Link_var link_if = this->lookup_->link_if (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Obtaining references to traders directly"
              " linked to the root trader.\n"));
  CosTrading::LinkNameSeq_var link_name_seq = link_if->list_links (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Exporting service types with each of the linked"
              " traders.\n"));
  for (CORBA::ULong i = link_name_seq->length () - 1; i > 0; i--)
    {
      CosTradingRepos::ServiceTypeRepository_ptr str = 0;
      ACE_TRY
        {
          ACE_DEBUG ((LM_DEBUG, "Getting link information for %s\n",
                      static_cast<const char*> (link_name_seq[i])));
          CosTrading::Link::LinkInfo_var link_info =
            link_if->describe_link (link_name_seq[i] ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG, "Adding service types to %s\n",
                      static_cast<const char*> (link_name_seq[i])));

          CosTrading::TypeRepository_var remote_repos =
            link_info->target->type_repos (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          str =
            CosTradingRepos::ServiceTypeRepository::_narrow (remote_repos.in ()
                                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }
      ACE_CATCHANY
        {
          // @@ Seth, Ignore all these exceptions?
        }
      ACE_ENDTRY;
      ACE_CHECK;

      // @@ Seth, But this one?
      this->add_all_types_to (str ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_Service_Type_Exporter::
add_all_types_to (CosTradingRepos::ServiceTypeRepository_ptr repos
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::IllegalServiceType,
                   CosTradingRepos::ServiceTypeRepository::ServiceTypeExists,
                   CosTradingRepos::ServiceTypeRepository::InterfaceTypeMismatch,
                   CosTrading::IllegalPropertyName,
                   CosTrading::DuplicatePropertyName,
                   CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition,
                   CosTrading::UnknownServiceType,
                   CosTradingRepos::ServiceTypeRepository::DuplicateServiceTypeName))
{
  for (int i = 0; i < NUM_TYPES; i++)
    {
      ACE_TRY
        {
          repos->add_type (TT_Info::INTERFACE_NAMES[i],
                           this->type_structs_[i].if_name,
                           this->type_structs_[i].props,
                           this->type_structs_[i].super_types
                           ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CosTradingRepos::ServiceTypeRepository::ServiceTypeExists, ste)
        {
          ACE_PRINT_EXCEPTION (ste, "TAO_Service_Type_Exporter::add_all_types");

          if (ste.name.in () != 0)
            ACE_DEBUG ((LM_DEBUG, "Invalid name: %s\n", ste.name.in ()));
        }
      ACE_CATCH (CosTrading::IllegalPropertyName, excp)
        {
          ACE_PRINT_EXCEPTION (excp, "TAO_Service_Type_Exporter::add_all_types");

          if (excp.name.in () != 0)
            ACE_DEBUG ((LM_DEBUG, "Invalid name: %s\n", excp.name.in ()));
        }
      ACE_CATCH (CosTradingRepos::ServiceTypeRepository::ValueTypeRedefinition, vtr)
        {
          ACE_PRINT_EXCEPTION (vtr, "TAO_Service_Type_Exporter::add_all_types");

          if (vtr.type_1.in () != 0)
            ACE_DEBUG ((LM_DEBUG, "Type One: %s\n", vtr.type_2.in ()));
          if (vtr.type_2.in () != 0)
            ACE_DEBUG ((LM_DEBUG, "Type Two: %s\n", vtr.type_2.in ()));
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "TAO_Service_Type_Exporter::add_all_types");
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

void
TAO_Service_Type_Exporter::list_all_types (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes sst;
      ACE_DEBUG ((LM_DEBUG, "*** TAO_Service_Type_Exporter::"
                  "listing all types in the Repository.\n"));

      sst.all_ (1);
      CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq_var type_names =
        this->repos_->list_types (sst ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong l = type_names->length ();
      for (CORBA::ULong i = 0; i != l; ++i)
        {
          CORBA::ULong index = l - 1 - i;
          if (this->verbose_)
            {
              ACE_DEBUG ((LM_DEBUG, "type name: %s\n",
                          static_cast<const char *> (type_names[index])));
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_Service_Type_Exporter::list_all_types");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  // ACE_CHECK;
}

void
TAO_Service_Type_Exporter::describe_all_types (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::IllegalServiceType,
                   CosTrading::UnknownServiceType))
{
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "*** TAO_Service_Type_Exporter::"
                  "describing all types in the Repository.\n"));

      for (int i = 0; i < NUM_TYPES; i++)
        {
          CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct =
            this->repos_->describe_type (TT_Info::INTERFACE_NAMES[i]
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (this->verbose_)
            {
              this->dump_typestruct (TT_Info::INTERFACE_NAMES[i], type_struct.in ());
              ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_Service_Type_Exporter::describe_all_types");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
TAO_Service_Type_Exporter::fully_describe_all_types (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::IllegalServiceType,
                   CosTrading::UnknownServiceType))
{
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "*** TAO_Service_Type_Exporter::"
                  "fully describing all types in the Repository.\n"));

      for (int i = 0; i < NUM_TYPES; i++)
        {
          CosTradingRepos::ServiceTypeRepository::TypeStruct_var type_struct =
            this->repos_->fully_describe_type (TT_Info::INTERFACE_NAMES[i]
                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (this->verbose_)
            {
              this->dump_typestruct (TT_Info::INTERFACE_NAMES[i], type_struct.in ());
              ACE_DEBUG ((LM_DEBUG, "------------------------------\n"));
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_Service_Type_Exporter::fully_describe_all_types");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
TAO_Service_Type_Exporter::
dump_typestruct (const char* type_name,
                 const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct) const
{
  const char* mode_str[] =
    {
      "Normal",
      "Read only",
      "Mandatory",
      "Mandatory and Readonly"
    };

  ACE_DEBUG ((LM_DEBUG, "Type Name: %s\n", type_name));
  ACE_DEBUG ((LM_DEBUG, "Interface Name: %s\n", type_struct.if_name.in ()));

  int i = 0;
  for (i = type_struct.super_types.length () - 1; i >= 0; i--)
    {
      ACE_DEBUG ((LM_DEBUG, "Super Type: %s\n",
                  (const char *) type_struct.super_types[i]));
    }

  for (i = type_struct.props.length () - 1; i >= 0; i--)
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
  this->type_structs_[TT_Info::REMOTE_IO].props.length (6);
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
    CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
  this->type_structs_[TT_Info::REMOTE_IO].props[2].name       =
    TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::DESCRIPTION];
  this->type_structs_[TT_Info::REMOTE_IO].props[2].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::REMOTE_IO].props[2].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::REMOTE_IO].props[3].name       =
    TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::HOST_NAME];
  this->type_structs_[TT_Info::REMOTE_IO].props[3].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::REMOTE_IO].props[3].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
  this->type_structs_[TT_Info::REMOTE_IO].props[4].name       =
    TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::TRADER_NAME];
  this->type_structs_[TT_Info::REMOTE_IO].props[4].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::REMOTE_IO].props[4].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::REMOTE_IO].props[5].name       =
    TT_Info::REMOTE_IO_PROPERTY_NAMES[TT_Info::MISCELLANEOUS];
  this->type_structs_[TT_Info::REMOTE_IO].props[5].value_type =
    CORBA::TypeCode::_duplicate (CORBA::_tc_string);
  this->type_structs_[TT_Info::REMOTE_IO].props[5].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;

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
  this->type_structs_[TT_Info::PLOTTER].props[4].name       =
    TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_USER_QUEUE];
  this->type_structs_[TT_Info::PLOTTER].props[4].value_type =
    CORBA::TypeCode::_duplicate (TAO_Trader_Test::_tc_StringSeq);
  this->type_structs_[TT_Info::PLOTTER].props[4].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PLOTTER].props[5].name       =
    TT_Info::PLOTTER_PROPERTY_NAMES[TT_Info::PLOTTER_FILE_SIZES_PENDING];
  this->type_structs_[TT_Info::PLOTTER].props[5].value_type =
    CORBA::TypeCode::_duplicate (TAO_Trader_Test::_tc_ULongSeq);
  this->type_structs_[TT_Info::PLOTTER].props[5].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PLOTTER].if_name =
    pl._interface_repository_id ();

  TT_Info::Printer pr;
  this->type_structs_[TT_Info::PRINTER].props.length (7);
  this->type_structs_[TT_Info::PRINTER].super_types.length (1);
  this->type_structs_[TT_Info::PRINTER].super_types[0] =
    TT_Info::INTERFACE_NAMES[TT_Info::REMOTE_IO];
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
    CORBA::TypeCode::_duplicate (TAO_Trader_Test::_tc_StringSeq);
  this->type_structs_[TT_Info::PRINTER].props[5].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PRINTER].props[6].name       =
    TT_Info::PRINTER_PROPERTY_NAMES[TT_Info::PRINTER_FILE_SIZES_PENDING];
  this->type_structs_[TT_Info::PRINTER].props[6].value_type =
    CORBA::TypeCode::_duplicate (TAO_Trader_Test::_tc_ULongSeq);
  this->type_structs_[TT_Info::PRINTER].props[6].mode       =
    CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
  this->type_structs_[TT_Info::PRINTER].if_name =
    pr._interface_repository_id ();

  TT_Info::File_System fs;
  this->type_structs_[TT_Info::FILESYSTEM].props.length (3);
  this->type_structs_[TT_Info::FILESYSTEM].super_types.length (1);
  this->type_structs_[TT_Info::FILESYSTEM].super_types[0] =
    TT_Info::INTERFACE_NAMES[TT_Info::REMOTE_IO];
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
