/*  Copyright (C) 2006, 2007 William McCune

    This file is part of the LADR Deduction Library.

    The LADR Deduction Library is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License,
    version 2.

    The LADR Deduction Library is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the LADR Deduction Library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#define PROVER_NAME     "Iterate"
#define PROVER_VERSION  "4"
#include "../VERSION_DATE.h"

#include "../ladr/nonport.h"
#include "../ladr/top_input.h"
#include "utilities.h"
#include "search.h"
#include "provers.h"

/*************
 *
 *    main -- iterate max_weight
 *
 *************/

int main(int argc, char **argv)
{
  Prover_input input;
  Prover_results results;
  BOOL go;
  int max_wt_flag;

  print_banner(argc, argv, PROVER_NAME, PROGRAM_VERSION, PROGRAM_DATE, FALSE);

  /***************** Initialize and read the input ***************************/

  input = std_prover_init_and_input(argc, argv,
			   TRUE,           // clausify
			   TRUE,           // echo input to stdout
			   KILL_UNKNOWN);  // unknown flags/parms are fatal

  /******************** Search for a proof *******************************/

  // This does a sequence of searches with increasing max_weight.
  // If no initial max_weight is in the input, it is set to 2.
  // While the sos goes empty, increment max_weight (up to 100),
  // and search again.  

  max_wt_flag = input->options->max_weight;

  if (parm(max_wt_flag) == INT_MAX) {
    fprintf(stdout, "\nSetting initial max_weight to 25.\n");
    fprintf(stderr, "\nSetting initial max_weight to 25.\n");
    assign_parm(max_wt_flag, 25, TRUE);
  }

  go = TRUE;
  while (go) {
    fprintf(stdout,"\nNew environment, max_weight=%d.\n",
	    parm(max_wt_flag));
    fprintf(stderr,"\nNew environment, max_weight=%d.\n",
	    parm(max_wt_flag));

    // The search() function does not alter the Plists of objects that
    // it receives.  It returns an prover_results structure which contains
    // the results of the search (stats, clocks, proofs).
    // (In this example, we don't look at it.)

    results = forking_search(input);

    go = (results->return_code == SOS_EMPTY_EXIT &&
	  parm(max_wt_flag) < 100);

    if (go)
      assign_parm(max_wt_flag, parm(max_wt_flag) + 25, TRUE);

  }  // end of while loop
      
  exit_with_message(stdout, results->return_code);

  exit(1);  // to satisfy the compiler (won't be called)

}  // main
