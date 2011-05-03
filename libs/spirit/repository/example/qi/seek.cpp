/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/

//  [ Jamboree May 3, 2011 ]        first ver.


#include <cstdlib>
#include <iostream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/repository/include/qi_seek.hpp>

   
int main(int argc, char *argv[])
{
    //[reference_qi_seek_namespace
    namespace qi = boost::spirit::qi;
    namespace ka = boost::spirit::karma;
    namespace repo = boost::spirit::repository;
    using namespace boost::spirit::standard;
    //]

    std::string line;
    typedef std::string::const_iterator iter;
    
    while (std::cout << ">>> ", std::getline(std::cin, line))
    {   
        //[reference_qi_seek_vars
        iter it = line.begin();
        iter end = line.end();
        std::vector<int> val;
        //]
        
        //[reference_qi_seek_parse
        if (qi::phrase_parse(it, end, +repo::qi::seek["i" >> qi::int_], space, val))
        {
            std::cout << "-------------------------------- \n";
            std::cout << "Parsing succeeded, got: "
                      << ka::format("[ " << ka::int_ % ", " << " ]\n", val);            
            std::cout << "---------------------------------\n";
        }//]
        else
        {
            std::cout << "-------------------------------- \n";
            std::cout << "Parsing failed, rest: " << std::string(it, end) << "\n";
            std::cout << "-------------------------------- \n";
        }       
    }

    return EXIT_SUCCESS;
}
