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

// ---- md5ժҪ��ϣ ---- //
void md5(const std::string& srcStr, std::string& encodedStr, std::string& encodedHexStr);

// ---- sha256ժҪ��ϣ ---- //  
void sha256(const std::string& srcStr, std::string& encodedStr, std::string& encodedHexStr);

// ---- des�ԳƼӽ��� ---- //  

// ���� ecbģʽ  
std::string des_encrypt(const std::string& clearText, const std::string& key);

// ���� ecbģʽ  
std::string des_decrypt(const std::string& cipherText, const std::string& key);


// ---- rsa�ǶԳƼӽ��� ---- //  
#define KEY_LENGTH  2048               // ��Կ����
#define PUB_KEY_FILE "pubkey.pem"    // ��Կ·��
#define PRI_KEY_FILE "prikey.pem"    // ˽Կ·��

// ��������������Կ�� 
void generateRSAKey(std::string strKey[2]);

// �����з������ɹ�˽Կ�ԣ�begin public key/ begin private key��
// �ҵ�openssl�����й��ߣ���������
// openssl genrsa -out prikey.pem 1024 
// openssl rsa - in privkey.pem - pubout - out pubkey.pem

// ��Կ����  
std::string rsa_pub_encrypt(const std::string& clearText, const std::string& pubKey);

// ˽Կ����  
std::string rsa_pri_decrypt(const std::string& cipherText, const std::string& priKey);

class data_openssl
{
public:

private:


};

#endif // !SECURITY_H

