;
; CHR ROM data
;
; The files below will be loaded into the game's chr banks directly, and available for your
; use.
;

; First chr bank, this is used by default.
; Each segment is 8kb, but we store data in 4kb tilesets. Each segment thus contains 2 tilesets. 
.segment "CHR_00"
    .incbin "./background_desert.chr"
    .incbin "./sprites_mantle.chr"

; Other chr banks need some data in them - repeated chr data can be replaced.
.segment "CHR_01"
    .incbin "./background_island.chr"
    .incbin "./sprites_mantle.chr"
.segment "CHR_02"
    .incbin "./background_city.chr"
    .incbin "./sprites_mantle.chr"
.segment "CHR_03"
    .incbin "./background_intro.chr"
    .incbin "./sprites_mantle.chr"
