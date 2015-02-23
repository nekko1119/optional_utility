#ifndef OPTIONAL_UTILITY_HPP
#define OPTIONAL_UTILITY_HPP

#include <functional>
#include <utility>
#include <boost/optional.hpp>

namespace optional_utility
{
    template <typename T>
    inline typename boost::optional<T>::reference_const_type value(boost::optional<T> const& op)
    {
        return op.value();
    }

    template <typename T>
    inline typename boost::optional<T>::reference_type value(boost::optional<T>& op)
    {
        return op.value();
    }

    template <typename T>
    inline typename boost::optional<T>::reference_type_of_temporary_wrapper value(boost::optional<T>&& op)
    {
        return std::move(op.value());
    }

    template <typename T, typename U>
    inline typename boost::optional<T>::value_type value_or(boost::optional<T> const& op, U&& v)
    {
        return std::move(op.value_or(std::forward<U>(v)));
    }

    template <typename T, typename U>
    inline typename boost::optional<T>::value_type value_or(boost::optional<T>&& op, U&& v)
    {
        return std::move(op.value_or(std::forward<U>(v)));
    }

    template <typename T, typename F>
    inline typename boost::optional<T>::value_type value_or_eval(boost::optional<T> const& op, F f)
    {
        return std::move(op.value_or_eval(std::move(f)));
    }

    template <typename T, typename F>
    inline typename boost::optional<T>::value_type value_or_eval(boost::optional<T>&& op, F f)
    {
        return std::move(op.value_or_eval(std::move(f)));
    }

    template <typename Exception, typename T, typename ...Args>
    typename boost::optional<T>::reference_type value_or_throw(boost::optional<T>& op, Args&&... args)
    {
        if (!op) {
            throw Exception{std::forward<Args>(args)...};
        }
        return op.value();
    }

    template <typename Exception, typename T, typename ...Args>
    typename boost::optional<T>::reference_const_type value_or_throw(boost::optional<T> const& op, Args&&... args)
    {
        if (!op) {
            throw Exception{std::forward<Args>(args)...};
        }
        return op.value();
    }

    template <typename Exception, typename T, typename ...Args>
    typename boost::optional<T>::reference_type_of_temporary_wrapper value_or_throw(boost::optional<T>&& op, Args&&... args)
    {
        if (!op) {
            throw Exception{std::forward<Args>(args)...};
        }
        return std::move(op.value());
    }

    template <typename F>
    class flat_mapped_t
    {
        F f;
    public:
        explicit flat_mapped_t(F f)
            : f(std::forward<F>(f))
        {
        }

        template <typename T>
        auto operator()(boost::optional<T> const& op) const
            -> typename std::result_of<F (T)>::type
        {
            if (!op) {
                return boost::none;
            }
            return f(op.get());
        }
    };

    template <typename F>
    inline flat_mapped_t<F> flat_mapped(F f)
    {
        return flat_mapped_t<F>(std::forward<F>(f));
    }

    template <typename F>
    class mapped_t
    {
        F f;
    public:
        explicit mapped_t(F f)
            : f(std::forward<F>(f))
        {
        }

        template <typename T>
        auto operator()(boost::optional<T> const& op) const
            -> boost::optional<typename std::result_of<F (T)>::type>
        {
            if (!op) {
                return boost::none;
            }
            return boost::make_optional(f(op.get()));
        }
    };

    template <typename F>
    inline mapped_t<F> mapped(F f)
    {
        return mapped_t<F>(std::forward<F>(f));
    }

    template <typename T, typename F>
    inline auto operator|(boost::optional<T> const& op, F f) -> decltype(f(op))
    {
        return f(op);
    }

    template <typename F>
    inline boost::none_t operator|(boost::none_t const&, F)
    {
        return boost::none;
    }
}

#endif
