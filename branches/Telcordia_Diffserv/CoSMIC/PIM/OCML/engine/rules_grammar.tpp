namespace OCML
{

  namespace Rule_Parser
  {

    /*
     * Option_Value grammar
     */

    template <typename ScannerT>
    Option_Value_Grammar::definition<ScannerT>
    ::definition(Option_Value_Grammar const& self)
    {
      using namespace boost::spirit;
      using namespace phoenix;
   
      top =
        int_value [self.value = arg1]
        | bool_value [self.value = arg1]
        | str_value [self.value = arg1]
        ;

      int_value = lexeme_d[
        (+digit_p)
           [
             int_value.value = new_<Integer_Option_Value>(arg1, arg2)
           ]
        ]
        ;

      bool_value =
        (str_p("true") | "false")
          [
            bool_value.value = new_<Boolean_Option_Value>(arg1, arg2)
          ]
        ;

      str_value = lexeme_d[
        confix_p('"'
            , (*anychar_p)
                [
                  str_value.value = new_<String_Option_Value>(arg1, arg2)
                ]
            , '"'
          )
        ]
        ;

      BOOST_SPIRIT_DEBUG_RULE(top);
      BOOST_SPIRIT_DEBUG_RULE(int_value);
      BOOST_SPIRIT_DEBUG_RULE(bool_value);
      BOOST_SPIRIT_DEBUG_RULE(str_value);
    }

    /*
     * Option_Path grammar
     */

    template <typename ScannerT>
    Option_Path_Grammar::definition<ScannerT>
    ::definition(Option_Path_Grammar const& self)
    {
      using namespace boost::spirit;
      using namespace phoenix;

      // @todo Currently it allows "path / opt" , which shouldn't.
      
      option = 
        ch_p('[')
        >> lexeme_d[
             word
               [
                  bind(&Option_Path::push_back)(self.value, arg1)
               ]
             >> *('/'
                  >> word
                     [
                       bind(&Option_Path::push_back)(self.value, arg1)
                     ]
               )
         ]
        >> ch_p(']')
        ;

      BOOST_SPIRIT_DEBUG_RULE(option);
      BOOST_SPIRIT_DEBUG_RULE(word);
    }

    /*
     * Expression grammar
     */

    template <typename ScannerT>
    Expression_Grammar::definition<ScannerT>::
    definition(Expression_Grammar const& self)
    {
      using namespace boost::spirit;
      using namespace phoenix;

      top =
         ('(' >> expression [self.value = arg1] >> ')')
         | expression [self.value = arg1]
        ;
       
       expression =
         option_equality [expression.value = arg1]
         | option_selection [expression.value = arg1]
         | negated_expression [expression.value = arg1]
         | and_combination [expression.value = arg1]
         | or_combination [expression.value = arg1]
         | if_rule [expression.value = arg1]
         | word [expression.value = do_find_rule(arg1)]
         ;

      option_selection =
        option_path
          [
            option_selection.value = do_define_selection_rule(arg1)
          ]
        ;

      option_equality =
        (
          option_path [option_equality.o_path = arg1]
          >> '='
          >> option_value [option_equality.o_value = arg1]
        )
          [
            option_equality.value =
              do_define_equality_rule (option_equality.o_path,
                                       option_equality.o_value)
          ]
        ;

      negated_expression =
        ch_p('~') >> expression
          [
            negated_expression.value = do_define_not_rule (arg1)
          ]
        ;

      and_combination =
        (
          ch_p('(')
          >> expression [do_push_back(and_combination.exprs, arg1)]
          >> +('&' >> expression
                        [
                          do_push_back(and_combination.exprs, arg1)
                        ]
              )
          >> ')'
        )
          [
            and_combination.value = do_define_and_rule(and_combination.exprs)
          ]
        ;

      or_combination =
        (
          ch_p('(')
          >> expression [do_push_back(or_combination.exprs, arg1)]
          >> +('|' >> expression
                        [
                          do_push_back(or_combination.exprs, arg1)
                        ]
              )
          >> ')'
        )
          [
            or_combination.value = do_define_or_rule(or_combination.exprs)
          ]
        ;

      if_rule =
        (
          ch_p('(')
          >> expression [if_rule.lhs = arg1]
          >> "=>" 
          >> expression [if_rule.rhs = arg1]
          >> ')'
        )
          [
            if_rule.value = do_define_if_rule(if_rule.lhs, if_rule.rhs)
          ]
        ;

      BOOST_SPIRIT_DEBUG_RULE(top);
      BOOST_SPIRIT_DEBUG_RULE(expression);
      BOOST_SPIRIT_DEBUG_RULE(option_selection);
      BOOST_SPIRIT_DEBUG_RULE(option_path);
      BOOST_SPIRIT_DEBUG_RULE(option_value);
      BOOST_SPIRIT_DEBUG_RULE(option_equality);
      BOOST_SPIRIT_DEBUG_RULE(word);
      BOOST_SPIRIT_DEBUG_RULE(if_rule);
      BOOST_SPIRIT_DEBUG_RULE(or_combination);
      BOOST_SPIRIT_DEBUG_RULE(and_combination);
      BOOST_SPIRIT_DEBUG_RULE(negated_expression);
    }
 
    /*
     * Rule_Definition grammar
     */
 
    template <typename ScannerT>
    Rule_Definition_Grammar::definition<ScannerT>::
    definition(Rule_Definition_Grammar const& self)
    {
      using namespace boost::spirit;
      using namespace phoenix;

      typedef std::pair<std::string, OCML::Constraint_Engine::rule_index>
        value_type;

      rule_definition =  
       word
          [
            bind(&value_type::first)(self.value) = arg1
          ]
        >> ':'
        >> expression
          [
            bind(&value_type::second)(self.value) = arg1
          ]
       ;
    
       BOOST_SPIRIT_DEBUG_RULE(rule_definition);
       BOOST_SPIRIT_DEBUG_RULE(word);
       BOOST_SPIRIT_DEBUG_RULE(expression);
     }

    /*
     * Syntax grammar  -- spirit entry point --
     */
 
    template <typename ScannerT>
    Syntax::definition<ScannerT>::
    definition(Syntax const& self)
    {
      using namespace boost::spirit;

      file =  +(
         (
          rule_definition [ insert_rule_map() ]
          | expression [ insert_rule() ]
         )
         >> ch_p(';')
       )
      ;

      BOOST_SPIRIT_DEBUG_RULE(file);
      BOOST_SPIRIT_DEBUG_RULE(rule_definition);
      BOOST_SPIRIT_DEBUG_RULE(expression);
    }

  } // namespace Rule_Parser

} // namespace OCML
