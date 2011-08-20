//  Copyright (c) 2001-2011 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SPIRIT_STRICT_KARMA_RELAXED_APR_22_2010_0959AM)
#define SPIRIT_STRICT_KARMA_RELAXED_APR_22_2010_0959AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/modify.hpp>
#include <boost/spirit/home/karma/domain.hpp>
#include <boost/spirit/home/karma/meta_compiler.hpp>
#include <boost/spirit/home/karma/sepcial_terminals.hpp>

namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct use_directive<karma::domain, _karma::tag::strict>  // enables strict[]
      : mpl::true_ {};

    template <>
    struct use_directive<karma::domain, _karma::tag::relaxed> // enables relaxed[]
      : mpl::true_ {};

    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct is_modifier_directive<karma::domain, _karma::tag::strict>
      : mpl::true_ {};

    template <>
    struct is_modifier_directive<karma::domain, _karma::tag::relaxed>
      : mpl::true_ {};

    ///////////////////////////////////////////////////////////////////////////
    // Don't add karma::tag::strict or karma::tag::relaxed if there is already
    // one of those in the modifier list
    template <typename Current>
    struct compound_modifier<Current, _karma::tag::strict
          , typename enable_if<has_modifier<Current, _karma::tag::relaxed> >::type>
      : Current
    {
        compound_modifier()
          : Current() {}

        compound_modifier(Current const& current, _karma::tag::strict const&)
          : Current(current) {}
    };

    template <typename Current>
    struct compound_modifier<Current, _karma::tag::relaxed
          , typename enable_if<has_modifier<Current, _karma::tag::strict> >::type>
      : Current
    {
        compound_modifier()
          : Current() {}

        compound_modifier(Current const& current, _karma::tag::relaxed const&)
          : Current(current) {}
    };

    namespace karma
    {
#ifndef BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
        using boost::spirit::_karma::strict;
        using boost::spirit::_karma::relaxed;
#endif
        using boost::spirit::_karma::strict_type;
        using boost::spirit::_karma::relaxed_type;
    }
}}

#endif
