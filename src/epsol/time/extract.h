#ifndef EPSOL_TIME_TIMER_EXTRACT_
#define EPSOL_TIME_TIMER_EXTRACT_

#include <chrono>
#include <tuple>
#include "epsol/time/time-zone.h"

namespace epsol {
namespace time {

/**
 * tags passing as the parameter of `extract`
 */
namespace tags {

class year {};
class month {};
class day {};
class hour {};
class minute {};
class second {};

}  // namespace tags

namespace detail {

/**
 * generate a bunch of helper function to extract field of tm according to tags
 */
#define DEF_EXTRACT_IMPL(time, field, bias)     \
    int extract_impl(tm* t, tags::time) { \
        return t->field + bias;                  \
    }

DEF_EXTRACT_IMPL(year, tm_year, 1900)
DEF_EXTRACT_IMPL(month, tm_mon, 1)
DEF_EXTRACT_IMPL(day, tm_mday, 0)
DEF_EXTRACT_IMPL(hour, tm_hour, 0)
DEF_EXTRACT_IMPL(minute, tm_min, 0)
DEF_EXTRACT_IMPL(second, tm_sec, 0)

#undef EXTRACT_IMPL

}  // namespace detail

template <typename... TimeTags, typename TimePoint>
auto extract(TimePoint tp, TimeZone tz = TimeZone::LOCAL) {
    std::tm* the_time = to_tm_ptr(tp, tz);
    return std::make_tuple(detail::extract_impl(the_time, TimeTags())...);
}

#define DEF_EXTRACT_SINGLE(time)                                      \
    template <typename TimePoint>                                     \
    int extract_##time(TimePoint tp, TimeZone tz = TimeZone::LOCAL) { \
        return std::get<0>(extract<tags::time>(tp, tz));              \
    }

DEF_EXTRACT_SINGLE(year)
DEF_EXTRACT_SINGLE(month)
DEF_EXTRACT_SINGLE(day)
DEF_EXTRACT_SINGLE(hour)
DEF_EXTRACT_SINGLE(minute)
DEF_EXTRACT_SINGLE(second)

#undef DEF_EXTRACT_SINGLE

}  // namespace time
}  // namespace epsol

#endif  // EPSOL_TIME_EXTRACT_
