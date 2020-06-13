#ifndef INDENT_H
#define INDENT_H
#include <iostream>
#include <cstdio>

using std::cout;
using std::ostream;
using std::streambuf;
using std::endl;

namespace mpcs {
namespace v1 {
int indent_spaces{4};
class IndentStreambuf: public streambuf
{
public:
    IndentStreambuf(streambuf *buffer) :
        buffer(buffer), total_indent(0), begin_line(true){}
    virtual streambuf::int_type overflow(streambuf::int_type value)
    {
        if (begin_line == true) {
            for (int i = 0; i < total_indent; i++) buffer->sputc(' ');
            begin_line = false;
        }
        if (value == '\n') {
            begin_line = true;
        } 
        else {
            begin_line = false;
        }
        return buffer->sputc(value);
    };
    virtual void indent() {
        total_indent += indent_spaces;
    }
    virtual void unindent() {
        if (total_indent > 0) total_indent -= indent_spaces;
    }
private:
    streambuf *buffer;
    int total_indent;
    bool begin_line; 
};



class IndentStream: public ostream
{
public:
    IndentStream(ostream &stream) :
        ostream(buf = new IndentStreambuf(stream.rdbuf())) {}
    void indent() {
        buf->indent();
    }
    void unindent() {
        buf->unindent();
    }
    virtual ~IndentStream()
    {
        delete rdbuf();
    }
private:
    IndentStreambuf* buf;
};

ostream &
indent(ostream &os)
{
    IndentStream* indent_os = dynamic_cast<IndentStream*>(&os);
    indent_os->indent();
    return os;
}

ostream &
unindent(ostream &os)

{
    IndentStream* indent_os = dynamic_cast<IndentStream*>(&os);
    indent_os->unindent();
    return os;
}
} // v1

using namespace v1;
} // mpcs



#endif