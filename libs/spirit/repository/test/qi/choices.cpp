/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/mpl/print.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_directive.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_auxiliary.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/repository/include/qi_choices.hpp>
#include <boost/spirit/include/support_argument.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>
#include <boost/assert.hpp>

#include <string>
#include <iostream>
#include <vector>
#include "test.hpp"

int
main()
{
    using spirit_test::test;
    using spirit_test::test_attr;

    using boost::spirit::ascii::char_;
    using boost::spirit::qi::int_;
    using boost::spirit::qi::lit;
    using boost::spirit::qi::eps;
    using boost::spirit::qi::unused_type;
    using boost::spirit::qi::omit;
    using boost::spirit::qi::repeat;
    using boost::spirit::qi::as_string;
    using boost::spirit::qi::rule;
    using boost::spirit::qi::locals;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::_a;
    using boost::spirit::qi::_r1;
    using boost::spirit::qi::_val;
    using boost::spirit::qi::space;
    using boost::spirit::qi::space_type;

    using boost::spirit::repository::qi::choices;

    namespace phx = boost::phoenix;

    {
        choices<char const*> c;
        c.next() = 'x';
        c.next() = 'i';

        BOOST_TEST((test("x", c)));
        BOOST_TEST((test("i", c)));
        BOOST_TEST((!test("z", c)));
    }

    {
        choices<char const*> c;
        c.next() = "rock";
        c.next() = "roll";
        c.next() = int_;

        BOOST_TEST((test("rock", c)));
        BOOST_TEST((test("roll", c)));
        BOOST_TEST((test("12345", c)));
    }

    {
        choices<char const*, int> c;
        c.next() = int_(5);
        c.next() = int_(10);

        int i = 0;
        BOOST_TEST( (test_attr("10", int_(5) | int_(10), i)) );
        BOOST_TEST(i == 10);
    }

    {
        typedef boost::variant<unused_type, int, char> V;
        V v;

        choices<char const*, V()> c;
        c.next() = eps >> "rock";
        c.next() = int_;
        c.next() = char_;

        BOOST_TEST((test_attr("12345", c, v)));
        BOOST_TEST(boost::get<int>(v) == 12345);

        BOOST_TEST((test_attr("rock", c, v)));
        BOOST_TEST(v.which() == 1);

        BOOST_TEST((test_attr("x", c, v)));
        BOOST_TEST(boost::get<char>(v) == 'x');
    }

    {   // Make sure that we are using the actual supplied attribute types
        // from the variant and not the expected type.
        typedef boost::variant<int, std::string> V;
        V v;

        choices<char const*, V()> c;
        c.next() = int_;
        c.next() = as_string[+char_];

        BOOST_TEST((test_attr("12345", c, v)));
        BOOST_TEST(boost::get<int>(v) == 12345);

        BOOST_TEST((test_attr("abc", c, v)));
        BOOST_TEST(boost::get<std::string>(v) == "abc");

        c.clear();
        c.next() = as_string[+char_];
        c.next() = int_;

        BOOST_TEST((test_attr("12345", c, v)));
        BOOST_TEST(boost::get<std::string>(v) == "12345");
    }

    // test action
    {
        typedef boost::optional<boost::variant<int, char> > V;
        V v;

        choices<char const*, V()> c;
        c.next() = eps >> "rock";
        c.next() = int_;
        c.next() = char_;

        BOOST_TEST((test("12345", c[phx::ref(v) = _1])));
        BOOST_TEST(boost::get<int>(boost::get(v)) == 12345);
        BOOST_TEST((test("rock", c[phx::ref(v) = _1])));
        BOOST_TEST(!v);
    }

    // test action & attribute-propagation rule
    {
        choices<char const*, int()> c;
        c.next() = lit("123")[_val = 123];
        c.next() %= int_[++_val];

        int i = 0;
        BOOST_TEST((test_attr("123", c, i) && i == 123));
        BOOST_TEST((test_attr("124", c, i) && i == 125));
    }

    // test inherited-attribute
    {
        choices<char const*, void(char)> c;
        rule<char const*, void(char)> r;
        r = repeat(2)[char_(_r1 + 1)];
        c.next() = lit(_r1);
        c.next() = r(_r1);

        BOOST_TEST((test("abba", +c('a'))));
    }

    // test locals
    {
        choices<char const*, locals<int> > c;
        int i = 0;
        c.next() = lit('a')[_a = 1] >> eps[phx::ref(i) = _a * 100];
        c.next() = lit('b')[_a = 2] >> eps[phx::ref(i) = _a * 100];

        BOOST_TEST((test("a", c) && i == 100));
        BOOST_TEST((test("b", c) && i == 200));
    }

    // test skipper
    {
        choices<char const*, space_type> c;
        c.next() = 'a';
        c.next() = 'b';

        BOOST_TEST((test("a b\n  b\t  ab", +c, space)));
    }

    // test rule copy & alias
    {
        choices<char const*> c;
        rule<char const*> r;
        r = 'a';
        c.next() = r.copy();
        c.next() = r; // alias
        r = 'b';

        BOOST_TEST((test("abba", +c)));
    }

    // test RHS & dynamic behavior
    {
        rule<char const*> r;
        choices<char const*> c;

        r = 'a' | c;
        BOOST_TEST((test("a", r)));
        BOOST_TEST((!test("b", r)));

        c.next() = 'b';
        BOOST_TEST((test("a", r)));
        BOOST_TEST((test("b", r)));

        c.clear();
        BOOST_TEST((test("a", r)));
        BOOST_TEST((!test("b", r)));
    }

    return boost::report_errors();
}

