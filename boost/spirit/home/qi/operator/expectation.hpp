/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_QI_EXPECTATION
#define BOOST_SPIRIT_QI_EXPECTATION


#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/qi/expectation_failure.hpp>
#include <boost/spirit/home/qi/detail/attributes.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/has_semantic_action.hpp>
#include <boost/spirit/home/support/handles_container.hpp>
#include <boost/spirit/home/support/multi_pass_wrapper.hpp>
#include <boost/throw_exception.hpp>
#include <boost/utility/declval.hpp>
#include <boost/typeof/typeof.hpp>


namespace boost { namespace spirit { namespace qi
{
    template <typename ID, typename Subject>
    struct expectation;
    
    template <typename T> 
    struct use_expectation
      : mpl::false_
    {};
}}} // namespace boost::spirit::qi


namespace boost { namespace spirit { namespace qi { namespace detail
{
    template <typename Grammar>
    struct make_expectation
      : proto::transform<make_expectation<Grammar> >
    {
        template <typename Expr, typename State, typename Data>
        struct impl : proto::transform_impl<Expr, State, Data>
        {
            // Strangely proto::result_of::value not works, we resort to
            // BOOST_TYPEOF. 
            typedef
                BOOST_TYPEOF(proto::value(proto::child_c<0>(declval<Expr>())))
            id_type;
            
            typedef typename Grammar::
                template result<Grammar(
                    typename proto::result_of::child_c<Expr, 1>::type
                  , State, Data)>::type
            subject_type;
            
            typedef expectation<id_type, subject_type> result_type;
            
            result_type operator()
            (
                typename impl::expr_param expr
              , typename impl::state_param state
              , typename impl::data_param data
            ) const
            {
                return result_type
                (
                    proto::value(proto::child_c<0>(expr))
                  , Grammar()(proto::child_c<1>(expr), state, data)
                );
            }
        };
    };
    
    struct is_expectation; // declaration only 
}}}} // namespace boost::spirit::qi::detail


namespace boost { namespace proto { namespace detail
{
    template <typename T>
    struct terminal_matches<T, spirit::qi::detail::is_expectation>
      : spirit::qi::use_expectation
        <
            typename remove_const<typename remove_reference<T>::type>::type
        > 
    {};
}}} // namespace boost::proto::detail


namespace boost { namespace spirit
{
    template <>
    template <> 
    struct meta_compiler<qi::domain>::cases::case_<proto::tag::divides>
      : proto::or_
        < 
            proto::when
            <
                proto::divides
                <
                    proto::terminal<qi::detail::is_expectation>
                  , meta_grammar
                >
              , qi::detail::make_expectation<meta_grammar>
            >
          , proto::otherwise // fallback 
            <
                detail::make_binary<qi::domain, proto::tag::divides, meta_grammar>
            >
        > 
    {};
}} // namespace boost::spirit


namespace boost { namespace spirit { namespace qi
{
    template <typename ID, typename Subject>
    struct expectation
      : unary_parser<expectation<ID, Subject> > 
    {
        typedef Subject subject_type;
        
        template <typename Context, typename Iterator>
        struct attribute
          : traits::attribute_of<subject_type, Context, Iterator>
        {};
        
        expectation(ID id, Subject const& subject)
          : id(id), subject(subject)
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
            // flush any multi_pass iterator we might be acting on 
            spirit::traits::clear_queue(first);
            
            if (subject.parse(first, last, context, skipper, attr))
                return true;
                 
            boost::throw_exception(
                expectation_failure2<ID, Iterator>(id, first, last));
#   if defined(BOOST_NO_EXCEPTIONS)
            return false;            // for systems not supporting exceptions
#   endif
        }
        
        template <typename Context>
        info what(Context& context) const
        {
            return info("expectation", subject.what(context));
        }
        
        ID id;
        Subject subject;
    };    
}}} // namespace boost::spirit::qi


namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename ID, typename Subject>
    struct has_semantic_action<qi::expectation<ID, Subject> >
      : unary_has_semantic_action<Subject> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename ID, typename Subject, typename Attribute, typename Context
        , typename Iterator>
    struct handles_container<qi::expectation<ID, Subject>, Attribute
        , Context, Iterator>
      : unary_handles_container<Subject, Attribute, Context, Iterator> {};
}}} // namespace boost::spirit::traits


#define BOOST_SPIRIT_QI_USE_EXPECTATION( type )                                 \
namespace boost { namespace spirit { namespace qi                               \
{                                                                               \
    template <>                                                                 \
    struct use_expectation<type>                                                \
      : mpl::true_                                                              \
    {};                                                                         \
}}}
/***/
 

#endif
