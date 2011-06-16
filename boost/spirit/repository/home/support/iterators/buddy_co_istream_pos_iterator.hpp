/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_CO_ISTREAM_POS_ITERATOR
#define BOOST_SPIRIT_REPOSITORY_SUPPORT_BUDDY_CO_ISTREAM_POS_ITERATOR


#include <deque> // as default Cooperator

#include <boost/spirit/home/support/iterators/detail/ref_counted_policy.hpp>
#if defined(BOOST_SPIRIT_DEBUG)
#include <boost/spirit/home/support/iterators/detail/buf_id_check_policy.hpp>
#else
#include <boost/spirit/home/support/iterators/detail/no_check_policy.hpp>
#endif
#include <boost/spirit/home/support/iterators/detail/split_std_deque_policy.hpp>
#include <boost/spirit/home/support/iterators/detail/combine_policies.hpp>
#include <boost/spirit/home/support/iterators/multi_pass.hpp>
#include <boost/spirit/home/support/iterators/detail/unformatted_istream_policy.hpp>

#include <boost/spirit/repository/home/support/buddy_pos_iterator.hpp>


namespace boost { namespace spirit { namespace repository
{
    ///////////////////////////////////////////////////////////////////////////
    template
    <
        typename Elem
      , typename Traits = std::char_traits<Elem>
      , class Cooperator = std::deque<std::size_t>
    >
    class basic_buddy_co_istream_pos_iterator
      : public buddy_pos_iterator
        <basic_buddy_co_istream_pos_iterator<Elem, Traits, Cooperator> > 
      , public multi_pass<
            std::basic_istream<Elem, Traits>
          , iterator_policies::default_policy<
                iterator_policies::ref_counted
#if defined(BOOST_SPIRIT_DEBUG)
              , iterator_policies::buf_id_check
#else
              , iterator_policies::no_check
#endif
              , iterator_policies::unformatted_istream
              , iterator_policies::split_std_deque> 
        >
    {
        typedef multi_pass<
            std::basic_istream<Elem, Traits>
          , iterator_policies::default_policy<
                iterator_policies::ref_counted
#if defined(BOOST_SPIRIT_DEBUG)
              , iterator_policies::buf_id_check
#else
              , iterator_policies::no_check
#endif
              , iterator_policies::unformatted_istream
              , iterator_policies::split_std_deque> 
        > base_type;
        
        typedef basic_buddy_co_istream_pos_iterator this_type; 

    public:
        
        typedef Cooperator cooperator_type;
        
        basic_buddy_co_istream_pos_iterator()
          : base_type(), _pos(), _co()
        {}
        
        explicit basic_buddy_co_istream_pos_iterator(Cooperator& co)
          : base_type(), _pos(), _co(&co)
        {
            _co->resize(1, 0);
        }

        explicit basic_buddy_co_istream_pos_iterator
        (std::basic_istream<Elem, Traits>& x)
          : base_type(x), _pos(), _co()
        {}
        
        basic_buddy_co_istream_pos_iterator
        (std::basic_istream<Elem, Traits>& x, Cooperator& co)
          : base_type(x), _pos(), _co(&co)
        {
            _co->resize(1, 0);
        }
        
        this_type& operator++()
        {
            this->base_type::operator++();
            ++_pos;
            return *this;
        }

        this_type operator++(int)
        {
            this_type ret(*this); 
            this->operator++();
            return ret;
        }
                
        std::size_t pos() const
        {
            return _pos;
        }
   
    private:
        
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

    typedef basic_buddy_co_istream_pos_iterator<char>
        buddy_co_istream_pos_iterator;

}}}


#endif
