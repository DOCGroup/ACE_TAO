/* -*- C++ -*- */

// ============================================================================
// $Id$
//
// = FILENAME
//    Machine_Properties.h
//
// = DESCRIPTION
//    As a trading service dynamic property, retrieves machine
//    statistics from the rstatd using Sun RPC.
//
// = AUTHORS
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ============================================================================

#if (! defined TAO_MACHINE_PROPERTIES)
#define TAO_MACHINE_PROPERTIES

#include "orbsvcs/Trader/Trader_Utils.h"
#include "Property_Exporter.h"

//RPC related includes
#include <rpc/rpc.h>
#include <rpcsvc/rstat.h>

class TAO_Machine_Properties :
  public TAO_Dynamic_Property,
  public TAO_Exportable
{
public:

  enum PROP_TYPES
  {
    CPU,
    DISK,
    PAGES,
    SWAPS,
    PACKETS,
    ERRORS,
    CONTEXT_SWITCHES,
    COLLISIONS,
    INTERRUPTS,
    LOAD
  };

  static const int NUM_PROPERTIES;
  static const char* PROP_NAMES[];

  TAO_Machine_Properties (void);

  TAO_Machine_Properties (const ACE_Time_Value& timeout);

  virtual CORBA::Any* evalDP (const char* name,
                              CORBA::TypeCode_ptr returned_type,
                              const CORBA::Any& extra_info,
                              CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure));
  // Return the value of a machine performance property whose name is
  // contained in <extra_info>, which in essence is another
  // demultiplexing key.

  virtual void export_properties (TAO_Property_Exporter& prop_exporter);

  virtual int define_properties
    (CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq,
     CORBA::ULong offset = 0) const;

private:

  void init (void);

  int retrieve_stats (void);

  void compute_cpu (CORBA::Any& value, int elapsed_time);
  void compute_disk (CORBA::Any& value, int elapsed_time);
  void compute_pages (CORBA::Any& value, int elapsed_time);
  void compute_swaps (CORBA::Any& value, int elapsed_time);
  void compute_packets (CORBA::Any& value, int elapsed_time);
  void compute_errors (CORBA::Any& value, int elapsed_time);
  void compute_context (CORBA::Any& value, int elapsed_time);
  void compute_collisions (CORBA::Any& value, int elapsed_time);
  void compute_interrupts (CORBA::Any& value, int elapsed_time);
  void compute_load (CORBA::Any& value, int elapsed_time);

  const ACE_Time_Value timeout_;
  ACE_Time_Value timestamp_;
  ACE_Time_Value sample_time_;

  statstime stats_;
  statstime old_stats_;
  CLIENT* rstat_client_;

};

#endif /* TAO_MACHINE_PROPERTIES */
