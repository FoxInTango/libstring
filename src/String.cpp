#include "String.h"
using namespace foxintango;
/**  UTF-8, a transformation format of ISO 10646
 * https://www.ietf.org/rfc/rfc3629.txt  
 */

/**
 * 赋值运算符 与 拷贝运算符
 * 
 * 数值的科学计数法
 */
template <typename T>
Size string_length(T* s) {
    Size l = 0;
    while(s[l]){
        l ++;
    }

    return l;
}
template <typename T>
Size string_copy(T* t,T* s,const Size& length = 0){
    if(!t || !s) return 0;
    Index index = 0;
    Size l = length > 0 ? length : UNSIGNED_LONG_MAX;
    while(index < length && s[index] != 0){
        t[index] = s[index];
    }
    return 0;
}

template <typename T>
bool string_compare(T* t, T* s, const Size& length = 0){
    Index index = 0;
    while(s[index] && t[index] && index < length){
        if(s[index] != t[index]) return false;
    }

    return true;
}

Size unicode_swap_endian(Unicode* unicode){
    return 0;
}

/** UNICODE -- UTF8
 *  0000 0000-0000 007F    0xxxxxxx
 *  0000 0080-0000 07FF    110xxxxx 10xxxxxx
 *  0000 0800-0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
 *  0001 0000-0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 */

inline Size utf8_length_to_unicode(const char* content) {
    Index index = 0;
    while (content[index] != 0) {
        char prefix = (content[index] & 0b11110000) < 0b11110000 ?
                     ((content[index] & 0b11100000) < 0b11100000 ?
                     ((content[index] & 0b11000000) < 0b11000000 ? 0b00000000 : 0b11000000) : 0b11100000) : 0b11110000;

        switch (prefix) {
        case 0b00000000: { index += 1; }break;
        case 0b11000000: { index += 2; }break;
        case 0b11100000: { index += 3; }break;
        case 0b11110000: { index += 4; }break;
        default:break;
        }
    }

    return index;
}

inline Size utf8_length_from_unicode(const Unicode* content){
    Index index  = 0;
    Size  length = 0;
    while (content[index])
    {
        Index segment = content[index] < 0x0000007F ? 1 : (content[index] < 0x000007FF ? 2 : (content[index] < 0x0000FFFF ? 3 : 4));

        switch (segment) {
        case 1: { length += 1; }break;//0000 0000 - 0000 007F    0xxxxxxx
        case 2: { length += 2; }break;// 0000 0080 - 0000 07FF    110xxxxx 10xxxxxx
        case 3: { length += 3; }break;// 0000 0800 - 0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
        case 4: { length += 4; }break;// 0001 0000 - 0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        default:break;
        }

        index ++;
    }

    return length;
}
// little endian
inline Size utf_8_32(const char* utf8,Unicode** utf32){
    Index index8  = 0;
    Index index32 = 0;

    Size length = utf8_length_to_unicode(utf8);
    
    (*utf32) = new Unicode[length + 1];
    (*utf32)[length] = 0;
    
    while(utf8[index8]){
        char prefix =  (utf8[index8] & 0b11110000) < 0b11110000 ?
                      ((utf8[index8] & 0b11100000) < 0b11100000 ?
                      ((utf8[index8] & 0b11000000) < 0b11000000 ? 0b00000000 : 0b11000000) : 0b11100000) : 0b11110000;
        prefix = 0b00000000;
        Unicode u1 = 0,u2 = 0,u3 = 0,u4 = 0;
        switch(prefix){
        case 0b00000000: { 
            u1 = static_cast<Unicode>(utf8[index8]); //0000 0000 - 0000 007F    0xxxxxxx
            (*utf32)[index32] =  u1; 
            index8 += 1;}break;
        case 0b11000000: { 
            u1 = static_cast<Unicode>(utf8[index8]);
            u2 = static_cast<Unicode>(utf8[index8 + 1]);// 0000 0080-0000 07FF    110xxxxx 10xxxxxx
            (*utf32)[index32] = ((u1 << 2) >> 2  | ( u2 << 6 )) | ( ((u2 << 3) >> 3) << 8 );
            index8 += 2; }break;
        case 0b11100000: { 
            u1 = static_cast<Unicode>(utf8[index8]);
            u2 = static_cast<Unicode>(utf8[index8 + 1]);
            u3 = static_cast<Unicode>(utf8[index8 + 2]);//0000 0800-0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
            (*utf32)[index32] = ((u1 << 2) >> 2 | (u2 << 6)) | ((((u2 << 2) >> 2) | (u3 << 6) ) << 8) | (((u3 << 4) >> 4) << 16);
            index8 += 3; }break;
        case 0b11110000: { 
            u1 = static_cast<Unicode>(utf8[index8]);
            u2 = static_cast<Unicode>(utf8[index8 + 1]);
            u3 = static_cast<Unicode>(utf8[index8 + 2]);
            u4 = static_cast<Unicode>(utf8[index8 + 3]); //0001 0000 - 0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            (*utf32)[index32] = ((u1 << 2) >> 2 | (u2 << 6)) | ((((u2 << 2) >> 2) | (u3 << 6)) << 8) | ((((u3 << 2) >> 2) | (u3 << 6)) << 16) | (((u4 << 5) >> 5) << 24);
            index8 += 4; }break;
        default:break;
        }

        index32 += 1;
    }

    return length;
}

