#ifndef CPPSTRING_
#define CPPSTRING_

#include <cstddef>
#include <iostream>
#include <stdexcept>

class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
 private:
  size_t size_;
  size_t capacity_;
  char* string_;

 public:
  String();
  String(const size_t, const char);
  String(const char*);  // NOLINT
  String(const char*, size_t);
  String(const String& other);
  String(String&&) noexcept;
  String& operator=(String&&) noexcept;
  String& operator=(const String& other);
  ~String();
  char& operator[](size_t index);
  const char& operator[](size_t index) const;
  char& At(size_t index);
  const char& At(size_t index) const;
  char& Front();
  const char& Front() const;
  char& Back();
  const char& Back() const;
  char* CStr();
  char* Data();
  const char* CStr() const;
  const char* Data() const;
  bool Empty() const;
  size_t Size() const;
  size_t Length() const;
  size_t Capacity() const;
  void Clear();
  void Swap(String& other);
  void PopBack();
  void PushBack(const char symbol);
  String& operator+=(const String& other);
  void Resize(size_t new_size, char symbol);
  void Reserve(size_t new_capacity);
  void ShrinkToFit();
  friend String operator+(const String&, const String&);
  friend std::ostream& operator<<(std::ostream&, const String&);
  friend bool operator<(const String&, const String&);
};

bool operator==(const String&, const String&);
bool operator!=(const String&, const String&);
bool operator>(const String&, const String&);
bool operator>=(const String&, const String&);
bool operator<=(const String&, const String&);
#endif