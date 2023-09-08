#ifndef BASE_LOGGING_H
#define BASE_LOGGING_H

#include <bits/stdc++.h>

#include "clock.h"

#define LOG                                                                \
  Stream().stream() << "[" << __FILE__ << "(" << __LINE__ << ")(" << Now() \
                    << ")] "

class Stream {
 public:
  ~Stream() {
    stream() << std::endl;
    std::cout << stream().str();
  }
  std::ostringstream& stream() { return stream_; };

 private:
  std::ostringstream stream_;
};

#endif  // BASE_LOGGING_H