biome_types = {
    ocean=1,
    plains=2,
    hills=3,
    mountains=4,
    marsh=5,
    plateau=6,
    highlands=7,
    coast=8,
    salt_marsh=9,
    coast=10
}

biomes = {
    -- Ocean areas
    frozen_ocean = {
        name = "Frozen Ocean", min_temp = -100, max_temp = -5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['white'] },
        plants = { none=20, grass=20, lavendar=1, daisy=1, reeds=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer"}, nouns = { "Arctic", "Pole", "Frozen Sea" }
    },
    icy_ocean = {
        name = "Icy Ocean", min_temp = -10, max_temp = 10, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"], soils = { soil=50, sand=50 } },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['cyan'] },
        plants = { none=10, grass=20, lavendar=1, daisy=1, reeds=2, potato=1, radish=1, turnip=1 },
        trees = { deciduous = 0, evergreen = 5 },
        wildlife = { "deer"}, nouns = { "Arctic", "Ice Sea", "Sea of Razors", "Iceberg" }
    },
    ocean = {
        name = "Ocean", min_temp = 0, max_temp = 30, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"], soils = { soil=50, sand=50 } },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['blue'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, strawberry=1, cabbage=1, caper=1, celery=1, leek=1, lentil=1, onion=1, parsnip=1, potato=1, radish=1, red_bean=1, soybean=1, spinach=1, turnip=1, hemp=1, cotton=1 },
        trees = { deciduous = 5, evergreen = 5 },
        wildlife = { "deer","boar"}, nouns = { "Sea", "Ocean", "Drink", "High Sea" }
    },
    hot_ocean = {
        name = "Tropical Ocean", min_temp = 30, max_temp = 100, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["ocean"], biome_types["coast"], soils = { soil=50, sand=50 } },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['blue'] },
        plants = { none=5, grass=20, reeds_giant=2, reeds_paper=2, sage=1, daisy=1, reeds=2, strawberry=1, goya=1, cabbage=1, caper=1, cassava=1, celery=1, horned_melon=1, lentil=1, onion=1, parsnip=1, soybean=1, yam=1, jute=1, hemp=1, ramie=1, cotton=1 },
        trees = { deciduous = 10, evergreen = 0 },
        wildlife = { "deer","armadillo","antelope"}, nouns = { "Tropical Sea", "Equatorial Sea", "Warm Ocean" }
    },

    -- Coastal Zones
    arctic_coastal = {
        name = "Arctic Coast", min_temp = -100, max_temp = -5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] },
        plants = { none=20, grass=20, lavendar=1, daisy=1, reeds=2, heather=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer"}, nouns = { "Ice Cliffs", "Ice Shelf", "Glacier", "Needles" }
    },
    cold_coastal = {
        name = "Cold Coast", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['cyan'] },
        plants = { none=10, grass=20, lavendar=1, daisy=1, reeds=2, heather=2, cabbage=1, cress=1, garlic=1, leek=1, onion=1, parsnip=1, potato=1, radish=1, turnip=1, hemp=1 },
        trees = { deciduous = 0, evergreen = 5 },
        wildlife = { "deer","boar" }, nouns = { "Coast", "Seashore", "Littoral" }
    },
    temperate_coastal = {
        name = "Temperate Coast", min_temp = 5, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, strawberry=1, beetroot=1, cabbage=1, cress=1, leek=1, lentil=1, onion=1, parsnip=1, pea=1, potato=1, radish=1, red_bean=1, rhubarb=1, soybean=1, spinach=1, turnip=1, hemp=1, cotton=1 },
        trees = { deciduous = 5, evergreen = 5 },
        wildlife = { "deer","badger","boar","antelope","horse"}, nouns = { "Coast", "Cliffs", "Seashore", "Littoral" }
    },
    tropical_coastal = {
        name = "Tropical Coast", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['light_green'] },
        plants = { none=5, grass=20, reeds_paper=2, reeds_giant=2, sage=1, daisy=1, reeds=2, strawberry=1, goya=1, cabbage=1, caper=1, cassava=1, horned_melon=1, lentil=1, lettuce=1, onion=1, parsnip=1, soybean=1, watermelon=1, yam=1, jute=1, ramie=1, cotton=1 },
        trees = { deciduous = 10, evergreen = 0 },
        wildlife = { "deer","armadillo","boar","antelope","horse"},
        nouns = { "Coast", "Cliffs", "Seashore", "Seaside", "Resort", "Littoral", "Bay" }
    },

    -- Salt-marsh Zones
    arctic_salt = {
        name = "Arctic Coast", min_temp = -100, max_temp = -5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] }, soils = { soil=70, sand=30 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['white'] },
        plants = { none=20, grass=20, lavendar=1, daisy=1, reeds=2 },
        trees = { deciduous = 0, evergreen = 5 },
        wildlife = { "deer","mammoth"},
        nouns = { "Arctic", "Tundra", "Devil's Icebox" }
    },
    cold_salt = {
        name = "Cold Coast", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] }, soils = { soil=70, sand=30 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['cyan'] },
        plants = { none=10, grass=20, lavendar=1, daisy=1, reeds=2, cabbage=1, cress=1 },
        trees = { deciduous = 3, evergreen = 10 },
        wildlife = { "deer","mammoth"},
        nouns = { "Tundra", "Cold Desert" }
    },
    temperate_salt = {
        name = "Temperate Coast", min_temp = 5, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] }, soils = { soil=70, sand=30 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['green'] },
        trees = { deciduous = 15, evergreen = 0 },
        wildlife = { "deer","boar"},
        nouns = { "Badlands", "Waste", "Flats" }
    },
    tropical_salt = {
        name = "Tropical Coast", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["salt_marsh"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['double_wave'], color=colors['light_green'] },
        trees = { deciduous = 15, evergreen = 0 },
        wildlife = { "deer","boar"},
        nouns = { "Badlands", "Waste", "Flats", "Alkali Flat" }
    },

    -- Flatlands
    permafrost_plain = {
        name = "Permafrost Plain", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['white'] },
        plants = { none=20, grass=20, lavendar=1, daisy=1, reeds=2, heather=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer","mammoth"},
        nouns = { "Permafrost", "Tundra", "Frozen Plain" }
    },
    rocky_plain = {
        name = "Rocky Plain", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['grey'] },
        plants = { none=25, grass=20, sage=1, daisy=1, reeds=2, cabbage=1, leek=1, hemp=1 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer","horse"},
        nouns = { "Plain", "Scarp", "Scree", "Boulderland" }
    },
    grass_plain = {
        name = "Grass Plain", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] }, soils = { soil=75, sand=25 },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['green'] },
        plants = { none=3, grass=20, sage=1, daisy=1, reeds=2, strawberry=1, artichoke=1, asparagus=1, string_bean=1, broad_bean=1, beetroot=1, cabbage=1, carrot=1, celery=1, chickpea=1, chickory=1, cucumber=1, cress=1, garlic=1, leek=1, lentil=1, onion=1, parsnip=1, pea=1, potato=1, radish=1, red_bean=1, rhubarb=1, soybean=1, spinach=1, turnip=1, hemp=1, ramie=1, cotton=1 },
        trees = { deciduous = 20, evergreen = 3 },
        wildlife = { "deer","boar","hedgehog","horse"},
        nouns = { "Grasslands", "Plain", "Prairie", "Heath", "Level" }
    },
    savannah_plain = {
        name = "Savannah Plain", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["coast"], biome_types["marsh"] }, soils = { soil=25, sand=75 },
        worldgen_render = { glyph=glyphs['one_half_solid'], color=colors['earth_brown'] },
        plants = { none=3, grass=25, reeds_giant=2, sage=1, daisy=1, reeds=2, strawberry=1,asparagus=1, string_bean=1, broad_bean=1, caper=1, carrot=1, cassava=1, celery=1, chickpea=1, chickory=1, cowpea=1, cucumber=1, eggplant=1, garlic=1, horned_melon=1, lentil=1, lettuce=1, mung_bean=1, onion=1, parsnip=1, peanut=1, pepper=1, soybean=1, spinach=1, squash=1, sweet_potato=1, tomatillo=1, watermelon=1, yam=1, reeds_paper=2, jute=1, hemp=1, ramie=1, cotton=1 },
        trees = { deciduous = 20, evergreen = 0 },
        wildlife = { "deer","armadillo","boar","elephant","antelope","horse"},
        nouns = { "Savannah", "Grassland", "Heath", "Scrub", "Level" }
    },

    -- Hills
    permafrost_hills = {
        name = "Permafrost Hills", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['white'] },
        plants = { none=20, grass=20, lavendar=1, daisy=1, reeds=2, heather=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer","mammoth"},
        nouns = { "Ice Hill", "Hill", "Mound", "Roughs" }
    },
    rocky_hills = {
        name = "Rocky Hills", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['grey'] },
        plants = { none=30, grass=20, lavendar=1, daisy=1, reeds=2, heather=2, cabbage=1, parsnip=1, hemp=1 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = {"deer","horse"},
        nouns = { "Rocky Hills", "Hill", "Mound", "Bump", "Scree", "Scar" }
    },
    grass_hills = {
        name = "Grass Hills", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] }, soils = { soil=75, sand=25 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, strawberry=1, artichoke=1,asparagus=1, string_bean=1, broad_bean=1, beetroot=1, cabbage=1, carrot=1, celery=1, chickpea=1, chickory=1, cress=1, garlic=1, lentil=1, onion=1, parsnip=1, pea=1, soybean=1, turnip=1, hemp=1, ramie=1, cotton=1 },
        trees = { deciduous = 5, evergreen = 3 },
        wildlife = { "deer","boar","horse"},
        nouns = { "Rolling Hill", "Hill", "Rolling Heath", "Moor" }
    },
    savannah_hills = {
        name = "Savannah Hills", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["hills"], biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['earth_brown'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2,asparagus=1, cabbage=1, caper=1, carrot=1, celery=1, chickpea=1, chickory=1, eggplant=1, lentil=1, onion=1, peanut=1, pepper=1, soybean=1, jute=1, ramie=1, cotton=1 },
        trees = { deciduous = 10, evergreen = 0 },
        wildlife = { "deer","boar","elephant","antelope","horse"},
        nouns = { "Savannah Hills", "Hill", "Rolling Savannah", "Savannah Moor" }
    },

     -- Plateau
    permafrost_plateau = {
        name = "Permafrost Plateau", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['white'] },
        plants = { none=20, grass=20, lavendar=1, daisy=1, reeds=2, heather=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer"},
        nouns = { "Glacier", "Plateau", "Ice Plain", "Steppe" }
    },
    rocky_plateau = {
        name = "Rocky Plateau", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['grey'] },
        plants = { none=15, grass=20, sage=1, daisy=1, reeds=2, cabbage=1, hemp=1 },
        trees = { deciduous = 0, evergreen = 3 },
        wildlife = {"deer","horse"},
        nouns = { "Tableland", "Tablerock", "Plateau", "Rocky Plateau" }
    },
    grass_plateau = {
        name = "Grass Plateau", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, strawberry=1, asparagus=1, beetroot=1, cabbage=1, celery=1, chickpea=1, chickory=1, cucumber=1, lentil=1, garlic=1, onion=1, soybean=1, turnip=1, hemp=1, ramie=1, cotton=1 },
        trees = { deciduous = 20, evergreen = 5 },
        wildlife = { "deer","badger","boar","horse"},
        nouns = { "Hog's Back", "Plateau", "Table", "Fell", "Downs" }
    },
    savannah_plateau = {
        name = "Savannah Plateau", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['earth_brown'] },
        plants = { none=2, grass=20, sage=1, daisy=1, reeds=2, strawberry=1, asparagus=1, bambara_groundnut=1, goya=1, cabbage=1, caper=1, celery=1, chickpea=1, chickory=1, cucumber=1, eggplant=1, horned_melon=1, lentil=1, onion=1, soybean=1, tomatillo=1, hemp=1, ramie=1, cotton=1 },
        trees = { deciduous = 25, evergreen = 0 },
        wildlife = { "deer","armadillo","boar","antelope","horse"},
        nouns = { "Upland", "Table", "Plateau" }
    },
    badland_plateau = {
        name = "Badlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plateau"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['arch'], color=colors['red_sand'] },
        trees = { deciduous = 1, evergreen = 0 },
        wildlife = { "deer","armadillo"},
        nouns = { "Devil's Table", "Badland Plateau" }
    },

    -- Highlands
    permafrost_highlands = {
        name = "Permafrost Highlands", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] },
        plants = { none=20, grass=10, lavendar=1, daisy=1, reeds=2, heather=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer"},
        nouns = { "High Waste", "High Tundra", "Hillock", "Promontary" }
    },
    rocky_highlands = {
        name = "Rocky Highlands", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['grey'] },
        plants = { none=25, grass=10, sage=1, daisy=1, reeds=2, cabbage=1 },
        trees = { deciduous = 0, evergreen = 5 },
        wildlife = { "deer","boar","horse"},
        nouns = { "Waste", "Scree", "Fell", "Promontary", "Dales", "Pike" }
    },
    grass_highlands = {
        name = "Grass Highlands", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, beetroot=1, strawberry=1, cabbage=1, onion=1, turnip=1, hemp=1 },
        trees = { deciduous = 10, evergreen = 3 },
        wildlife = { "deer","badger","boar","horse"},
        nouns = { "Moor", "Heath", "Uplands", "Dales", "Scar" }
    },
    savannah_highlands = {
        name = "Savannah Highlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=25, sand=75 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['earth_brown'] },
        plants = { none=2, grass=20, sage=1, daisy=1, reeds=2, beetroot=1, strawberry=1, goya=1, cabbage=1, hemp=1 },
        trees = { deciduous = 20, evergreen = 1 },
        wildlife = { "deer","armadillo","boar","horse"},
        nouns = { "Savannah Hills", "Highlands", "Uplands", "Peaks", "Mounds" }
    },
    badland_highlands = {
        name = "Badland Highlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["highlands"] }, soils = { soil=10, sand=90 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['red_sand'] },
        trees = { deciduous = 1, evergreen = 0 },
        wildlife = { "deer","armadillo"},
        nouns = { "Scree", "Heights" }
    },

    -- Mountains
    permafrost_mountains = {
        name = "Permafrost Mountains", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['white'] },
        plants = { none=40, grass=20, lavendar=1, daisy=1, reeds=2, heather=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer"},
        nouns = { "Mountains", "Peaks", "Pikes", "Alps" }
    },
    rocky_mountains = {
        name = "Rocky Mountains", min_temp = -5, max_temp = 5, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['grey'] },
        plants = { none=25, grass=20, sage=1, daisy=1, reeds=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer","horse"},
        nouns = { "Mountains", "Peaks", "Pikes", "Alps" }
    },
    grass_mountains = {
        name = "Grass Mountains", min_temp = 0, max_temp = 25, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, hemp=1 },
        trees = { deciduous = 0, evergreen = 5 },
        wildlife = { "deer","boar","horse"},
        nouns = { "Mountains", "Peaks", "Pikes", "Alps" }
    },
    savannah_mountains = {
        name = "Savannah Mountains", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 100, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=25, sand=75 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['earth_brown'] },
        plants = { none=2, grass=20, sage=1, daisy=1, reeds=2, goya=1, hemp=1 },
        trees = { deciduous = 3, evergreen = 0 },
        wildlife = { "deer","armadillo","boar","horse"},
        nouns = { "Mountains", "Peaks", "Pikes", "Alps" }
    },
    badland_mountains = {
        name = "Badlands", min_temp = 25, max_temp = 55, min_rain = 0, max_rain = 20, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"] }, soils = { soil=10, sand=90 },
        worldgen_render = { glyph=glyphs['ramp_up'], color=colors['red_sand'] },
        trees = { deciduous = 1, evergreen = 0 },
        wildlife = { "deer","armadillo"},
        nouns = { "Mountains", "Peaks", "Pikes", "Alps" }
    },

    -- Low precipitation desert areas
    tundra = {
        name = "Tundra", min_temp = -100, max_temp = 3, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["highlands"], biome_types["plateau"], biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['tilde'], color=colors['white'] },
        plants = { none=40, grass=20, lavendar=1, daisy=1, reeds=2, heather=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer"},
        nouns = { "Tundra", "Waste", "Ice Waste" }
    },
    cold_desert = {
        name = "Cold Desert", min_temp = 1, max_temp = 10, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["highlands"], biome_types["plateau"], biome_types["coast"] }, soils = { soil=15, sand=85 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['white'] },
        plants = { none=40, grass=20, lavendar=1, daisy=1, reeds=2, heather=2 },
        trees = { deciduous = 0, evergreen = 1 },
        wildlife = { "deer"},
        nouns = { "Semi-Tundra", "Expanse", "Steppe" }
    },
    sand_desert = {
        name = "Sand Desert", min_temp = 9, max_temp = 100, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] }, soils = { soil=5, sand=95 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['yellow_sand'] },
        trees = { deciduous = 1, evergreen = 0 },
        wildlife = { "deer","horse"},
        nouns = { "Desert", "Waste", "Wild", "Dunes", "Barren" }
    },
    rock_desert = {
        name = "Rocky Desert", min_temp = 9, max_temp = 100, min_rain = 0, max_rain = 10, min_mutation = 0, max_mutation = 100,       
        occurs = { biome_types["mountains"], biome_types["plateau"], biome_types["hills"], biome_types["coast"] }, soils = { soil=50, sand=50 },
        worldgen_render = { glyph=glyphs['one_quarter_solid'], color=colors['red_sand'] },
        trees = { deciduous = 1, evergreen = 0 },
        wildlife = { "deer","armadillo","horse"},
        nouns = { "Barrens", "Platter" }
    },

    -- High-precipitation areas
    deciduous_broadleaf = {
        name = "Deciduous Broadleaf", min_temp = 5, max_temp = 34, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, bambara_groundnut=1, strawberry=1, cucumber=1, eggplant=1, garlic=1, tomato=1, tomatillo=1, hemp=1 },
        trees = { deciduous = 100, evergreen = 0 },
        wildlife = { "deer","badger","boar","hedgehog","antelope","horse"},
        nouns = { "Forest", "Backwoods", "Old Forest", "Grove", "Timberland", "Chase" }
    },
    deciduous_needleleaf = {
        name = "Deciduous Needleleaf", min_temp = 5, max_temp = 34, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["hills"], biome_types["plateau"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['clubs'], color=colors['tree_green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, cucumber=1, garlic=1, tomato=1, tomatillo=1, hemp=1 },
        trees = { deciduous = 100, evergreen = 5 },
        wildlife = { "deer","badger","boar","hedgehog","antelope","horse"},
        nouns = { "Forest", "Woods", "Old WOods", "Grove", "Chase", "Weald" }
    },
    evergreen_broadleef = {
        name = "Evergreen Broadleaf", min_temp = -5, max_temp = 20, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["hills"], biome_types["plateau"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, garlic=1, hemp=1 },
        trees = { deciduous = 0, evergreen = 100 },
        wildlife = { "deer","badger","boar","hedgehog","horse"},
        nouns = { "Forest", "Coppice", "Chase", "Weald", "Pines" }
    },
    evergreen_needleleef = {
        name = "Evergreen Broadleaf", min_temp = -15, max_temp = 30, min_rain = 15, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["highlands"], biome_types["mountains"], biome_types["plateau"] }, soils = { soil=80, sand=20 },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] },
        plants = { none=5, grass=20, sage=1, daisy=1, reeds=2, garlic=1, hemp=1 },
        trees = { deciduous = 5, evergreen = 100 },
        wildlife = { "deer","badger","boar","horse"},
        nouns = { "Forest", "Coppice", "Chase", "Weald", "Pines", "Timber" }
    },
    rainforest = {
        name = "Rainforest", min_temp = 20, max_temp = 34, min_rain = 25, max_rain = 100, min_mutation = 0, max_mutation = 100,        
        occurs = { biome_types["plains"], biome_types["hills"], biome_types["coast"] }, soils = { soil=90, sand=10 },
        worldgen_render = { glyph=glyphs['spades'], color=colors['tree_green'] },
        trees = { deciduous = 200, evergreen = 0 },
        wildlife = { "deer","antelope"},
        nouns = { "Jungle", "Morass", "Tangle" }
    }
}