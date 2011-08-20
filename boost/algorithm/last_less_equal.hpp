/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_ALGORITHM_LAST_LESS_EQUAL_HPP_INCLUDED
#define BOOST_ALGORITHM_LAST_LESS_EQUAL_HPP_INCLUDED


#include <iterator>


namespace boost
{
    template<class ForwardIterator, class T>
    ForwardIterator last_less_equal
    (
        ForwardIterator first
      , ForwardIterator last
      , T const& val
    )
    {
        ForwardIterator ret(last);

        if (first == last)
            return ret;

        for ( ; ; )
        {
            ForwardIterator pivot(first);
            std::advance(pivot, std::distance(first, last) >> 1);

            // not less equal
            if (val < *pivot)
            {
                if (pivot == first)
                    return ret;

                last = pivot;
            }
            // less equal
            else
            {
                first = pivot;
                // the last one
                if (++first == last)
                    return pivot;

                ret = pivot;
            }
        }
    }
} // namespace boost


#endif
