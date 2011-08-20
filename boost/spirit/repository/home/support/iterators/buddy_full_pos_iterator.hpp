/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_FULL_POS_ITERATOR
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_FULL_POS_ITERATOR


#include <boost/iterator/iterator_adaptor.hpp>

#include <boost/spirit/repository/home/support/buddy_pos_iterator.hpp>


namespace boost { namespace spirit { namespace repository
{
    template <class Iterator, typename Row = std::size_t, typename Col = Row>
    class buddy_full_pos_iterator
      : public buddy_pos_iterator<buddy_full_pos_iterator<Iterator, Row, Col> >
      , public boost::iterator_adaptor
        <
            buddy_full_pos_iterator<Iterator, Row, Col>     // Derived
          , Iterator                                        // Base
          , boost::use_default                              // Value
          , boost::forward_traversal_tag                    // CategoryOrTraversal
        >
    {
    public:

        typedef Row row_type;
        typedef Col col_type;

        buddy_full_pos_iterator()
          : buddy_full_pos_iterator::iterator_adaptor_(), _row(), _col()
        {}

        explicit buddy_full_pos_iterator(Iterator const& base)
          : buddy_full_pos_iterator::iterator_adaptor_(base), _row(), _col()
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

        void increment()
        {
            ++_col;
            ++this->base_reference();
        }

        template<class>
        friend class buddy_component;

        void inc_row()
        {
            ++_row;
            _col = 0;
        }

        void add_col(std::size_t n)
        {
            _col += n;
        }

        row_type _row;
        col_type _col;
    };
}}}


#endif

