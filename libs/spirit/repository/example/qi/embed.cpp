/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/

//  [ Jamboree April 24, 2011 ]        first ver.


#include <cstdlib>
#include <iostream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/repository/include/qi_embed.hpp>


int main(int argc, char *argv[])
{
    //[reference_qi_embed_namespace
    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;
    namespace repo = boost::spirit::repository;
    using namespace boost::spirit::standard;
    //]

    //[reference_qi_embed_typedef
    typedef boost::variant<int, double, std::string> V;
    typedef std::string::const_iterator iter;
    typedef qi::rule<iter, V()> rule_t;
    typedef boost::iterator_range<iter> range;
    //]

    //[reference_qi_embed_rule_decl
    qi::rule<iter, V(), qi::locals<range, rule_t*>, space_type> demo;
    qi::rule<iter, range()> kwd_patt;
    qi::rule<iter, rule_t*()> full_kwd;
    qi::symbols<char, rule_t*> kwd;
    rule_t ri, rd, rs;
    //]

    //[reference_qi_embed_rule_def_p1
    demo %=
        qi::omit
        [
            kwd_patt                                  [qi::_a = qi::_1]
         >> repo::qi::embed(qi::_a)[full_kwd]         [qi::_b = qi::_1]
        ]
      > qi::lazy(*qi::_b)
        ;

    // matches whole string without trailing chars
    full_kwd = kwd > qi::eoi;

    kwd_patt = // the keyword pattern
        qi::raw[!qi::digit >> +(qi::alnum | '_')][qi::_val = qi::_1]
        ;
    //]
    //[reference_qi_embed_rule_def_p2
    ri = qi::int_;
    rd = qi::double_;
    rs = qi::as_string['"' >> *~qi::char_('"') >> '"'];

    kwd.add("int"      , &ri)
           ("double"   , &rd)
           ("string"   , &rs)
        ;
    //]

    //[reference_qi_embed_rule_error_handling
    ri.name("int");
    rd.name("double");
    rs.name("string");

    qi::on_error<qi::fail>
    (
        demo
      , std::cout
            << phx::val("[demo] Error! Expecting ")
            << qi::_4                                       // what failed?
            << phx::val(" here: \"")
            << phx::construct<std::string>(qi::_3, qi::_2)  // iterators to error-pos, end
            << phx::val("\"")
            << std::endl
    );

    qi::on_error<qi::fail>
    (
        full_kwd
      , std::cout
            << phx::val("[full_kwd] Error! Expecting ")
            << qi::_4                                       // what failed?
            << phx::val(" here: \"")
            << phx::construct<std::string>(qi::_3, qi::_2)  // iterators to error-pos, end
            << phx::val("\"")
            << std::endl
    );
    //]

    std::string line;

    while (std::cout << ">>> ", std::getline(std::cin, line))
    {
        iter it = line.begin();
        iter end = line.end();
        V val;
        try
        {
            if (qi::phrase_parse(it, end, demo, space, val) && it == end)
            {
                std::cout << "-------------------------------- \n";
                std::cout << "Parsing succeeded, got: " << val << "\n";
                std::cout << "---------------------------------\n";
            }
            else
            {
                std::cout << "-------------------------------- \n";
                std::cout << "Parsing failed, rest: " << std::string(it, end) << "\n";
                std::cout << "-------------------------------- \n";
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
