#pragma once


namespace detail
{
enum class ScopeGuardOnExit {};


template <class Fun>
struct ScopeGuard {
    ScopeGuard(Fun&& fn) : fn(std::forward<Fun>(fn)) {}
    ~ScopeGuard() { fn(); }
    Fun fn;
};



template <typename Fun>
ScopeGuard<Fun> operator+(ScopeGuardOnExit, Fun&& fn) {
    return ScopeGuard<Fun>(std::forward<Fun>(fn));
}



} // namespace detail

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) \
    CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) \
    CONCATENATE(str, __LINE__)

#endif // __COUNTER__




#define ANONYMOUS_VARIABLE(str) \
    JOIN(str, __COUNTER__)

#define SCOPE_EXIT \
    auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) \
    = ::detail::ScopeGuardOnExit() + [&]()
