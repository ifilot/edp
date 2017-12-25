/**************************************************************************
 *                                                                        *
 *   Author: Ivo Filot <i.a.w.filot@tue.nl>                               *
 *                                                                        *
 *   DEN2BIN is free software:                                            *
 *   you can redistribute it and/or modify it under the terms of the      *
 *   GNU General Public License as published by the Free Software         *
 *   Foundation, either version 3 of the License, or (at your option)     *
 *   any later version.                                                   *
 *                                                                        *
 *   DEN2BIN is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#ifndef _FLOAT_PARSER_H
#define _FLOAT_PARSER_H

#include <vector>
#include <boost/spirit/include/qi.hpp>

struct float_parser : boost::spirit::qi::grammar<std::string::const_iterator,
                                                  std::vector<float>(),
                                                  boost::spirit::ascii::space_type> {

  float_parser() : float_parser::base_type( vector ) {
    vector  %= +(boost::spirit::qi::float_);
  }

  boost::spirit::qi::rule<std::string::const_iterator, std::vector<float>(), boost::spirit::ascii::space_type> vector;
};

#endif //_FLOAT_PARSER_H
