#ifndef EXTRACTION_HELPER_FUNCTIONS_HPP
#define EXTRACTION_HELPER_FUNCTIONS_HPP

#include <boost/spirit/home/x3.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <algorithm>
#include <cctype>
#include <iterator>
#include <limits>
#include <string>
#include <iostream>

#include "guidance/parsing_toolkit.hpp"

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

namespace osrm::extractor
{

namespace parser
{
        using x3::eps;
        using x3::int_;
        using x3::lit;
        using x3::lexeme;
        using ascii::char_;

		auto const hh = x3::int_; // < 24
		auto const mm = x3::int_; // < 60
		auto const ss = x3::int_; // < 60
/*
		auto const osm_time = 

        osm_time = (uint_p[_a = _1] >> eoi)[_val = _a * 60] |
                   (uint_p[_a = _1] >> ':' >> uint_p[_b = _1] >> eoi)[_val = _a * 3600 + _b * 60] |
                   (uint_p[_a = _1] >> ':' >> uint_p[_b = _1] >> ':' >> uint_p[_c = _1] >>
                    eoi)[_val = _a * 3600 + _b * 60 + _c];

        alternative_time =
            ('T' >> hh[_a = _1] >> mm[_b = _1] >> ss[_c = _1])[_val = _a * 3600 + _b * 60 + _c];

        extended_time = ('T' >> hh[_a = _1] >> ':' >> mm[_b = _1] >> ':' >>
                         ss[_c = _1])[_val = _a * 3600 + _b * 60 + _c];

        standard_time =
            ('T' >> -(uint_ >> char_("Hh"))[_a = _1] >> -(uint_ >> char_("Mm"))[_b = _1] >>
             -(uint_ >> char_("Ss"))[_c = _1])[_val = _a * 3600 + _b * 60 + _c];

        standard_date = (uint_ >> char_("Dd"))[_val = _1 * 86400];

        standard_week = (uint_ >> char_("Ww"))[_val = _1 * 604800];
		
        iso_period =
            osm_time[_val = _1] | ('P' >> standard_week >> eoi)[_val = _1] |
            ('P' >> (alternative_time[_a = 0, _b = _1] | extended_time[_a = 0, _b = _1] |
                     (eps[_a = 0, _b = 0] >> -standard_date[_a = _1] >> -standard_time[_b = _1])) >>
             eoi)[_val = _a + _b];

        root = iso_period;*/

    //x3::uint_parser<unsigned, 10, 1, 2> uint_p;
    //x3::uint_parser<unsigned, 10, 2, 2> uint2_p;

    auto set_zero = [](auto& ctx){ _val(ctx) = 0; };
/*
    x3::rule<unsigned> standard_time const standard_time = "standard_time";
    x3::rule<unsigned> standard_week const standard_week = "standard_week";

	x3::rule<class iso_period, unsigned, unsigned> const iso_period = "iso_period";
    x3::rule<class osm_time, unsigned, unsigned, unsigned> const osm_time = "osm_time";
    x3::rule<class alternative_time, unsigned, unsigned, unsigned> const alternative_time = "alternative_time";
    x3:.rule<class extended_time, unsigned, unsigned, unsigned> const extended_time = "extended_time";
*/

	x3::rule<class iso_8601, unsigned> const iso_8601 = "iso_8601";
	
    auto const iso_8601_def = 
    	eps						[set_zero]
    ;
			//-(+lit(''))
			//>> -		[standard_week]
			//>> -		[standard_time]
			//>> -		[osm_time]
			//>> - 		[alternative_time]
			//>> -		[extended_time]
			//>> -iso_period	[iso_period]
    	//)
    	
	BOOST_SPIRIT_DEFINE(iso_8601);
	
} // namespace parser

inline bool durationIsValid(const std::string &s)
{
	std::cout << "GREPME " << s << '\n';
    using parser::iso_8601;

    std::string::const_iterator iter = s.begin();
    unsigned duration = 0;
    boost::spirit::x3::parse(iter, s.end(), iso_8601, duration);
	return true;
    return !s.empty() && iter == s.end();
}

inline unsigned parseDuration(const std::string &s)
{
	std::cout << "GREPME " << s << '\n';
	using parser::iso_8601;

    std::string::const_iterator iter = s.begin();
    unsigned duration = 0;
    boost::spirit::x3::parse(iter, s.end(), iso_8601, duration);
	return true;
    return !s.empty() && iter == s.end() ? 0 : std::numeric_limits<unsigned>::max();
}

inline std::string
trimLaneString(std::string lane_string, std::int32_t count_left, std::int32_t count_right)
{
    return guidance::trimLaneString(std::move(lane_string), count_left, count_right);
}

inline std::string applyAccessTokens(const std::string &lane_string,
                                     const std::string &access_tokens)
{
    return guidance::applyAccessTokens(lane_string, access_tokens);
}

// Takes a string representing a list separated by delim and canonicalizes containing spaces.
// Example: "aaa;bbb; ccc;  d;dd" => "aaa; bbb; ccc; d; dd"
inline std::string canonicalizeStringList(std::string strlist, const std::string &delim)
{
    // expand space after delimiter: ";X" => "; X"
    boost::replace_all(strlist, delim, delim + " ");

    // collapse spaces; this is needed in case we expand "; X" => ";  X" above
    // but also makes sense to do irregardless of the fact - canonicalizing strings.
    const auto spaces = [](unsigned char lhs, unsigned char rhs) {
        return ::isspace(lhs) && ::isspace(rhs);
    };
    auto it = std::unique(begin(strlist), end(strlist), spaces);
    strlist.erase(it, end(strlist));

    return strlist;
}

} // namespace osrm::extractor

#endif // EXTRACTION_HELPER_FUNCTIONS_HPP
