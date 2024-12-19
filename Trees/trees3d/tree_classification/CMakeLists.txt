set(module tree_classification)
set(private_dependencies 3rd_lastools)
set(public_dependencies )

set(${module}_headers
        BasicStructures.h
        nanoflann.hpp
        TreeClassifier.h
        #resource.hpp
        )

set(${module}_sources
        TreeClassifier.cpp
        )

add_module(${module} "${${module}_headers}" "${${module}_sources}" "${private_dependencies}" "${public_dependencies}")
install_module(${module})