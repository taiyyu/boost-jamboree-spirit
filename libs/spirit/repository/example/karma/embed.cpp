/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    
//////////////////////////////////////////////////////////////////////////////*/

//  [ Jamboree April 22, 2011 ]     adapted example from Robert Nelson.


#include <cstdlib>
#include <iostream>
#include <iterator>

#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/repository/include/karma_embed.hpp>

//[reference_karma_embed_typedefs
typedef boost::fusion::vector2<float,float> vec;
typedef boost::fusion::vector2<vec,vec> vec_pair;
typedef std::back_insert_iterator<std::string> iter;
//]

//[reference_karma_embed_demo_func
vec sum(const vec& point1,const vec& point2)
{
    using boost::fusion::at_c;
    return vec(at_c<0>(point1) + at_c<0>(point2), at_c<1>(point1) + at_c<1>(point2));
}
//]

int main(int argc, char *argv[])
{
    //[reference_karma_embed_namespace
    using namespace boost::spirit::karma;
    using namespace boost::spirit::repository::karma;
    using namespace boost::phoenix;
    //]
    
    //[reference_karma_embed_rule_decl
    rule<iter, vec()> vec_rule;
    rule<iter, vec_pair()> main_rule;
    //]
    
    //[reference_karma_embed_rule_def
    main_rule %=
        "Point 1 is: " << vec_rule << eol 
     << "Point 2 is: " << vec_rule << eol
     << "Sum is:     " << embed(bind(&sum, at_c<0>(_val), at_c<1>(_val)))[vec_rule] << eol
     ;
    
    vec_rule = '[' << float_ << ',' << float_ << ']';
    //]
    
    std::string output;
    iter sink(output);
    
    //[reference_karma_embed_generate
    vec_pair data(vec(1,2),vec(3,4));    
    bool success = generate(sink, main_rule, data);
    //]
    
    assert(success);
    std::cout << output << std::endl;
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
