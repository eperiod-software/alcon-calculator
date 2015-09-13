/*
 * util.hpp
 *
 *  Created on: Apr 22, 2013
 *      Author: Vaughn Friesen
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#define ALWAYS_DEBUG

#ifndef QT_NO_DEBUG
#define DEBUG
#endif

#ifdef ALWAYS_DEBUG
#define DEBUG
#endif

#ifdef DEBUG
#include <cstdio>
#endif

#ifndef DEBUG_CLASS
#define DEBUG_CLASS "Unknown"
#endif

#ifdef DEBUG

inline void debugPrint(const char *debugClass, const char *x) {
	std::fprintf(stderr, "%s: %s\n", debugClass, x);
}

template <typename Arg0, typename Arg1>
inline void debugPrint(const char *debugClass, const char *x, Arg0 const &arg0) {
	std::fprintf(stderr, "%s: ", debugClass);
	std::fprintf(stderr, x, arg0);
	std::fprintf(stderr, "\n");
}

template <typename Arg0, typename Arg1>
inline void debugPrint(const char *debugClass, const char *x, Arg0 const &arg0, Arg1 const &arg1) {
	std::fprintf(stderr, "%s: ", debugClass);
	std::fprintf(stderr, x, arg0, arg1);
	std::fprintf(stderr, "\n");
}

#else

inline void debugPrint(const char *debugClass, const char *x) {
	(void)x;
}

template <typename Arg0, typename Arg1>
inline void debugPrint(const char *debugClass, const char *x, Arg0 const &arg0) {
	(void)x;
	(void)arg0;
}

template <typename Arg0, typename Arg1>
inline void debugPrint(const char *debugClass, const char *x, Arg0 const &arg0, Arg1 const &arg1) {
	(void)x;
	(void)arg0;
	(void)arg1;
}

#endif

#define DEBUG_PRINT(x) debugPrint(DEBUG_CLASS, x)
#define DEBUG_PRINT1(x, y) debugPrint(DEBUG_CLASS, x, y)
#define DEBUG_PRINT2(x, y, z) debugPrint(DEBUG_CLASS, x, y, z)

#endif /* DEBUG_HPP_ */
