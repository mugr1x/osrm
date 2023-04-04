#ifndef EXTRACTION_HELPER_FUNCTIONS_HPP
#define EXTRACTION_HELPER_FUNCTIONS_HPP

#include <boost/spirit/home/x3.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <algorithm>
#include <cctype>
#include <iterator>
#include <limits>
#include <string>

#include "guidance/parsing_toolkit.hpp"

namespace x3 = boost::spirit::x3;

namespace osrm::extractor
{
/*
namespace parser
{
        using x3::_pass;
        using x3::_val;
        using x3::_attr;
        using x3::char_;
        using x3::eoi;
        using x3::eps;
        using x3::uint_;

// OK trying to understand this now

		/// hours preasumeably in 01, etc 24 etc., includes checks < 24 etc. assigns _val  _1
        hh = uint2_p[_pass = bind([](unsigned x) { return x < 24; }, _1), _val = _1];
        // minutes
        mm = uint2_p[_pass = bind([](unsigned x) { return x < 60; }, _1), _val = _1];
        // seconds
        ss = uint2_p[_pass = bind([](unsigned x) { return x < 60; }, _1), _val = _1];

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

        root = iso_period;
    }

    x3::rule<unsigned()> root;
    x3::rule<unsigned()> iso_period;
    x3::rule<unsigned()> osm_time, standard_time, alternative_time, extended_time;
    x3::rule<unsigned()> standard_date, standard_week;
    x3::rule<unsigned()> hh, mm, ss;

    x3::uint_parser<unsigned, 10, 1, 2> uint_p;
    x3::uint_parser<unsigned, 10, 2, 2> uint2_p;
} // namespace parser
*/
inline bool durationIsValid(const std::string &s)
{	
	std::cout << s << '\n';
	/*
    static detail::iso_8601_grammar<std::string::const_iterator> const iso_8601_grammar;

    std::string::const_iterator iter = s.begin();
    unsigned duration = 0;
    boost::spirit::qi::parse(iter, s.end(), iso_8601_grammar, duration);

    return !s.empty() && iter == s.end();*/
    
    return true;
}

inline unsigned parseDuration(const std::string &s)
{
	std::cout << s << '\n';
	/*
    static detail::iso_8601_grammar<std::string::const_iterator> const iso_8601_grammar;

    std::string::const_iterator iter = s.begin();
    unsigned duration = 0;
    boost::spirit::x3::parse(iter, s.end(), iso_8601_grammar, duration);

	return !s.empty() && iter == s.end() ? duration : std::numeric_limits<unsigned>::max();
	*/
	return true;
}

inline std::string trimLaneString(std::string lane_string, std::int32_t count_left, std::int32_t count_right)
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
