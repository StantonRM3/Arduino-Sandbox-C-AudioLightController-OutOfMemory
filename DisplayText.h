#ifndef DISPLAYTEXT_H
#define DISPLAYTEXT_H

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MENU TEXT
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char ds_NowPlaying[] = "sirenONE";
const char ds_Menu[] = "Main Menu";
const char ds_Clock[] = "Clock";
const char ds_Display[] = "Display";
const char ds_Display_Colour[] = "Colour";
const char ds_Display_Colour_Red[] = "Fireside";
const char ds_Display_Colour_Orange[] = "WhiteHot";
const char ds_Display_Colour_Yellow[] = "Mint";
const char ds_Display_Colour_Green[] = "Gaia";
const char ds_Display_Colour_Aqua[] = "Ocean";
const char ds_Display_Colour_Blue[] = "BlueSky";
const char ds_Display_Colour_Purple[] = "NightSky";
const char ds_Display_Colour_Pink[] = "Pastel";
const char ds_Display_Colour_Random[] = "RANDOM";
const char ds_Display_Pattern[] = "Pattern";
const char ds_Display_Pattern_VU[] = "VU";
const char ds_Display_Pattern_VU_Peak[] = "VU Peak";
const char ds_Display_Pattern_VU_Huebars[] = "VU Huebars";
const char ds_Display_Pattern_VU_Pulsar[] = "Pulsar";
const char ds_Display_Pattern_VU_Reverse[] = "Reverse";
const char ds_Display_Pattern_VU_Side[] = "Sideways";
const char ds_Display_Pattern_VU_SidePulse[] = "Side Pulse";
const char ds_Display_Pattern_Oscilloscope[] = "Scope";
const char ds_Settings[] = "Settings";
const char ds_Settings_SetTone[] = "Set Tone";

#define SIRENLOGO_WIDTH 64

// SQUINT YOUR EYES AND YOU'LL SEE THE LOGO IN THERE SOMEWHERE...
const int sirenOne[8][4] = {
  { 0b0000000000100000, 0b0000000000000000, 0b0000011110000110, 0b0001001111111000 },
  { 0b0000000000000000, 0b0000000000000000, 0b0000100001000101, 0b0001001000000000 }, 
  { 0b0000011100100101, 0b1101111000101110, 0b0001000000100101, 0b0001001000000000 }, 
  { 0b0000100000100110, 0b0010000100110001, 0b0001000000100100, 0b1001001111100000 }, 
  { 0b0000011000100100, 0b0011111100100001, 0b0001000000100100, 0b1001001000000000 },
  { 0b0000000100100100, 0b0010000000100001, 0b0001000000100100, 0b0101001000000000 },
  { 0b0000000100100100, 0b0010000100100001, 0b0000100001000100, 0b0101001000000000 }, 
  { 0b0000111000100100, 0b0001111000100001, 0b0000011110000100, 0b0011001111111000 } 
};

#endif
