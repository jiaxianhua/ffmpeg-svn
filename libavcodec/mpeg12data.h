/*
 * MPEG1/2 tables
 */

const int16_t ff_mpeg1_default_intra_matrix[64] = {
	8, 16, 19, 22, 26, 27, 29, 34,
	16, 16, 22, 24, 27, 29, 34, 37,
	19, 22, 26, 27, 29, 34, 34, 38,
	22, 22, 26, 27, 29, 34, 37, 40,
	22, 26, 27, 29, 32, 35, 40, 48,
	26, 27, 29, 32, 35, 40, 48, 58,
	26, 27, 29, 34, 38, 46, 56, 69,
	27, 29, 35, 38, 46, 56, 69, 83
};

const int16_t ff_mpeg1_default_non_intra_matrix[64] = {
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
};

const unsigned char vlc_dc_table[256] = {
    0, 1, 2, 2,
    3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,

    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};

const uint16_t vlc_dc_lum_code[12] = {
    0x4, 0x0, 0x1, 0x5, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe, 0x1ff,
};
const unsigned char vlc_dc_lum_bits[12] = {
    3, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9, 9,
};

const uint16_t vlc_dc_chroma_code[12] = {
    0x0, 0x1, 0x2, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe, 0x3fe, 0x3ff,
};
const unsigned char vlc_dc_chroma_bits[12] = {
    2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10,
};

/* simple include everything table for dc, first byte is bits number next 3 are code*/
static uint32_t mpeg1_lum_dc_uni[512];
static uint32_t mpeg1_chr_dc_uni[512];

static const uint16_t mpeg1_vlc[113][2] = {
 { 0x3, 2 }, { 0x4, 4 }, { 0x5, 5 }, { 0x6, 7 },
 { 0x26, 8 }, { 0x21, 8 }, { 0xa, 10 }, { 0x1d, 12 },
 { 0x18, 12 }, { 0x13, 12 }, { 0x10, 12 }, { 0x1a, 13 },
 { 0x19, 13 }, { 0x18, 13 }, { 0x17, 13 }, { 0x1f, 14 },
 { 0x1e, 14 }, { 0x1d, 14 }, { 0x1c, 14 }, { 0x1b, 14 },
 { 0x1a, 14 }, { 0x19, 14 }, { 0x18, 14 }, { 0x17, 14 },
 { 0x16, 14 }, { 0x15, 14 }, { 0x14, 14 }, { 0x13, 14 },
 { 0x12, 14 }, { 0x11, 14 }, { 0x10, 14 }, { 0x18, 15 },
 { 0x17, 15 }, { 0x16, 15 }, { 0x15, 15 }, { 0x14, 15 },
 { 0x13, 15 }, { 0x12, 15 }, { 0x11, 15 }, { 0x10, 15 },
 { 0x3, 3 }, { 0x6, 6 }, { 0x25, 8 }, { 0xc, 10 },
 { 0x1b, 12 }, { 0x16, 13 }, { 0x15, 13 }, { 0x1f, 15 },
 { 0x1e, 15 }, { 0x1d, 15 }, { 0x1c, 15 }, { 0x1b, 15 },
 { 0x1a, 15 }, { 0x19, 15 }, { 0x13, 16 }, { 0x12, 16 },
 { 0x11, 16 }, { 0x10, 16 }, { 0x5, 4 }, { 0x4, 7 },
 { 0xb, 10 }, { 0x14, 12 }, { 0x14, 13 }, { 0x7, 5 },
 { 0x24, 8 }, { 0x1c, 12 }, { 0x13, 13 }, { 0x6, 5 },
 { 0xf, 10 }, { 0x12, 12 }, { 0x7, 6 }, { 0x9, 10 },
 { 0x12, 13 }, { 0x5, 6 }, { 0x1e, 12 }, { 0x14, 16 },
 { 0x4, 6 }, { 0x15, 12 }, { 0x7, 7 }, { 0x11, 12 },
 { 0x5, 7 }, { 0x11, 13 }, { 0x27, 8 }, { 0x10, 13 },
 { 0x23, 8 }, { 0x1a, 16 }, { 0x22, 8 }, { 0x19, 16 },
 { 0x20, 8 }, { 0x18, 16 }, { 0xe, 10 }, { 0x17, 16 },
 { 0xd, 10 }, { 0x16, 16 }, { 0x8, 10 }, { 0x15, 16 },
 { 0x1f, 12 }, { 0x1a, 12 }, { 0x19, 12 }, { 0x17, 12 },
 { 0x16, 12 }, { 0x1f, 13 }, { 0x1e, 13 }, { 0x1d, 13 },
 { 0x1c, 13 }, { 0x1b, 13 }, { 0x1f, 16 }, { 0x1e, 16 },
 { 0x1d, 16 }, { 0x1c, 16 }, { 0x1b, 16 },
 { 0x1, 6 }, /* escape */
 { 0x2, 2 }, /* EOB */
};

