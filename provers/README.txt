When adding new flags or parms, decide if they can be changed by
actions during the search, and if so, whether the sos index has
to be reconstructed when they are.  If they can be changed, update
the tables at the beginning of actions.c.

------------------------------------------------------------------------------

When adding new flags or parms, decide if they can be changed by
actions during the search, and if so, whether the sos index has
to be reconstructed when they are.  If they can be changed, update
the tables at the beginning of actions.c.

------------------------------------------------------------------------------

MANAGING SOS_LIMIT

A. Deciding whether to keep a clause (C)

   Find its would-be position in each matching property list;
   If it is in the better half in one of the lists, keep it.

B. Deleting the worst sos clause.

Calculate priority(Clause C) -- lower is better

   pi: selection properties that apply to C,
       also, the corresponding list, ordered by weight.
   ri: corresponding ratios.

   position(C, pi): in the range 1--size(pi)

   L = LCM(ri)

   priority(C) = min_i(L / ri * position(C, pi))

To calculate the worst clause:

   Consider all nonempty property lists, pi;
   Worst_clause = max_i(last_clause(pi))

------------------------------------------------------------------------------

To expand on our discussion of selection-by-parts ...

What about this?

The selection rules:

    list(given_selection).
    <ordered_list_of_rules>
    end_of_list.

Each rule looks like one of these

    part(<order>, <boolean_expression>) = loop.
    part(<order>, <boolean_expression>) = <positive_integer>.

The <order> is "age", "weight", or "random".

The <boolean_expression> is made up of clause properties, e.g.,

 (signs of literals)    positive, negative, nonpositive, nonnegative, mixed
 (matches a hint)       hint
 (semantics)            true_in_all, true_in_some, false_in_all, false_in_some,
                        not_evaluated (e.g., too many variables)
 (other properties)     has_equality, horn, definite, unit
 (for positive integer n)
                        weight = n, weight < n, weight > n,
                        vars = n, vars < n, vars > n,
                        depth = n, depth < n, depth > n,
                        literals = n, literals < n, literals > n,
                        level = n, level < n, level > n

For example,

  part(weight, horn & -hint & vars = 3 & weight > 10) = 6.

Disjunction and negation are allowed.

If <boolean_expression> is $T (true), the rules can be abbreviated:

    part(<order>) = loop.
    part(<order>) = <positive_integer>.

Examples for rules we already use

    part(age, hint) = loop.   % loop rules always come first
    part(weight) = 3.
    part(weight, false_in_all) = 2.
    part(random) = 1.
    part(age) = 1.

The "loop" is used *as soon as any* satisfying the property are available,
and when the loop is done, it picks up where it left off in the ordinary
(nonloop) rules.

There can be more than one loop rule, and they are applied in order,
that is, when selecting a given clause,

  if (some clause satisfies the first loop rule)
    use it;
  else if (some clause satisfies the second loop rule)
    use it;
  etc., for all of the loop rules;
  else
    resume in the ordinary cycle.
  
For example

   part(weight, hint & positive) = loop.
   part(age, hint) = loop.
   part(age) = 1.
   part(weight) = 4.

picks positive hint-matchers by weight, when available,
then picks other hint-matchers by age, when available,
then cycles through age and weight.

-------------------------------------

This raises the question of whether we should use the same
kind of <boolean_expression> for keeping generated clauses, e.g.,

  list(rules_for_keeping_generated_clauses).

  keep(weight < 3 & depth < 10).
  discard(weight = 8 & -horn & has_equality).

  end_of_list.

Disjunctions give us something new:

  keep(weight < 50 | vars < 3).

  Bill

------------------------------------------------------------------------------

Process inferred clause (c):

	check time limit
	report

	simplify:
		print generated
		renumber?
		demodulate
		orient equalities
		simplify literals (x=x, $T)
		merge literals
		unit deletion
		cac redundancy

	empty clause check
	tautology check
	max_literals
	max_vars
	safe unit conflict
	weight check (max_weight, semantic eval, sos_keep)
	subsume
	KEEP:
		kept actions
		renumber?
		assign ID
		mark parents used
		mark maximal literals
		print
	
	unsafe unit conflict
	new demod
	index
	add to limbo

Limbo process:

	factor
	new_constants
	fold demial (if input or copy just)
	back subsumption
	back demodulation
	back unit deletion
	move to sos (maybe displacing another)
	index for back demod
	
------------------------------------------------------------------------------

Limiting the size of sos.

Assume a fixed limit, e.g., assign(sos_limit, 10000).

When sos gets half full, start being more selective, based on clause
weight, about what to keep.

(Max_weight is used in the normal way, and this is a secondary
constraint.)

The official sos is ordered by ID.  Consider an alternate order: first by
weight, then by ID.  We'll be looking at weights of clauses
along in this alternate order.  For example the weight of the clause
at 50% ... up to 100%.

When the sos is P% full (P > 50), keep a clause only if its weight is
less than the weight of the clause at (150 - P)%.

For example, if 55% full, keep only if weight is less than the
weight of the clause at 95%.  (This should keep most clauses.)

For example, if 100% full, keep only if weight is less than the
weight of the clause at 50%.  (This should keep a few clauses.)

---------------------------------------------------------------

If sos is full and the above scheme says to keep another clause,
we have to delete the "worst" clause from sos:

Ratio == -1 (best-first): delete last heaviest clause;
Ratio == 0 (breadth-first): delete last clause;
else (some ratio): delete the last heaviest clause in the second half of sos.

---------------------------------------------------------------

Dynamic sos_limit e.g., assign(lrs_seconds, 600).

This works with the above schemes.  If we lower the limit,
simply delete "worst" clauses from sos (as described above).
If we raise the limit, all is well (although we may have
deleted some clasues that we would keep were they generated later.)

------------------------------------------------------------------------------

Where to put the default symbol ordering?

    symbol_order(usable, sos, demodulators, goals, defns);

------------------------------------------------------------------------------

flag breadth_first_hints goes away?

max_weight, etc. changed by action: must update black_rules.

weight_part, etc. changed: must update giv_select.

max_weight, etc. ignored when list(keep).

------------------------------------------------------------------------------

1.  Parameter demod_limit (default 1000) limits the number
of rewrite steps for each generated clause.  There is
another demodulation limit (wired in), which limits
the size of terms as they are demodulated.  Terms
being demodulated are allow to increase in size by
demod_limit/2 symbols.  See fdemodulate() in flatdemod.c

2.  When deciding whether to keep a clause:

  else if (c->weight > 50 &&
	   clist_number_of_weight(Glob.limbo, c->weight) >= 3)
	discard;

This means that if the weight is > 50 and there are already 3
clauses in the Limbo list of that weight, don't keep the clause.
This prevents cases where the given clause produces lots
of very similar clauses, which can decrease the effectiveness
of the "age" part of the given clause selection ratio.  A safer
and more general way to address the problem would be to keep
those clauses and have the given clause selection function take
that into account.
