#include "String.h"
using namespace foxintango;

/** UNICODE -- UTF8
 *  0000 0000-0000 007F    0xxxxxxx
 *  0000 0080-0000 07FF    110xxxxx 10xxxxxx
 *  0000 0800-0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
 *  0001 0000-0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 11110xxx ** 10xxxxxx 10xxxxxx 10xxxxxx ** 1110xxxx 10xxxxxx 10xxxxxx
 */

 /**  UTF8 编码排序方式 差异
        endian byte 1 : 10000000
        endian byte 2 : 11000000
        endian byte 3 : 11100000
        endian byte 4 : 11110000
        unsigned int endian = 0b11110000111000001100000010000000;
        unsigned char* ep = (unsigned char*)&endian;
        std::cout << " endian byte 1 : " << std::bitset<8>(ep[0]) << std::endl;
        std::cout << " endian byte 2 : " << std::bitset<8>(ep[1]) << std::endl;
        std::cout << " endian byte 3 : " << std::bitset<8>(ep[2]) << std::endl;
        std::cout << " endian byte 4 : " << std::bitset<8>(ep[3]) << std::endl;
        unsigned char p = 255;
        char utf[] = "这是一个悲伤的故事";

        for (int i = 0; i < sizeof(utf); i++) {
            std::cout << " code " << i << " : " << std::bitset<8>(utf[i]) << std::endl;
        }
*/

inline void utf8_prefix(const unsigned char* content, unsigned char& prefix) {
    Index o = 0;
    while ((content[o] < 0b11000000) && content[o] && o < 4) {
        if (content[o] < 0b10000000) { break; }
        o++;
    }

    switch (o) {
    case 0: { prefix = 0b00000000; }
    case 1: { if (content[o] >= 0b11000000 && content[o] < 0b11100000) prefix = 0b11000000; }break;
    case 2: { if (content[o] >= 0b11100000 && content[o] < 0b11110000) prefix = 0b11100000; }break;
    case 3: { if (content[o] >= 0b11110000) prefix = 0b11110000; }break;
    default: {prefix = 0b11110001; }break;//报错
    }
}
inline Size utf8_length_from_unicode(const Unicode* content){
    Index index = 0;
    while(content[index]){
        if(content[index] < 0b10000000){
            index ++;
        }else if(content[index] < 0b11100000){
            index += 2;
        }else if(content[index] < 0b11110000){
            index += 3;
        }else { index += 4; }
    }

    return index;
}
inline Size utf8_length_to_unicode(const unsigned char* content){
    Index index = 0;
    Size  length = 0;
    while (content[index])
    {
        Index segment = content[index] < 0b11000000 ? 1 : (content[index] < 0b11100000 ? 2 : (content[index] < 0b11110000 ? 3 : 4));

        switch (segment) {
        case 1: { index += 1; }break;// 0000 0000 - 0000 007F    0xxxxxxx
        case 2: { index += 2; }break;// 0000 0080 - 0000 07FF    110xxxxx 10xxxxxx
        case 3: { index += 3; }break;// 0000 0800 - 0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
        case 4: { index += 4; }break;// 0001 0000 - 0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        default:break;
        }

        length++;
    }

    return length;
}

