/*
Copyright Frank Bösing, 2017  

  This file is part of Teensy64.

    Teensy64 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Teensy64 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Teensy64.  If not, see <http://www.gnu.org/licenses/>.

    Diese Datei ist Teil von Teensy64.

    Teensy64 ist Freie Software: Sie können es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
    veröffentlichten Version, weiterverbreiten und/oder modifizieren.

    Teensy64 wird in der Hoffnung, dass es nützlich sein wird, aber
    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License für weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
    
*/

//Array values are keyboard-values returned from USB
//LeftShift, RightShift, Control, and Commodore are special codes, represented here with 0xfc..0xff
static const uint8_t ktabUSB[8][8] = {
  {0x2a, 0x28, 0x4f, 0x40, 0x3a, 0x3c, 0x3e, 0x51}, //DEL, Return, Cursor Right, F7, F1, F3, F5, Cursor Down
  {0x20, 0x1a, 0x04, 0x21, 0x1d, 0x16, 0x08, 0xff}, //3, W, A, 4, Z, S, E, LeftShift
  {0x22, 0x15, 0x07, 0x23, 0x06, 0x09, 0x17, 0x1b}, //5, R, D, 6, C, F, T, X
  {0x24, 0x1c, 0x0a, 0x25, 0x05, 0x0B, 0x18, 0x19}, //7, Y, G, 8, B, H, U, V
  {0x26, 0x0c, 0x0D, 0x27, 0x10, 0x0E, 0x12, 0x11}, //9, I, J, 0, M, K, O, N
  {0x57, 0x13, 0x0F, 0x56, 0x37, 0x33, 0x2f, 0x36}, //+(Keypad), P, L, -(Keypad), ",", ":", "@", ","
  {0x49, 0x55, 0x34, 0x4A, 0xfe, 0x32, 0x4b, 0x54}, //Pound(ins), *(Keypad), ";", HOME (Pos1), RightShift, =, UP Arrow (Bild hoch), /(Keypad)
  {0x1e, 0x4e, 0xfd, 0x1f, 0x2c, 0xfc, 0x14, 0x29} //1,LEFT ARROW(Bild runter) , CTRL, 2, Space, Commodore, Q, RUN/STOP(ESC)
};

//Array values are keyboard-values returned from PS2
static const uint8_t ktabPS2[8][8] = {
  {0x66, 0x5a, 0x74, 0x83, 0x05, 0x04, 0x03, 0x72}, //DEL, Return, Cursor Right, F7, F1, F3, F5, Cursor Down
  {0x26, 0x1d, 0x1c, 0x25, 0x1a, 0x1b, 0x24, 0xff}, //3, W, A, 4, Z, S, E, LeftShift
  {0x2e, 0x2d, 0x23, 0x36, 0x21, 0x2b, 0x2c, 0x22}, //5, R, D, 6, C, F, T, X
  {0x3d, 0x35, 0x34, 0x3e, 0x32, 0x33, 0x3c, 0x2a}, //7, Y, G, 8, B, H, U, V
  {0x46, 0x43, 0x3b, 0x45, 0x3a, 0x42, 0x44, 0x31}, //9, I, J, 0, M, K, O, N
  {0x79, 0x4d, 0x4b, 0x7b, 0x41, 0x4c, 0x54, 0x41}, //+(Keypad), P, L, -(Keypad), ",", ":", "@", ","
  {0x70, 0x7c, 0x52, 0x6c, 0xfe, 0x5d, 0x7d, 0x4a}, //Pound(ins), *(Keypad), ";", HOME (Pos1), RightShift, =, UP Arrow (Bild hoch), /(Keypad)
  {0x16, 0x7a, 0xfd, 0x1e, 0x29, 0xfc, 0x15, 0x76} //1,LEFT ARROW(Bild runter) , CTRL, 2, Space, Commodore, Q, RUN/STOP(ESC)
};

uint8_t rows[256];
uint8_t cols[256];

void printTables(void) {
  Serial.println("static const uint8_t keymatrixmap[2][256] = {");
  Serial.println("//Rows:");
  Serial.println("// 0    1     2     3    4     5     6      7     8      9     A     B     C     D     E     F   ");
  Serial.print("{");
  for (int r = 0; r < 16; r++) {
    for (int c = 0; c < 16; c++) {
      Serial.printf("0x%02x%s ", rows[r * 16 + c], ((c < 15) || (r < 15)) ? "," : "},");
    }
    Serial.printf("//0x%02X\n ", r * 16);
  }
  //Serial.println("");
  Serial.println("//Columns:");
  Serial.println("// 0    1     2     3    4     5     6      7     8      9     A     B     C     D     E     F   ");
  Serial.print("{");
  for (int r = 0; r < 16; r++) {
    for (int c = 0; c < 16; c++) {
      Serial.printf("0x%02x%s ", cols[r * 16 + c], (c < 15 || r < 15) ? "," : "}};");
    }
    Serial.printf("//0x%02X\n ", r * 16);
  }
}

void setup() {
  while (!Serial);

  Serial.println();
  Serial.println("//Autogenerated with C64_keyboardtool.ino :");
  Serial.println("#if USBHOST");

  memset(rows, 0, sizeof(rows));
  memset(cols, 0, sizeof(cols));
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      uint8_t ch = ktabUSB[r][c];
      if (ch > 0) {
        cols[ch] = 1 << c;
        rows[ch] = 1 << r;
      }
    }
  }
  printTables();

  Serial.println();
  Serial.println("#elif PS2KEYBOARD");
  memset(rows, 0, sizeof(rows));
  memset(cols, 0, sizeof(cols));
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      uint8_t ch = ktabPS2[r][c];
      if (ch > 0) {
        cols[ch] = 1 << c;
        rows[ch] = 1 << r;
      }
    }
  }
  printTables();
  Serial.println("#endif");
}

void loop() {
  // put your main code here, to run repeatedly:

}