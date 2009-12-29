// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Question.h
 *
 * This file contains the declarations of Question classes for use with
 * the command line input adapter (InputCL).
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef USER_INTERFACE_QUESTION_H_
#define USER_INTERFACE_QUESTION_H_

#include <string>
#include <sstream>
#include <map>

namespace UserInterface {

  namespace Default {
    const std::string INVALID_RESPONSE_STR = "Sorry, that is not a valid answer.";
  };  /* Default namespace */

  /**
   * @class Question
   *
   * @brief Base class for a user question (or info for user).
   */
  class Question {
  public:
    /// Constructor.
    /**
     * @param prompt  Prompt/question (or info) for user.
     *
     * @param invalid_response  Response string for when user provides an invalid answer.
     */
    Question (std::string prompt, std::string invalid_response = UserInterface::Default::INVALID_RESPONSE_STR)
      : prompt_ (prompt), invalid_response_ (invalid_response), answer_ ("") { };

    /// Destructor.
    virtual ~Question (void) { };

    /// Set prompt.
    /**
     * @param prompt  Prompt/question (or info) for user.
     */
    void set_prompt (std::string prompt) { this->prompt_ = prompt; };

    /// Get prompt.
    /**
     * @return  Prompt/question (or info) for user.
     */
    std::string get_prompt () { return this->prompt_; };

    /// Get response string for when user provides an invalid answer.
    /**
     * @return  Response string for when user provides an invalid answer.
     */
    std::string get_invalid_response () { return this->invalid_response_; };

    /// Is an answer valid for this question?
    /**
     * @param answer  Answer to check validity.
     *
     * @return  True for a valid answer or false for an invalid answer.
     */
    virtual bool is_valid_answer (std::string answer) { return true; };

    /// Provide user answer.
    /**
     * @param answer  User answer.
     *
     * @return  True for a valid answer or false for an invalid answer.
     */
    virtual bool answer (std::string answer) { if (this->is_valid_answer (answer)) {this->answer_ = answer; return true;} return false; };

    /// Get answer.
    /**
     * @return  Answer from user (if no valid answer provided, returns empty string).
     */
    std::string get_answer () { return this->answer_; };

  protected:
    /// Prompt/question (or info) for user.
    std::string prompt_;

    /// Response to user when they provide an invalid answer to the question.
    std::string invalid_response_;

    /// Answer.
    std::string answer_;
  };
  

  /**
   * @class QuestionChoice
   *
   * @brief Class for a user question with a mapping from answers to values.
   */
  template <typename T>
  class QuestionChoice : public Question  {
  public:
    /// Class trait of its answer value type.
    typedef T TYPE;

    /// Constructor.
    /**
     * @param prompt  Prompt/question for user.
     *
     * @param invalid_val  Value to return if no valid answer provided.
     *
     * @param match_case  Match case of answer strings? (i.e., case sensitive?)
     *
     * @param invalid_response  Response string for when user provides an invalid answer.
     */
    QuestionChoice (std::string prompt, T invalid_val, bool match_case = false, std::string invalid_response = UserInterface::Default::INVALID_RESPONSE_STR);

    /// Destructor.
    virtual ~QuestionChoice (void);

    /// Is an answer valid for this question?
    /**
     * @param answer  Answer to check validity.
     *
     * @return  True for a valid answer or false for an invalid answer.
     */
    virtual bool is_valid_answer (std::string answer);

    /// Get answer.
    /**
     * @return  Value corresponding to answer from user (if no valid answer provided, returns invalid_val parameter from constructor).
     */
    T get_answer_val (void);

    /// Set complete mapping from answers to values.
    /// (WARNING:  Map will completely replace all existing mappings.)
    /**
     * @param answer_map  Map from valid answer strings to corresponding values.
     */
    void set_map (const std::map<std::string, T> &answer_map);

    /// Add a mapping from an answer to a value (or replace with provided
    /// value if a mapping already exists for the answer).
    /**
     * @param answer  Valid answer string.
     *
     * @param value  Value corresponding to answer string.
     */
    void add_mapping (std::string answer, T value);

  protected:
    /// Match case of strings? (i.e., case sensitive?)
    bool match_case_;

    /// Value to indicate invalid answer.
    T invalid_val_;

    /// Map from answers to values.
    std::map<std::string, T> answer_map_;
  };



};  /* UserInterface namespace */

#include "Question.cpp"

#endif /* USER_INTERFACE_QUESTION_H_ */
