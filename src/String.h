/**
 * String :: from char short int long etc .
 * 
 * 拼接,剪切
 */
#ifndef _STRING_H_foxintango
#define _STRING_H_foxintango
#include <libcpp/libcpp.h>

namespaceBegin(foxintango)

typedef char32_t Unicode;

class foxintangoAPI String {
protected:
    Size mLength;// 检查length赋值情况
    Unicode* content;
public:
    String();
    String(const char* string);
    String(const char& number);
    String(const unsigned char& number);
    String(const short& number);
    String(const unsigned short& number);
    String(const int& number);
    String(const unsigned int& number);
    String(const float& number);
    String(const long& number);
    String(const double& number);
    virtual ~String();
public:
    String(const String& string);
    String& operator = (const String& string);
public:
    operator char();
    operator unsigned char();
    operator short();
    operator unsigned short();
    operator int();
    operator unsigned int();
    operator float();
    operator long();
    operator double();

    bool operator ==(const char* bytes);
    bool operator ==(const String& string);

    bool operator ==(const char& v);
    bool operator ==(const unsigned char& v);
    bool operator ==(const short& v);
    bool operator ==(const unsigned short& v);
    bool operator ==(const int& v);
    bool operator ==(const unsigned int& v);
    bool operator ==(const float& v);
    bool operator ==(const long& v);
    bool operator ==(const double& v);
    String& operator +=(const String& s);

    Size as(char** string);

    String& contact(const String& s);
    Size split(Array<String>& array,char* spliter, const int& max);
public:
    const Unicode* unicode() const;
    Size length() const ;
};
namespaceEnd
#endif // !_EVENT_TARGET_H_
