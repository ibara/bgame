add_library(work_types STATIC
        equip_melee_work.cpp
        equip_ranged_work.cpp
        equip_armor_work.cpp
        demolition_work.cpp
        hauling_work.cpp
        )
target_link_libraries(work_types rltk utils components raws tasks)
cotire(work_types)
