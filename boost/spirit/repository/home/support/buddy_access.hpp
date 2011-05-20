/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_ACCESS
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_ACCESS


namespace boost { namespace spirit { namespace repository
{
    template <class Iterator, typename Row, typename Col>
    class buddy_pos_iterator;
    
    struct buddy_access
    {
        template<class Iterator, typename R, typename C>
        static inline
        void add_col(buddy_pos_iterator<Iterator, R, C>& it, std::size_t n)
        {
            it._col += n;
        }

        template<class Iterator, typename R, typename C>
        static inline
        void inc_row(buddy_pos_iterator<Iterator, R, C>& it)
        {
            ++it._row;
            it._col = 0; 
        }
        
        template<class Iterator>
        static inline
        void add_col(Iterator&, std::size_t)
        {}

        template<class Iterator>
        static inline
        void inc_row(Iterator&)
        {}
    };
}}}


#endif


