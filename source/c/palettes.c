#include "palettes.h"


// Color palette for the screen to use
const unsigned char paletteDesert[] = {
    0x17, 0x0f, 0x16, 0x30, // Text
    0x17, 0x27, 0x37, 0x28, // Desert Sand & Vegetation
    0x17, 0x05, 0x16, 0x26, // Red Rocks, Bridges
    0x17, 0x27, 0x22, 0x31 // Water & Watery Rock
};
const unsigned char paletteDesertIce[] = {
    0x12, 0x0f, 0x16, 0x30, // Text
    0x12, 0x21, 0x31, 0x2C, // Desert Sand & Vegetation
    0x12, 0x03, 0x13, 0x23, // Red Rocks, Bridges
    0x12, 0x27, 0x22, 0x31 // Water & Watery Rock
};
const unsigned char paletteIsland[] = {
    0x09, 0x0f, 0x16, 0x30, // Text
    0x09, 0x27, 0x37, 0x2A, // Desert Sand & Vegetation
    0x09, 0x05, 0x16, 0x26, // Red Rocks, Bridges
    0x09, 0x27, 0x1C, 0x3C // Water & Watery Rock
};
// BG Palettes per environment
// Making this mutable lets us alter the desert palette as needed
const unsigned char* envPalettes[] =
{
    paletteDesert,
    paletteIsland,
//    paletteIcePalace,
//    paletteCity,
//    paletteShelter
};

// Color palettes for sprites (Kris & Monsters)
const unsigned char palSpritesDesert[16] = {
    0x0f, 0x03, 0x22, 0x3c,
    0x0f, 0x0f, 0x21, 0x30,
    0x0f, 0x28, 0x30, 0x15,
    0x0f, 0x10, 0x12, 0x16
};

const unsigned char* envSprPalettes[] =
{
    palSpritesDesert
};
