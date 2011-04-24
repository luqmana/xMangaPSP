// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file BOOST_LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SINK_HPP_2333_03042008
#define SINK_HPP_2333_03042008

#include <algorithm>
#include <cstddef>

namespace imagexx
{
    //! This is a class used to write to a custom location. iterator_sink<> derives from this to implement the interface
    //! for a templated iterator type.
    class sink
    {
        public:
            //! Destroys the object mercilessly
            virtual ~sink();

            //! Writes the num_bytes bytes in buff to a custom location. Derived classes can change the behaviour of this function by
            //! overriding write_impl()
            void write(const unsigned char *buff, std::size_t num_bytes);

        protected:
            sink();

        private:
            //! Derived classes should override this function to write the num_bytes bytes pointed to by buff to a custom location.
            //! If an error needs to be reported, an exception may be thrown.
            virtual void write_impl(const unsigned char *buff, std::size_t num_bytes) = 0;

            //! Remains undefined; copying is not allowed
            sink(const sink &);

            //! Remains undefined; copying is not allowed
            sink &operator= (const sink &);
    };

    //! Implements the pngxx::sink interface to write data to an output iterator when the write() member function is called.
    template<typename OutputIterator>
    class iterator_sink : public sink
    {
        public:
            //! When write is called, data will be written to out, which is held by reference. So after write() is called, you
            //! should find that the initial iterator passed to this constructor has been modified.
            iterator_sink(OutputIterator &out) :
                out_(out)
            {
            }

        private:
            //! Writes the data to the iterator given at construction time
            void write_impl(const unsigned char *buff, std::size_t num_bytes)
            {
                out_ = std::copy(buff, buff + num_bytes, out_);
            }

        private:
            OutputIterator &out_;
    };

} // close namespace imagexx

#endif // WRITE_HPP_2333_03042008

