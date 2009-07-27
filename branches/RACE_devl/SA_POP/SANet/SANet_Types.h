// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SANet_Types.h
 *
 * This file contains the definitions of types used throughout SANet.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SANET_TYPES_H_
#define SANET_TYPES_H_

#include <string>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <utility>


#if !defined (SANET_STANDALONE)

#include "SA_POP/SA_POP_Types.h"

namespace SANet {
  /// Type of a node id (tasks and conditions).
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef SA_POP::NodeID NodeID;

  /// Type of a condition id.
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef SA_POP::CondID CondID;

  /// Type of a condition value.
  typedef SA_POP::CondValue CondValue;

  /// Type of a condition kind/type.
  typedef SA_POP::CondKind CondKind;

  /// Type of a condition.
  typedef SA_POP::Condition Condition;

  /// Type of a task ID.
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef SA_POP::TaskID TaskID;

  /// NULL task ID (for unknown/uninitialized tasks).
  const TaskID NULL_TASK_ID = SA_POP::NULL_TASK_ID;

  /// NULL condition ID (for unknown/uninitialized conditions).
  const CondID NULL_COND_ID = SA_POP::NULL_TASK_ID;

  /// Type of a port id.
  typedef SA_POP::PortID PortID;

  /// Type of an expected utility calculation (basetype for others).
  /// (N.B. this must be a double or float to handle [0,1] range probabilities
  /// and multiplication factors).
  typedef SA_POP::EUCalc EUCalc;
          
  /// Type of a condition utility.
  typedef SA_POP::Utility Utility;

  /// Type of a probability.
  typedef SA_POP::Probability Probability;

  /// Type of a (precondition or effect) link weight.
  typedef SA_POP::LinkWeight LinkWeight;

  /// Type of a task cost.
  typedef SA_POP::TaskCost TaskCost;

  /// Type of a multiplicative factor (e.g. attenuation factor).
  typedef SA_POP::MultFactor MultFactor;

  /// Type of a precondition link.
  typedef SA_POP::PrecondLink PrecondLink;

  /// Type of an effect link.
  typedef SA_POP::EffectLink EffectLink;

  /// Type of a pair of ports for a link.
  typedef SA_POP::LinkPorts LinkPorts;

  /// Type of a list of tasks.
  typedef SA_POP::TaskList TaskList;

  /// Type of a set of goal conditions with associated utilities.
  typedef SA_POP::GoalMap GoalMap;

  /// Type of a set of tasks with associated expected utilities.
  typedef SA_POP::TaskEUMap TaskEUMap;

  /// Type of a set of conditions (condition & value).
  typedef SA_POP::CondSet CondSet;

  /// Type of a set of task ids.
  typedef SA_POP::TaskSet TaskSet;

    /// Type of duration
  typedef SA_POP::TimeValue TimeValue;

    /// NULL time (for unknown or unconstrained times).
  #define NULL_TIME -1

  inline std::string to_string(int x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(int) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(long x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(long) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(unsigned long x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(unsigned long) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(double x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(double) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(float x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(float) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(CondKind x)
  {
    std::ostringstream o;
    std::string x_str = "";

    switch (x) {
      case SA_POP::DATA :
        x_str = "DATA";
        break;
      case SA_POP::ENVIRON :
        x_str = "ENVIRON";
        break;
      case SA_POP::SYSTEM :
        x_str = "SYSTEM";
        break;
      default :
        x_str = "UNKNOWN";
        break;
    }
    if (!(o << x_str))
      throw "to_string(float) error writing to ostringstream";
    return o.str();
  };

  inline SANet::CondKind to_kind(std::string x)
  {
    if (x == "DATA")
      return SA_POP::DATA;
    else if (x == "ENVIRON")
      return SA_POP::ENVIRON;
    else if (x == "SYSTEM")
      return SA_POP::SYSTEM;
    throw "to_kind() error unknown condition kind";
  };

};  /* SANet namespace */

#endif /* SANET_STANDALONE not defined */


#if defined (SANET_STANDALONE)
namespace SANet {

  /// Type of a node id (tasks and conditions).
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef int NodeID;

  /// Type of a condition id.
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef NodeID CondID;

  /// Type of a condition value.
  typedef bool CondValue;

  /// Type of a condition kind/type.
  enum CondKind {ENVIRON, SYSTEM, DATA};

  /// Type of a condition.
  struct Condition {
    CondID id;
    CondValue value;
    CondKind kind;
    bool operator== (const Condition &s) const { return this->id == s.id; };
    bool operator!= (const Condition &s) const { return !(*this == s); };
    bool operator< (const Condition &s) const { return this->id < s.id; };
  };

  /// Type of a task ID.
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef NodeID TaskID;

  /// NULL task ID (for unknown/uninitialized tasks).
  const TaskID NULL_TASK_ID = 0;

  /// NULL condition ID (for unknown/uninitialized conditions).
  const CondID NULL_COND_ID = 0;

  /// Type of a port id.
  typedef std::string PortID;

  /// Type of an expected utility calculation (basetype for others).
  /// (N.B. this must be a double or float to handle [0,1] range probabilities
  /// and multiplication factors).
  typedef double EUCalc;
          
  /// Type of a condition utility.
  typedef EUCalc Utility;

  /// Type of a probability.
  typedef EUCalc Probability;

  /// Type of a (precondition or effect) link weight.
  typedef EUCalc LinkWeight;

  /// Type of a task cost.
  typedef EUCalc TaskCost;

  /// Type of a multiplicative factor (e.g. attenuation factor).
  typedef EUCalc MultFactor;

  /// Type of duration
  typedef int TimeValue;

  /// NULL time (for unknown or unconstrained times).
  #define NULL_TIME -1

  /// Type of a precondition link.
  typedef std::pair<CondID, TaskID> PrecondLink;

  /// Type of an effect link.
  typedef std::pair<TaskID, CondID> EffectLink;

  /// Type of a pair of ports for a link.
  typedef std::pair<PortID, PortID> LinkPorts;

  /// Type of a list of tasks.
  typedef std::list<TaskID> TaskList;

  /// Type of a set of goal conditions with associated utilities.
  typedef std::map<CondID, Utility> GoalMap;

  /// Type of a set of tasks with associated expected utilities.
  typedef std::map<TaskID, Utility> TaskEUMap;

  /// Type of a set of conditions (condition & value).
  typedef std::set<Condition> CondSet;

  /// Type of a set of task ids.
  typedef std::set<TaskID> TaskSet;

  inline std::string to_string(int x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(int) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(long x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(long) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(unsigned long x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(unsigned long) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(double x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(double) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(float x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(float) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(CondKind x)
  {
    std::ostringstream o;
    std::string x_str = "";

    switch (x) {
      case SANet::DATA :
        x_str = "DATA";
        break;
      case SANet::ENVIRON :
        x_str = "ENVIRON";
        break;
      case SANet::SYSTEM :
        x_str = "SYSTEM";
        break;
      default :
        x_str = "UNKNOWN";
        break;
    }
    if (!(o << x_str))
      throw "to_string(float) error writing to ostringstream";
    return o.str();
  };

  inline SANet::CondKind to_kind(std::string x)
  {
    if (x == "DATA")
      return SANet::DATA;
    else if (x == "ENVIRON")
      return SANet::ENVIRON;
    else if (x == "SYSTEM")
      return SANet::SYSTEM;
    throw "to_kind() error unknown condition kind";
  };
};  /* SANet namespace */
#endif /* SANET_STANDALONE */

#endif /* SANET_TYPES_H_ */
