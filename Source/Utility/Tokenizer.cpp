#include "Tokenizer.hpp"

#include <stdexcept>

namespace CppUtils
{
Tokenizer::ConstIterator Tokenizer::ConstIterator::create_end(const Tokenizer *tok)
{
    ConstIterator it(tok);
    it.current = std::string_view::npos;
    return it;
}

Tokenizer::ConstIterator::ConstIterator(const Tokenizer *tok) noexcept
    : tokenizer(tok)
    , current(tokenizer->string.find_first_not_of(tokenizer->delims, 0))
    , next(tokenizer->string.find_first_of(tokenizer->delims, current))
{}

Tokenizer::ConstIterator& Tokenizer::ConstIterator::operator++()
{
    if (current == std::string_view::npos)
    {
        throw std::out_of_range("Can't increment past end of the string '" + std::string(tokenizer->string) + "'");
    }
    current = next == std::string_view::npos ? next : tokenizer->string.find_first_not_of(tokenizer->delims, next + 1);
    next = tokenizer->string.find_first_of(tokenizer->delims, current);
    return *this;
}

Tokenizer::ConstIterator Tokenizer::ConstIterator::operator++(int)
{
    const auto tmp = *this;
    ++*this;
    return tmp;
}

bool Tokenizer::ConstIterator::operator==(const ConstIterator &other) const noexcept
{
    return tokenizer == other.tokenizer && current == other.current;
}

bool Tokenizer::ConstIterator::operator!=(const ConstIterator &other) const noexcept
{
    return !(*this == other);
}

Tokenizer::ConstIterator::reference Tokenizer::ConstIterator::operator*() const
{
    if (current >= tokenizer->string.size())
    {
        throw std::out_of_range("Can't access a token of the string '" + std::string(tokenizer->string) + "' at index=" + std::to_string(current));
    }
    const auto substr_length = next == std::string_view::npos ? tokenizer->string.size() - current : next - current;
    return tokenizer->string.substr(current, substr_length);
}

Tokenizer::ConstIterator::pointer Tokenizer::ConstIterator::operator->() const
{
    tmpStorage = this->operator*();
    return &tmpStorage;
}

Tokenizer::Tokenizer(const ValueType str, const ValueType delims) noexcept
    : string(str), delims(delims)
{}

Tokenizer::ConstIterator Tokenizer::begin() const noexcept
{
    return ConstIterator(this);
}

Tokenizer::ConstIterator Tokenizer::end() const noexcept
{
    return ConstIterator::create_end(this);
}

Tokenizer::ConstIterator Tokenizer::cbegin() const noexcept
{
    return begin();
}

Tokenizer::ConstIterator Tokenizer::cend() const noexcept
{
    return end();
}
}
