
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  @author Source code used in TAO has been modified and adapted from thecode provided in the book
 *  @author "Advanced CORBA Programming with C++"by Michi Henning and Steve Vinoski. Copyright1999. Addison-Wesley
 *  @author Reading
 *  @author MA.  Used with permission ofAddison-Wesley.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================


#include <ace/streams.h>
#include <strstream>
#include "server.h"
#include <algorithm>
#include "icp.h"

// The following headers are #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
// #include <fstream.h>
// #include <iostream.h>

const char *Controller_oid = "Controller";
const unsigned int           DeviceLocator_impl::MAX_EQ_SIZE = 100;
// Generic ostream inserter for exceptions. Inserts the exception
// name, if available, and the repository ID otherwise.

#if 0   // This inserter may or may not be needed for your ORB.

static ostream &
operator<< (ostream & os, const CORBA::Exception & e)
{
  CORBA::Any tmp;
  tmp <<= e;

  CORBA::TypeCode_var tc = tmp.type ();
  const char * p = tc->name ();
  if  (*p != '\0')
    os << p;
  else
    os << tc->id ();
  return os;
}

#endif

// Helper function to create object references.

static CCS::Thermometer_ptr
make_dref (PortableServer::POA_ptr poa, CCS::AssetType anum)
{
  // Convert asset number to OID.
  ostrstream ostr;
  ostr << anum << ends;
  char * anum_str = ostr.str ();
  PortableServer::ObjectId_var oid
    = PortableServer::string_to_ObjectId (anum_str);
  delete[] anum_str;

  // Look at the model via the network to determine
  // the repository ID.
  char buf[32];
  assert (ICP_get (anum, "model", buf, sizeof (buf)) == 0);
  const char * rep_id = ACE_OS::strcmp (buf, "Sens-A-Temp") == 0
    ? "IDL:acme.com/CCS/Thermometer:1.0"
    : "IDL:acme.com/CCS/Thermostat:1.0";

  // Make a new reference.
  CORBA::Object_var obj
    = poa->create_reference_with_id (oid.in (), rep_id);
  return CCS::Thermometer::_narrow (obj.in ());
}

Controller_impl * Thermometer_impl::m_ctrl;     // static member

// Helper function to read the model string from a device.

CCS::ModelType
Thermometer_impl::
get_model ()
{
  char buf[32];
  assert (ICP_get (m_anum, "model", buf, sizeof (buf)) == 0);
  return CORBA::string_dup (buf);
}

// Helper function to read the temperature from a device.

CCS::TempType
Thermometer_impl::
get_temp ()
{
  short temp;
  assert (ICP_get (m_anum, "temperature", &temp, sizeof (temp)) == 0);
  return temp;
}

// Helper function to read the location from a device.

CCS::LocType
Thermometer_impl::
get_loc ()
{
  char buf[32];
  assert (ICP_get (m_anum, "location", buf, sizeof (buf)) == 0);
  return CORBA::string_dup (buf);
}

// Helper function to set the location of a device.

void
Thermometer_impl::
set_loc (const char * loc)
{
  assert (ICP_set (m_anum, "location", loc) == 0);
}

// Constructor.

Thermometer_impl::
Thermometer_impl (CCS::AssetType anum) : m_anum (anum)
{
  m_ctrl->add_impl (anum, this);   // Add self to controller's set
}

// Destructor.

Thermometer_impl::
~Thermometer_impl ()
{
  if  (m_ctrl->exists (m_anum))
    m_ctrl->add_impl (m_anum, 0);    // Clear servant pointer
}

// IDL model attribute.

CCS::ModelType
Thermometer_impl::model ()
{
  return get_model ();
}

// IDL asset_num attribute.

CCS::AssetType
Thermometer_impl::asset_num ()
{
  return m_anum;
}

// IDL temperature attribute.

CCS::TempType
Thermometer_impl::temperature ()
{
  return get_temp ();
}

// IDL location attribute accessor.

CCS::LocType
Thermometer_impl::location ()
{
  return get_loc ();
}

// IDL remove operation.

void
Thermometer_impl::remove ()
{
  m_ctrl->remove_impl (m_anum);
  assert (ICP_offline (m_anum) == 0);
  // delete this;
}

// IDL location attribute modifier.

void
Thermometer_impl::location (const char *loc)
{
  set_loc (loc);
}

//----------------------------------------------------------------

// Helper function to get a thermostat's nominal temperature.

CCS::TempType
Thermostat_impl::
get_nominal_temp ()
{
  short temp;
  assert (ICP_get (m_anum, "nominal_temp", &temp, sizeof (temp)) == 0);
  return temp;
}

// Helper function to set a thermostat's nominal temperature.

