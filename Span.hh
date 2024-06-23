#ifndef Span_hh_INCLUDED
#define Span_hh_INCLUDED

#include "hdr.hh"

namespace pvc {
template<class T> struct Span {
    T* p_;
    uz n_;

    T* begin() const { return p_; }
    T* end() const { return p_ + n_; }
};
}

#endif // Span_hh_INCLUDED

