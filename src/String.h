/**
 * String :: from char short int long etc .
 * 
 * 拼接,剪切
 */
#ifndef _STRING_H_foxintango
#define _STRING_H_foxintango
#include <libcpp/libcpp.h>

namespaceBegin(foxintango)
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

/** Endian -- Little
 */
class foxintangoAPI String {
public:
    enum Encoding{UTF8,UTF16,UTF32};
    class ASCII{};
    class UTF8{};
    class UTF16{};
    class UTF32{};
protected:
    Size m_length;// 检查length赋值情况
    Unicode* m_content;
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

    String& operator +=(const String& s);

    Size as(char** string) const;

    String& contact(const String& s);
    Size split(Array<String>& array,char* spliter, const int& max);
    Error load(const char* path);
public:
    const Unicode* unicode() const;
    Size length() const;
public:
    NumberSystem numberSystem(char* number);
};
namespaceEnd
#endif // !_EVENT_TARGET_H_
