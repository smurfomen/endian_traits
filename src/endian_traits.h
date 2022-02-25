#ifndef QENDIANCONVERTER
#define QENDIANCONVERTER
#include <algorithm>
#include <stdint.h>

#define LITTLE_ENDIAN_TRAITS __ORDER_LITTLE_ENDIAN__
#define LITTLE_ENDIAN_TRAITS_DESC "Little Endian"

#define BIG_ENDIAN_TRAITS __ORDER_BIG_ENDIAN__
#define BIG_ENDIAN_TRAITS_DESC "Big Endian"

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define ENDIAN_TRAITS BIG_ENDIAN_TRAITS
#define ENDIAN_TRAITS_DESC BIG_ENDIAN_TRAITS_DESC
#elif __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
#define ENDIAN_TRAITS LITTLE_ENDIAN_TRAITS
#define ENDIAN_TRAITS_DESC LITTLE_ENDIAN_TRAITS_DESC
#else
#error "Undefined endian for platform"
#endif


template <typename T> inline T __endian_swap(const T src)
{
    static_assert(sizeof(src) > 1 && sizeof(src) % 2 == 0, "sizeof src is lower 2");

    char * p = (char*)&src;
    for(size_t i = 0; i < sizeof(src)/2; ++i) {
        std::swap(p[i], p[sizeof(src) - i - 1]);
    }

    return src;
}

#if ENDIAN_TRAITS == BIG_ENDIAN_TRAITS
template <typename T> inline T __to_be(const T src) {
    return src;
}

template <typename T> inline T __to_le(const T src) {
    return __ednian_swap(src);
}


template <typename T> inline T __from_be(const T src) {
    return src;
}

template <typename T> inline T __from_le(const T src) {
    return __ednian_swap(src);
}
#elif ENDIAN_TRAITS == LITTLE_ENDIAN_TRAITS
template <typename T> inline T __to_be(const T src) {
    return __endian_swap(src);
}

template <typename T> inline T __to_le(const T src) {
    return src;
}


template <typename T> inline T __from_be(const T src) {
    return __endian_swap(src);
}

template <typename T> inline T __from_le(const T src) {
    return src;
}
#endif


template<typename T>
struct __endian_traits {
    static constexpr bool valid = false;
};

template<> struct __endian_traits<uint16_t> {
    static constexpr bool valid = true;
};

template<> struct __endian_traits<uint32_t> {
    static constexpr bool valid = true;
};

template<> struct __endian_traits<uint64_t> {
    static constexpr bool valid = true;
};

template<> struct __endian_traits<int16_t> {
    static constexpr bool valid = true;
};

template<> struct __endian_traits<int32_t> {
    static constexpr bool valid = true;
};

template<> struct __endian_traits<int64_t> {
    static constexpr bool valid = true;
};



template<typename T> struct le;

template<typename T> struct be;

template<typename T, class = typename std::enable_if<__endian_traits<typename T::value_t>::valid, bool>::type>
struct endian {
    using value_t = typename T::value_t;
    endian(value_t v) noexcept
        : value(v)
    { }

    template<typename U>
    endian(endian<U> e)
        : value(e.value)
    {

    }

    explicit operator value_t() {
        return static_cast<value_t>(value);
    }

    value_t to_big_endian() const
    { return value.to_big_endian(); }

    value_t to_little_endian() const
    { return value.to_little_endian(); }

    template<typename U>
    endian & operator=(endian<U> rhs) {
        value = rhs.value;
        return *this;
    }

    T value;
};

template<typename T>
struct le {
    using value_t = T;

    le(T v)
        : value(v)
    { }

    le(be<T> o)
        : value(o.to_little_endian())
    { }

    explicit operator value_t() const {
        return __from_le<value_t>(value);
    }

    le & operator=(value_t rhs) {
        value = __to_le<value_t>(rhs);
        return *this;
    }

    T to_big_endian() const
    { return __endian_swap<value_t>(value); }

    T to_little_endian() const
    { return value; }

    le & operator=(be<T> rhs) {
        value = rhs.to_little_endian();
        return *this;
    }

    T value;
};

template<typename T>
struct be {
    using value_t = T;

    be(T v)
        : value(v)
    { }

    be(le<T> o)
        : value(o.to_big_endian())
    { }

    explicit operator value_t() const {
        return __from_le<value_t>(value);
    }

    be & operator=(value_t rhs) {
        value = __to_be<value_t>(rhs);
        return *this;
    }

    T to_big_endian() const
    { return value; }

    T to_little_endian() const
    { return __endian_swap<value_t>(value); }

    be & operator=(le<T> rhs) {
        value = rhs.to_big_endian();
        return *this;
    }

    T value;
};

using le64 = endian<le<uint64_t>>;
using le32 = endian<le<uint32_t>>;
using le16 = endian<le<uint16_t>>;

using be64 = endian<be<uint64_t>>;
using be32 = endian<be<uint32_t>>;
using be16 = endian<be<uint16_t>>;
#endif
