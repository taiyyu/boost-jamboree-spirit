/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#include "../measure.hpp"
#include <cstdlib>
#include <boost/assert.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/include/qi_choices.hpp>
 

namespace
{
    namespace qi = boost::spirit::qi;
    namespace repo = boost::spirit::repository;
    
    char test_str[] = "abcde";
    char const* first = test_str;
    char const* last = first + sizeof(test_str) - 1; 
    
    ///////////////////////////////////////////////////////////////////////////
    struct alternative_test : test::base
    {
        bool parse(char const* first, char const* last)
        {
            using qi::lit;
            return qi::parse(first, last, +(lit('a') | 'b' | 'c' | 'd' | 'e'))
                && first == last;
        }
        
        void benchmark()
        {           
            parse(first, last);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    struct ruled_alternative_test : test::base
    {
        qi::rule<char const*> a, b, c, d, e; 
        
        ruled_alternative_test()
        {
            a = 'a';
            b = 'b';
            c = 'c';
            d = 'd';
            e = 'e';
        } 
        
        bool parse(char const* first, char const* last)
        {
            using qi::lit;
            return qi::parse(first, last, +(a | b | c | d | e))
                && first == last;
        }
        
        void benchmark()
        {           
            parse(first, last);
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////
    struct chained_alternative_test : test::base
    {
        qi::rule<char const*> r;
        
        chained_alternative_test()
        {
            r = 'a';
            r = r.copy() | 'b';
            r = r.copy() | 'c';
            r = r.copy() | 'd';
            r = r.copy() | 'e';
        } 
        
        bool parse(char const* first, char const* last)
        {
            return qi::parse(first, last, +r) && first == last;
        } 

        void benchmark()
        {                       
            parse(first, last);
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////
    struct choices_test : test::base
    {
        repo::qi::choices<char const*> c;
        
        choices_test()
        {
            c.next() = 'a';
            c.next() = 'b';
            c.next() = 'c';
            c.next() = 'd';
            c.next() = 'e';
        } 
        
        bool parse(char const* first, char const* last)
        {
            return qi::parse(first, last, +c) && first == last;
        } 

        void benchmark()
        {                       
            parse(first, last);
        }
    };
}

int main()
{
    BOOST_ASSERT(alternative_test().parse(first, last));
    BOOST_ASSERT(ruled_alternative_test().parse(first, last));
    BOOST_ASSERT(chained_alternative_test().parse(first, last));
    BOOST_ASSERT(choices_test().parse(first, last));
    
    BOOST_SPIRIT_TEST_BENCHMARK(
        10000000,     // This is the maximum repetitions to execute
        (alternative_test)
        (ruled_alternative_test) 
        (chained_alternative_test)
        (choices_test)
    )
    
    // This is ultimately responsible for preventing all the test code
    // from being optimized away.  Change this to return 0 and you
    // unplug the whole test's life support system.
    return test::live_code != 0;
}

