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

#define TRUE sfTrue
#define FALSE sfFalse


#endif // WRAP_H_INCLUDED
