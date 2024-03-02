#include "cppstring.h"

String::String() : size_(0), capacity_(0), string_(nullptr) {
}

void TransferArray(char* array1, const char* array2, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    array1[i] = array2[i];
  }
}

String::String(const char* string, size_t size) : size_(size), capacity_(size), string_(nullptr) {
  if (!Empty()) {
    string_ = new char[capacity_ + 1];
    TransferArray(string_, string, size);
    string_[size_] = '\0';
  }
}

void String::Reserve(size_t new_capacity) {
  if (capacity_ < new_capacity) {
    capacity_ = new_capacity;
    auto string_temp = new char[capacity_ + 1];
    TransferArray(string_temp, string_, size_);
    string_temp[size_] = '\0';
    delete[] string_;
    string_ = string_temp;
  }
}

String::String(const size_t size, const char symbol) : size_(size), capacity_(size), string_(nullptr) {
  if (!Empty()) {
    string_ = new char[capacity_ + 1];
    for (size_t i = 0; i < size; ++i) {
      string_[i] = symbol;
    }
    string_[size_] = '\0';
  }
}

String::String(const char* string) : size_(0), capacity_(0) {
  if (string[0] == '\0') {
    string_ = nullptr;
  } else {
    capacity_ = 1;
    string_ = new char[capacity_ + 1];
    while (string[size_] != '\0') {
      if (size_ < capacity_) {
        string_[size_] = string[size_];
      } else {
        Reserve(2 * capacity_);
        string_[size_] = string[size_];
      }
      ++size_;
    }
    string_[size_] = '\0';
  }
}

String::~String() {
  delete[] string_;
}

String::String(const String& other) : size_(other.size_), capacity_(other.capacity_), string_(other.string_) {
  if (capacity_ != 0) {
    string_ = new char[capacity_ + 1];
    TransferArray(string_, other.string_, size_ + 1);
  }
}

String& String::operator=(const String& other) {
  if (&other != this) {
    delete[] string_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (capacity_ == 0) {
      string_ = nullptr;
    } else {
      string_ = new char[capacity_ + 1];
      TransferArray(string_, other.string_, size_ + 1);
    }
  }
  return *this;
}

String::String(String&& other) noexcept : size_(other.size_), capacity_(other.capacity_), string_(other.string_) {
  other.string_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

String& String::operator=(String&& other) noexcept {
  if (&other != this) {
    delete[] string_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    string_ = other.string_;
    other.string_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

char& String::operator[](size_t index) {
  return string_[index];
}

const char& String::operator[](size_t index) const {
  return string_[index];
}

char& String::At(size_t index) {
  if (index >= size_) {
    throw StringOutOfRange{};
  }
  return string_[index];
}

const char& String::At(size_t index) const {
  if (index >= size_) {
    throw StringOutOfRange{};
  }
  return string_[index];
}

char& String::Front() {
  return string_[0];
}

const char& String::Front() const {
  return string_[0];
}

char& String::Back() {
  return string_[size_ - 1];
}

const char& String::Back() const {
  return string_[size_ - 1];
}

const char* String::CStr() const {
  return string_;
}

char* String::Data() {
  return string_;
}

char* String::CStr() {
  return string_;
}

const char* String::Data() const {
  return string_;
}

bool String::Empty() const {
  return (size_ == 0);
}

size_t String::Size() const {
  return size_;
}

size_t String::Length() const {
  return size_;
}

size_t String::Capacity() const {
  return capacity_;
}

void String::Clear() {
  if (!Empty()) {
    size_ = 0;
    string_[0] = '\0';
  }
}

void String::Swap(String& other) {
  String temp = std::move(*this);
  *this = std::move(other);
  other = std::move(temp);
}

void String::PopBack() {
  if (!Empty()) {
    --size_;
    string_[size_] = '\0';
  }
}

void String::PushBack(const char symbol) {
  if (capacity_ == 0) {
    capacity_ = 1;
    string_ = new char[capacity_ + 1];
  }
  if (size_ == capacity_) {
    Reserve(2 * capacity_);
    string_[size_++] = symbol;
  } else {
    string_[size_++] = symbol;
  }
  string_[size_] = '\0';
}

String& String::operator+=(const String& other) {
  if (!other.Empty()) {
    if (other.size_ + size_ > capacity_) {
      Reserve(2 * (other.size_ + size_));
    }
    size_t prev_size = size_;
    size_ += other.size_;
    TransferArray(string_ + prev_size, other.string_, other.size_ + 1);
  }
  return *this;
}

void String::Resize(size_t new_size, char symbol) {
  if (new_size > capacity_) {
    Reserve(new_size);
  }
  if (size_ < new_size) {
    for (size_t i = size_; i < new_size; ++i) {
      string_[i] = symbol;
    }
    string_[new_size] = '\0';
    size_ = new_size;
  }
  if ((size_ > new_size) && (capacity_ != 0)) {
    size_ = new_size;
    string_[size_] = '\0';
  }
}

void String::ShrinkToFit() {
  if (Empty()) {
    delete[] string_;
    capacity_ = 0;
    string_ = nullptr;
  } else {
    capacity_ = size_;
    auto string_temp = new char[capacity_ + 1];
    TransferArray(string_temp, string_, size_);
    delete[] string_;
    string_ = string_temp;
    string_[size_] = '\0';
  }
}

String operator+(const String& string1, const String& string2) {
  String result = string1;
  result += string2;
  return result;
}

bool operator<(const String& string1, const String& string2) {
  size_t min_size = std::min(string1.size_, string2.size_);
  for (size_t i = 0; i < min_size; ++i) {
    if (string1[i] != string2[i]) {
      return string1[i] < string2[i];
    }
  }
  return string1.Size() < string2.Size();
}

bool operator>=(const String& string1, const String& string2) {
  return !(string1 < string2);
}

bool operator==(const String& string1, const String& string2) {
  return !(string1 < string2) && !(string2 < string1);
}

bool operator<=(const String& string1, const String& string2) {
  return !(string1 > string2);
}

bool operator>(const String& string1, const String& string2) {
  return string2 < string1;
}

bool operator!=(const String& string1, const String& string2) {
  return !(string1 == string2);
}

std::ostream& operator<<(std::ostream& os, const String& string) {
  if (string.string_ != nullptr) {
    os << string.string_;
  }
  return os;
}