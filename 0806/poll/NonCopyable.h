#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_

class Noncopyable
{
    public:
        Noncopyable();
        ~Noncopyable();
    private:
        Noncopyable(const Noncopyable &);
        void operator = (const Noncopyable &);
};

#endif
