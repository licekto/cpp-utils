#pragma once
#include <string>
#include <cassert>
#include <cstring>
#include <optional>
#include <iostream>
#include <sstream>
#include <boost/preprocessor.hpp>

#define __ENUMERATOR_TO_STRING(r, data, elem) \
    BOOST_PP_STRINGIZE(elem),

#define DEF_ENUM(__ENUM_CLASS_NAME, ...)                                                                \
class __ENUM_CLASS_NAME {                                                                               \
public:                                                                                                 \
    enum _enumerated { __VA_ARGS__ };                                                                   \
                                                                                                        \
    constexpr __ENUM_CLASS_NAME() noexcept : _value(static_cast<_enumerated>(0)) {}                     \
    constexpr __ENUM_CLASS_NAME(_enumerated val) noexcept : _value(val) {}                              \
    constexpr operator _enumerated() const noexcept { return _value; }                                  \
                                                                                                        \
    constexpr size_t size() const noexcept { return BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); }              \
    std::string toString() const {                                                                      \
        return toCString();                                                                             \
    }                                                                                                   \
    constexpr const char * toCString() const noexcept {                                                 \
        assert(_value < BOOST_PP_VARIADIC_SIZE(__VA_ARGS__));                                           \
        return _names[static_cast<size_t>(_value)];                                                     \
    }                                                                                                   \
    static std::optional<__ENUM_CLASS_NAME> fromString(const char * const enumStr) {                    \
        for (size_t i = 0; i < BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); ++i) {                              \
            if (!std::strcmp(enumStr, _names[i])) {                                                     \
                return static_cast<_enumerated>(i);                                                     \
            }                                                                                           \
        }                                                                                               \
        return std::nullopt;                                                                            \
    }                                                                                                   \
    static std::optional<__ENUM_CLASS_NAME> fromString(const std::string &enumStr) {                    \
        return fromString(enumStr.c_str());                                                             \
    }                                                                                                   \
    friend std::ostream &operator<<(std::ostream &os, const _enumerated e) {                            \
        return os << __ENUM_CLASS_NAME(e).toCString();                                                  \
    }                                                                                                   \
    friend std::istream &operator>>(std::istream &is, __ENUM_CLASS_NAME &e) {                           \
        std::string enumStr;                                                                            \
        is >> enumStr;                                                                                  \
        auto val = __ENUM_CLASS_NAME::fromString(enumStr.c_str());                                      \
        if (val.has_value()) {                                                                          \
            e = val.value();                                                                            \
        } else {                                                                                        \
            is.setstate(std::ios_base::failbit);                                                        \
        }                                                                                               \
        return is;                                                                                      \
    }                                                                                                   \
private:                                                                                                \
    _enumerated _value;                                                                                 \
    inline static constexpr const char * const _names[] = {                                             \
        BOOST_PP_SEQ_FOR_EACH(__ENUMERATOR_TO_STRING, _a, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) };     \
};
