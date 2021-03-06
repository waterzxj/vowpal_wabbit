/*
Copyright (c) by respective owners including Yahoo!, Microsoft, and
individual contributors. All rights reserved.  Released under a BSD (revised)
license as described in the file LICENSE.
*/

#pragma once
#include <stdexcept>
#include <sstream>

#ifndef _NOEXCEPT
// _NOEXCEPT is required on Mac OS
// making sure other platforms don't barf
#define _NOEXCEPT throw()
#endif

namespace VW
{
class vw_exception : public std::exception
{
 private:
  // source file exception was thrown
  const char* file;

  std::string message;

  // line number exception was thrown
  int lineNumber;

 public:
  vw_exception(const char* file, int lineNumber, std::string message);
  vw_exception(const vw_exception& ex);
  vw_exception& operator=(const vw_exception& other);

  ~vw_exception() _NOEXCEPT;

  virtual const char* what() const _NOEXCEPT;
  const char* Filename() const;
  int LineNumber() const;
};

class vw_argument_disagreement_exception : public vw_exception
{
 public:
  vw_argument_disagreement_exception(const char* file, int lineNumber, std::string message)
      : vw_exception(file, lineNumber, message)
  {
  }

  vw_argument_disagreement_exception(const vw_argument_disagreement_exception& ex) : vw_exception(ex) {}

  vw_argument_disagreement_exception& operator=(const vw_argument_disagreement_exception& other)
  {
    // check for self-assignment
    if (&other == this)
      return *this;
    vw_exception::operator=(other);
    return *this;
  }

  ~vw_argument_disagreement_exception() _NOEXCEPT {}
};

class vw_argument_invalid_value_exception : public vw_exception
{
 public:
  vw_argument_invalid_value_exception(const char* file, int lineNumber, std::string message)
      : vw_exception(file, lineNumber, message)
  {
  }

  vw_argument_invalid_value_exception(const vw_argument_invalid_value_exception& ex) : vw_exception(ex) {}

  vw_argument_invalid_value_exception& operator=(const vw_argument_invalid_value_exception& other)
  {
    // check for self-assignment
    if (&other == this)
      return *this;
    vw_exception::operator=(other);
    return *this;
  }

  ~vw_argument_invalid_value_exception() _NOEXCEPT {}
};

class vw_unrecognised_option_exception : public vw_exception
{
 public:
  vw_unrecognised_option_exception(const char* file, int lineNumber, std::string message)
      : vw_exception(file, lineNumber, message)
  {
  }

  vw_unrecognised_option_exception(const vw_unrecognised_option_exception& ex) : vw_exception(ex) {}

  vw_unrecognised_option_exception& operator=(const vw_unrecognised_option_exception& other)
  {
    // check for self-assignment
    if (&other == this)
      return *this;
    vw_exception::operator=(other);
    return *this;
  }

  ~vw_unrecognised_option_exception() _NOEXCEPT {}
};

#ifdef _WIN32
void vw_trace(const char* filename, int linenumber, const char* fmt, ...);

// useful when hunting down release mode bugs
#define VW_TRACE(fmt, ...) VW::vw_trace(__FILE__, __LINE__, fmt, __VA_ARGS__)

struct StopWatchData;

class StopWatch
{
  StopWatchData* data;

 public:
  StopWatch();
  ~StopWatch();

  double MilliSeconds() const;
};

// Equivalent to System::Diagnostics::Debugger::Launch();
bool launchDebugger();

#define THROWERRNO(args)                                     \
  {                                                          \
    std::stringstream __msg;                                 \
    __msg << args;                                           \
    char __errmsg[256];                                      \
    if (strerror_s(__errmsg, sizeof __errmsg, errno) != 0)   \
      __msg << ", errno = unknown";                          \
    else                                                     \
      __msg << ", errno = " << __errmsg;                     \
    throw VW::vw_exception(__FILE__, __LINE__, __msg.str()); \
  }
#else
#define THROWERRNO(args)                                     \
  {                                                          \
    std::stringstream __msg;                                 \
    __msg << args;                                           \
    char __errmsg[256];                                      \
    if (strerror_r(errno, __errmsg, sizeof __errmsg) != 0)   \
      __msg << "errno = unknown";                            \
    else                                                     \
      __msg << "errno = " << __errmsg;                       \
    throw VW::vw_exception(__FILE__, __LINE__, __msg.str()); \
  }
#endif

// ease error handling and also log filename and line number
#define THROW(args)                                          \
  {                                                          \
    std::stringstream __msg;                                 \
    __msg << args;                                           \
    throw VW::vw_exception(__FILE__, __LINE__, __msg.str()); \
  }

#define THROW_EX(ex, args)                     \
  {                                            \
    std::stringstream __msg;                   \
    __msg << args;                             \
    throw ex(__FILE__, __LINE__, __msg.str()); \
  }

}  // namespace VW

#define VW_ASSERT(condition, args) \
  if (!(condition))                \
  {                                \
    THROW(args);                   \
  }
