#include "String.h"
using namespace foxintango;

String::String(){}
String::String(const char* string) {}
String::String(const char& number) {}
String::String(const unsigned char& number) {}
String::String(const short& number) {}
String::String(const unsigned short& number) {}
String::String(const int& number) {}
String::String(const unsigned int& number) {}
String::String(const float& number) {}
String::String(const long& number) {}
String::String(const double& number) {}
String::~String() {}

String::operator char(){ return 0; }
String::operator unsigned char() { return 0; }
String::operator short() { return 0; }
String::operator unsigned short() { return 0; }
String::operator int() { return 0; }
String::operator unsigned int() { return 0; }
String::operator float() { return 0; }
String::operator long() { return 0; }
String::operator double() { return 0; }

bool String::operator == (const char* bytes){
    return false;
}

bool String::operator == (const String& string) {
    return false;
}

const char* String::byte() const { return 0; }
Size String::length() const { return 0; }