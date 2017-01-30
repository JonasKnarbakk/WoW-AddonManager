#ifndef GUI_HPP
#define GUI_HPP

#include <gtk/gtk.h>
#include <string>

class GUI{
    private:
        GtkWidget * m_Window;
        unsigned int m_Width;
        unsigned int m_Height;
    public:
        GUI(int * argc, char * argv[]);
        ~GUI();

        void setTitle(std::string title);
        void setDefaultSize(unsigned int width, unsigned int height);
        void setWidth(unsigned int width);
        void setHeigth(unsigned int heigth);
        void setResizable(bool resizable);
};

#endif
