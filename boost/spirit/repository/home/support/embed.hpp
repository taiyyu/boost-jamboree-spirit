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
    // The embed extended terminal
    BOOST_SPIRIT_DEFINE_TERMINALS_NAME_EX(( embed, embed_type ))

}}}

/***/

namespace boost { namespace spirit
{
    // We handle the lazy version ourselves, bypassing the normal 'lazy_terminal'
    // infrastructure.
    template <typename A0>
    struct make_lazy<repository::embed_type, A0>
    {
        typedef typename
            proto::terminal<
                terminal_ex<
                    repository::embed_type::terminal_type
                  , typename detail::result_of::make_vector<
                        typename detail::to_nonlazy_arg<A0>::type
                    >::type
                >
            >::type
        result_type;

        typedef result_type type;

        result_type
        operator()(repository::embed_type f, A0 const& _0) const
        {
            typedef typename result_type::proto_child0 child_type;
            return result_type::make(
                child_type(
                    detail::make_vector(_0)
                  , f.proto_base().child0)
            );
        }
    };
}}


#endif
