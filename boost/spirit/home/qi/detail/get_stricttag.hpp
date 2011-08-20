/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_QI_GET_STRICTTAG
#define BOOST_SPIRIT_QI_GET_STRICTTAG

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/spirit/home/qi/sepcial_terminals.hpp>

namespace boost { namespace spirit { namespace qi { namespace detail
{
    // the default mode for Qi is 'relaxed'
    template <
        typename Modifiers
      , typename StrictModifier = typename mpl::or_<
            has_modifier<Modifiers, _qi::tag::strict>
          , has_modifier<Modifiers, _qi::tag::relaxed> >::type>
    struct get_stricttag : mpl::false_ {};

    // strict mode is enforced only when tag::strict is on the modifiers list
    // and attached after any tag::relaxed.
    template <typename Modifiers>
    struct get_stricttag<Modifiers, mpl::true_>
      : mpl::eval_if
        <
            is_same<typename Modifiers::head, _qi::tag::strict>
          , mpl::true_
          , mpl::eval_if
            <
                is_same<typename Modifiers::head, _qi::tag::relaxed>
              , mpl::false_
              , get_stricttag<typename Modifiers::tail, mpl::true_>
            >
        >::type
    {};
}}}}

#endif
