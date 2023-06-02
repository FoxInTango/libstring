#include "String.h"
using namespace foxintango;
#include <codecvt>
#include <string>
#include <iostream>

/**  UTF-8, a transformation format of ISO 10646
 * https://www.ietf.org/rfc/rfc3629.txt  
 */

/**
 * 赋值运算符 与 拷贝运算符
 */
template <typename T>
inline Size string_length() { return 0; }
template <typename T>
inline Size string_copy(){
    return 0;
}

template <typename T>
inline Size string_compare(){ return 0; }

/** UNICODE -- UTF8
 *  0000 0000-0000 007F    0xxxxxxx
 *  0000 0080-0000 07FF    110xxxxx 10xxxxxx
 *  0000 0800-0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
 *  0001 0000-0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 */


inline Size utf_8_16(){ return 0; }
inline Size utf_8_32(){ return 0; }
inline Size utf_16_8(){ return 0; }
inline Size utf_32_8(){ return 0; }

inline Size utf8_length(const char* content){
    Index index = 0;
    while(content[index] != 0){
        char prefix = ( content[index] & 0b11110000) < 0b11110000 ? 
                      ((content[index] & 0b11100000) < 0b11100000 ? 
                      ((content[index] & 0b11000000) < 0b11000000 ? 0b00000000:0b11000000):0b11100000) : 0b11110000;

        switch(prefix){
        case 0b00000000:{ index += 1; }break;
        case 0b11000000:{ index += 2; }break;
        case 0b11100000:{ index += 3; }break;
        case 0b11110000:{ index += 4; }break;
        default:break;
        }
    }

    return index;
}

#define UNICODE_SIZE sizeof(wchar_t)

String::String(){
    content = 0;
}
String::String(const char* string) {
    Size l = strlen(string);
    content = new Unicode[l + 1];

    if(content){
        std::wstring ws = std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.from_bytes(string);
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const char& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if(unicode){
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const unsigned char& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if (unicode) {
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const short& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if (unicode) {
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const unsigned short& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if (unicode) {
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const int& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if (unicode) {
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const unsigned int& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if (unicode) {
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const float& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if (unicode) {
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const long& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if (unicode) {
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::String(const double& number) {
    std::wstring ws = std::to_wstring(number);
    Size l = ws.length();
    content = new Unicode[l + 1];

    if (unicode) {
        wcscpy_s<sizeof(wchar_t)>(content, ws.c_str);
        content[l] = 0;
    }
}
String::~String() {
    if(content) delete[] content;
}

String::operator char(){ 
    return 0; 
}
String::operator unsigned char() { return 0; }
String::operator short() { return 0; }
String::operator unsigned short() { return 0; }
String::operator int() { return 0; }
String::operator unsigned int() { return 0; }
String::operator float() { return 0; }
String::operator long() { return 0; }
String::operator double() { return 0; }

bool String::operator == (const char* bytes){
    if(bytes && content){
        std::wstring ws = std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.from_bytes(string);
        return !std::wcscmp(content,ws.c_str()) ? true : false;
    }
    return false;
}

bool String::operator == (const String& string) {
    if(content && string.content){
        return !std::wcscmp(content, string.content) ? true : false;
    }
    return false;
}

String& String::operator += (const String& s);

Size String::as(char** string){
    std::string s = std::wstring_convert<std::codecvt_utf8<char>>{}.to_bytes(content);
    Size l = s.length();

    string = new char[l + 1];
    std::strcpy(string,s.c_str());
    string[l] = 0;
    return l;
}


String& String::contact(const String& s){
    Size tl = this->length();
    Size sl = s.length();

    Unicode* unicode = new Unicode[tl + sl + 1];

    if(unicode){
        std::wcscpy(unicode,content);
        std::wcscpy(&unicode[tl],s.content);
    }

    return *this;
}

Size String::split(Array<String>& array,char* spliter,const int& max){
    
    std::wstring ws = std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.from_bytes(spliter);
    Size l   = this->length();
    Index c  = 0;
    Index i  = 0;
    Index ls = -1;
    while(c <= max && i + ws.length() < l){
        if(0 == std::memcmp(&content[i],ws.c_str(),ws.length())){
            String s;
            s.content = new Unicode[i - ls + 1 + 1];
            if(s.content){
                std::memcpy(s.content,&content[ls + 1],i - ls);
                array.append(s);
            }
            ls = i;
            c ++;
        }

        i ++;
    }
}

const Unicode* String::byte() const { return this->content; }

Size String::length() const { 
    if(content){
        return std::wcslen(content);
    }
    return 0; 
}