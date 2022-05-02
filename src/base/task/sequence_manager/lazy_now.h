// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TASK_SEQUENCE_MANAGER_LAZY_NOW_H_
#define BASE_TASK_SEQUENCE_MANAGER_LAZY_NOW_H_

#include "base/base_export.h"
#include "base/time/time.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace base {

class TickClock;

namespace sequence_manager {

// Now() is somewhat expensive so it makes sense not to call Now() unless we
// really need to and to avoid subsequent calls if already called once.
// LazyNow objects are expected to be short-living to represent accurate time.
class BASE_EXPORT LazyNow {
 public:
  explicit LazyNow(TimeTicks now);
  explicit LazyNow(const TickClock* tick_clock);
  LazyNow(const LazyNow&) = delete;
  LazyNow& operator=(const LazyNow&) = delete;

  LazyNow(LazyNow&& move_from) noexcept;

  // Result will not be updated on any subsesequent calls.
  TimeTicks Now();

  bool has_value() const { return !!now_; }

 private:
  // `tick_clock_` is not a raw_ptr<TickClock> as a performance optimization:
  // The pointee doesn't need UaF protection (it has the same lifetime as the
  // theead/sequence).
  const TickClock* tick_clock_;  // Not owned.
  absl::optional<TimeTicks> now_;
};

}  // namespace sequence_manager
}  // namespace base

#endif  // BASE_TASK_SEQUENCE_MANAGER_LAZY_NOW_H_
