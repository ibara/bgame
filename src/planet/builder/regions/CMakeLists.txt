add_library(regions STATIC
        starting_point.cpp
        heightmap.cpp
        water_features.cpp
        strata.cpp
        decorators.cpp
        trees.cpp
        buildings.cpp
        blight_builder.cpp
        )
target_link_libraries(regions rltk raws FastNoise region)
cotire(regions)
