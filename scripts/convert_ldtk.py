#!/usr/bin/python -w
from ldtkjson import *
import sys
import json

ldtk_file_name = sys.argv[1]
c_file_path = sys.argv[2]
print("Loading: ", ldtk_file_name)
print("Target C/H files: ", c_file_path)
with open(ldtk_file_name) as ldtk_file:
    ldtk_dict = json.load(ldtk_file)
    ldtk_data = ldtk_json_from_dict(ldtk_dict)
    print("Successfully loaded LDTK file!")
    print("Levels:", len(ldtk_data.levels))
    ldtk_level_prefix = ldtk_data.level_name_pattern.split("%")[0].lower()
    print("Level name pattern: ", ldtk_level_prefix)

    # Step 1: Associate every UUID of each level with the integer of the level's order
    level_indices = {}
    for i in range(0, len(ldtk_data.levels)):
        level_indices[ldtk_data.levels[i].iid] = i

    # Step 2: For each level, produce C file and H file string, using tile information, neighbour information, and entity information.
    h_string_for_ldtk_files = "#ifndef __" + ldtk_level_prefix.upper() + "_ROOMS_H_\n"
    h_string_for_ldtk_files += "#define __" + ldtk_level_prefix.upper() + "_ROOMS_H_\n\n"
    h_string_for_ldtk_files += "extern const unsigned char* const " + ldtk_level_prefix + "rooms[];\n"
    c_string_for_ldtk_files = "#include \""+ c_file_path +".h\"\n"
    c_string_for_ldtk_files += "#include \"bank_helpers.h\"\n"
    c_string_for_ldtk_files += "#include \"globals.h\"\n\n"
    c_string_for_ldtk_files += "CODE_BANK(ROOM_LOGIC_BANK);\n"
    environment_ent_counter = 0

    for i in range(0, len(ldtk_data.levels)):
        h_string_for_level = "extern const unsigned char " + ldtk_level_prefix + "room_" + str(i) + "[];\n"
        c_string_for_level = "const unsigned char " + ldtk_level_prefix + "room_" + str(i) + "[] = {\n"
        # Get neighbours.
        # Any non-existent neighbour is set to a special index.
        # This is used by the game to move the player to the "tree room".
        s_neighbour = 255
        e_neighbour = 255
        n_neighbour = 255
        w_neighbour = 255

        # Extract neighbour indices.
        for neighbour_data in ldtk_data.levels[i].neighbours:
            if(neighbour_data.dir == "s"):
                s_neighbour = level_indices[neighbour_data.level_iid]
            if(neighbour_data.dir == "e"):
                e_neighbour = level_indices[neighbour_data.level_iid]
            if(neighbour_data.dir == "n"):
                n_neighbour = level_indices[neighbour_data.level_iid]
            if(neighbour_data.dir == "w"):
                w_neighbour = level_indices[neighbour_data.level_iid]

        # Write neighbour info into C string.
        c_string_for_level += str(s_neighbour) + ", " + str(e_neighbour) + ", " + str(n_neighbour) + ", " + str(w_neighbour) + ",\n"

        # Then write tile info
        #x_num = 0
        #for tile in ldtk_data.levels[i].layer_instances[1].grid_tiles:
        #    c_string_for_level += str(tile.t) + ", "
        #    x_num += 1
        #    if(x_num == 12):
        #        c_string_for_level += "\n"
        #        x_num = 0

        # Write tile info with RLE!
        last_tile = 255
        bytes_written = 0
        tile_count = 0
        tile_num = 0
        for tile in ldtk_data.levels[i].layer_instances[1].grid_tiles:
            tile_id = tile.t
            if(tile_id != last_tile):
                # new tile entry
                if(tile_num > 0): #write previous entry count
                    c_string_for_level += str(tile_count) + ", "
                tile_count = 1
                c_string_for_level += str(tile_id) + ", "
                last_tile = tile_id
                bytes_written += 2
            else:
                tile_count += 1
            tile_num += 1
        c_string_for_level += str(tile_count) + ", \n"
        if(bytes_written > 12*8):
            print("RLE bytes over 96 for level " + str(i) + ": " + str(bytes_written))


        # Then write entity info
        for entity in ldtk_data.levels[i].layer_instances[0].entity_instances:
            if(entity.identifier == "Walker"):
                c_string_for_level += "0, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "0, "
                c_string_for_level += "0, "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "BeefyWalker"):
                c_string_for_level += "0, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "0, "
                c_string_for_level += "1, "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "Shooter"):
                c_string_for_level += "0, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "1, "
                c_string_for_level += "0, "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "Fish"):
                c_string_for_level += "0, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "2, "
                c_string_for_level += "0, "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "Lizard"):
                c_string_for_level += "0, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "3, "
                c_string_for_level += "0, "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "Flower"):
                c_string_for_level += "0, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "4, "
                c_string_for_level += "0, "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "Bird"):
                c_string_for_level += "0, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "5, "
                c_string_for_level += "0, "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "Cat"):
                c_string_for_level += "0, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "6, "
                c_string_for_level += str(entity.field_instances[0].value) + ", "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "Sword"):
                c_string_for_level += "2, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "0, "
                c_string_for_level += "0, "
                c_string_for_level += str(environment_ent_counter) + ", \n"
            if(entity.identifier == "Teleporter"):
                c_string_for_level += "1, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += str(entity.field_instances[0].value) + ", "
                c_string_for_level += str(entity.field_instances[1].value) + ", "
                c_string_for_level += str(entity.field_instances[2].value) + ", \n"
            if(entity.identifier == "Raft"):
                c_string_for_level += "3, "
                c_string_for_level += str(entity.grid[0]) + ", "
                c_string_for_level += str(entity.grid[1]) + ", "
                c_string_for_level += "0, "
                c_string_for_level += "0, "
                c_string_for_level += str(environment_ent_counter) + ", \n"

            environment_ent_counter += 1

        # Then write entity data terminator
        c_string_for_level += "128\n};\n"

        # Now write entries into their respective file strings.
        h_string_for_ldtk_files += h_string_for_level
        c_string_for_ldtk_files += c_string_for_level + "\n"

    print("Entities in environment: " + str(environment_ent_counter) + "/256")

    # Now it's time to write the C/H files.
    h_string_for_ldtk_files += "#endif"

    c_string_for_ldtk_files += "const unsigned char* const " + ldtk_level_prefix + "rooms[] = {\n"
    for i in range(0, len(ldtk_data.levels)):
        c_string_for_ldtk_files += ldtk_level_prefix + "room_" + str(i) + ", \n"
    c_string_for_ldtk_files += "};\n"
    c_string_for_ldtk_files += "CODE_BANK_POP();"

    with open(c_file_path + ".h", "w") as headerfile:
        headerfile.write(h_string_for_ldtk_files)

    with open(c_file_path + ".c", "w") as codefile:
        codefile.write(c_string_for_ldtk_files)
