#!/usr/bin/bash

python scripts/convert_ldtk.py ldtk_proj/mantle_desert.ldtk source/c/desert_maps
create-nes-game build
mesen2 rom/mantle.nes
