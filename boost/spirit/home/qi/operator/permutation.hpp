/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
//  [ Jamboree June 24, 2011 ]        add strict mode support.
#if !defined(SPIRIT_PERMUTATION_OR_MARCH_13_2007_1145PM)
#define SPIRIT_PERMUTATION_OR_MARCH_13_2007_1145PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/detail/permute_function.hpp>
#include <boost/spirit/home/qi/detail/attributes.hpp>
#include <boost/spirit/home/support/algorithm/any_if_ns.hpp>
#include <boost/spirit/home/support/detail/what_function.hpp>
#include <boost/spirit/home/support/has_semantic_action.hpp>
#include <boost/spirit/home/support/handles_container.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/array.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct use_operator<qi::domain, proto::tag::bitwise_xor> // enables ^
      : mpl::true_ {};

    template <>
    struct flatten_tree<qi::domain, proto::tag::bitwise_xor> // flattens ^
      : mpl::true_ {};
}}

namespace boost { namespace spirit { namespace qi
{
    template <typename Elements, bool strict>
    struct permutation : nary_parser<permutation<Elements, strict> >
    {
        template <typename Context, typename Iterator>
        struct attribute
        {
            // For strict:
            // Put all the element attributes in a tuple.
            typedef traits::build_attribute_sequence<
                Elements, Context, traits::sequence_attribute_transform
              , Iterator, qi::domain
            > strict_all_attributes;
            
            // For non-strict:
            // Put all the element attributes in a tuple,
            // wrapping each element in a boost::optional.
            typedef traits::build_attribute_sequence<
                Elements, Context, traits::permutation_attribute_transform
              , Iterator, qi::domain
            > non_strict_all_attributes;

            // Decide which it is.
            typedef typename 
                mpl::eval_if_c
                <
                    strict
                  , strict_all_attributes
                  , non_strict_all_attributes
                >::type
            all_attributes;

            // Now, build a fusion vector over the attributes. Note
            // that build_fusion_vector 1) removes all unused attributes
            // and 2) may return unused_type if all elements have
            // unused_type(s).
            typedef typename
                traits::build_fusion_vector<all_attributes>::type
            type;
        };

        permutation(Elements const& elements)
          : elements(elements) {}
        
        template <typename Iterator, typename Context
          , typename Skipper, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper
          , Attribute& attr_) const
        {
            typedef traits::attribute_not_unused<Context, Iterator> predicate;
            detail::permute_function<Iterator, Context, Skipper>
                f(first, last, context, skipper);

            boost::array<bool, fusion::result_of::size<Elements>::value> flags;
            BOOST_FOREACH(bool& taken, flags)
            {
                taken = false;
            }
            // We have a bool array 'flags' with one flag for each parser.
            // permute_function sets the slot to true when the corresponding
            // parser successful matches.
            f.taken = flags.begin();

            // wrap the attribute in a tuple if it is not a tuple
            typename traits::wrap_if_not_tuple<Attribute>::type attr(attr_);

            return do_parse(attr, f, predicate(), mpl::bool_<strict>()); 
        }
        
        // strict parse
        template <typename Attribute, typename F, typename Pred>
        bool do_parse(Attribute& attr, F const& f, Pred p, mpl::true_) const
        {
            std::size_t count = 0;
            detail::counted<F> cf(f, count);
            
            // We loop until there are no more successful parsers.
            // Succeeds only when all parsers have succeeded.
            while (spirit::any_if_ns(elements, attr, cf, p))
            {
                if (count == fusion::result_of::size<Elements>::value)
                    return true;
            }
            return false;
        }
        
        // non-strict parse
        template <typename Attribute, typename F, typename Pred>
        bool do_parse(Attribute& attr, F const& f, Pred p, mpl::false_) const
        {
            bool result = false;
            
            // We loop until there are no more successful parsers.
            // Succeeds when at least one parser has succeeded.
            while (spirit::any_if_ns(elements, attr, f, p))
            {
                result = true;
            }
            return result;
        }
        
        template <typename Context>
        info what(Context& context) const
        {
            info result("permutation");
            fusion::for_each(elements,
                spirit::detail::what_function<Context>(result, context));
            return result;
        }

        Elements elements;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Parser generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename Elements, typename Modifiers>
    struct make_composite<proto::tag::bitwise_xor, Elements, Modifiers>
    {
        static bool const strict =
            has_modifier<Modifiers, tag::strict>::value;
            
        typedef permutation<Elements, strict> result_type;
        
        result_type operator()(Elements const& elements, unused_type) const
        {
            return result_type(elements);
        }
    };
}}}

namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // We specialize this for permutation (see support/attributes.hpp).
    // For permutation, we only wrap the attribute in a tuple IFF
    // it is not already a fusion tuple.
    template <typename Elements, typename Attribute, bool strict>
    struct pass_attribute<qi::permutation<Elements, strict>, Attribute>
      : wrap_if_not_tuple<Attribute> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Elements, bool strict>
    struct has_semantic_action<qi::permutation<Elements, strict> >
      : nary_has_semantic_action<Elements> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Elements, bool strict, typename Attribute, typename Context
      , typename Iterator>
    struct handles_container<qi::permutation<Elements, strict>, Attribute, Context
      , Iterator>
      : nary_handles_container<Elements, Attribute, Context, Iterator> {};
}}}

#endif