/** 编码规律
    char     utf8[] = "这是一个悲伤的故事";
    wchar_t utf32[] = L"这是一个悲伤的故事";
    utf8_length((unsigned char*)utf8);
    for(int i = 0;i < 9;i ++){
        std::cout << " utf8  " << i << " : " << std::bitset<8>(utf8[i * 3]) << std::bitset<8>(utf8[i * 3 + 1]) << std::bitset<8>(utf8[i * 3 + 2]) << std::endl;
        std::cout << " utf32 " << i << " : " << std::bitset<32>(utf32[i]) << std::endl;
    }

utf8  0 : 111010001011111110011001 
utf32 0 : 00000000000000001000111111011001
utf8  1 : 111001101001100010101111
utf32 1 : 00000000000000000110011000101111
utf8  2 : 111001001011100010000000
utf32 2 : 00000000000000000100111000000000
utf8  3 : 111001001011100010101010
utf32 3 : 00000000000000000100111000101010
utf8  4 : 111001101000001010110010
utf32 4 : 00000000000000000110000010110010
utf8  5 : 111001001011110010100100
utf32 5 : 00000000000000000100111100100100
utf8  6 : 111001111001101010000100
utf32 6 : 00000000000000000111011010000100
utf8  7 : 111001101001010110000101
utf32 7 : 00000000000000000110010101000101
utf8  8 : 111001001011101010001011
utf32 8 : 00000000000000000100111010001011
for (int i = 0; i < s.length(); i++) {
        std::cout << " utf8   " << i << " : " << std::bitset<8>(utf8[i * 3]) << std::bitset<8>(utf8[i * 3 + 1]) << std::bitset<8>(utf8[i * 3 + 2]) << std::endl;
        std::cout << " utf32  " << i << " : " << std::bitset<32>(utf32[i]) << std::endl;
        std::cout << " unicode" << i << " : " << std::bitset<32>(s.unicode()[i]) << std::endl;
    }
移位 -- 与字面直观表示相同
    unsigned char m = 0b111110000;
    std::cout << "原位:" << std::bitset<8>(m) << std::endl;
    std::cout << "左移:" << std::bitset<8>(m << 1) << std::endl;
    std::cout << "右移:" << std::bitset<8>(m >> 1) << std::endl;
 */
