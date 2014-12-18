#ifndef WEBMOCK_DIRECTIVE_DETAIL_MOCK_HPP
#define WEBMOCK_DIRECTIVE_DETAIL_MOCK_HPP

#include <webmock/directive/detail/registry.hpp>
#include <webmock/directive/condition.hpp>
#include <initializer_list>
#include <boost/optional.hpp>

namespace webmock { namespace directive { namespace detail {
    template <typename Derived>
    class mock_base {
    protected:
        using condition_type = core::condition_list::condition_type;
        
        core::condition_list conditions_;
        core::stub_registry & registry;
        boost::optional<std::size_t> mutable cached_count;
        
    public:
        mock_base(
            core::stub_registry & registry = detail::registry()
        ) :
            registry(registry)
        {}
        
        mock_base(
            with_url const & url,
            core::stub_registry & registry = detail::registry()
        ) :
            conditions_({url}), registry(registry)
        {}
        
        mock_base(
            with_method const & method,
            with_url const & url,
            core::stub_registry & registry = detail::registry()
        ) :
            conditions_({method, url}), registry(registry)
        {}
        
        std::size_t count() const {
            if (!this->cached_count) {
                this->cached_count = this->registry.count_requests(this->conditions_);
            }
            return *this->cached_count;
        }
        
        operator std::size_t() const {
            return this->count();
        }
        
        template <typename... Types>
        auto conditions(Types... conditions) {
            this->add_conditions({conditions...});
            return this->derived();
        }
        
        auto operator <<(condition_type rop) {
            return this->conditions(rop);
        }
        
    protected:
        Derived & derived() & {
            return *static_cast<Derived *>(this);
        }
        
        Derived && derived() && {
            return std::move(*static_cast<Derived *>(this));
        }
        
        virtual void add_conditions(std::initializer_list<condition_type> conditions) {
            for (auto && c: conditions) this->conditions_.add(c);
        }
    };
}}}

#endif