#include <QDebug>
#include "crypt.h"
/**
  * Inititiaize the contexts using the provided key and initialization vector according to the provided parameters
**/
template<CRYPT mode>
Crypt<mode>::Crypt(const std::string& key_, const std::string& initializationVector_):
    key{key_},initializationVector{initializationVector_},
    encryptionContext{(mode!=CRYPT::DECRYPT)? EVP_CIPHER_CTX_new():nullptr},
    decryptionContext{(mode!=CRYPT::ENCRYPT)? EVP_CIPHER_CTX_new():nullptr}
{

}

template<CRYPT mode>
Crypt<mode>::Crypt():
    key{},initializationVector{},
    encryptionContext{(mode!=CRYPT::DECRYPT)? EVP_CIPHER_CTX_new():nullptr},
    decryptionContext{(mode!=CRYPT::ENCRYPT)? EVP_CIPHER_CTX_new():nullptr}
{

}

template<CRYPT mode>
void Crypt<mode>::init(){
    if(key.size()!=32)
        throw CryptException{"The key size is "+ std::to_string(key.size())+ " while it must be 32 bytes (256 bits)."};
    if(initializationVector.size()!=16)
        throw CryptException{"The initialization vector is "+ std::to_string(initializationVector.size())+ " while it must be 16 bytes (128 bits)."};
    if(encryptionContext){
        EVP_CIPHER_CTX_init(&*encryptionContext);
        EVP_EncryptInit_ex(
            &*encryptionContext,
            EVP_aes_256_cbc(),
            nullptr,
            reinterpret_cast<const unsigned char*>(key.c_str()),
            reinterpret_cast<const unsigned char*>(initializationVector.c_str())
            );
    }
    if(decryptionContext){
        EVP_CIPHER_CTX_init(&*decryptionContext);
        EVP_DecryptInit_ex(
            &*decryptionContext,
            EVP_aes_256_cbc(),
            nullptr,
            reinterpret_cast<const unsigned char*>(key.c_str()),
            reinterpret_cast<const unsigned char*>(initializationVector.c_str())
            );
    }

}

/**
  * Used to encrypt only. It's available on ENCRYPT and BOTH modes only
**/
template<CRYPT mode>
template<class> requires (mode!=CRYPT::DECRYPT)
void Crypt<mode>::encrypt(IStream& input, OStream& output) {
    std::basic_string<unsigned char> plaintext;
    plaintext.resize(KB);
    int outLength;

    std::string encryptedText;
    encryptedText.resize(KB + AES_BLOCK_SIZE);

    // allows reusing of 'e' for multiple encryption cycles
    init();
    size_t current_buff_size =0;

    //reading the bytes chunck by chunck then writing the encrypted data the output
    while((current_buff_size=input.read_some((char*)plaintext.data(), KB))){
        if(!EVP_EncryptUpdate(
                encryptionContext,
                reinterpret_cast<unsigned char*>(encryptedText.data()),
                &outLength,
                plaintext.c_str(),
                current_buff_size
                ))
            handle_errors();
        output.write(encryptedText.c_str(), outLength);
    }

    // finalizing the remaining bytes and then writing the last encrypted chunck then flushing
    if(!EVP_EncryptFinal_ex(
            encryptionContext,
            reinterpret_cast<unsigned char*>(encryptedText.data()),
            &outLength)
        ){
        handle_errors();
    }
    output.write(encryptedText.c_str(), outLength);
    output.flush();
}


/**
  * Used to decrypt only. It's available on DECRYPT and BOTH modes only
**/
template<CRYPT mode>
    template<class>requires (mode!=CRYPT::ENCRYPT)
    void Crypt<mode>::decrypt(IStream& input, OStream& output) {
    std::basic_string<char> encryptedText;
    encryptedText.resize(KB);
    int outLength;

    std::string decryptedText;
    decryptedText.resize(KB);

    // allows reusing of 'e' for multiple encryption cycles
    init();

    size_t current_buff_size =0;
    while((current_buff_size=input.read_some(encryptedText.data(), KB))){

        //reading the bytes chunck by chunck then writing the decrypted data the output
        if(!EVP_DecryptUpdate(/*c_len plaintext len*/
                decryptionContext,
                reinterpret_cast<unsigned char*>(decryptedText.data()),
                &outLength,
                reinterpret_cast<const unsigned char*>(encryptedText.c_str()),
                current_buff_size
                ))
            handle_errors();
        output.write(decryptedText.c_str(), outLength);
    }

    // finalizing the remaining bytes and then writing the last decrypted chunck then flushing
    if(!EVP_DecryptFinal_ex(decryptionContext, reinterpret_cast<unsigned char*>(decryptedText.data()), &outLength))
        handle_errors();
    output.write(decryptedText.c_str(), outLength);
    output.flush();
}

template<CRYPT mode>
void Crypt<mode>::handle_errors(){
    std::string error_info;
    error_info.resize(100);
    ERR_error_string_n(ERR_get_error(), error_info.data(), 100);

    throw CryptException{error_info};
}

template<CRYPT mode>
void Crypt<mode>::setKey(const std::string & key_){
    key = key_;

}


template<CRYPT mode>
void Crypt<mode>::setInitilizationVector(const std::string & initilizationVector_){
    initializationVector = initilizationVector_;

}

template<CRYPT mode>
Crypt<mode>::EnvelopCipherContext::EnvelopCipherContext(EnvelopCipherContext&& lhs)noexcept:ptr_{lhs.ptr_}{
    lhs.ptr_ = nullptr;
    qDebug()<<"f";
}




//instanting the needed functions, classes
template class Crypt<CRYPT::BOTH>;
template void Crypt<CRYPT::BOTH>::encrypt<void>(IStream&, OStream&);
template void Crypt<CRYPT::BOTH>::decrypt<void>(IStream&, OStream&);
template class Crypt<CRYPT::ENCRYPT>;
template void Crypt<CRYPT::ENCRYPT>::encrypt<void>(IStream&, OStream&);
template class Crypt<CRYPT::DECRYPT>;
template void Crypt<CRYPT::DECRYPT>::decrypt<void>(IStream&, OStream&);
