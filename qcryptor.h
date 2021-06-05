#ifndef QCRYPTOR_H
#define QCRYPTOR_H

#include <QObject>
#include <crypt.h>
#include <fstream>
#include <istream.h>
#include <ostream.h>

struct Input:IStream{
    std::ifstream& file;
    explicit Input(std::ifstream& f):file{f}{}
    std::size_t read_some(char* buff, std::size_t buff_size)override{
        return file.readsome(buff, buff_size);
    }
};

struct Output:OStream{
    std::ofstream& file;
    explicit Output(std::ofstream& f):file{f}{}
    void write(const char* buff, std::size_t buff_size)override{
        file.write(buff, buff_size);
    }
    void flush()override{
        file.flush();
    }

};


using namespace std::string_literals;
class QCryptor : public QObject
{
    Q_OBJECT
    Crypt<CRYPT::BOTH> cryptor;
    std::string cryptionSrc;
    std::string cryptionDest;
    bool crypt( void(Crypt<CRYPT::BOTH>::*)(IStream&, OStream&));
    QString errorMsg;
public:
    explicit QCryptor(QObject *parent = nullptr);
    Q_INVOKABLE void setKey(const QString &);
    Q_INVOKABLE void setIV(const QString &);
    Q_INVOKABLE void setCryptionSrc(const QString &);
    Q_INVOKABLE void setCryptionDest(const QString &);
    Q_INVOKABLE bool isSrcEmpty()const;
    Q_INVOKABLE bool isDestEmpty()const;
    Q_INVOKABLE bool encrypt();
    Q_INVOKABLE bool decrypt();
    Q_INVOKABLE QString getError() const;

};

#endif // QCRYPTOR_H
