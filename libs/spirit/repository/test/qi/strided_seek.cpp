/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/


#include <vector>

#include <boost/config/warning_disable.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_binary.hpp>
#include <boost/spirit/include/qi_sequence.hpp>
#include <boost/spirit/include/qi_plus.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/include/qi_action.hpp>

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/spirit/repository/include/qi_seek.hpp>

#include "test.hpp"


///////////////////////////////////////////////////////////////////////////////
int main()
{
    using namespace spirit_test;
    namespace qi = boost::spirit::qi;
    namespace phx = boost::phoenix;
    using boost::spirit::repository::qi::seek;
    using boost::spirit::standard::space;

    // test eoi
    {
        using qi::eoi;

        BOOST_TEST(test("", seek(2)[eoi]));

        BOOST_TEST(!test("1", seek(2)[eoi]));
        BOOST_TEST(test("12", seek(2)[eoi]));
        BOOST_TEST(!test("123", seek(2)[eoi]));
    }

    // test literal finding
    {
        using qi::int_;

        int i = 0;

        BOOST_TEST(
            test_attr("abcd@123", seek(4)['@'] >> int_, i)
            && i == 123
        );
    }

    // test sequence finding
    {
        using qi::int_;
        using qi::lit;

        int i = 0;

        BOOST_TEST(
            test_attr("xxxxxxxxxxxxxxxx   KEY : 123", seek(16)[lit("KEY") >> ':'] >> int_, i, space)
            && i == 123
        );
    }

    // test attr
    {
        using qi::int_;

        int i = 0;

        BOOST_TEST( // expect partial match
            test_attr("aaaa95bb", seek(4)[int_], i, false)
            && i == 95
        );
    }

    // test action
    {
        using phx::ref;

        bool b = false;

        BOOST_TEST( // expect partial match
            test("#123", seek(4)['#'][ref(b) = true], false)
            && b
        );
    }

    // test lazy stride
    {
        using phx::ref;
        using qi::byte_;
        using qi::_1;

        boost::uint8_t b = 0xff; // a value which must fail

        BOOST_TEST(test("\x04""1234#567", byte_[ref(b) = _1] >> seek(4)['#'], false));
    }

    return boost::report_errors();
}
