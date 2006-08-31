// $Id$

#include "Preprocess_Data.h"

#if !defined (__CUTS_INLINE__)
#include "Preprocess_Data.inl"
#endif

#include "Uml.h"
#include "boost/bind.hpp"
#include <algorithm>

//
// CUTS_BE_Preprocess_Data
//
CUTS_BE_Preprocess_Data::CUTS_BE_Preprocess_Data (void)
{

}

//
// ~CUTS_BE_Preprocess_Data
//
CUTS_BE_Preprocess_Data::~CUTS_BE_Preprocess_Data (void)
{

}

//
// clear
//
void CUTS_BE_Preprocess_Data::clear (void)
{
  this->headers_.clear ();
  this->includes_.clear ();
  this->libpaths_.clear ();
  this->libs_.clear ();
}

//
// Visit_Component
//
void CUTS_BE_Preprocess_Data::
Visit_Component (const PICML::Component & component)
{
  typedef std::vector <PICML::WorkerType> WType_Set;
  WType_Set types = component.WorkerType_children ();

  std::for_each (types.begin (),
                 types.end (),
                 boost::bind (&WType_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));
}


//
// Visit_WorkerType
//
void CUTS_BE_Preprocess_Data::
Visit_WorkerType (const PICML::WorkerType & type)
{
  PICML::Worker worker = type.ref ();

  if (worker != Udm::null)
    worker.Accept (*this);
}

//
// Visit_Worker
//
void CUTS_BE_Preprocess_Data::
Visit_Worker (const PICML::Worker & worker)
{
  PICML::MgaObject parent = worker.parent ();

  if ((std::string)parent.type ().name () ==
      (std::string)PICML::Package::meta.name ())
  {
    PICML::WorkerPackage::Cast (parent).Accept (*this);
  }
  else
  {
    PICML::WorkerFile::Cast (parent).Accept (*this);
  }
}

//
// Visit_WorkerPackage
//
void CUTS_BE_Preprocess_Data::
Visit_WorkerPackage (const PICML::WorkerPackage & package)
{
  PICML::MgaObject parent = package.parent ();

  while ((std::string)parent.type ().name () ==
         (std::string)PICML::WorkerPackage::meta.name ())
  {
    parent = PICML::MgaObject::Cast (parent.parent ());
  }

  PICML::WorkerFile::Cast (parent).Accept (*this);
}

//
// Visit_WorkerFile
//
void CUTS_BE_Preprocess_Data::
Visit_WorkerFile (const PICML::WorkerFile & file)
{
  // Insert the name of this file as a header.
  this->headers_.insert ((std::string)file.name ());

  // Save the location of this file for later usage.
  std::string location = file.Location ();
  if (!location.empty ())
    this->includes_.insert (location);

  // Let's go the library for this file.
  PICML::WorkerLibrary library = file.WorkerLibrary_parent ();
  library.Accept (*this);
}

//
// Visit_WorkerLibrary
//
void CUTS_BE_Preprocess_Data::
Visit_WorkerLibrary (const PICML::WorkerLibrary & library)
{
  this->libs_.insert (library.name ());

  std::string location = library.Location ();
  if (!location.empty ())
    this->libpaths_.insert (location);
}
