Nov 11, 2004.

Flag ac_redundancy (default set).

This is related to AC unification, but it does not use the
AC unification code.

If this flag is set, usable and sos are scanned for AC axioms.
For example, say there are A and C axioms for ^ and v.

Then, when a generated clause is processed, if it contains
an equality atom alpha=beta, in which alpha is AC-equal to beta,
it is reduced to TRUE.

For example, if x ^ (y ^ c) = c ^ (x ^ y) will be treated as
if it is an instance of x=x, and it will be deleted.

Annoying clauses like the following will not be kept.

  x ^ (y ^ (z ^ u)) = z ^ (y ^ (x ^ u))

Otter does something similar if you set lex_order_vars and
have the appropriate lex-dep demoulators.  In that example,
both sides will be demodulated to the same term, so it will
be deleted.

However, LADR's demoulation does not allow lex_order_vars
(because it doesn't obey LRPO's rules).

The main difference between LADR's ac_redundancy and Otter's
lex_order_vars is that LADR does not actually rewrite AC
expressions with A and C (unless LRPO says it can).  For example,
LADR can keep both of the following

   x ^ (y ^ x) = x ^ y
   x ^ (x ^ y) = x ^ y

and Otter with lex_order_vars will canonicalize and keep only one.
The tradeoff is simple: Otter's is more restrictive, but it can
block all proofs.

----------------------------------------------------------------------

Each function (including constant) symbol has a positive weight.

  exception: there can be a unary function with weight 0 (think group inverse)

There is a symbol precedence, independent of the symbol weight.

  exception: if there is a unary of weight 0, it is greatest in the precedence.

The exceptions are so that equations like g(f(x,y)) = f(g(y),g(x)) can
be demodulators (in that orientation).

The weight of a term is simply the sym of the weights of the function and
constant symbols.

Is alpha > beta in KBO?  See kbo() in termorder.c.

----------------------------------------------------------------------


ORIENTING EQUALITIES

orient_equalities() (parautil.c) possibly flips equalities and maybe sets
a flag in each equality atom:

  mark_oriented_eq(atom);     /* if the "METHOD" says left side is greater */
  mark_renamable_flip(atom);  /* e.g., commutativity, x(yz)=y(xz) */


The methods are (see symbols.h, assign_order_method in termorder.c):

typedef enum { LPO_METHOD,
	       RPO_METHOD,
	       KBO_METHOD
             } Order_method;

----------------------------------------------------------------------

DECIDING DEDMOULATORS

demod.h:

 enum { NOT_DEMODULATOR, ORIENTED, LEX_DEP_LR, LEX_DEP_RL, LEX_DEP_BOTH };

demod.c:

 int demodulator_type(Clause c, int lex_dep_demod_lim) returns one of preceding

If marked as "oriented", it becomes an ORIENTED demodulator; otherwise,
various checks are done.

DEMODULATION

When a potential demodulator is found, if it is marked as "oriented",
it is used (always left-to-right).  Otherwise the instance is checked
by the "METHOD".

PARAMODULATION

Oriented equalities:       are used left-to-right only (from).
Non-oriented equalities:
   if flag(ordered_instance), then don't use if instance is backward
                              else used both ways (except for renamable flips)
  