static const uint16_t mpeg2_vlc[113][2] = {
  {0x02, 2}, {0x06, 3}, {0x07, 4}, {0x1c, 5},
  {0x1d, 5}, {0x05, 6}, {0x04, 6}, {0x7b, 7},
  {0x7c, 7}, {0x23, 8}, {0x22, 8}, {0xfa, 8},
  {0xfb, 8}, {0xfe, 8}, {0xff, 8}, {0x1f,14},
  {0x1e,14}, {0x1d,14}, {0x1c,14}, {0x1b,14},
  {0x1a,14}, {0x19,14}, {0x18,14}, {0x17,14},
  {0x16,14}, {0x15,14}, {0x14,14}, {0x13,14},
  {0x12,14}, {0x11,14}, {0x10,14}, {0x18,15},
  {0x17,15}, {0x16,15}, {0x15,15}, {0x14,15},
  {0x13,15}, {0x12,15}, {0x11,15}, {0x10,15},
  {0x02, 3}, {0x06, 5}, {0x79, 7}, {0x27, 8},
  {0x20, 8}, {0x16,13}, {0x15,13}, {0x1f,15},
  {0x1e,15}, {0x1d,15}, {0x1c,15}, {0x1b,15},
  {0x1a,15}, {0x19,15}, {0x13,16}, {0x12,16},
  {0x11,16}, {0x10,16}, {0x05, 5}, {0x07, 7}, 
  {0xfc, 8}, {0x0c,10}, {0x14,13}, {0x07, 5}, 
  {0x26, 8}, {0x1c,12}, {0x13,13}, {0x06, 6}, 
  {0xfd, 8}, {0x12,12}, {0x07, 6}, {0x04, 9}, 
  {0x12,13}, {0x06, 7}, {0x1e,12}, {0x14,16}, 
  {0x04, 7}, {0x15,12}, {0x05, 7}, {0x11,12}, 
  {0x78, 7}, {0x11,13}, {0x7a, 7}, {0x10,13}, 
  {0x21, 8}, {0x1a,16}, {0x25, 8}, {0x19,16}, 
  {0x24, 8}, {0x18,16}, {0x05, 9}, {0x17,16}, 
  {0x07, 9}, {0x16,16}, {0x0d,10}, {0x15,16}, 
  {0x1f,12}, {0x1a,12}, {0x19,12}, {0x17,12}, 
  {0x16,12}, {0x1f,13}, {0x1e,13}, {0x1d,13}, 
  {0x1c,13}, {0x1b,13}, {0x1f,16}, {0x1e,16}, 
  {0x1d,16}, {0x1c,16}, {0x1b,16}, 
  {0x01,6}, /* escape */
  {0x06,4}, /* EOB */
};

static const int8_t mpeg1_level[111] = {
  1,  2,  3,  4,  5,  6,  7,  8,
  9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24,
 25, 26, 27, 28, 29, 30, 31, 32,
 33, 34, 35, 36, 37, 38, 39, 40,
  1,  2,  3,  4,  5,  6,  7,  8,
  9, 10, 11, 12, 13, 14, 15, 16,
 17, 18,  1,  2,  3,  4,  5,  1,
  2,  3,  4,  1,  2,  3,  1,  2,
  3,  1,  2,  3,  1,  2,  1,  2,
  1,  2,  1,  2,  1,  2,  1,  2,
  1,  2,  1,  2,  1,  2,  1,  2,
  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,
};

static const int8_t mpeg1_run[111] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  2,  2,  2,  2,  2,  3,
  3,  3,  3,  4,  4,  4,  5,  5,
  5,  6,  6,  6,  7,  7,  8,  8,
  9,  9, 10, 10, 11, 11, 12, 12,
 13, 13, 14, 14, 15, 15, 16, 16,
 17, 18, 19, 20, 21, 22, 23, 24,
 25, 26, 27, 28, 29, 30, 31,
};