CCS::TempType
Thermostat_impl::
set_nominal_temp (CCS::TempType new_temp)
{
  short old_temp;

  // We need to return the previous nominal temperature,
  // so we first read the current nominal temperature before
  // changing it.
  assert (ICP_get (m_anum, "nominal_temp", &old_temp, sizeof (old_temp)) == 0);

  // Now set the nominal temperature to the new value.
  if  (ICP_set (m_anum, "nominal_temp", &new_temp) != 0)
    {
      // If ICP_set () failed, read this thermostat's minimum and
      // maximum so we can initialize the BadTemp exception.
      CCS::Thermostat::BtData btd;
      ICP_get (m_anum, "MIN_TEMP",
               &btd.min_permitted, sizeof (btd.min_permitted));
      ICP_get (m_anum, "MAX_TEMP",
               &btd.max_permitted, sizeof (btd.max_permitted));
      btd.requested = new_temp;
      btd.error_msg = CORBA::string_dup (new_temp > btd.max_permitted ? "Too hot" : "Too cold");
      throw CCS::Thermostat::BadTemp (btd);
    }
  return old_temp;
}

// Constructor.

Thermostat_impl::
Thermostat_impl (CCS::AssetType anum) : Thermometer_impl (anum)
{
  // Intentionally empty.
}

// Destructor.

Thermostat_impl::
~Thermostat_impl ()
{
  // Intentionally empty.
}

// IDL get_nominal operation.

CCS::TempType
Thermostat_impl::get_nominal ()
{
  return get_nominal_temp ();
}

// IDL set_nominal operation.

CCS::TempType
Thermostat_impl::
set_nominal (CCS::TempType new_temp)
{
  return set_nominal_temp (new_temp);
}

// Helper function to add an entry to the asset map.

void
Controller_impl::
add_impl (CCS::AssetType anum, Thermometer_impl * tip)
{
  m_assets[anum] = tip;
}

// Helper function to remove an entry from the asset map.

void
Controller_impl::
remove_impl (CCS::AssetType anum)
{
  m_assets.erase (anum);
}

// Helper function to locate a servant in the asset map.

bool
Controller_impl::
exists (CCS::AssetType anum)
{
  return m_assets.find (anum) != m_assets.end ();
}

// Constructor

Controller_impl::
Controller_impl (PortableServer::POA_ptr poa,
                 const char *asset_file)
    : m_poa (PortableServer::POA::_duplicate (poa)),
      m_asset_file (asset_file)
{
  std::ifstream afile (m_asset_file.in (), std::ios::in|std::ios::out);//, 0666);
  if  (!afile)
    {
      std::cerr << "Cannot open " << m_asset_file.in () << std::endl;
      throw 0;
    }
  CCS::AssetType anum;

  while  (afile >> anum)
    m_assets[anum] = 0;

  //afile.close ();
  //if  (!afile) {
  //    cerr << "Cannot close " << m_asset_file << endl;
  //    throw 0;
  //}
}

// Destructor

Controller_impl::
~Controller_impl ()
{
  // Write out the current set of asset numbers
  // and clean up all servant instances.
  std::ofstream afile (m_asset_file.in ());
  if  (!afile)
    {
      std::cerr << "Cannot open " << m_asset_file.in () << std::endl;
      assert (0);
    }
  AssetMap::iterator i;

  for  (i = m_assets.begin (); i != m_assets.end (); i++)
    {
      afile << i->first << std::endl;
      if  (!afile)
        {
          std::cerr << "Cannot update " << m_asset_file.in () << std::endl;
          assert (0);
        }
      delete i->second;
    }
  afile.close ();
  if  (!afile)
    {
      std::cerr << "Cannot close " << m_asset_file.in () << std::endl;
      assert (0);
    }
}

CCS::Thermometer_ptr
Controller_impl::
create_thermometer (CCS::AssetType anum, const char *loc)
{
  if  (anum % 2 == 0)
    throw CORBA::BAD_PARAM ();  // Thermometers have odd numbers
  if  (exists (anum))
    throw CCS::Controller::DuplicateAsset ();

  assert (ICP_online (anum) == 0);
  assert (ICP_set (anum, "location", loc) == 0);
  add_impl (anum, 0);
  return make_dref (m_poa.in (), anum);
}

