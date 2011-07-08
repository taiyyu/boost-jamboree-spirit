/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_QI_CHOICES
#define BOOST_SPIRIT_REPOSITORY_QI_CHOICES


#if defined(_MSC_VER)
#pragma once
#endif


#include <deque> 

#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/cons.hpp>
#include <boost/fusion/include/as_list.hpp>
#include <boost/fusion/include/as_vector.hpp>

#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/argument.hpp>
#include <boost/spirit/home/support/context.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/qi/detail/attributes.hpp>
#include <boost/spirit/home/support/nonterminal/extract_param.hpp>
#include <boost/spirit/home/support/nonterminal/locals.hpp>
#include <boost/spirit/home/qi/reference.hpp>
#include <boost/spirit/home/qi/nonterminal/detail/parameterized.hpp>
#include <boost/spirit/home/qi/nonterminal/detail/parser_binder.hpp>
#include <boost/spirit/home/qi/skip_over.hpp>
#include <boost/spirit/repository/home/qi/nonterminal/nonterminal_fwd.hpp>


#if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable: 4355) // 'this' : used in base member initializer list warning
#endif


namespace boost { namespace spirit { namespace repository {namespace qi
{
    template <
        typename Iterator, typename T1, typename T2, typename T3
      , typename T4>
    struct choices
      : proto::extends<
            typename proto::terminal<
                spirit::qi::reference<choices<Iterator, T1, T2, T3, T4> const>
            >::type
          , choices<Iterator, T1, T2, T3, T4>
        >
      , spirit::qi::parser<choices<Iterator, T1, T2, T3, T4> >
    {
        typedef Iterator iterator_type;
        typedef choices<Iterator, T1, T2, T3, T4> this_type;
        typedef spirit::qi::reference<this_type const> reference_;
        typedef typename proto::terminal<reference_>::type terminal;
        typedef proto::extends<terminal, this_type> base_type;
        typedef mpl::vector<T1, T2, T3, T4> template_params;

        // The choices' locals_type: a sequence of types to be used as local variables
        typedef typename
            spirit::detail::extract_locals<template_params>::type
        locals_type;

        // The choices' skip-parser type
        typedef typename
            spirit::detail::extract_component<
                spirit::qi::domain, template_params>::type
        skipper_type;

        // The choices' signature
        typedef typename
            spirit::detail::extract_sig<template_params>::type
        sig_type;

        // The choices' encoding type
        typedef typename
            spirit::detail::extract_encoding<template_params>::type
        encoding_type;

        // This is the choices' attribute type
        typedef typename
            spirit::detail::attr_from_sig<sig_type>::type
        attr_type;
        typedef typename add_reference<attr_type>::type attr_reference_type;

        // parameter_types is a sequence of types passed as parameters to the choices
        typedef typename
            spirit::detail::params_from_sig<sig_type>::type
        parameter_types;

        static size_t const params_size =
            fusion::result_of::size<parameter_types>::type::value;

        typedef context<
            fusion::cons<attr_reference_type, parameter_types>
          , locals_type>
        context_type;

        typedef function<
            bool(Iterator& first, Iterator const& last
              , context_type& context
              , skipper_type const& skipper
            )>
        function_type;
        
        typedef std::deque<function_type> functions_type; 

        typedef typename
            mpl::if_<
                is_same<encoding_type, unused_type>
              , unused_type
              , tag::char_code<tag::encoding, encoding_type>
            >::type
        encoding_modifier_type;
        
        explicit choices(std::string const& name_ = "unnamed-choices")
          : base_type(terminal::make(reference_(*this)))
          , name_(name_)
        {
        }

        choices(choices const& choice)
          : base_type(terminal::make(reference_(*this)))
          , name_(choice.name_)
          , fs(choice.fs)
        {
        }
        
        template <typename Auto, typename Expr>
        void define_next(Expr const& expr, mpl::false_)
        {
            // Report invalid expression error as early as possible.
            // If you got an error_invalid_expression error message here,
            // then the expression (expr) is not a valid spirit qi expression.
            BOOST_SPIRIT_ASSERT_MATCH(spirit::qi::domain, Expr);
        }

        template <typename Auto, typename Expr>
        void define_next(Expr const& expr, mpl::true_)
        {
            fs.push_back(
                spirit::qi::detail::bind_parser<Auto>(
                    compile<spirit::qi::domain>(expr, encoding_modifier_type())));
        }
        
        void clear()
        {
            fs.clear();
        }
        
        struct adder;
        
        adder next()
        {
            return adder(this);
        }
        
        template <typename /*Context*/, typename /*Iterator*/>
        struct attribute
        {
            typedef attr_type type;
        };
        
        template <typename Context, typename Skipper, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& /*context*/, Skipper const& skipper
          , Attribute& attr) const
        {
            // do a preskip if this is an implied lexeme
            if (is_same<skipper_type, unused_type>::value)
                spirit::qi::skip_over(first, last, skipper);

            typedef traits::make_attribute<attr_type, Attribute> make_attribute;

            // do down-stream transformation, provides attribute for
            // choice parser
            typedef traits::transform_attribute<
                typename make_attribute::type, attr_type, spirit::qi::domain>
            transform;

            typename functions_type::const_iterator it(fs.begin()), end(fs.end()); 

            for ( ; it != end; ++it)
            {
                typename make_attribute::type made_attr = make_attribute::call(attr);
                typename transform::type attr_ = transform::pre(made_attr);
    
                // If you are seeing a compilation error here, you are probably
                // trying to use a rule or a grammar which has inherited
                // attributes, without passing values for them.
                context_type context(attr_);
                
                // If you are seeing a compilation error here stating that the
                // fourth parameter can't be converted to a required target type
                // then you are probably trying to use choices with
                // an incompatible skipper type.
                if ((*it)(first, last, context, skipper))
                {
                    // do up-stream transformation, this integrates the results
                    // back into the original attribute value, if appropriate
                    traits::post_transform(attr, attr_);
                    return true;
                }
    
                // inform attribute transformation of failed choice
                traits::fail_transform(attr, attr_);
            } 
            return false;
        }

