/* -*- C++ -*- */

// ============================================================================
// $Id$
//
//
// = FILENAME
//    Video_Repository.h
//
// = DESCRIPTION
//    As a trading service dynamic property, creates a sequence of
//    structures, each structure describing a movie in the video
//    repository.
//
// = AUTHORS
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ============================================================================

#if (! defined TAO_VIDEO_REPOSITORY_H)
#define TAO_VIDEO_REPOSITORY_H

#include "orbsvcs/Trader/Trader_Utils.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Video_RepositoryC.h"
#include "Property_Exporter.h"

class TAO_Video_Repository :
  public TAO_Dynamic_Property,
  public TAO_Exportable
// = DESCRIPTION
//   When evalDP is invoked by the Dynamic Property Dispatcher, the
//   TAO_Video_Repository parses the database file and creates a Movie
//   Sequence, each structure of which describes a movie in the
//   database. The evalDP then crams the sequence into a dynamically
//   allocated Any, which it returns to the dispatcher.
//
//   The format of the database file is the following:
//
//   <file name>%<Movie Name>%<URL for description>
{
public:

  static const char* MOVIE_INFO;

  TAO_Video_Repository (const char* filename);
  // TAO_Video_Repository requires the pathname of the video
  // repository database.

  ~TAO_Video_Repository (void);

  virtual CORBA::Any* evalDP (const char* name,
                              CORBA::TypeCode_ptr returned_type,
                              const CORBA::Any& extra_info,
                              CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure));

  virtual void export_properties (TAO_Property_Exporter& prop_exporter);

  virtual int define_properties
    (CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq,
     CORBA::ULong offet = 0) const;

private:

  void parse_file (const char* database, int num_lines);

  static void obtain_movie_info (const char* file_name,
                                 TAO_VR::Movie& movie) {}
  // Extract the techincal information from the actual MPEG file.

  TAO_Video_Repository (void) {}
  TAO_Video_Repository& operator= (const TAO_Video_Repository& rep)
    { return *this; }
  // TAO_Video_Repository requires the path of the video repository
  // database.

  const char* filename_;
  TAO_VR::Movie* movie_info_;
  int num_movies_;
  time_t lastchanged_;
};

#endif /* TAO_VIDEO_REPOSITORY_H */