CCS::Thermostat_ptr
Controller_impl::
create_thermostat (CCS::AssetType anum,
                   const char *loc,
                   CCS::TempType temp)
{
  if  (anum % 2 != 0)
    throw CORBA::BAD_PARAM ();   // Thermostats have even numbers
  if  (exists (anum))
    throw CCS::Controller::DuplicateAsset ();

  assert (ICP_online (anum) == 0);
  assert (ICP_set (anum, "location", loc) == 0);
  // Set the nominal temperature.
  if  (ICP_set (anum, "nominal_temp", &temp) != 0)
    {
      // If ICP_set () failed, read this thermostat's minimum
      // and maximum so we can initialize the BadTemp exception.
      CCS::Thermostat::BtData btd;
      ICP_get (anum, "MIN_TEMP",
               &btd.min_permitted, sizeof (btd.min_permitted));
      ICP_get (anum, "MAX_TEMP", &btd.max_permitted, sizeof (btd.max_permitted));
      btd.requested = temp;
      btd.error_msg = CORBA::string_dup (temp > btd.max_permitted ? "Too hot" : "Too cold");
      ICP_offline (anum);
      throw CCS::Thermostat::BadTemp (btd);
    }

  add_impl (anum, 0);
  CORBA::Object_var obj = make_dref (m_poa.in (), anum);
  return CCS::Thermostat::_narrow (obj.in ());
}

// IDL list operation.

CCS::Controller::ThermometerSeq *
Controller_impl::list ()
{
  // Create a new thermometer sequence. Because we know
  // the number of elements we will put onto the sequence,
  // we use the maximum constructor.
  CCS::Controller::ThermometerSeq_var listv
    = new CCS::Controller::ThermometerSeq (m_assets.size ());
  listv->length (m_assets.size ());

  // Loop over the m_assets set and create a
  // reference for each device.
  CORBA::ULong count = 0;
  AssetMap::iterator i;

  for  (i = m_assets.begin (); i != m_assets.end (); i++)
    listv[count++] = make_dref (m_poa.in (), i->first);

  return listv._retn ();
}

// IDL change operation.

void
Controller_impl::
change (const CCS::Controller::ThermostatSeq &  tlist,
        CORBA::Short delta)
{
  CCS::Controller::EChange ec;    // Just in case we need it

  // We cannot add a delta value to a thermostat's temperature
  // directly, so for each thermostat, we read the nominal
  // temperature, add the delta value to it, and write
  // it back again.
  for  (CORBA::ULong i = 0; i < tlist.length (); i++)
    {
      if  (CORBA::is_nil (tlist[i]))
        continue;                       // Skip nil references

      // Read nominal temp and update it.
      CCS::TempType tnom = tlist[i]->get_nominal ();
      tnom += delta;
      try
        {
          tlist[i]->set_nominal (tnom);
        }
      catch  (const CCS::Thermostat::BadTemp &bt)
        {
          // If the update failed because the temperature
          // is out of range, we add the thermostat's info
          // to the errors sequence.
          CORBA::ULong len = ec.errors.length ();
          ec.errors.length (len + 1);
          ec.errors[len].tmstat_ref = tlist[i].in ();
          ec.errors[len].info = bt.details;
        }
    }

  // If we encountered errors in the above loop, we will have added
  // elements to the errors sequence.
  if  (ec.errors.length () != 0)
    throw ec;
}

// IDL find operation

void
Controller_impl::
find (CCS::Controller::SearchSeq & slist)
{
  // Loop over input list and lookup each device.
  CORBA::ULong listlen = slist.length ();
  for  (CORBA::ULong i = 0; i < listlen; i++)
    {

      AssetMap::iterator where;   // Iterator for asset set
      int num_found = 0;          // Num matched per iteration

      // Assume we will not find a matching device.
      slist[i].device = CCS::Thermometer::_nil ();

      // Work out whether we are searching by asset,
      // model, or location.
      CCS::Controller::SearchCriterion sc = slist[i].key._d ();
      if  (sc == CCS::Controller::ASSET)
        {
          // Search for matching asset number.
          where = m_assets.find (slist[i].key.asset_num ());
          if  (where != m_assets.end ())
            slist[i].device = make_dref (m_poa.in (), where->first);
        }
      else
        {
          // Search for model or location string.
          const char *search_str;
          if  (sc == CCS::Controller::LOCATION)
            search_str = slist[i].key.loc ();
          else
            search_str = slist[i].key.model_desc ();

          // Find first matching device  (if any).
          where = find_if (m_assets.begin (), m_assets.end (),
                           StrFinder (sc, search_str));

          // While there are matches...
          while  (where != m_assets.end ())
            {
              if  (num_found == 0)
                {
                  // First match overwrites reference
                  // in search record.
                  slist[i].device = make_dref (m_poa.in (), where->first);
                }
              else
                {
                  // Further matches each append a new
                  // element to the search sequence.
                  CORBA::ULong len = slist.length ();
                  slist.length (len + 1);
                  slist[len].key = slist[i].key;
                  slist[len].device = make_dref (m_poa.in (), where->first);
                }
              num_found++;

              // Find next matching device with this key.
              where = find_if (++where, m_assets.end (),
                               StrFinder (sc, search_str));
            }
        }
    }
}

DeviceLocator_impl::
DeviceLocator_impl (Controller_impl * ctrl) : m_ctrl (ctrl)
{
  // Intentionally empty
}

