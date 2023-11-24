/**
 * String :: from char short int long etc .
 * 
 * 拼接,剪切
 */
#ifndef _STRING_H_foxintango
#define _STRING_H_foxintango
#include <libcpp/libcpp.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
namespaceBegin(foxintango)
/** some thing funny.
 */
class C1 {
public:
    C1() {}
    ~C1() {};
public:
    void print() {
        printf("Here C1 printing.\n");
    }
};
class C2 {
public:
    C2() {}
    ~C2() {};
public:
    void print() {
        printf("Here C2 printing.\n");
    }
};

template <class T>
class C :public T {
public:
    C() {
        this->print();
    }
    ~C() {}
};
/** Number System
 * 
 */
typedef enum _NumberSystem{
    NS_NULL, // 非数值
    NS_BIN,  // 二进制
    NS_OCT,  // 八进制
    NS_DEC,  // 十进制
    NS_HEX,  // 十六进制
    NS_SCI   // 科学计数法
}NumberSystem;

enum Encoding { UTF8, UTF16, UTF32 };
class String;
class foxintangoAPI UTF8 {
friend class String;
protected:
    char* m_content;
public:
    UTF8();
    UTF8(const char*   urf8);
    UTF8(const UTF8&   utf8);
    UTF8(const String& string);
    ~UTF8();
public:
    UTF8& operator = (const UTF8& utf8);
    UTF8& operator = (const String& string);
public:
    char* content();
};

class foxintangoAPI UTF16 {
    friend class String;
protected:
    char16_t* m_content;
public:
    UTF16();
    UTF16(const char*   urf16);
    UTF16(const UTF16&  utf16);
    UTF16(const String& string);
    ~UTF16();
public:
    UTF16& operator = (const UTF16& utf16);
    UTF16& operator = (const String& string);
public:
    char16_t* content();
};
/** Endian -- Little
 */
class foxintangoAPI String {
public:
protected:
    Endian   m_endian;
    Size     m_length;// 检查length赋值情况
    Unicode* m_content;
public:
    String();
    String(const Unicode* unicode);
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

    /**
     * should be deleted by user. 
     */
    operator char*();
    bool operator ==(const char* bytes);
    bool operator ==(const Unicode* unicode);
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

    bool operator >(const String& v);
    bool operator <(const String& v);

    String operator +(const String& string);
    String operator +(const Unicode* unicode);
    String operator +(const Unicode& unicode);
    String operator +(const char* string);
    String operator +(const char& number);
    String operator +(const unsigned char& number);
    String operator +(const short& number);
    String operator +(const unsigned short& number);
    String operator +(const int& number);
    String operator +(const unsigned int& number);
    String operator +(const float& number);
    String operator +(const long& number);
    String operator +(const double& number);

    String& operator +=(const String& string);
    String& operator +=(const Unicode* unicode);
    String& operator +=(const Unicode& unicode);
    String& operator +=(const char* string);
    String& operator +=(const char& number);
    String& operator +=(const unsigned char& number);
    String& operator +=(const short& number);
    String& operator +=(const unsigned short& number);
    String& operator +=(const int& number);
    String& operator +=(const unsigned int& number);
    String& operator +=(const float& number);
    String& operator +=(const long& number);
    String& operator +=(const double& number);

    Unicode operator[](const Index& index);

    Size as(char** string) const;

    String& contact(const String& s);
    Size split(Array<String>& array,char* spliter, const int& max);
    Error load(const char* path);
public:
    const Unicode* unicode() const;
    Size length() const;
    Endian endian() const;
public:
    NumberSystem numberSystem(char* number);
};
namespaceEnd
#endif // !_EVENT_TARGET_H_
