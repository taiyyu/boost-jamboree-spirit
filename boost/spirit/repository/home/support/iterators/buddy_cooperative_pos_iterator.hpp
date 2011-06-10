/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_COOPERATIVE_POS_ITERATOR
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_COOPERATIVE_POS_ITERATOR


#include <vector> // as default Cooperator

#include <boost/iterator/iterator_adaptor.hpp>

#include <boost/spirit/repository/home/support/buddy_pos_iterator.hpp>


namespace boost { namespace spirit { namespace repository
{            
    template <class Iterator, class Cooperator = std::vector<std::size_t> >
    class buddy_cooperative_pos_iterator
      : public buddy_pos_iterator<buddy_cooperative_pos_iterator<Iterator> >
      , public boost::iterator_adaptor
        <
            buddy_cooperative_pos_iterator<Iterator>  // Derived
          , Iterator                                  // Base
          , boost::use_default                        // Value
          , boost::forward_traversal_tag              // CategoryOrTraversal
        >
    {
    public:
        
        typedef Cooperator cooperator_type;
        
        buddy_cooperative_pos_iterator(Cooperator& co)
          : buddy_cooperative_pos_iterator::iterator_adaptor_()
          , _pos(), _co(co)
        {
            _co.resize(1, 0);
        } 

        buddy_cooperative_pos_iterator(Iterator const& base, Cooperator& co)
          : buddy_cooperative_pos_iterator::iterator_adaptor_(base)
          , _pos(), _co(co)
        {
            _co.resize(1, 0);
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
            if (_co.back() < _pos)
                _co.push_back(_pos);
        }

        void add_col(std::size_t n)
        {
            _pos += n;
        }

        std::size_t _pos;
        Cooperator& _co;    // stores the start-positions of lines
    };
}}}


#endif

