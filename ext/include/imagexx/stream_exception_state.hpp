// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef STREAM_EXCEPTION_STATE_HPP_1253_10112008
#define STREAM_EXCEPTION_STATE_HPP_1253_10112008

#include <ios>

namespace imagexx
{
    //! This class is used to set the exceptions() state of an ios_base so that it throws whenever
    //! an error is encountered. The destructor restores the state to whatever it was at construction
    //! time
    class stream_exception_state
    {
        public:
            typedef std::basic_ios<char, std::char_traits<char> > stream_base;

            stream_exception_state(stream_base &b, std::ios_base::iostate flags) :
                stream_(b),
                state_(b.exceptions())
            {
                b.exceptions(flags);
            }

            ~stream_exception_state() { stream_.exceptions(state_); }

        private:
            //! Remains undefined; copying not allowed
            stream_exception_state(const stream_exception_state &);

            //! Remains undefined; copying not allowed
            stream_exception_state &operator= (const stream_exception_state &);

        private:
            stream_base &stream_;
            std::ios_base::iostate state_;
    };

} // close namespace imagexx

#endif // STREAM_EXCEPTION_STATE_HPP_1253_10112008

