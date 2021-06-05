#ifndef ISTREAM_H
#define ISTREAM_H


struct IStream
{
    IStream(){}
    virtual std::size_t read_some(char* buff, std::size_t buff_size)=0;
};

#endif // ISTREAM_H
