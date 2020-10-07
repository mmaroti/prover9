add_library(provers STATIC
    index_lits.c
    forward_subsume.c 
    demodulate.c
    pred_elim.c 
    unfold.c
    semantics.c
    giv_select.c 
    white_black.c 
    actions.c
    search.c
    utilities.c
    provers.c
    foffer.c
)
target_link_libraries(provers PUBLIC ladr m)

add_executable(prover9
    prover9.c
)
target_link_libraries(prover9 PUBLIC provers)

add_executable(fof-prover9
    fof-prover9.c
)
target_link_libraries(fof-prover9 PUBLIC provers)

add_executable(ladr_to_tptp
    ladr_to_tptp.c
)
target_link_libraries(ladr_to_tptp PUBLIC provers)

add_executable(tptp_to_ladr
    tptp_to_ladr.c
)
target_link_libraries(tptp_to_ladr PUBLIC provers)

add_executable(autosketches4
    autosketches4.c
)
target_link_libraries(autosketches4 PUBLIC provers)

add_executable(newauto
    newauto.c
)
target_link_libraries(newauto PUBLIC provers)

add_executable(newsax
    newsax.c
)
target_link_libraries(newsax PUBLIC provers)

add_executable(cgrep
    cgrep.c
)
target_link_libraries(cgrep PUBLIC provers)

add_executable(mprover
    mprover.c
)
target_link_libraries(mprover PUBLIC provers maces)

add_executable(iterate4
    iterate4.c
)
target_link_libraries(iterate4 PUBLIC provers)