inline Size utf_32_8(const Unicode* utf32,char** utf8){
    Size l = utf8_length_from_unicode(utf32);
    (*utf8) = new char[l + 1];
    (*utf8)[l] = 0;
    Index index8  = 0;
    Index index32 = 0;
    while(utf32[index32]){
        Index segment = utf32[index32]  < 0x0000007F ? 1 : ( utf32[index32] < 0x000007FF ? 2 : (utf32[index32] < 0x0000FFFF ? 3 : 4));
        char* p = (char*)&utf32[index32];
        p[0] = p[1] = p[2] = p[3] = 0;
        char c1 = p[0];
        char c2 = p[1];
        char c3 = p[2];
        char c4 = p[3];
        switch(segment){
        case 1:{
            (*utf8)[index8] = c1;index8 += 1;
        }break;//0000 0000 - 0000 007F    0xxxxxxx
        case 2:{
            (*utf8)[index8]     = ((c1 << 2) >> 2) | 0b10000000;
            (*utf8)[index8 + 1] = ((( c2 << 3) >> 3) << 2) | (c1 >> 6) | 0b11000000;
            index8 += 2;
        }break;// 0000 0080 - 0000 07FF    110xxxxx 10xxxxxx
        case 3:{
            (*utf8)[index8]     = ((c1 << 2) >> 2) | 0b10000000;
            (*utf8)[index8 + 1] = (((c2 << 2) >> 2) << 2) | (c1 >> 6) | 0b10000000;
            (*utf8)[index8 + 2] = (((c3 << 4) >> 4) << 2) | (c2 >> 6) | 0b11100000;
            index8 += 3;
        }break;// 0000 0800 - 0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
        case 4:{
            (*utf8)[index8] = ((c1 << 2) >> 2) | 0b10000000;
            (*utf8)[index8 + 1] = (((c2 << 2) >> 2) << 2) | (c1 >> 6) | 0b10000000;
            (*utf8)[index8 + 2] = (((c3 << 2) >> 2) << 2) | (c2 >> 6) | 0b10000000;
            (*utf8)[index8 + 3] = (((c4 << 5) >> 5) << 2) | (c3 >> 6) | 0b11110000; 
        }break;// 0001 0000 - 0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        default:break;
        }

        index32 ++;
    }

    return l;
}

#define UNICODE_SIZE sizeof(wchar_t)

String::String(){
    content = 0;
    mLength = 0;
}
String::String(const char* string) {
    this->content = 0;
    this->mLength = utf_8_32(string,&this->content);
}
String::String(const char& number) {
    
}
String::String(const unsigned char& number) {
    
}
String::String(const short& number) {
    
}
String::String(const unsigned short& number) {
    
}
String::String(const int& number) {
    
}
String::String(const unsigned int& number) {
    
}
String::String(const float& number) {
    
}
String::String(const long& number) {
    
}
String::String(const double& number) {
    
}
String::~String() {
    if(content) delete[] content;
}

String::String(const String& string){
    Size l = string.length();
    this->content = new Unicode[l + 1];
    if(this->content){
        string_copy<Unicode>(this->content,string.content);
        this->content[l] = 0;
        this->mLength = l;
    }
}

String& String::operator = (const String& string){
    Size l = string.length();
    this->content = new Unicode[l + 1];
    if (this->content) {
        string_copy<Unicode>(this->content, string.content);
        this->content[l] = 0;
        this->mLength = l;
    }

    return *this;
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
    Unicode* unicode;

    utf_8_32(bytes,&unicode);

    if(unicode){
        return string_compare<Unicode>(this->content,unicode);
    }

    return false;
}

bool String::operator == (const String& string) {
    return string_compare<Unicode>(content,string.content);
}

String& String::operator += (const String& s){
    if(!this->content || !s.content) return *this;
    Size l = this->length() + s.length();
    Unicode* unicode = new Unicode[l + 1];
    if(unicode){
        unicode[l] = 0;

        string_copy<Unicode>(unicode,this->content);
        string_copy<Unicode>(&unicode[this->length()],s.content);

        delete[] this->content;
        this->content = unicode;
        this->mLength = l;
    }
    return *this;
}

Size String::as(char** string) const {
    return content ? utf_32_8(this->content,string) : 0;
}


String& String::contact(const String& s){
    Size tl = this->length();
    Size sl = s.length();

    Unicode* unicode = new Unicode[tl + sl + 1];

    if(unicode){
        string_copy<Unicode>(unicode, this->content);
        string_copy<Unicode>(&unicode[this->length()], s.content);
        delete[] this->content;
        this->content = unicode;
        this->mLength = tl + sl;
    }

    return *this;
}

Size String::split(Array<String>& array,char* spliter,const int& max){
    String ws(spliter);
    Size l   = this->length();
    Index c  = 0;
    Index i  = 0;
    Index ls = -1;
    while(c <= max && i + ws.length() < l){
        if(string_compare<Unicode>(&content[i],ws.content,ws.length())){
            String s;
            s.content = new Unicode[i - ls + 1 + 1];
            if(s.content){
                string_copy(s.content,&content[ls + 1],i - ls);
                s.mLength = i - ls + 1;
                array.append(s);
            }
            ls = i;
            c ++;
        }
        i ++;
    }
}

const Unicode* String::unicode() const { return this->content; }

Size String::length() const { 
    return mLength;
}