/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_ACCESS
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_ACCESS


#include <boost/utility/enable_if.hpp>

#include <boost/spirit/repository/home/support/buddy_pos_iterator.hpp>


namespace boost { namespace spirit { namespace repository
{
    struct buddy_access
    {
        template<class Derived>
        static inline
        void add_col(buddy_pos_iterator<Derived>& it, std::size_t n)
        {
            it.derived().add_col(n);
        }

        template<class Derived>
        static inline
        void inc_row(buddy_pos_iterator<Derived>& it)
        {
            it.derived().inc_row(); 
        }

        template<class Iterator>
        static inline
        typename disable_if<traits::is_buddy_pos_iterator<Iterator> >::type
        add_col(Iterator&, std::size_t)
        {}

        template<class Iterator>
        static inline
        typename disable_if<traits::is_buddy_pos_iterator<Iterator> >::type
        inc_row(Iterator&)
        {}
    };
}}}


#endif