PortableServer::Servant
DeviceLocator_impl::
preinvoke (const PortableServer::ObjectId & oid,
           PortableServer::POA_ptr          /*poa*/,
           const char *                     operation,
           void * &                         /*cookie*/)
{
  // Convert object id into asset number.
  CORBA::String_var oid_string;
  try
    {
      oid_string = PortableServer::ObjectId_to_string (oid);
    }
  catch  (const CORBA::BAD_PARAM &)
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }

  if  (ACE_OS::strcmp (oid_string.in (), Controller_oid) == 0)
    return m_ctrl;

  istrstream istr (oid_string.in ());
  CCS::AssetType anum;
  istr >> anum;
  if  (istr.fail ())
    throw CORBA::OBJECT_NOT_EXIST ();

  // Check whether the device is known.
  if  (!m_ctrl->exists (anum))
    throw CORBA::OBJECT_NOT_EXIST ();

  // Look at the object map to find out whether
  // we have a servant in memory.
  Thermometer_impl * servant;
  ActiveObjectMap::iterator servant_pos = m_aom.find (anum);
  if  (servant_pos  == m_aom.end ())
    {
      // No servant in memory. If evictor queue is full,
      // evict servant at head of queue.
      if  (m_eq.size () == MAX_EQ_SIZE)
        {
          servant = m_eq.back ();
          m_aom.erase (servant->m_anum);
          m_eq.pop_back ();
          delete servant;
        }
      // Instantiate correct type of servant.
      char buf[32];
      assert (ICP_get (anum, "model", buf, sizeof (buf)) == 0);
      if  (ACE_OS::strcmp (buf, "Sens-A-Temp") == 0)
        servant = new Thermometer_impl (anum);
      else
        servant = new Thermostat_impl (anum);
    }
  else
    {
      // Servant already in memory.
      servant = * (servant_pos->second);   // Remember servant
      m_eq.erase (servant_pos->second);    // Remove from queue

      // If operation is "remove", also remove entry from
      // active object map -- the object is about to be deleted.
      if (ACE_OS::strcmp (operation, "remove") == 0)
        m_aom.erase (servant_pos);
    }

  // We found a servant, or just instantiated it.  If the operation is
  // not a remove, move the servant to the tail of the evictor queue
  // and update its queue position in the map.
  if  (ACE_OS::strcmp (operation, "remove") != 0)
    {
      m_eq.push_front (servant);
      m_aom[anum] = m_eq.begin ();
    }

  return servant;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;

  try
    {
      // Initialize orb
      orb = CORBA::ORB_init (argc, argv);

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      // Get POA manager
      PortableServer::POAManager_var poa_mgr = poa->the_POAManager ();

      // Create a policy list. We use persistent objects with
      // user-assigned IDs, and explicit activation.
      CORBA::PolicyList policy_list;
      policy_list.length (6);
      policy_list[0] = poa->create_lifespan_policy (PortableServer::TRANSIENT);   // REVISIT
      policy_list[1] = poa->create_id_assignment_policy (PortableServer::USER_ID);
      policy_list[2] = poa->create_implicit_activation_policy (PortableServer::NO_IMPLICIT_ACTIVATION);
      policy_list[3] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
      policy_list[4] = poa->create_servant_retention_policy (PortableServer::NON_RETAIN);
      policy_list[5] = poa->create_thread_policy (PortableServer::SINGLE_THREAD_MODEL);

      // Create a POA for all CCS elements.
      PortableServer::POA_var ccs_poa
        = poa->create_POA ("CCS_POA", poa_mgr.in (), policy_list);

      // Create a controller and set static m_ctrl member
      // for thermostats and thermometers.
      Controller_impl ctrl_servant (ccs_poa.in (), "/tmp/CCS_assets");
      Thermometer_impl::m_ctrl = &ctrl_servant;

      // Create a reference for the controller and
      // create the corresponding CORBA object.
      PortableServer::ObjectId_var oid
        = PortableServer::string_to_ObjectId (Controller_oid);
      obj = ccs_poa->create_reference_with_id (oid.in (), "IDL:acme.com/CCS/Controller:1.0");

      // Write a reference for the controller to stdout.
      CORBA::String_var str = orb->object_to_string (obj.in ());
      cout << str.in () << std::endl << std::endl;

      // Instantiate the servant locator for devices.
      PortableServer::ServantManager_var locator =
        new DeviceLocator_impl (&ctrl_servant);

      // Set servant locator.
      ccs_poa->set_servant_manager (locator.in ());

      // Activate the POA manager.
      poa_mgr->activate ();

      // Accept requests
      orb->run ();
    }
  catch  (const CORBA::Exception & e)
    {
      std::cerr << "Uncaught CORBA exception: "
                << e
                << std::endl;
      return 1;
    }
  catch  (...)
    {
      assert (0);  // Uncaught exception, dump core
    }
  return 0;
}
