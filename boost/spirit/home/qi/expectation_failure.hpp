/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_QI_EXPECTATION_FAILURE
#define BOOST_SPIRIT_QI_EXPECTATION_FAILURE


#include <stdexcept>


namespace boost { namespace spirit { namespace qi
{
    template <typename Iterator>
    struct expectation_failure : std::runtime_error
    {
        expectation_failure(Iterator first, Iterator last, info const& what)
          : std::runtime_error("boost::spirit::qi::expectation_failure")
          , first(first), last(last), what_(what)
        {}
        ~expectation_failure() throw() {}

        Iterator first;
        Iterator last;
        info what_;
    };
    
    template <typename ID, typename Iterator>
    struct expectation_failure2 : std::runtime_error
    {
        expectation_failure2(ID id, Iterator first, Iterator last)
          : std::runtime_error("boost::spirit::qi::expectation_failure2")
          , id(id), first(first), last(last)
        {}
        ~expectation_failure2() throw() {}

        ID id;
        Iterator first;
        Iterator last;
    };
}}} // namespace boost::spirit::qi


#endif
