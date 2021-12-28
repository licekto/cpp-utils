#pragma once

#include <string_view>

namespace CppUtils
{
class Tokenizer
{
    using ValueType = std::string_view;

    class ConstIterator
    {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = ValueType;
        using difference_type = size_t;
        using pointer = const ValueType *;
        using reference = const ValueType;

        static ConstIterator create_end(const Tokenizer *_tokenizer);
        explicit ConstIterator(const Tokenizer *_tokenizer) noexcept;

        ConstIterator(const ConstIterator&) = default;
        ConstIterator& operator=(const ConstIterator&) = default;
        ConstIterator(ConstIterator&&) = default;
        ConstIterator& operator=(ConstIterator&&) = default;

        ConstIterator &operator++();
        ConstIterator operator++(int);
        [[nodiscard]] bool operator==(const ConstIterator& other) const noexcept;
        [[nodiscard]] bool operator!=(const ConstIterator& other) const noexcept;
        [[nodiscard]] reference operator*() const;
        [[nodiscard]] pointer operator->() const;

    private:
        const Tokenizer *tokenizer;
        size_t current, next;
        mutable std::string_view tmpStorage{}; // A little hack to allow support for operator->.
    };

public:
    Tokenizer(const ValueType str, const ValueType delims) noexcept;

    Tokenizer(std::string&& str, const ValueType delims) = delete;
    Tokenizer(const Tokenizer&) = default;
    Tokenizer& operator=(const Tokenizer&) = default;
    Tokenizer(Tokenizer&&) = default;
    Tokenizer& operator=(Tokenizer&&) = default;

    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

private:
    ValueType string, delims;
};
}
