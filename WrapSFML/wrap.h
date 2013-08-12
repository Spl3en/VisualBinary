#ifndef WRAP_H_INCLUDED
#define WRAP_H_INCLUDED

// Héritage
#define SFML(x) (x->obj)

// Booléans
#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#ifdef true
#undef true
#endif

#ifdef false
#undef false
#endif

#define TRUE sfTrue
#define FALSE sfFalse
#define true TRUE
#define false FALSE

#ifndef bool
#define bool char
#endif

#endif // WRAP_H_INCLUDED
