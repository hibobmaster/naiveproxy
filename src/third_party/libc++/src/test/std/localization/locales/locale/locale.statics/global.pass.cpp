//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: locale.en_US.UTF-8

// XFAIL: availability-char8_t_support-missing

// This test runs in C++20, but we have deprecated codecvt<char(16|32), char, mbstate_t> in C++20.
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <locale>

// static const locale& classic();

#include <locale>
#include <cassert>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

template<class CharT>
void check_for(const std::locale& loc)
{
    assert(std::has_facet<std::collate<CharT> >(loc));

    assert(std::has_facet<std::ctype<CharT> >(loc));

    assert((std::has_facet<std::codecvt<CharT, char, std::mbstate_t> >(loc)));

    assert(std::has_facet<std::moneypunct<CharT> >(loc));
    assert(std::has_facet<std::money_get<CharT> >(loc));
    assert(std::has_facet<std::money_put<CharT> >(loc));

    assert(std::has_facet<std::numpunct<CharT> >(loc));
    assert(std::has_facet<std::num_get<CharT> >(loc));
    assert(std::has_facet<std::num_put<CharT> >(loc));

    assert(std::has_facet<std::time_get<CharT> >(loc));
    assert(std::has_facet<std::time_put<CharT> >(loc));

    assert(std::has_facet<std::messages<CharT> >(loc));
}

void check(const std::locale& loc)
{
    check_for<char>(loc);
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    check_for<wchar_t>(loc);
#endif

    assert((std::has_facet<std::codecvt<char16_t, char, std::mbstate_t> >(loc)));
    assert((std::has_facet<std::codecvt<char32_t, char, std::mbstate_t> >(loc)));
#if TEST_STD_VER > 17
    assert((std::has_facet<std::codecvt<char16_t, char8_t, std::mbstate_t> >(loc)));
    assert((std::has_facet<std::codecvt<char32_t, char8_t, std::mbstate_t> >(loc)));
#endif
}

int main(int, char**)
{
    std::locale loc;
    assert(loc.name() == "C");
    check(loc);
    assert(std::locale::global(std::locale(LOCALE_en_US_UTF_8)) == loc);
    std::locale loc2;
    check(loc2);
    assert(loc2 == std::locale(LOCALE_en_US_UTF_8));

  return 0;
}
