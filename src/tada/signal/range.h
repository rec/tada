#pragma once

namespace tada {

template <typename T = float>
struct Normal {
    using Number = T;

    static constexpr auto begin = Number(0);
    static constexpr auto range = Number(1);
};

template <typename T = float>
struct EightBit {
    using Number = T;

    static constexpr auto begin = Number(0);
    static constexpr auto range = Number(255);
};

template <typename Range>
using NumberOf = typename Range::Number;

/** Unscale a ranged number to a range of [0, 1].  Numbers out of band get
    scaled proportionately. */
template <typename Range>
NumberOf<Range> unscale(NumberOf<Range> x) {
    return (x - Range::begin) / Range::range;
}

/** Scale a number with a range of [0, 1] to a ranged number.
    Numbers out of band get scaled proportionately. */
template <typename Range>
NumberOf<Range> scale(NumberOf<Range> y) {
    return Range::begin + y * Range::range;
}

/* TODO: above two functions only work for floating types! write/repurpose
   something for integral types. */

/** A Ranged number looks like an underlying numeric type, but with a generally
    attached Range.

    "Generic" means that there is no cost at run-time to carrying this
    information around - the downside is that we have to instantiate a new
    template for each range we want, but since the total number is very small,
    this is almost free. */
template <typename Range>
struct Ranged {
    using Number = NumberOf<Range>;

    Number number;

    Ranged() = default;
    Ranged(Ranged const&) = default;
    Ranged(Number n) : number(n) {}

    template <typename Range2>
    operator Ranged<Range2>() const {
        return scale<Range2>(unscale<Range>(number));
    }

    operator Number() const { return number; }
    operator Number&() { return number; }
};

}  // tada