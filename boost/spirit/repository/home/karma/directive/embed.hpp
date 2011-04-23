/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_KARMA_EMBED
#define BOOST_SPIRIT_REPOSITORY_KARMA_EMBED


#if defined(_MSC_VER)
#pragma once
#endif


#include <boost/call_traits.hpp>

#include <boost/fusion/include/at.hpp>

#include <boost/spirit/home/karma/meta_compiler.hpp>
#include <boost/spirit/home/karma/generator.hpp>
#include <boost/spirit/home/karma/auxiliary/lazy.hpp>
#include <boost/spirit/home/karma/detail/attributes.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/has_semantic_action.hpp>
#include <boost/spirit/home/support/handles_container.hpp>
#include <boost/spirit/repository/home/support/embed.hpp>


namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////

    // enables embed(...)[...]
    // @note: neccessary to define this, but we usually don't use it directly
    template <typename Stuff>
    struct use_directive<karma::domain
          , terminal_ex<repository::tag::embed, fusion::vector1<Stuff> > >
      : mpl::true_ {};

    // enables *lazy* embed(...)[...]
    template <>
    struct use_lazy_directive<karma::domain, repository::tag::embed, 1> 
      : mpl::true_ {};
}} // namespace boost::spirit


namespace boost { namespace spirit { namespace repository {namespace karma
{        
#ifndef BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
    using repository::embed;
#endif
    using repository::embed_type;
    
    template <typename Subject, typename Stuff>
    struct embed_directive
      : spirit::karma::unary_generator<embed_directive<Subject, Stuff> >
    {
        typedef Subject subject_type;
        
        typedef typename
            call_traits<Stuff>::param_type
        stuff_param;
        
        template <typename Context, typename /*Iterator*/>
        struct attribute
        {
            typedef unused_type type;
        };
            
        embed_directive(Subject const& subject, stuff_param stuff)
          : subject(subject), stuff(stuff)
        {}
        
        template
        <
            typename OutputIterator, typename Context
          , typename Delimiter, typename Attribute
        >
        bool generate
        (
            OutputIterator& sink, Context& ctx, Delimiter const& d
          , Attribute const& /*attr*/
        ) const
        {
            return subject.generate(sink, ctx, d, stuff);
        }
        
        template <typename Context>
        info what(Context& context) const
        {
            return info("embed", subject.what(context));
        }
        
        Subject subject;
        Stuff stuff;
    };    
}}}} // namespace boost::spirit::repository::karma


namespace boost { namespace spirit { namespace karma
{
    ///////////////////////////////////////////////////////////////////////////
    // Generator generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename Stuff, typename Subject, typename Modifiers>
    struct make_directive
    <
        terminal_ex<repository::tag::embed, fusion::vector1<Stuff> >
      , Subject, Modifiers
    >
    {
        typedef repository::karma::embed_directive<Subject, Stuff> result_type;
        
        template <typename Terminal>
        result_type operator()(
            Terminal const& term, Subject const& subject, unused_type) const
        {
            return result_type(subject, fusion::at_c<0>(term.args));
        }
    };
}}} // namespace boost::spirit::karma


namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Stuff>
    struct has_semantic_action<repository::karma::embed_directive<Subject, Stuff> >
      : unary_has_semantic_action<Subject> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Stuff
        , typename Attribute, typename Context, typename Iterator>
    struct handles_container
    <
        repository::karma::embed_directive<Subject, Stuff>
      , Attribute, Context, Iterator
    >
      : mpl::false_ {}; // FIXME
}}} // namespace boost::spirit::traits


#endif
