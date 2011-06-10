/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_POS_ITERATOR
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_POS_ITERATOR


#include <boost/mpl/has_xxx.hpp>


namespace boost { namespace spirit { namespace repository
{            
    template <class Derived>
    struct buddy_pos_iterator
    {        
        struct buddy_pos_iterator_id;
        typedef Derived derived_type;

        // Requirement: it.inc_row()
        // Requirement: it.add_col(n)
        //
        //  it:         an iterator
        //  n:          a std::size_t
        
        Derived& derived()
        {
            return *static_cast<Derived*>(this);
        }
        
        Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }
    };
}}}


namespace boost { namespace spirit { namespace repository { namespace traits
{
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(buddy_pos_iterator_id)
    }
                    
    template <class Iterator>
    struct is_buddy_pos_iterator
      : detail::has_buddy_pos_iterator_id<Iterator> {};
}}}}


#endif

