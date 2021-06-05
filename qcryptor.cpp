#include "qcryptor.h"
#include <QDebug>
QCryptor::QCryptor(QObject *parent) :QObject(parent), cryptor{Crypt<CRYPT::BOTH>{}}{}
void QCryptor::setKey(const QString & key){
    cryptor.setKey(key.toStdString());

}

void QCryptor::setIV(const QString & iv){
    cryptor.setInitilizationVector(iv.toStdString());
}

bool QCryptor::crypt( void(Crypt<CRYPT::BOTH>::* crypt)(IStream&, OStream&)) {
    errorMsg = "";
    try{
        auto fileToBeEncrypted = std::ifstream{(cryptionSrc.find("file://")!=std::string::npos)?cryptionSrc.substr(6):cryptionSrc};
        if(!fileToBeEncrypted) {
            errorMsg = QString("Couldn't open the source file");
            return false;
        }
        auto encryptedFile = std::ofstream{(cryptionDest.find("file://")!=std::string::npos)?cryptionDest.substr(6):cryptionDest};
        if(!encryptedFile) {
            errorMsg = QString("Couldn't create/open the destination file");
            return false;
        }

        auto src = Input{fileToBeEncrypted};
        auto dest = Output{encryptedFile};

        (cryptor.*(crypt))(src, dest);
    }catch(const CryptException& exc){
        errorMsg = QString(exc.what());
        return false;
    }
    return true;
}

bool QCryptor::encrypt() {
    return crypt(&Crypt<CRYPT::BOTH>::encrypt);
}

bool QCryptor::decrypt() {
    return crypt(&Crypt<CRYPT::BOTH>::decrypt);

}

void QCryptor::setCryptionSrc(const QString & src){
    cryptionSrc = src.toStdString();
}

void QCryptor::setCryptionDest(const QString & dest){
    cryptionDest = dest.toStdString();
}

bool QCryptor::isSrcEmpty() const{
    return cryptionSrc.empty();
}

bool QCryptor::isDestEmpty() const{
    return cryptionDest.empty();
}

QString QCryptor::getError() const{
    return errorMsg+ "\nPlz, Check the key, iv and the chosen files";
}
