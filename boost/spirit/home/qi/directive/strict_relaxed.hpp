/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_QI_STRICT_RELAXED
#define BOOST_SPIRIT_QI_STRICT_RELAXED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/modify.hpp>
#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/detail/get_stricttag.hpp>
#include <boost/spirit/home/qi/sepcial_terminals.hpp>

namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct use_directive<qi::domain, _qi::tag::strict>  // enables strict[]
      : mpl::true_ {};

    template <>
    struct use_directive<qi::domain, _qi::tag::relaxed> // enables relaxed[]
      : mpl::true_ {};

    ///////////////////////////////////////////////////////////////////////////
    // Do add qi::tag::strict if current modifier's non-strict 
    template <typename Current>
    struct compound_modifier
    <
        Current, _qi::tag::strict
      , typename disable_if
        <
            mpl::or_
            <
                qi::detail::get_stricttag<Current>
              , is_same<Current, unused_type>
            >
        >::type
    >
      : Current, _qi::tag::strict
    {
        typedef Current tail;
        typedef _qi::tag::strict head; 
        
        compound_modifier()
          : Current(), head() {}

        compound_modifier(Current const& current, head const& new_)
          : Current(current), head(new_) {}
    };

    // Do add qi::tag::relaxed if current modifier's strict 
    template <typename Current>
    struct compound_modifier<Current, _qi::tag::relaxed
          , typename enable_if<qi::detail::get_stricttag<Current> >::type>
      : Current, _qi::tag::relaxed
    {
        typedef Current tail;
        typedef _qi::tag::relaxed head; 
        
        compound_modifier()
          : Current(), head() {}

        compound_modifier(Current const& current, head const& new_)
          : Current(current), head(new_) {}
    };

    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct is_modifier_directive<qi::domain, _qi::tag::strict>
      : mpl::true_ {};

    template <>
    struct is_modifier_directive<qi::domain, _qi::tag::relaxed>
      : mpl::true_ {};
      
    namespace qi 
    {
#ifndef BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
        using boost::spirit::_qi::strict;
        using boost::spirit::_qi::relaxed;
#endif
        using boost::spirit::_qi::strict_type;
        using boost::spirit::_qi::relaxed_type;
    }
}}

#endif
