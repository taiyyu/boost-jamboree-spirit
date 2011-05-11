/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_POS_ITERATOR
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_POS_ITERATOR


#include <boost/iterator/iterator_adaptor.hpp>


namespace boost { namespace spirit { namespace repository
{            
    template <class Iterator, typename Row = std::size_t, typename Col = Row>
    class buddy_pos_iterator
      : public boost::iterator_adaptor
        <
            buddy_pos_iterator<Iterator>    // Derived
          , Iterator                        // Base
          , boost::use_default              // Value
          , boost::forward_traversal_tag    // CategoryOrTraversal
        >
    {
    public:
        
        typedef Row row_type;
        typedef Col col_type;
        
        buddy_pos_iterator()
          : buddy_pos_iterator::iterator_adaptor_(), _row(), _col() 
        {} 

        explicit buddy_pos_iterator(Iterator const& base)
          : buddy_pos_iterator::iterator_adaptor_(base), _row(), _col() 
        {} 

        row_type row() const
        {
            return _row;
        }

        col_type col() const
        {
            return _col;
        }
        
    private:
        
        friend class boost::iterator_core_access;
        friend class buddy_access;

        void increment()
        {
            ++_col;
            ++this->base_reference();
        }

        row_type _row;
        col_type _col;
    };
    
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

