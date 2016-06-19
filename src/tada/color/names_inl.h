#pragma once

#include <stdlib.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>

#include <tada/color/names.h>
#include <tada/base/enum.h>
#include <tada/base/throw.h>
#include <tada/base/math_inl.h>
#include <tada/color/toString.h>
#include <tada/color/names_table_inl.h>

namespace tada {
namespace detail {

inline Color toNormalColor(unsigned int hex) {
    static const auto BYTE = 256;
    auto b = hex % BYTE;

    hex /= BYTE;
    auto g = hex % BYTE;

    hex /= BYTE;
    auto r = hex % BYTE;

    return {r, g, b};
};

inline float strtof(const char *nptr, char const **endptr) {
    char* ep;
    auto r = ::strtof(nptr, &ep);
    *endptr = ep;
    return r;
}

template <Base>
struct BaseColor {
    // Temporary class while I dismantle this mess.
    class color_t;
};

template <>
struct BaseColor<Base::normal> {
    using color_t = Color;
};

template <>
struct BaseColor<Base::integer> {
    using color_t = Color255;
};

template <Base base>
using ColorType = typename BaseColor<base>::color_t;

template <typename Color>
Color colorFromCommaSeparated(char const* p) {
    auto originalP = p;
    auto getNumber = [&]() {
        auto x = strtof(p, &p);
        skipSpaces(p);
        return static_cast<float>(x);
    };

    auto skipComma = [&]() {
        THROW_IF_NE(*p++, ',', "Expected a comma", originalP);
        skipSpaces(p);
    };

    auto r = getNumber();
    skipComma();

    auto g = getNumber();
    skipComma();

    auto b = getNumber();
    THROW_IF(*p, "Extra characters after end", originalP);

    return {r, g, b};
}

template <Base BASE>
struct ColorTraits {
    static constexpr float denormalize(float x) {
        return (BASE == Base::normal) ? x / 255.0 : x;
    }
    static constexpr float normalize(float x) {
        return (BASE == Base::integer) ? x * 255.0 : x;
    }

    static Color colorFromHex(unsigned int hex) {
        auto c = toNormalColor(hex);
        return {denormalize(c[0]), denormalize(c[1]), denormalize(c[2])};
    };

    static bool isNearHex(float decimal) {
        auto denominator = (BASE == Base::normal) ? 255 : 1;
        return isNearFraction(decimal, denominator);
    }

    static uint32_t toHex(Color c) {
        if (BASE == Base::integer) {
            for (auto& i: c)
                *i /= 255;
        }
        return toHexNormal(c);
    }

    static std::string toString(Color c) {
        ColorType<BASE> c2(*c[0], *c[1], *c[2]);
        return tada::detail::toString(c2);
    }

    static bool toColorNonNegative(char const* name, Color& result) {
        if (not *name)
            return false;

        auto i = colorMap().find(name);
        if (i != colorMap().end()) {
            result = colorFromHex(i->second);
            return true;
        }

        uint hex;
        if (fromHexWithPrefix(name, hex)) {
            result = colorFromHex(hex);
            return true;
        }

        char* endptr;

        // Special case for grey and gray.
        if (strstr(name, "gray ") or strstr(name, "grey ")) {
            auto gray = static_cast<float>(strtod(name + 5, &endptr)) / 100;
            if (not *endptr) {
                gray = normalize(gray);
                result = {gray, gray, gray};
                return true;
            }
            return false;
        }

        try {
            result = colorFromCommaSeparated<Color>(name);
            return true;
        } catch (...) {
            return false;
        }
    }

    static bool toColor(char const* name, Color& result) {
        auto isSign = [](char ch) { return ch == '-' or ch == '+'; };
        auto len = strlen(name), end = len;
        for (; end > 0 && isSign(name[end - 1]); --end);

        if (end == len)
            return toColorNonNegative(name, result);

        if ((not end) or len - end != 3)
            return false;

        std::string n(name, len - 3);
        if (not toColorNonNegative(n.c_str(), result))
            return false;

        for (auto i = 0; i < 3; ++i) {
            if (name[len - 3 + i] == '-')
                result[i] = - result[i];
        }
        return true;
    }

    static Color toColor(char const* name) {
        Color result;
        THROW_IF(not toColor(name, result), "Bad color name", name);
        return result;
    }
};

} // detail

inline bool stringToColor(char const* s, Color& c, Base base) {
    using namespace tada::detail;
    return base == Base::normal ?
            ColorTraits<Base::normal>::toColor(s, c) :
            ColorTraits<Base::integer>::toColor(s, c);
}

inline bool stringToColor(char const* s, Color& c) {
    return detail::ColorTraits<Base::normal>::toColor(s, c);
}

inline Color stringToColor(char const* name, Base base) {
    using namespace tada::detail;
    return base == Base::normal ?
            ColorTraits<Base::normal>::toColor(name) :
            ColorTraits<Base::integer>::toColor(name);
}

inline std::string colorToString(Color c, Base base) {
    using namespace tada::detail;
    return base == Base::normal ?
            ColorTraits<Base::normal>::toString(c) :
            ColorTraits<Base::integer>::toString(c);
}

inline std::string colorToString(Color c) {
    return detail::ColorTraits<Base::normal>::toString(c);
}

inline std::string colorToString(float r, float g, float b, Base base) {
    return colorToString({r, g, b}, base);
}

inline bool stringToColor(char const* name, ColorS& cs, Base base) {
    Color c;
    if (not stringToColor(name, c, base))
        return false;
    cs = c;
    return true;
}

inline Color colorFromHex(uint32_t hex, Base base) {
    if (base == Base::normal)
        return detail::ColorTraits<Base::normal>::colorFromHex(hex);
    return detail::ColorTraits<Base::integer>::colorFromHex(hex);
}

inline uint32_t hexFromColor(Color const& c, Base base) {
    if (base == Base::normal)
        return detail::ColorTraits<Base::normal>::toHex(c);
    return detail::ColorTraits<Base::integer>::toHex(c);
}

}  // tada
