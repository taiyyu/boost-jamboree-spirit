/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/


#include <boost/config/warning_disable.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_bool.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_sequence.hpp>
#include <boost/spirit/include/qi_plus.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_raw.hpp>
#include <boost/spirit/include/qi_skip.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/range/as_literal.hpp>

#include <boost/spirit/repository/include/qi_embed.hpp>

#include "test.hpp"


///////////////////////////////////////////////////////////////////////////////
int main()
{
    using namespace spirit_test;
    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;
    using boost::spirit::repository::qi::embed;
    using boost::as_literal;

    // test immediate range
    {
        using qi::int_;
        using qi::digit;

        int i = 0;

        BOOST_TEST(test("abc", embed(as_literal("95"))[+digit] >> "abc"));
        BOOST_TEST(test_attr("", embed(as_literal("95"))[int_], i) && i == 95);
    }

    // test skip
    {
        using qi::int_;
        using qi::skip;
        using boost::spirit::standard::space_type;
        using boost::spirit::standard::space;

        BOOST_TEST(
            test("abc xyz"
                , "abc" >> embed(as_literal("95 96"))[int_ >> int_] >> "xyz"
                , space
            )
        );
        BOOST_TEST(
            test("abc xyz"
                , "abc" >> embed(as_literal("95:96"))[skip(':')[int_ >> int_]] >> "xyz"
                , space
            )
        );
    }

    // with rule
    // ---------
    using qi::rule;
    using qi::locals;
    typedef char const* iter;
    typedef boost::iterator_range<iter> range;

    // test lazy raw range
    {
        using qi::int_;
        using qi::digit;
        using qi::raw;
        using qi::_val;
        using qi::_1;
        using qi::_a;

        rule<iter, int(), locals<range> > r;
        int i = 0;

        r = raw[+digit][_a = _1] >> embed(_a)[int_[_val = _1]];

        BOOST_TEST(test_attr("95", r, i) && i == 95);
    }

    // test lazy string range
    {
        using qi::int_;
        using qi::_r1;

        rule<iter, int(std::string const&)> r;
        int i = 0;

        r = embed(_r1)[int_];

        BOOST_TEST(test_attr("", r(std::string("95")), i) && i == 95);
    }

    // test action
    {
        using qi::bool_;
        using qi::_1;
        using qi::_r1;
        using phx::ref;

        rule<iter, void(std::string const&)> r;
        bool b = false;

        r = embed(_r1)[bool_][ref(b) = _1];

        BOOST_TEST(test("", r(std::string("true"))) && b);
        BOOST_TEST(test("", r(std::string("false"))) && !b);
    }

    return boost::report_errors();
}
