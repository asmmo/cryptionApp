#ifndef OSTREAM_H
#define OSTREAM_H


struct OStream
{
    OStream(){}
    virtual void write(const char* buff, std::size_t buff_size)=0;
    virtual void flush()=0;
};

#endif // OSTREAM_H
