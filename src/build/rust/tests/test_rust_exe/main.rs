// Copyright 2021 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

use test_rlib_crate::say_hello_from_crate;

fn main() {
    assert_eq!(test_mixed_static_library::add_two_ints_via_rust_then_cpp(12, 13), 25);
    assert_eq!(test_proc_macro_crate::calculate_using_proc_macro!(), 30);
    assert_eq!(test_rust_static_library::add_two_ints_via_rust(3, 4), 7);
    assert_eq!(test_rust_static_library_non_standard_arrangement::do_subtract(4, 3), 1);
    say_hello_from_crate();
}

/// These tests are largely all to just test different permutations of builds,
/// e.g. calling into mixed_static_librarys, crates, proc macros, etc.
#[cfg(test)]
mod tests {
    #[test]
    fn test_call_to_rust() {
        assert_eq!(test_rust_static_library::add_two_ints_via_rust(3, 4), 7);
    }

    #[test]
    fn test_call_to_rust_non_standard_arrangement() {
        assert_eq!(test_rust_static_library_non_standard_arrangement::do_subtract(8, 4), 4);
    }

    #[test]
    fn test_call_to_rust_then_cpp() {
        assert_eq!(test_mixed_static_library::add_two_ints_via_rust_then_cpp(12, 13), 25)
    }

    #[test]
    fn test_proc_macro() {
        assert_eq!(test_proc_macro_crate::calculate_using_proc_macro!(), 30)
    }
}
