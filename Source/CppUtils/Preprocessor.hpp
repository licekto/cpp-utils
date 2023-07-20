// Source: https://www.scs.stanford.edu/~dm/blog/va-opt.html#the-for_each-macro

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN PARENS(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define VA_SIZE(...) 0 __VA_OPT__(EXPAND(VA_SIZE_HELPER(__VA_ARGS__)))

#define VA_SIZE_HELPER(a1, ...) + 1 __VA_OPT__(VA_SIZE_AGAIN PARENS(__VA_ARGS__))
#define VA_SIZE_AGAIN() VA_SIZE_HELPER

#define STRINGIFY(x) #x
#define COMMA_SEP(x) STRINGIFY(x),

#define TO_STRINGS(...) FOR_EACH(COMMA_SEP, __VA_ARGS__)
