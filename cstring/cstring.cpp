#include "cstring.h"

size_t Strlen(const char* str) {
  size_t len = 0;
  while (*(str + len) != '\0') {
    ++len;
  }
  return len;
}
int Strcmp(const char* first, const char* second) {
  while ((*first != '\0') && (*first == *second)) {
    ++first;
    ++second;
  }
  return *first - *second;
}
int Strncmp(const char* first, const char* second, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    if ((*first == *second) && (*first == '\0')) {
      return 0;
    }
    if (*first == '\0') {
      return -1;
    }
    if (*second == '\0') {
      return 1;
    }
    if (*first < *second) {
      return -1;
    }
    if (*first > *second) {
      return 1;
    }
    ++first;
    ++second;
  }
  return 0;
}
char* Strcpy(char* dest, const char* src) {
  char* dest_temp = dest;
  while (*src != '\0') {
    *dest_temp++ = *src++;
  }
  *dest_temp = *src;
  return dest;
}
char* Strncpy(char* dest, const char* src, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    if (*src == '\0') {
      *dest++ = '\0';
    } else {
      *dest++ = *src++;
    }
  }
  return dest - count;
}
char* Strcat(char* dest, const char* src) {
  char* dest_temp = dest + Strlen(dest);
  while (*src != '\0') {
    *dest_temp++ = *src++;
  }
  *dest_temp = '\0';
  return dest;
}
char* Strncat(char* dest, const char* src, size_t count) {
  char* dest_temp = dest + Strlen(dest);
  for (size_t i = 0; i < count; ++i) {
    if (*src == '\0') {
      break;
    }
    *dest_temp++ = *src++;
  }
  *dest_temp = '\0';
  return dest;
}
const char* Strchr(const char* str, char symbol) {
  while (*str != '\0') {
    if (*str == symbol) {
      return str;
    }
    ++str;
  }
  if (*str == symbol) {
    return str;
  }
  return nullptr;
}
const char* Strrchr(const char* str, char symbol) {
  size_t len_str = Strlen(str);
  str = str + len_str;
  for (size_t i = 0; i < len_str + 1; ++i) {
    if (*str == symbol) {
      return str;
    }
    --str;
  }
  return nullptr;
}
size_t Strspn(const char* dest, const char* src) {
  size_t counter = 0;
  while (*dest != '\0') {
    size_t counter_temp = counter;
    const char* src_temp = src;
    while (*src_temp != '\0') {
      if (*dest == *src_temp) {
        counter += 1;
        break;
      }
      ++src_temp;
    }
    if (counter == counter_temp) {
      return counter;
    }
    ++dest;
  }
  return counter;
}
size_t Strcspn(const char* dest, const char* src) {
  size_t counter = 0;
  while (*dest != '\0') {
    size_t counter_temp = counter;
    const char* src_temp = src;
    while (*src_temp != '\0') {
      if (*dest == *src_temp) {
        counter += 1;
        break;
      }
      ++src_temp;
    }
    if (counter > counter_temp) {
      return counter_temp;
    }
    ++dest;
    ++counter;
  }
  return counter;
}
const char* Strpbrk(const char* dest, const char* breakset) {
  while (*dest != '\0') {
    const char* breakset_temp = breakset;
    while (*breakset_temp != '\0') {
      if (*dest == *breakset_temp) {
        return dest;
      }
      ++breakset_temp;
    }
    ++dest;
  }
  return nullptr;
}
const char* Strstr(const char* str, const char* pattern) {
  if (*pattern == '\0') {
    return str;
  }
  while (*str != '\0') {
    if (*str == *pattern) {
      const char* str_temp = str;
      const char* pattern_temp = pattern;
      while (*pattern_temp != '\0') {
        if (*pattern_temp != *str_temp) {
          break;
        }
        ++pattern_temp;
        ++str_temp;
        if (*pattern_temp == '\0') {
          return str;
        }
        if (*str_temp == '\0') {
          return nullptr;
        }
      }
    }
    ++str;
    if (*str == '\0') {
      return nullptr;
    }
  }
  return nullptr;
}