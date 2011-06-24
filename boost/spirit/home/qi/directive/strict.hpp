/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_QI_STRICT
#define BOOST_SPIRIT_QI_STRICT


#if defined(_MSC_VER)
#pragma once
#endif


#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/home/support/modify.hpp>
#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/meta_compiler.hpp>


namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct use_directive<qi::domain, tag::strict>  // enables strict[]
      : mpl::true_ {};

    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct is_modifier_directive<qi::domain, tag::strict>
      : mpl::true_ {};

    namespace qi
    {
#ifndef BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
        using boost::spirit::strict;
#endif
        using boost::spirit::strict_type;
    }
}}


#endif
