/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_EMBED
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_EMBED

#if defined(_MSC_VER)
#pragma once
#endif


#include <boost/spirit/home/support/terminal.hpp>


namespace boost { namespace spirit { namespace repository
{
    // The distinct extended terminal
    BOOST_SPIRIT_DEFINE_TERMINALS_NAME_EX(( embed, embed_type ))

}}}

#endif
