-- Woodwork facilities

------------------------------------------------------------------------------------------------------------------------
-- Carpentry workshops are where woodwork is performed.
------------------------------------------------------------------------------------------------------------------------
buildings["carpenter"] = {
    name = "Carpentry Workshop",
    components = { { item="block", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['carpenter_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};

reactions["make_wooden_table"] = {
    name = "Make Wooden Table",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="table", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_wooden_chair"] = {
    name = "Make Wooden Chair",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="chair", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_wooden_door"] = {
    name = "Make Wooden Door",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="door", qty=1 } },
    skill = "Carpentry",
    difficulty = 12,
    automatic = false
};

reactions["make_simple_bed"] = {
    name = "Make Simple Bed",
    workshop = "carpenter",
    inputs = { { item="block", material="wood", qty=1} },
    outputs = { { item="bed_item", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_cage"] = {
    name = "Make Cage",
    workshop = "carpenter",
    inputs = { { item="block", qty=1} },
    outputs = { { item="cage", qty=1 } },
    skill = "Mechanics",
    difficulty = 10,
    automatic = false
};

reactions["make_wooden_club"] = {
    name = "Make Wooden Club",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="club", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_great_club"] = {
    name = "Make HUGE Wooden Club",
    workshop = "carpenter",
    inputs = { { item="block", qty=2, material="wood" } },
    outputs = { { item="greatclub", qty=1 } },
    skill = "Carpentry",
    difficulty = 12,
    automatic = false
};

reactions["make_spiked_club"] = {
    name = "Make Board with Nail",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" }, { item="block", qty=1, material="metal" } },
    outputs = { { item="club", qty=1 } },
    skill = "Carpentry",
    difficulty = 10,
    automatic = false
};

reactions["make_pointy_stick"] = {
    name = "Make Wooden Pointy Stick",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="pointy_stick", qty=1 } },
    skill = "Carpentry",
    difficulty = 7,
    automatic = false
};

reactions["make_quarterstaff"] = {
    name = "Make Wooden Pointy Stick",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="quarterstaff", qty=1 } },
    skill = "Carpentry",
    difficulty = 8,
    automatic = false
};

reactions["make_atlatl"] = {
    name = "Make Atlatl",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="atlatl", qty=1 } },
    skill = "Carpentry",
    difficulty = 15,
    automatic = false
};

reactions["make_blowgun"] = {
    name = "Make Blowgun",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="blowgun", qty=1 } },
    skill = "Carpentry",
    difficulty = 15,
    automatic = false
};

reactions["make_wood_dart"] = {
    name = "Make Wooden Dart",
    workshop = "carpenter",
    inputs = { { item="block", qty=1, material="wood" } },
    outputs = { { item="dart", qty=1 } },
    skill = "Carpentry",
    difficulty = 12,
    automatic = false
};

------------------------------------------------------------------------------------------------------------------------
-- Intermediate Carpentry workshops are where woodwork is performed, requiring a lathe or other advancement.
------------------------------------------------------------------------------------------------------------------------
buildings["intermediate_carpenter"] = {
    name = "Intermediate Carpentry Workshop",
    components = { { item="block", qty=1 }, {item="simple_lathe", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    render = {
        width=2, height=2, tiles= {
            {glyph= glyphs['carpenter_1'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_2'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_3'], foreground = colors['white'], background = colors['black']},
            {glyph= glyphs['carpenter_4'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=2, height=2, tiles= {
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['cabinet'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['table'], foreground = colors['wood_brown'], background = colors['black']},
            {glyph= glyphs['paragraph'], foreground = colors['wood_brown'], background = colors['black']}
        }
    },
};
