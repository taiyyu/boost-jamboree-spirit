/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_CO_POS_ITERATOR
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_CO_POS_ITERATOR


#include <deque> // as default Cooperator

#include <boost/iterator/iterator_adaptor.hpp>

#include <boost/spirit/repository/home/support/buddy_pos_iterator.hpp>


namespace boost { namespace spirit { namespace repository
{
    template <class Iterator, class Cooperator = std::deque<std::size_t> >
    class buddy_co_pos_iterator
      : public buddy_pos_iterator
        <buddy_co_pos_iterator<Iterator, Cooperator> >
      , public boost::iterator_adaptor
        <
            buddy_co_pos_iterator<Iterator>  // Derived
          , Iterator                         // Base
          , boost::use_default               // Value
          , boost::forward_traversal_tag     // CategoryOrTraversal
        >
    {
        typedef buddy_co_pos_iterator this_type;

    public:

        typedef Cooperator cooperator_type;

        buddy_co_pos_iterator()
          : this_type::iterator_adaptor_()
          , _pos(), _co()
        {
        }

        explicit buddy_co_pos_iterator(Cooperator& co)
          : this_type::iterator_adaptor_()
          , _pos(), _co(&co)
        {
            _co->resize(1, 0);
        }

        explicit buddy_co_pos_iterator(Iterator const& base)
          : this_type::iterator_adaptor_(base)
          , _pos(), _co()
        {
        }

        buddy_co_pos_iterator(Iterator const& base, Cooperator& co)
          : this_type::iterator_adaptor_(base)
          , _pos(), _co(&co)
        {
            _co->resize(1, 0);
        }

        std::size_t pos() const
        {
            return _pos;
        }

    private:

        friend class boost::iterator_core_access;

        void increment()
        {
            ++_pos;
            ++this->base_reference();
        }

        template<class>
        friend class buddy_component;

        void inc_row()
        {
            // assert(_co);
            if (_co->back() < _pos)
                _co->push_back(_pos);
        }

        void add_col(std::size_t n)
        {
            _pos += n;
        }

        std::size_t _pos;
        Cooperator* _co;    // stores the start-positions of lines
    };
}}}


#endif

