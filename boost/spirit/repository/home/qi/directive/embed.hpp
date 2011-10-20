/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_QI_EMBED
#define BOOST_SPIRIT_REPOSITORY_QI_EMBED


#if defined(_MSC_VER)
#pragma once
#endif


#include <boost/call_traits.hpp>

#include <boost/fusion/include/at.hpp>

#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/qi/auxiliary/lazy.hpp>
#include <boost/spirit/home/qi/detail/attributes.hpp>
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
    template <typename Range>
    struct use_directive<qi::domain
          , terminal_ex<repository::tag::embed, fusion::vector1<Range> > >
      : mpl::true_ {};

    // *lazy* embed(...)[...]
    // @note: we deal with the laziness ourselves.

}} // namespace boost::spirit


namespace boost { namespace spirit { namespace repository {namespace qi
{
#ifndef BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
    using repository::embed;
#endif
    using repository::embed_type;

    template <typename Subject, typename Range>
    struct embed_directive
      : spirit::qi::unary_parser<embed_directive<Subject, Range> >
    {
        typedef Subject subject_type;

        typedef typename
            call_traits<Range>::param_type
        range_param;

        typedef typename
            range_iterator<Range const>::type
        embedded_iterator;

        template <typename Context, typename /*Iterator*/>
        struct attribute
          : spirit::traits::attribute_of<subject_type, Context, embedded_iterator>
        {};

        embed_directive(Subject const& subject, range_param range)
          : subject(subject), range(range)
        {}

        template
        <
            typename Iterator, typename Context
          , typename Skipper, typename Attribute
        >
        bool parse
        (
            Iterator& /*first*/, Iterator const& /*last*/
          , Context& context, Skipper const& skipper
          , Attribute& attr
        ) const
        {
            embedded_iterator it(boost::begin(range));

            return subject.parse(it, boost::end(range)
                , context, skipper, attr);
        }

        template <typename Context>
        info what(Context& context) const
        {
            return info("embed", subject.what(context));
        }

        Subject subject;
        Range range;
    };
}}}} // namespace boost::spirit::repository::qi


namespace boost { namespace spirit { namespace qi
{
    ///////////////////////////////////////////////////////////////////////////
    // Parser generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename Range, typename Subject, typename Modifiers>
    struct make_directive
    <
        terminal_ex<repository::tag::embed, fusion::vector1<Range> >
      , Subject, Modifiers
    >
    {
        template <bool is_lazy, typename Dummy = void>
        struct result_helper // non-lazy
        {
            typedef repository::qi::embed_directive<Subject, Range> type;

            template <typename Terminal>
            static inline
            type make(
                Terminal const& term, Subject const& subject, unused_type)
            {
                return type(subject, fusion::at_c<0>(term.args));
            }
        };

        template <typename Dummy>
        struct result_helper<true, Dummy> // lazy
        {
            typedef
                phoenix::detail::expression::function_eval<repository::embed_type, Range>
            actor_gen;

            typedef lazy_directive<typename actor_gen::type, Subject, Modifiers> type;

            template <typename Terminal>
            static inline
            type make(
                Terminal const& term, Subject const& subject, Modifiers const& modifiers)
            {
                return type(actor_gen::make(term.term, fusion::at_c<0>(term.args))
                    , subject, modifiers);
            }
        };

        typedef result_helper<phoenix::is_actor<Range>::value> result_gen;

        typedef typename result_gen::type result_type;

        template <typename Terminal>
        result_type operator()(Terminal const& term, Subject const& subject
            , Modifiers const& modifiers) const
        {
            return result_gen::make(term, subject, modifiers);
        }
    };
}}} // namespace boost::spirit::qi


namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Range>
    struct has_semantic_action<repository::qi::embed_directive<Subject, Range> >
      : unary_has_semantic_action<Subject> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Range
        , typename Attribute, typename Context, typename Iterator>
    struct handles_container
    <
        repository::qi::embed_directive<Subject, Range>
      , Attribute, Context, Iterator
    >
      : unary_handles_container
        <
            Subject, Attribute, Context
          , typename repository::qi::embed_directive<Subject, Range>
                ::embedded_iterator
        >
    {};
}}} // namespace boost::spirit::traits


#endif
