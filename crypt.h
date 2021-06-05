#ifndef CRYPT_H
#define CRYPT_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <stdexcept>
#include <string>
#include<istream.h>
#include<ostream.h>

enum class CRYPT {
    ENCRYPT,
    DECRYPT,
    BOTH
};

struct CryptException: std::runtime_error{
    using std::runtime_error::runtime_error;
};

template<CRYPT mode>
class Crypt
{
    class EnvelopCipherContext{
        EVP_CIPHER_CTX* ptr_;
    public:
        EnvelopCipherContext(EnvelopCipherContext&&)noexcept;
        explicit EnvelopCipherContext(EVP_CIPHER_CTX* ptr):ptr_{ptr}{}
        operator EVP_CIPHER_CTX*(){
            return ptr_;
        }
        ~EnvelopCipherContext(){
            EVP_CIPHER_CTX_free(ptr_);
        }
    };

    inline static constexpr size_t KB = 1024;
    std::string key;
    std::string initializationVector;
    EnvelopCipherContext encryptionContext;
    EnvelopCipherContext decryptionContext;
    void handle_errors();
    void init();
public:
    Crypt(const std::string&, const std::string&);
    Crypt();
    Crypt(const Crypt&) = delete;
    Crypt(Crypt&&)noexcept = default;
    template<class=void> requires (mode!=CRYPT::DECRYPT)
    void encrypt(IStream&, OStream&);
    template<class=void> requires (mode!=CRYPT::ENCRYPT)
    void decrypt(IStream&, OStream&);
    void setKey(const std::string &);
    void setInitilizationVector(const std::string &);

};
#endif // CRYPT_H
