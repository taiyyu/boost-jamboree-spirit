/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/


#include <boost/config/warning_disable.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/include/karma_generate.hpp>
#include <boost/spirit/include/karma_char.hpp>
#include <boost/spirit/include/karma_string.hpp>
#include <boost/spirit/include/karma_bool.hpp>
#include <boost/spirit/include/karma_int.hpp>
#include <boost/spirit/include/karma_sequence.hpp>
#include <boost/spirit/include/karma_nonterminal.hpp>
#include <boost/spirit/include/karma_action.hpp>
#include <boost/spirit/include/karma_delimit.hpp>
#include <boost/spirit/include/karma_eps.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include <boost/fusion/include/make_vector.hpp>

#include <boost/spirit/repository/include/karma_embed.hpp>

#include "test.hpp"


///////////////////////////////////////////////////////////////////////////////
int main()
{
    using namespace spirit_test;
    namespace karma = boost::spirit::karma;
    namespace fu = boost::fusion;
    namespace phx = boost::phoenix;
    using boost::spirit::repository::karma::embed;

    // test immediate attr
    {
        using karma::int_;

        BOOST_TEST(test("95", embed(95)[int_]));
        BOOST_TEST(test("9596", embed(95)[int_] << int_, 96));
    }

    // test delimited
    {
        using karma::int_;
        using karma::delimit;

        BOOST_TEST(test_delimited("1;2;3;", embed(1)[int_ << '2'] << '3', ';'));
        BOOST_TEST(
            test_delimited("1 2 3;"
                , embed(1)[delimit(' ')[int_ << '2']] << '3', ';'
            )
        );
    }

    // with rule
    // ---------
    typedef output_iterator<char>::type sink_type;
    using karma::rule;
    using karma::locals;

    // test attr & locals
    {
        using karma::int_;
        using karma::eps;
        using karma::_val;
        using karma::_r1;
        using karma::_a;

        rule<sink_type, int*()> r;
        rule<sink_type, void(int*)> r2;
        rule<sink_type, locals<int*> > r3;
        int i = 95;

        r = embed(*_val)[int_];
        r2 = embed(*_r1)[int_];
        r3 = eps[_a = &i] << embed(*_a)[int_];

        BOOST_TEST(test("95", r, &i));
        BOOST_TEST(test("95", r2(&i)));
        BOOST_TEST(test("95", r3));
    }

    // test action
    {
        using karma::bool_;
        using karma::_val;
        using phx::ref;

        rule<sink_type, bool()> r;
        bool b = false;

        r = embed(_val)[bool_][ref(b) = _val];

        BOOST_TEST(test("true", r, true) && b);
        BOOST_TEST(test("false", r, false) && !b);
    }

    // test embedded rule
    {
        using karma::int_;
        using karma::_val;
        using phx::at_c;

        rule<sink_type, int()> ri;
        rule<sink_type, fu::vector2<int, int>(), char> r;

        ri = "i: " << int_;
        r = ri << ri << embed(at_c<0>(_val) + at_c<1>(_val))[ri];

        BOOST_TEST(
            test_delimited("i: 100;i: 200;i: 300;"
                , r, fu::make_vector(100, 200), ';'
            )
        );
    }

    return boost::report_errors();
}