static uint8_t mpeg1_index_run[2][64];
static int8_t mpeg1_max_level[2][64];

static RLTable rl_mpeg1 = {
    111,
    111,
    mpeg1_vlc,
    mpeg1_run,
    mpeg1_level,
};

static RLTable rl_mpeg2 = {
    111,
    111,
    mpeg2_vlc,
    mpeg1_run,
    mpeg1_level,
};

static const uint8_t mbAddrIncrTable[35][2] = {
    {0x1, 1},
    {0x3, 3},
    {0x2, 3},
    {0x3, 4},
    {0x2, 4},
    {0x3, 5},
    {0x2, 5},
    {0x7, 7},
    {0x6, 7},
    {0xb, 8},
    {0xa, 8},
    {0x9, 8},
    {0x8, 8},
    {0x7, 8},
    {0x6, 8},
    {0x17, 10},
    {0x16, 10},
    {0x15, 10},
    {0x14, 10},
    {0x13, 10},
    {0x12, 10},
    {0x23, 11},
    {0x22, 11},
    {0x21, 11},
    {0x20, 11},
    {0x1f, 11},
    {0x1e, 11},
    {0x1d, 11},
    {0x1c, 11},
    {0x1b, 11},
    {0x1a, 11},
    {0x19, 11},
    {0x18, 11},
    {0x8, 11}, /* escape */
    {0xf, 11}, /* stuffing */
};

static const uint8_t mbPatTable[63][2] = {
    {0xb, 5},
    {0x9, 5},
    {0xd, 6},
    {0xd, 4},
    {0x17, 7},
    {0x13, 7},
    {0x1f, 8},
    {0xc, 4},
    {0x16, 7},
    {0x12, 7},
    {0x1e, 8},
    {0x13, 5},
    {0x1b, 8},
    {0x17, 8},
    {0x13, 8},
    {0xb, 4},
    {0x15, 7},
    {0x11, 7},
    {0x1d, 8},
    {0x11, 5},
    {0x19, 8},
    {0x15, 8},
    {0x11, 8},
    {0xf, 6},
    {0xf, 8},
    {0xd, 8},
    {0x3, 9},
    {0xf, 5},
    {0xb, 8},
    {0x7, 8},
    {0x7, 9},
    {0xa, 4},
    {0x14, 7},
    {0x10, 7},
    {0x1c, 8},
    {0xe, 6},
    {0xe, 8},
    {0xc, 8},
    {0x2, 9},
    {0x10, 5},
    {0x18, 8},
    {0x14, 8},
    {0x10, 8},
    {0xe, 5},
    {0xa, 8},
    {0x6, 8},
    {0x6, 9},
    {0x12, 5},
    {0x1a, 8},
    {0x16, 8},
    {0x12, 8},
    {0xd, 5},
    {0x9, 8},
    {0x5, 8},
    {0x5, 9},
    {0xc, 5},
    {0x8, 8},
    {0x4, 8},
    {0x4, 9},
    {0x7, 3},
    {0xa, 5},
    {0x8, 5},
    {0xc, 6}
};

#define MB_INTRA 0x01
#define MB_PAT   0x02
#define MB_BACK  0x04
#define MB_FOR   0x08
#define MB_QUANT 0x10  

static const uint8_t table_mb_ptype[32][2] = {
    { 0, 0 }, // 0x00
    { 3, 5 }, // 0x01 MB_INTRA
    { 1, 2 }, // 0x02 MB_PAT
    { 0, 0 }, // 0x03
    { 0, 0 }, // 0x04
    { 0, 0 }, // 0x05
    { 0, 0 }, // 0x06
    { 0, 0 }, // 0x07
    { 1, 3 }, // 0x08 MB_FOR
    { 0, 0 }, // 0x09
    { 1, 1 }, // 0x0A MB_FOR|MB_PAT
    { 0, 0 }, // 0x0B
    { 0, 0 }, // 0x0C
    { 0, 0 }, // 0x0D
    { 0, 0 }, // 0x0E
    { 0, 0 }, // 0x0F
    { 0, 0 }, // 0x10
    { 1, 6 }, // 0x11 MB_QUANT|MB_INTRA
    { 1, 5 }, // 0x12 MB_QUANT|MB_PAT
    { 0, 0 }, // 0x13
    { 0, 0 }, // 0x14
    { 0, 0 }, // 0x15
    { 0, 0 }, // 0x16
    { 0, 0 }, // 0x17
    { 0, 0 }, // 0x18
    { 0, 0 }, // 0x19
    { 2, 5 }, // 0x1A MB_QUANT|MB_FOR|MB_PAT
    { 0, 0 }, // 0x1B
    { 0, 0 }, // 0x1C
    { 0, 0 }, // 0x1D
    { 0, 0 }, // 0x1E
    { 0, 0 }, // 0x1F
};

