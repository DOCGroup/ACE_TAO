// $Id$

#include "Predefined_Type_Map.h"
#include "PICML/PICML.h"
#include "Uml.h"

CUTS_Predefined_Type_Map * CUTS_Predefined_Type_Map::instance_ = 0;

//
// CUTS_Predefined_Type_Map
//
CUTS_Predefined_Type_Map::CUTS_Predefined_Type_Map (void)
{
  this->init ();
}

//
// CUTS_Predefined_Type_Map
//
CUTS_Predefined_Type_Map::~CUTS_Predefined_Type_Map (void)
{

}

//
// c_str
//
const char * CUTS_Predefined_Type_Map::c_str (const char * element)
{
  Predefined_Type_Map::const_iterator iter = this->type_map_.find (element);

  return iter != this->type_map_.end () ? iter->second.c_str () : "unknown";
}

//
// init
//
void CUTS_Predefined_Type_Map::init (void)
{
  this->type_map_.insert (
    Predefined_Type_Map::value_type (
    PICML::Boolean::meta.name (), "bool"));

  this->type_map_.insert (
    Predefined_Type_Map::value_type (
    PICML::Byte::meta.name (), "unsigned char"));

  this->type_map_.insert (
    Predefined_Type_Map::value_type (
    PICML::RealNumber::meta.name (), "double"));

  this->type_map_.insert (
    Predefined_Type_Map::value_type (
    PICML::String::meta.name (), "ACE_CString"));

  this->type_map_.insert (
    Predefined_Type_Map::value_type (
    PICML::LongInteger::meta.name (), "long"));

  this->type_map_.insert (
    Predefined_Type_Map::value_type (
    PICML::ShortInteger::meta.name (), "short"));
}

//
// instance
//
CUTS_Predefined_Type_Map * CUTS_Predefined_Type_Map::instance (void)
{
  if (CUTS_Predefined_Type_Map::instance_ == 0)
  {
    CUTS_Predefined_Type_Map::instance_ = new CUTS_Predefined_Type_Map ();
  }

  return CUTS_Predefined_Type_Map::instance_;
}
