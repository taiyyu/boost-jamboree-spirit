/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
//  [ Jamboree June 25, 2011 ]        adapt for strict mode.
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_strict.hpp>
#include <boost/spirit/include/support_argument.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/optional.hpp>

#include <string>
#include <iostream>
#include "test.hpp"

using namespace spirit_test;

int
main()
{
    using boost::spirit::qi::int_;
    using boost::spirit::qi::strict;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::_2;
    using boost::spirit::qi::rule;
    using boost::spirit::ascii::alpha;
    using boost::spirit::ascii::char_;

    using boost::fusion::vector;
    using boost::fusion::at_c;
    using boost::optional;

    {
        BOOST_TEST((!test("a", strict[char_('a') ^ char_('b') ^ char_('c')])));
        BOOST_TEST((!test("b", strict[char_('a') ^ char_('b') ^ char_('c')])));
        BOOST_TEST((!test("ab", strict[char_('a') ^ char_('b') ^ char_('c')])));
        BOOST_TEST((!test("ba", strict[char_('a') ^ char_('b') ^ char_('c')])));
        
        BOOST_TEST((test("abc", strict[char_('a') ^ char_('b') ^ char_('c')])));
        BOOST_TEST((test("acb", strict[char_('a') ^ char_('b') ^ char_('c')])));
        BOOST_TEST((test("bca", strict[char_('a') ^ char_('b') ^ char_('c')])));
        BOOST_TEST((test("bac", strict[char_('a') ^ char_('b') ^ char_('c')])));
        BOOST_TEST((test("cab", strict[char_('a') ^ char_('b') ^ char_('c')])));
        BOOST_TEST((test("cba", strict[char_('a') ^ char_('b') ^ char_('c')])));

        BOOST_TEST((!test("cca", strict[char_('a') ^ char_('b') ^ char_('c')])));
    }

    {
        vector<int, char> attr;

        BOOST_TEST((!test_attr("a", strict[int_ ^ alpha], attr)));
        BOOST_TEST((at_c<0>(attr) == 0));
        BOOST_TEST((at_c<1>(attr) == 'a'));

        at_c<1>(attr) = 0;
        BOOST_TEST((!test_attr("123", strict[int_ ^ alpha], attr)));
        BOOST_TEST((at_c<0>(attr) == 123));
        BOOST_TEST((at_c<1>(attr) == 0));
        
        at_c<0>(attr) = 0;
        BOOST_TEST((test_attr("123a", strict[int_ ^ alpha], attr)));
        BOOST_TEST((at_c<0>(attr) == 123));
        BOOST_TEST((at_c<1>(attr) == 'a'));

        at_c<0>(attr) = 0;
        at_c<1>(attr) = 0;
        BOOST_TEST((test_attr("a123", strict[int_ ^ alpha], attr)));
        BOOST_TEST((at_c<0>(attr) == 123));
        BOOST_TEST((at_c<1>(attr) == 'a'));
    }

    {   // test action
        using namespace boost::phoenix;
        namespace phx = boost::phoenix;

        int i = 0;
        char c = 0;

        BOOST_TEST((test("123a", (strict[int_ ^ alpha])[phx::ref(i) = _1, phx::ref(c) = _2])));
        BOOST_TEST((i == 123));
        BOOST_TEST((c == 'a'));
    }

    {   // test rule %=

        typedef vector<int, char> attr_type;
        attr_type attr;

        rule<char const*, attr_type()> r;
        r %= strict[int_ ^ alpha];

        BOOST_TEST((!test_attr("a", r, attr)));
        BOOST_TEST((at_c<0>(attr) == 0));
        BOOST_TEST((at_c<1>(attr) == 'a'));

        at_c<1>(attr) = 0;
        BOOST_TEST((!test_attr("123", r, attr)));
        BOOST_TEST((at_c<0>(attr) == 123));
        BOOST_TEST((at_c<1>(attr) == 0));

        at_c<0>(attr) = 0;
        BOOST_TEST((test_attr("123a", r, attr)));
        BOOST_TEST((at_c<0>(attr) == 123));
        BOOST_TEST((at_c<1>(attr) == 'a'));

        at_c<0>(attr) = 0;
        at_c<1>(attr) = 0;
        BOOST_TEST((test_attr("a123", r, attr)));
        BOOST_TEST((at_c<0>(attr) == 123));
        BOOST_TEST((at_c<1>(attr) == 'a'));
    }

    return boost::report_errors();
}