inline Size utf_8_32(const unsigned char* utf8, Unicode** utf32){
    Index index8 = 0;
    Index index32 = 0;

    Size length = utf8_length_to_unicode(utf8);

    (*utf32) = new Unicode[length + 1];
    (*utf32)[length] = 0;
    
    while (utf8[index8]) {
        unsigned char* unicode = (unsigned char*)&((*utf32)[index32]);
        if (utf8[index8] < 0b10000000) {
            unicode[0] =  utf8[index8];
            unicode[1] = 0;
            unicode[2] = 0;
            unicode[3] = 0;
            index8++;
        }
        else if (utf8[index8] < 0b11100000) { // 110xxxxx 10xxxxxx
            unicode[0] = (utf8[index8 + 1] & 0b00111111) | (utf8[index8]<< 6);
            unicode[1] = ((utf8[index8] & 0b00011100) >> 2);
            unicode[2] = 0;
            unicode[3] = 0;
            index8 += 2; 
        }
        else if (utf8[index8] < 0b11110000) { // 1110xxxx 10xxxxxx 10xxxxxx
            unicode[0] =  (utf8[index8 + 2] & 0b00111111) | (utf8[index8 + 1] << 6);
            unicode[1] = ((utf8[index8 + 1] & 0b00111111 ) >> 2) | (utf8[index8] << 4);
            unicode[2] = 0;
            unicode[3] = 0;
            index8 += 3;
        }
        else { // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            unicode[0] =  (utf8[index8 + 3] & 0b00111111) | (utf8[index8 + 2] << 6);
            unicode[1] = ((utf8[index8 + 2] & 0b00111111) >> 2) | (utf8[index8 + 1] << 4);
            unicode[2] = ((utf8[index8 + 1] & 0b00111111) >> 4) | ((utf8[index8] & 0b00000111) << 2);
            unicode[3] = 0;
            index8 += 4;
         }

        index32 ++;
    }

    return length;
}
inline Size utf_32_8(const Unicode* utf32, unsigned char** utf8){
    Size length = utf8_length_from_unicode(utf32);
    (*utf8) = new unsigned char[length + 1];
    (*utf8)[length] = 0;
    Index index8 = 0;
    Index index32 = 0;
    
    while(utf32[index32]){
        unsigned char* unicode = (unsigned char*)&utf32[index32];
        unsigned char prefix = utf32[index32] < 0x0000007F ? 0b00000000 : (utf32[index32] < 0x000007FF ? 0b11000000 : (utf32[index32] < 0x0000FFFF ? 0b11100000 :0b11110000));
        switch (prefix) {
        case 0b00000000: {// 0000 0000 - 0000 007F    0xxxxxxx
            (*utf8)[index8] = unicode[0];
            index8 += 1;
            }break;
        case 0b11000000: {// 0000 0080 - 0000 07FF    110xxxxx 10xxxxxx
            (*utf8)[index8]     = (unicode[0] >> 6) | ((unicode[1] & 0b00000111) << 2) | 0b11000000;
            (*utf8)[index8 + 1] = (unicode[0] & 0b00111111) | 0b10000000;
            index8 += 2;
            }break;
        case 0b11100000: {// 0000 0800 - 0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
            (*utf8)[index8]     =  (unicode[1] >> 4) | 0b11100000;
            (*utf8)[index8 + 1] = ((unicode[0] & 0b11000000) >> 6) | ((unicode[1] & 0b00001111) << 2) | 0b10000000;
            (*utf8)[index8 + 2] =  (unicode[0] & 0b00111111) | 0b10000000;
            index8 += 3;
            }break;
        case 0b11110000: {// 0001 0000 - 0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            (*utf8)[index8] = ((unicode[1] & 0b00011100) >> 2) | 0b11110000;
            (*utf8)[index8 + 1] = ((unicode[2] & 0b11110000) >> 4) | ((unicode[1] & 0b00000011) << 6) | 0b10000000;
            (*utf8)[index8 + 2] = ((unicode[3] & 0b11000000) >> 6) | ((unicode[2] & 0b00001111) << 2) | 0b10000000;
            (*utf8)[index8 + 3] =  (unicode[3] & 0b00111111)|0b10000000;
            index8 += 4;
            }break;
        default: {}break;
        }

        index32 += 1;
    }

}
/*
inline Size utf8_length_to_unicode(const unsigned char* content) {
    Index index = 0;
    Size length = 0;
    Error error = 0;
    while (content[index] && !error) {
        unsigned char  prefix = (content[index] & 0b11110000) < 0b11110000 ?
                               ((content[index] & 0b11100000) < 0b11100000 ?
                               ((content[index] & 0b11000000) < 0b11000000 ? 0b00000000 : 0b11000000) : 0b11100000) : 0b11110000;
        utf8_prefix(&content[index],prefix);
        switch (prefix) {
        case 0b00000000: { index += 1; }break;
        case 0b11000000: { index += 2; }break;
        case 0b11100000: { index += 3; }break;
        case 0b11110000: { index += 4; }break;
        default:{error = 1;}break;
        }

        length += 1;
    }

    return length;
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
inline Size utf_8_32(const unsigned char* utf8,Unicode** utf32){
    Index index8  = 0;
    Index index32 = 0;

    Size length = utf8_length_to_unicode(utf8);
    
    (*utf32) = new Unicode[length + 1];
    (*utf32)[length] = 0;
    Error error = 0;
    while(utf8[index8] && !error){
        unsigned char prefix = (utf8[index8] & 0b11110000) < 0b11110000 ?
                               ((utf8[index8] & 0b11100000) < 0b11100000 ?
                               ((utf8[index8] & 0b11000000) < 0b11000000 ? 0b00000000 : 0b11000000) : 0b11100000) : 0b11110000;
        utf8_prefix(&utf8[index8], prefix);
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
        default:{error = 1;}break;
        }

        index32 += 1;
    }

    return length;
}

inline Size utf_32_8(const Unicode* utf32, unsigned char** utf8){
    Size l = utf8_length_from_unicode(utf32);
    (*utf8) = new unsigned char[l + 1];
    (*utf8)[l] = 0;
    Index index8  = 0;
    Index index32 = 0;
    while(utf32[index32]){
        Index segment = utf32[index32]  < 0x0000007F ? 1 : ( utf32[index32] < 0x000007FF ? 2 : (utf32[index32] < 0x0000FFFF ? 3 : 4));
        char* p = (char*)&utf32[index32];
        p[0] = p[1] = p[2] = p[3] = 0; // shit
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
            (*utf8)[index8 + 1] = (((c2 << 2) >> 2) << 2) | (c1 >> 6) | 0b10000000; // shit
            (*utf8)[index8 + 2] = (((c3 << 4) >> 4) << 2) | (c2 >> 6) | 0b11100000;
            index8 += 3;
        }break;// 0000 0800 - 0000 FFFF    1110xxxx 10xxxxxx 10xxxxxx
        case 4:{
            (*utf8)[index8] = ((c1 << 2) >> 2) | 0b10000000;
            (*utf8)[index8 + 1] = (((c2 << 2) >> 2) << 2) | (c1 >> 6) | 0b10000000;// shit
            (*utf8)[index8 + 2] = (((c3 << 2) >> 2) << 2) | (c2 >> 6) | 0b10000000;//shit
            (*utf8)[index8 + 3] = (((c4 << 5) >> 5) << 2) | (c3 >> 6) | 0b11110000; 
        }break;// 0001 0000 - 0010 FFFF    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        default:break;
        }

        index32 ++;
    }

    return l;
}
*/ 

