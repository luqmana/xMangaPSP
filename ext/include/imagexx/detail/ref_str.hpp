// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef REF_STR_HPP_1213_12112008
#define REF_STR_HPP_1213_12112008

#include <cstring>
#include <string>
#include <algorithm>

namespace imagexx
{
    namespace detail
    {
#ifndef __BORLANDC__
        // Borland 5.5.1: '__strcpy__' is not a member of 'std'
        using std::strcpy;
#endif

        //! A reference-counted C-string. No-throw guarantee once constructed. Copies don't throw.
        //! For use in exception classes where it's important that copying doesn't throw.
        class ref_str
        {
            private:
                struct data
                {
                    data(const char *str) : n(1), s(new char[std::strlen(str) + 1]) { strcpy(s, str); }
                    ~data() { delete[] s; }
                    void inc() { ++n; }
                    void dec() { if (--n == 0) delete this; }
                    std::size_t n;
                    char *s;
                };

            public:
                explicit ref_str(const char *str) : d_(new data(str)) { }
                explicit ref_str(const std::string &str) : d_(new data(str.c_str())) { }
                ~ref_str() { d_->dec(); }

                ref_str(const ref_str &other) : d_(other.d_) { d_->inc(); }

                ref_str &operator= (const ref_str &other)
                {
                    ref_str temp(other);
                    std::swap(d_, temp.d_);
                    return *this;
                }

                const char *c_str() const { return d_->s; }

            private:
                data *d_;
        };
    }
}

#endif // REF_STR_HPP_1213_12112008
