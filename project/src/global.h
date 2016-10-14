#ifndef GLOBAL_H
#define GLOBAL_H

#define GLOBAL(TYPE, NAME, ...)                                                                                                                                \
	namespace global {                                                                                                                                         \
		namespace detail {                                                                                                                                     \
			static TYPE &get##NAME() {                                                                                                                         \
				static TYPE NAME{__VA_ARGS__};                                                                                                                 \
				return NAME;                                                                                                                                   \
			}                                                                                                                                                  \
		}                                                                                                                                                      \
		static auto &&NAME = detail::get##NAME();                                                                                                              \
	}

#endif //GLOBAL_H