inline bool is_sci_number(const Unicode* number,const Size& length){
    if(number){
        Size l = length ? length : string_length<Unicode>(number);
        if(l){
            Index index;
            while(number[index]){
                if(number[index] == 'e' || number[index] == 'E') return true;
                index++;
            }
        }
    }

    return false;
}

inline bool is_float_number(const Unicode* number,const Size& length){
    if (number) {
        Size l = length ? length : string_length<Unicode>(number);
        if (l) {
            Index index;
            while (number[index]) {
                if (number[index] == '.') return true;
                index++;
            }
        }
    }

    return false;
}

inline Size end_of_number(const Unicode* number){
    Index index;
    while(number[index] && number[index] >= '0' && number[index] <= '9'){
        index ++;
    }

    return index - 1;
}

inline NumberSystem number_system_of_string(const Unicode* number,const Size& length){
    if(!number) return NS_NULL;
    Size l = length ? length : string_length<Unicode>(number);
    if(number && l){
        switch(number[0]){
        case '0':{
            switch(number[1]){
            case 'b':{return NS_BIN;}break;//二进制
            case 'B':{return NS_BIN;}break;//二进制
            case 'x':{return NS_HEX;}break;//十六进制
            case 'X':{return NS_HEX;}break;//十六进制
            default:{
                if(number[2] >= '0' && number[2] <= '9'){ // 八进制
                    return NS_OCT;
                }
            }break;
            }
        }break;
        default:{
            if(is_sci_number(number,length)){
                return NS_SCI;
            }else if(is_float_number(number,length)){
                return NS_DEC;
            }
        }break;
        }
    }

    return NS_NULL;
}

#define UNICODE_SIZE sizeof(wchar_t)

String::String(){
    content = 0;
    mLength = 0;
}
String::String(const char* string) {
    this->content = 0;
    this->mLength = utf_8_32((unsigned char*)string,&this->content);
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
    Size l = string.length() + mLength;
    this->content = new Unicode[l + 1];
    if(this->content){
        string_copy<Unicode>(this->content,string.content);
        this->content[l] = 0;
        this->mLength = l;
    }
}

String& String::operator = (const String& string){
    if(this->content) { delete this->content; this->content = 0; }
    Size l = string.length();
    this->content = new Unicode[l + 1];
    if (this->content) {
        string_copy<Unicode>(this->content, string.content);
        this->content[l] = 0;
        this->mLength = l;
    }

    return *this;
}

long long make_number(const Unicode* number,const Size& length = 0){
    NumberSystem ns = number_system_of_string(number,length);
    long long r;
    switch(ns){
    case NS_BIN:{
    }break;
    case NS_OCT:{}break;
    case NS_DEC:{}break;
    case NS_HEX:{}break;
    case NS_SCI:{}break;
    default:break;
    }
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
String::operator char*(){
    if(this->content && this->mLength){
        char* utf8 = 0;
        this->as(&utf8);
        return utf8;
    }

    return 0;
}

bool String::operator == (const char* bytes){
    bool r = false;

    Unicode* unicode;
    utf_8_32((unsigned char*)bytes,&unicode);
    
    if(unicode){
        r = string_compare<Unicode>(this->content,unicode);
        delete unicode;
    }

    return r;
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
    return content ? utf_32_8(this->content, (unsigned char**)string) : 0;
}


String& String::contact(const String& s){
    Size tl = this->length();
    Size sl = s.length();
    // 内存泄露
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
                string_copy<Unicode>(s.content,&content[ls + 1],i - ls);
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