static const uint8_t table_mb_btype[32][2] = {
    { 0, 0 }, // 0x00
    { 3, 5 }, // 0x01 MB_INTRA
    { 0, 0 }, // 0x02
    { 0, 0 }, // 0x03
    { 2, 3 }, // 0x04 MB_BACK
    { 0, 0 }, // 0x05
    { 3, 3 }, // 0x06 MB_BACK|MB_PAT
    { 0, 0 }, // 0x07
    { 2, 4 }, // 0x08 MB_FOR
    { 0, 0 }, // 0x09
    { 3, 4 }, // 0x0A MB_FOR|MB_PAT
    { 0, 0 }, // 0x0B
    { 2, 2 }, // 0x0C MB_FOR|MB_BACK
    { 0, 0 }, // 0x0D
    { 3, 2 }, // 0x0E MB_FOR|MB_BACK|MB_PAT
    { 0, 0 }, // 0x0F
    { 0, 0 }, // 0x10
    { 1, 6 }, // 0x11 MB_QUANT|MB_INTRA
    { 0, 0 }, // 0x12
    { 0, 0 }, // 0x13
    { 0, 0 }, // 0x14
    { 0, 0 }, // 0x15
    { 2, 6 }, // 0x16 MB_QUANT|MB_BACK|MB_PAT
    { 0, 0 }, // 0x17
    { 0, 0 }, // 0x18
    { 0, 0 }, // 0x19
    { 3, 6 }, // 0x1A MB_QUANT|MB_FOR|MB_PAT
    { 0, 0 }, // 0x1B
    { 0, 0 }, // 0x1C
    { 0, 0 }, // 0x1D
    { 2, 5 }, // 0x1E MB_QUANT|MB_FOR|MB_BACK|MB_PAT
    { 0, 0 }, // 0x1F
};

static const uint8_t mbMotionVectorTable[17][2] = {
{ 0x1, 1 },
{ 0x1, 2 },
{ 0x1, 3 },
{ 0x1, 4 },
{ 0x3, 6 },
{ 0x5, 7 },
{ 0x4, 7 },
{ 0x3, 7 },
{ 0xb, 9 },
{ 0xa, 9 },
{ 0x9, 9 },
{ 0x11, 10 },
{ 0x10, 10 },
{ 0xf, 10 },
{ 0xe, 10 },
{ 0xd, 10 },
{ 0xc, 10 },
};

static const int frame_rate_tab[9] = {
    0, 
    (int)(23.976 * FRAME_RATE_BASE), 
    (int)(24 * FRAME_RATE_BASE), 
    (int)(25 * FRAME_RATE_BASE), 
    (int)(29.97 * FRAME_RATE_BASE), 
    (int)(30 * FRAME_RATE_BASE), 
    (int)(50 * FRAME_RATE_BASE), 
    (int)(59.94 * FRAME_RATE_BASE), 
    (int)(60 * FRAME_RATE_BASE), 
};

static const uint8_t non_linear_qscale[32] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8,10,12,14,16,18,20,22,
    24,28,32,36,40,44,48,52,
    56,64,72,80,88,96,104,112,
};

uint8_t ff_mpeg1_dc_scale_table[128]={ // MN: mpeg2 really can have such large qscales?
//  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};

static const float mpeg1_aspect[16]={
    0.0000,
    1.0000,
    0.6735,
    0.7031,
    
    0.7615,
    0.8055,
    0.8437,
    0.8935,

    0.9157,
    0.9815,
    1.0255,
    1.0695,

    1.0950,
    1.1575,
    1.2015,
};

static const float mpeg2_aspect[16]={
    0,
    1.0,
    -3.0/4.0,
    -9.0/16.0,
    -1.0/2.21,
};

