/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_QI_BUDDY_SPACE
#define BOOST_SPIRIT_REPOSITORY_QI_BUDDY_SPACE


#if defined(_MSC_VER)
#pragma once
#endif


#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/qi/detail/attributes.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/char_class.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/repository/home/support/buddy_component.hpp>


namespace boost { namespace spirit { namespace repository
{
    namespace tag
    {
        template <typename CharEncoding>
        struct buddy_space_parser
        {
            BOOST_SPIRIT_IS_TAG()
        };
    }

    namespace qi
    {
        ///////////////////////////////////////////////////////////////////////
        // This one is the class that the user can instantiate directly in
        // order to create a customized buddy_space parser
        template <typename CharEncoding = spirit::char_encoding::standard>
        struct buddy_space_parser
          : spirit::terminal<tag::buddy_space_parser<CharEncoding> >
        {};
    }
}}}


namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////
    template <typename CharEncoding>
    struct use_terminal<qi::domain, repository::tag::buddy_space_parser<CharEncoding> >
      : mpl::true_ {};
}} // namespace boost::spirit


namespace boost { namespace spirit { namespace repository {namespace qi
{
    template <typename CharEncoding>
    struct any_buddy_space_parser
      : buddy_component<any_buddy_space_parser<CharEncoding> >
      , spirit::qi::primitive_parser<any_buddy_space_parser<CharEncoding> >
    {
        typedef CharEncoding char_encoding;
        typedef spirit::tag::space classification;

        template <typename Context, typename Iterator>
        struct attribute
        {
            typedef unused_type type;
        };

        template <typename CharParam>
        bool test(CharParam ch) const
        {
            using spirit::char_class::classify;
            return spirit::traits::ischar<CharParam, char_encoding>::call(ch)
                   && classify<char_encoding>::is(classification(), ch);
        }

        template<class Iterator>
        typename enable_if<repository::traits::is_buddy_pos_iterator<Iterator> >::type
        buddy_check_pos(Iterator& first, Iterator const& last) const
        {
            Iterator it(first);
            bool matched = false;
            if (*it == '\r')  // CR
            {
                matched = true;
                ++it;
            }
            if (it != last && *it == '\n')  // LF
            {
                matched = true;
                ++it;
            }

            if (matched)
                inc_row(it);
            else
                ++it;

            first = it;
        }

        template<class Iterator> // for non-buddy_pos_iterator
        typename disable_if<repository::traits::is_buddy_pos_iterator<Iterator> >::type
        buddy_check_pos(Iterator& first, Iterator const&) const
        {
            ++first;
        }

        template <typename Iterator, typename Context
          , typename Skipper, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& /*context*/, Skipper const& skipper
          , Attribute& /*attr*/) const
        {
            spirit::qi::skip_over(first, last, skipper);
            if (first != last && test(*first))
            {
                buddy_check_pos(first, last);
                return true;
            }
            return false;
        }

        template <typename Context>
        info what(Context& /*context*/) const
        {
            return info("space");
        }
    };
}}}} // namespace boost::spirit::repository::qi


namespace boost { namespace spirit { namespace qi
{
    ///////////////////////////////////////////////////////////////////////////
    // Parser generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename CharEncoding, typename Modifiers>
    struct make_primitive<repository::tag::buddy_space_parser<CharEncoding>, Modifiers>
    {
        typedef repository::qi::any_buddy_space_parser<CharEncoding> result_type;

        result_type operator()(unused_type, unused_type) const
        {
            return result_type();
        }
    };
}}}

#endif

