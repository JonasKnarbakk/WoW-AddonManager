#ifndef GUI_HPP
#define GUI_HPP

#include <gtk/gtk.h>
#include <string>

class GUI{
    private:
    public:
        GUI();
        ~GUI();

        void setTitle(std::string title);
        void setDefaultSize(unsigned int width, unsigned int height);
        void setWidth(unsigned int width);
        void setHeigth(unsigned int heigth);
};

#endif
