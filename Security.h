#ifndef SECURITY_H

#define SECURITY_H
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "openssl/md5.h"
#include "openssl/sha.h"
#include "openssl/des.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"

#pragma comment(lib,"libcrypto.lib")

// ---- md5摘要哈希 ---- //
void md5(const std::string& srcStr, std::string& encodedStr, std::string& encodedHexStr);

// ---- sha256摘要哈希 ---- //  
void sha256(const std::string& srcStr, std::string& encodedStr, std::string& encodedHexStr);

// ---- des对称加解密 ---- //  

// 加密 ecb模式  
std::string des_encrypt(const std::string& clearText, const std::string& key);

// 解密 ecb模式  
std::string des_decrypt(const std::string& cipherText, const std::string& key);


// ---- rsa非对称加解密 ---- //  
#define KEY_LENGTH  2048               // 密钥长度
#define PUB_KEY_FILE "pubkey.pem"    // 公钥路径
#define PRI_KEY_FILE "prikey.pem"    // 私钥路径

// 函数方法生成密钥对 
void generateRSAKey(std::string strKey[2]);

// 命令行方法生成公私钥对（begin public key/ begin private key）
// 找到openssl命令行工具，运行以下
// openssl genrsa -out prikey.pem 1024 
// openssl rsa - in privkey.pem - pubout - out pubkey.pem

// 公钥加密  
std::string rsa_pub_encrypt(const std::string& clearText, const std::string& pubKey);

// 私钥解密  
std::string rsa_pri_decrypt(const std::string& cipherText, const std::string& priKey);

class data_openssl
{
public:

private:


};

#endif // !SECURITY_H

