set(module tree_individualization)
set(private_dependencies 3rd_lastools)
set(public_dependencies )

set(${module}_headers
        nanoflann.hpp
        FoxTree.h
        #resource.hpp
        )

set(${module}_sources
        FoxTree.cpp
        )

add_module(${module} "${${module}_headers}" "${${module}_sources}" "${private_dependencies}" "${public_dependencies}")
install_module(${module})