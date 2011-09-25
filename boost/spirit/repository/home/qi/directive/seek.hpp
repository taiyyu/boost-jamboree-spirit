/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_QI_SEEK
#define BOOST_SPIRIT_REPOSITORY_QI_SEEK


#if defined(_MSC_VER)
#pragma once
#endif


#include <boost/iterator/iterator_traits.hpp>

#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/qi/auxiliary/lazy.hpp>
#include <boost/spirit/home/qi/detail/attributes.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/has_semantic_action.hpp>
#include <boost/spirit/home/support/handles_container.hpp>
#include <boost/spirit/repository/home/support/seek.hpp>


namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////

    // enables seek[...]
    template <>
    struct use_directive<qi::domain, repository::tag::seek>
      : mpl::true_ {};

    // enables seek(stride)[...]
    template <typename T>
    struct use_directive<qi::domain
        , terminal_ex<repository::tag::seek, fusion::vector1<T> > >
      : mpl::true_ {};
      
    // enables *lazy* seek(stride)[...]
    template <>
    struct use_lazy_directive<qi::domain, repository::tag::seek, 1>
      : mpl::true_ {};
}} // namespace boost::spirit


namespace boost { namespace spirit { namespace repository {namespace qi
{
#ifndef BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
    using repository::seek;
#endif
    using repository::seek_type;

    template <typename Subject>
    struct seek_directive
      : spirit::qi::unary_parser<seek_directive<Subject> >
    {
        typedef Subject subject_type;

        template <typename Context, typename Iterator>
        struct attribute
        {
            typedef typename
                traits::attribute_of<subject_type, Context, Iterator>::type
            type;
        };

        seek_directive(Subject const& subject)
          : subject(subject)
        {}

        template
        <
            typename Iterator, typename Context
          , typename Skipper, typename Attribute
        >
        bool parse
        (
            Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper
          , Attribute& attr
        ) const
        {
            for (Iterator it(first); ; ++it)
            {
                if (subject.parse(it, last, context, skipper, attr))
                {
                    first = it;
                    return true;
                }
                // fail only after subject fails & no input
                if (it == last)
                    return false;
            }
        }

        template <typename Context>
        info what(Context& context) const
        {
            return info("seek", subject.what(context));
        }

        Subject subject;
    };
    
    template <typename Subject>
    struct strided_seek_directive
      : spirit::qi::unary_parser<strided_seek_directive<Subject> >
    {
        typedef Subject subject_type;

        template <typename Context, typename Iterator>
        struct attribute
        {
            typedef typename
                traits::attribute_of<subject_type, Context, Iterator>::type
            type;
        };

        strided_seek_directive(Subject const& subject, std::size_t stride)
          : subject(subject), stride(stride)
        {}

        template
        <
            typename Iterator, typename Context
          , typename Skipper, typename Attribute
        >
        bool parse
        (
            Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper
          , Attribute& attr
        ) const
        {
            typedef typename boost::BOOST_ITERATOR_CATEGORY<Iterator>::type
                tag;
            
            return this->parse_impl(first, last, context, skipper, attr, tag());
        }

        template
        <
            typename Iterator, typename Context
          , typename Skipper, typename Attribute
        >
        bool parse_impl
        (
            Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper
          , Attribute& attr, std::random_access_iterator_tag
        ) const
        {
            for (Iterator start(first); ; )
            {
                Iterator it(start);
                
                if (subject.parse(it, last, context, skipper, attr))
                {
                    first = it;
                    return true;
                }

                if ((start += stride) > last)
                    return false;
            }
        }

        template
        <
            typename Iterator, typename Context
          , typename Skipper, typename Attribute
        >
        bool parse_impl
        (
            Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper
          , Attribute& attr, std::forward_iterator_tag
        ) const
        {
            for (Iterator start(first); ; )
            {
                Iterator it(start);
                
                if (subject.parse(it, last, context, skipper, attr))
                {
                    first = it;
                    return true;
                }

                for (std::size_t i = 0; i != stride; ++i, ++start)
                    if (start == last)
                        return false;
            }
        }
                
        template <typename Context>
        info what(Context& context) const
        {
            return info("seek", subject.what(context));
        }

        Subject subject;
        std::size_t stride;
    };
}}}} // namespace boost::spirit::repository::qi


namespace boost { namespace spirit { namespace qi
{
    ///////////////////////////////////////////////////////////////////////////
    // Parser generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Modifiers>
    struct make_directive<repository::tag::seek, Subject, Modifiers>
    {
        typedef repository::qi::seek_directive<Subject> result_type;

        result_type operator()(unused_type, Subject const& subject, unused_type) const
        {
            return result_type(subject);
        }
    };
    
    template <typename T, typename Subject, typename Modifiers>
    struct make_directive
    <
        terminal_ex<repository::tag::seek, fusion::vector1<T> >
      , Subject, Modifiers
    >
    {
        typedef repository::qi::strided_seek_directive<Subject> result_type;

        template <typename Terminal>
        result_type operator()(
            Terminal const& term, Subject const& subject, unused_type) const
        {
            return result_type(subject, fusion::at_c<0>(term.args));
        }
    };
}}} // namespace boost::spirit::qi


namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject>
    struct has_semantic_action<repository::qi::seek_directive<Subject> >
      : unary_has_semantic_action<Subject> {};
      
    template <typename Subject>
    struct has_semantic_action<repository::qi::strided_seek_directive<Subject> >
      : unary_has_semantic_action<Subject> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Attribute, typename Context
        , typename Iterator>
    struct handles_container<repository::qi::seek_directive<Subject>, Attribute
        , Context, Iterator>
      : unary_handles_container<Subject, Attribute, Context, Iterator> {};
      
    template <typename Subject, typename Attribute, typename Context
        , typename Iterator>
    struct handles_container<repository::qi::strided_seek_directive<Subject>, Attribute
        , Context, Iterator>
      : unary_handles_container<Subject, Attribute, Context, Iterator> {};
}}} // namespace boost::spirit::traits


#endif
