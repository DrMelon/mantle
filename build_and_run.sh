#!/usr/bin/bash

python scripts/convert_ldtk.py ldtk_proj/mantle_desert.ldtk source/c/desert_maps
python scripts/convert_ldtk.py ldtk_proj/mantle_island.ldtk source/c/island_maps
python scripts/convert_ldtk.py ldtk_proj/mantle_icepalace.ldtk source/c/icepalace_maps
python scripts/convert_ldtk.py ldtk_proj/mantle_city.ldtk source/c/city_maps
python scripts/convert_ldtk.py ldtk_proj/mantle_shelter_ext.ldtk source/c/forest_maps
create-nes-game build
mesen2 rom/mantle.nes
