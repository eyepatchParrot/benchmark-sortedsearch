#ifndef Span_hh_INCLUDED
#define Span_hh_INCLUDED

#include "hdr.hh"

namespace pvc {
template<class T> struct Span {
    T* p_;
    uz n_;

    T* begin() const { return p_; }
    T* end() const { return p_ + n_; }
    uz size() const { return n_; }
    T& operator[](uz i) const { return p_[i]; }
};
}

#endif // Span_hh_INCLUDED

