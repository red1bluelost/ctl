//===- scope_trace.hpp - TODO: description -------------------------*- C++
//-*-===//
//
// TODO: License
//
//===----------------------------------------------------------------------===//
///
/// \file
/// TODO: description
///
//===----------------------------------------------------------------------===//

#ifndef CTL_TEST_UTIL_SCOPE_TRACE_HPP
#define CTL_TEST_UTIL_SCOPE_TRACE_HPP

#include <gtest/gtest.h>

#include <source_location>

namespace ctl_tu {

using src_loc = std::source_location;

inline testing::ScopedTrace make_scoped_trace(src_loc loc, const char* info) {
  return testing::ScopedTrace{
      loc.file_name(), static_cast<int>(loc.line()), info};
}

} // namespace ctl_tu

#endif // CTL_TEST_UTIL_SCOPE_TRACE_HPP
