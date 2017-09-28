
// QU-BD silicone bed QWG-104F-3950 thermistor
// (nppc) Table adjusted to reflect chinese thermistor

#define tt_len 			50	// items in tt table

const short TEMPTABLE[][2] PROGMEM = {
  {    1 * OVERSAMPLENR, 769 },   // 938
  {   31 * OVERSAMPLENR, 257 },   // 314
  {   41 * OVERSAMPLENR, 238 },   // 290
  {   51 * OVERSAMPLENR, 223 },   // 272
  {   61 * OVERSAMPLENR, 212 },   // 258
  {   71 * OVERSAMPLENR, 203 },   // 247
  {   81 * OVERSAMPLENR, 194 },   // 237
  {   91 * OVERSAMPLENR, 188 },   // 229
  {  101 * OVERSAMPLENR, 181 },   // 221
  {  111 * OVERSAMPLENR, 176 },   // 215
  {  121 * OVERSAMPLENR, 171 },   // 209
  {  131 * OVERSAMPLENR, 167 },   // 204
  {  141 * OVERSAMPLENR, 163 },   // 199
  {  151 * OVERSAMPLENR, 160 },   // 195
  {  161 * OVERSAMPLENR, 156 },   // 190
  {  171 * OVERSAMPLENR, 153 },   // 187
  {  181 * OVERSAMPLENR, 150 },   // 183
  {  191 * OVERSAMPLENR, 147 },   // 179
  {  201 * OVERSAMPLENR, 143 },   // 176
  {  221 * OVERSAMPLENR, 139 },   // 170
  {  241 * OVERSAMPLENR, 135 },   // 165
  {  261 * OVERSAMPLENR, 132 },   // 160
  {  281 * OVERSAMPLENR, 128 },   // 155
  {  301 * OVERSAMPLENR, 125 },   // 150
  {  331 * OVERSAMPLENR, 122 },   // 144
  {  361 * OVERSAMPLENR, 119 },   // 139
  {  391 * OVERSAMPLENR, 115 },   // 133
  {  421 * OVERSAMPLENR, 111 },   // 128
  {  451 * OVERSAMPLENR, 107 },   // 123
  {  491 * OVERSAMPLENR, 103 },   // 117
  {  531 * OVERSAMPLENR,  99 },   // 111
  {  571 * OVERSAMPLENR,  94 },   // 105
  {  611 * OVERSAMPLENR,  89 },   // 100
  {  641 * OVERSAMPLENR,  85 },   //  95
  {  681 * OVERSAMPLENR,  80 },   //  90
  {  711 * OVERSAMPLENR,  76 },   //  85
  {  751 * OVERSAMPLENR,  70 },   //  79
  {  791 * OVERSAMPLENR,  65 },   //  72
  {  811 * OVERSAMPLENR,  61 },   //  69
  {  831 * OVERSAMPLENR,  57 },   //  65
  {  871 * OVERSAMPLENR,  52 },   //  57
  {  881 * OVERSAMPLENR,  49 },   //  55
  {  901 * OVERSAMPLENR,  45 },   //  51
  {  921 * OVERSAMPLENR,  40 },   //  45
  {  941 * OVERSAMPLENR,  33 },   //  39
  {  971 * OVERSAMPLENR,  27 },   //  28
  {  981 * OVERSAMPLENR,  21 },   //  23
  {  991 * OVERSAMPLENR,  15 },   //  17
  { 1001 * OVERSAMPLENR,   8 },   //   9
  { 1021 * OVERSAMPLENR, -24 }    // -27
};