        template <typename Context, typename Skipper
          , typename Attribute, typename Params>
        bool parse(Iterator& first, Iterator const& last
          , Context& caller_context, Skipper const& skipper
          , Attribute& attr, Params const& params) const
        {
            // do a preskip if this is an implied lexeme
            if (is_same<skipper_type, unused_type>::value)
                spirit::qi::skip_over(first, last, skipper);

            typedef traits::make_attribute<attr_type, Attribute> make_attribute;

            // do down-stream transformation, provides attribute for
            // choice parser
            typedef traits::transform_attribute<
                typename make_attribute::type, attr_type, spirit::qi::domain>
            transform;

            typename functions_type::const_iterator it(fs.begin()), end(fs.end()); 

            for ( ; it != end; ++it)
            {
                typename make_attribute::type made_attr = make_attribute::call(attr);
                typename transform::type attr_ = transform::pre(made_attr);

                // If you are seeing a compilation error here, you are probably
                // trying to use a rule or a grammar which has inherited
                // attributes, passing values of incompatible types for them.
                context_type context(attr_, params, caller_context);

                // If you are seeing a compilation error here stating that the
                // fourth parameter can't be converted to a required target type
                // then you are probably trying to use choices with
                // an incompatible skipper type.
                if ((*it)(first, last, context, skipper))
                {
                    // do up-stream transformation, this integrates the results
                    // back into the original attribute value, if appropriate
                    traits::post_transform(attr, attr_);
                    return true;
                }

                // inform attribute transformation of failed choice
                traits::fail_transform(attr, attr_);
            }
            return false;
        }
        
        template <typename Context>
        info what(Context& /*context*/) const
        {
            return info(name_);
        }

        void name(std::string const &str)
        {
            name_ = str;
        }
        std::string const &name() const
        {
            return name_;
        }
        
        struct adder
        {
            explicit adder(choices* that)
              : that(that)
            {}
            
            template <typename Expr>
            void operator=(Expr const& expr)
            {
                that->define_next<mpl::false_>(
                    expr, traits::matches<spirit::qi::domain, Expr>());
            }
            
            template <typename Expr>
            void operator%=(Expr const& expr)
            {
                that->define_next<mpl::true_>(
                    expr, traits::matches<spirit::qi::domain, Expr>());
            }
            
            choices* const that;
        };
        
        // bring in the operator() overloads
        choices const& get_parameterized_subject() const { return *this; }
        typedef choices parameterized_subject_type;
        #include <boost/spirit/home/qi/nonterminal/detail/fcall.hpp>
        
        std::string name_;
        functions_type fs;
    };
}}}} // namespace boost::spirit::repository::qi


namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename IteratorA, typename IteratorB, typename Attribute
      , typename Context, typename T1, typename T2, typename T3, typename T4>
    struct handles_container<
        repository::qi::choices<IteratorA, T1, T2, T3, T4>, Attribute, Context, IteratorB>
      : traits::is_container<
          typename attribute_of<
              repository::qi::choices<IteratorA, T1, T2, T3, T4>, Context, IteratorB
          >::type
        >
    {};
}}}


#if defined(BOOST_MSVC)
# pragma warning(pop)
#endif


#endif
 
