add_library(maces
    estack.c
    util.c
    print.c
    syms.c
    ground.c
    arithmetic.c
    select.c
	propagate.c
    mstate.c
    negpropindex.c
    negprop.c
    ordercells.c
	commandline.c
    msearch.c
)
target_link_libraries(maces PUBLIC ladr)

add_executable(mace4
    mace4.c
)
target_link_libraries(mace4 PUBLIC maces)
