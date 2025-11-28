#ifndef utf8RusGFX_h
#define  utf8RusGFX_h
#if 1
char* DspCore::utf8Rus(const char* str, bool uppercase) {
    static char strn[BUFLEN];
    int i = 0, j = 0;
    while (str[i] && j < BUFLEN - 1) {
        // UTF-8: кириллица
        if ((uint8_t)str[i] == 0xD0 && str[i+1]) {
            uint8_t next = (uint8_t)str[i+1];
            if (next >= 0x90 && next <= 0xAF) { // А-Я
                strn[j++] = next + 0x30;
            } else if(next >= 0xB0 && next <= 0xBF) { // а-п
                strn[j++] = next + (uppercase?0x10:0x30);
            } else if (next == 0x81) { // Ё
                strn[j++] = 0xA8;
            }
            i += 2;
            continue;
        } else if ((uint8_t)str[i] == 0xD1 && str[i+1]) {
            uint8_t next = (uint8_t)str[i+1];
            if (next >= 0x80 && next <= 0x8F) { // р-я
                strn[j++] = next + (uppercase?0x50:0x70);
            } else if (next == 0x91) { // ё
                strn[j++] = (uppercase?0xA8:0xB8);
            }
            i += 2;
            continue;
        // fix for coding ISO-8859-1 => win-1251
        } else if ((uint8_t)str[i] == 0xC3 && str[i+1]) {
            uint8_t next = (uint8_t)str[i+1];
            if (next >= 0x80 && next <= 0x9F) { // А-Я
              strn[j++] = next + 0x40;
            } else if (next >= 0xA0 && next <= 0xBF) { // а-я
              strn[j++] = next + (uppercase?0x20:0x40);
            }
            i += 2;
            continue;            
        // fix UTF-8 3 bytes point
        } else if ((uint8_t)str[i] >= 0xE0 && str[i+1] && str[i+2]) {
            strn[j++] = 0x20;
            i += 3;
            continue;            
        } else if ((uint8_t)str[i] <= 0x7F) {
        // Не кириллица — копируем как есть
            char ch = str[i++];
            if ((ch >= 'a' && ch <= 'z') && uppercase) ch = ch - 0x20;
            strn[j++] = ch;
        }
    }
    strn[j] = 0;
    return strn;
} 

#else
char* DspCore::utf8Rus(const char* str, bool uppercase) {
  int index = 0;
  static char strn[BUFLEN];
  bool E = false;
  strlcpy(strn, str, BUFLEN);
  if (uppercase) {
    bool next = false;
    for (char *iter = strn; *iter != '\0'; ++iter)
    {
      if (E) {
        E = false;
        continue;
      }
      uint8_t rus = (uint8_t) * iter;
      if (rus == 208 && (uint8_t) * (iter + 1) == 129) { // ёКостыли
        *iter = (char)209;
        *(iter + 1) = (char)145;
        E = true;
        continue;
      }
      if (rus == 209 && (uint8_t) * (iter + 1) == 145) {
        *iter = (char)209;
        *(iter + 1) = (char)145;
        E = true;
        continue;
      }
      if (next) {
        if (rus >= 128 && rus <= 143) *iter = (char)(rus + 32);
        if (rus >= 176 && rus <= 191) *iter = (char)(rus - 32);
        next = false;
      }
      if (rus == 208) next = true;
      if (rus == 209) {
        *iter = (char)208;
        next = true;
      }
      *iter = toupper(*iter);
    }
  }
  if(L10N_LANGUAGE==EN) return strn;
  while (strn[index])
  {
    if (strn[index] >= 0xBF)
    {
      switch (strn[index]) {
        case 0xD0: {
            if (strn[index + 1] == 0x81) {
              strn[index] = 0xA8;
              break;
            }
            if (strn[index + 1] >= 0x90 && strn[index + 1] <= 0xBF) strn[index] = strn[index + 1] + 0x30;
            break;
          }
        case 0xD1: {
            if (strn[index + 1] == 0x91) {
              //strn[index] = 0xB7;
              strn[index] = 0xB8;
              break;
            }
            if (strn[index + 1] >= 0x80 && strn[index + 1] <= 0x8F) strn[index] = strn[index + 1] + 0x70;
            break;
          }
      }
      int sind = index + 2;
      while (strn[sind]) {
        strn[sind - 1] = strn[sind];
        sind++;
      }
      strn[sind - 1] = 0;
    }
    index++;
  }
  return strn;
}
#endif
#endif
