#ifndef CMD_HPP
#define CMD_HPP

class CMD
{
    public:
        CMD();
        ~CMD();
        void run();

    protected:
        int state;
};

#endif // CMD_HPP
