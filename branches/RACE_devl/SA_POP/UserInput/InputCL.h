// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  InputCL.h
 *
 * This file contains the class definition of InputCL and related classes for
 * the command line input adapter (helper class for command line interfaces).
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef USER_INTERFACE_INPUT_CL_H_
#define USER_INTERFACE_INPUT_CL_H_

#include <iostream>
#include <string>

#include "Question.h"

namespace UserInterface {

  namespace Default {
    const size_t MAX_QUESTION_REPEAT = 5;
 };  /* Default namespace */


  /**
   * @class InputCL
   *
   * @brief Command line input adapter (helper class for command line interfaces).
   */
  class InputCL {
  public:
    /// Constructor.
    /**
     * @param ostrm  Output stream to display questions/prompts and info to user.
     *
     * @param istrm  Input stream to get user input.
     */
    InputCL (std::basic_ostream<char, std::char_traits<char> >& ostrm
      = std::cout, std::basic_istream<char, std::char_traits<char> >& istrm
      = std::cin);

    /// Destructor.
    virtual ~InputCL (void);

    /// Provide info to user.
    /**
     * @param info  Question class with info for user.
     */
    void info (UserInterface::Question info);

    /// Ask the user a question.
    /**
     * @param question  Question for user (question object will be updated with last user answer upon return).
     *
     * @param max_repeat  Maximum number of times to repeat question when user provides invalid answer(s).
     *
     * @return  True if user provided a valid answer to question, false otherwise.
     */
    bool ask (UserInterface::Question& question, size_t max_repeat = UserInterface::Default::MAX_QUESTION_REPEAT);

  protected:
    /// Output stream to display questions/prompts and info to user.
    std::basic_ostream<char, std::char_traits<char> >& ostrm_;

    /// Input stream to get user input.
    std::basic_istream<char, std::char_traits<char> >& istrm_;
  };













  /**
   * @class QuestionInt
   *
   * @brief Class for a user question with an integer answer.
   */
  class QuestionInt : public Question  {
  public:
    /// Constructor.
    /**
     * @param prompt  Prompt/question for user.
     *
     * @param min  Minimum (inclusive) valid answer.
     *
     * @param max  Maximum (inclusive) valid answer.
     *
     * @param invalid_response  Response string for when user provides an invalid answer.
     */
    QuestionInt (std::string prompt, int min, int max, std::string invalid_response = UserInterface::Default::INVALID_RESPONSE_STR);

    /// Destructor.
    virtual ~QuestionInt (void);

    /// Is an answer valid for this question?
    /**
     * @param answer  Answer to check validity.
     *
     * @return  True for a valid answer or false for an invalid answer.
     */
    virtual bool is_valid_answer (std::string answer);

    /// Get answer.
    /**
     * @return  Answer from user (if no valid answer provided, returns minimum valid answer).
     */
    int get_answer_int (void);

  protected:
    /// Minimum (inclusive) valid answer.
    int min_;

    /// Maximum (inclusive) valid answer.
    int max_;

  };

  /**
   * @class QuestionBool
   *
   * @brief Class for a user question with a boolean (yes/no) answer.
   *        Valid answer strings (case insensitive) corresponding to true are "y" and "yes".
   *        Valid answer strings (case insensitive) corresponding to false are "n" and "no".
   */
  class QuestionBool : public QuestionChoice<bool>  {
  public:
    /// Constructor.
    /**
     * @param prompt  Prompt/question (or info) for user.
     *
     * @param def_val  Default value returned if no valid answer provided.
     *
     * @param invalid_response  Response string for when user provides an invalid answer.
     */
    QuestionBool (std::string prompt, bool def_val, std::string invalid_response = UserInterface::Default::INVALID_RESPONSE_STR);

    /// Destructor.
    virtual ~QuestionBool (void);

    /// Get answer.
    /**
     * @return  Answer from user (if no valid answer provided, returns default value).
     */
    bool get_answer_bool (void);

  protected:
    /// Initialize mapping with yes/no answers.
    void init_map (void);
  };

};  /* UserInterface namespace */

#endif /* USER_INTERFACE_INPUT_CL_H_ */
