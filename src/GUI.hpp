#ifndef GUI_HPP
#define GUI_HPP

#include <gtk/gtk.h>
#include <string>

class GUI{
    private:
        GtkWidget *m_Window;
        GtkWidget *m_TabView;
        GtkWidget * m_ScrolledWindow;
        GtkWidget *m_InstalledContainer;
        GtkWidget *m_SettingsContainer;
        unsigned int m_Width;
        unsigned int m_Height;

        void setResizable(bool resizable);
        void addTabbedView();
        void addTabSearch();
        void addTabInstalled();
        void addTabSettings();
        void addScrollArea(GtkWidget *container);
        void addTopLabels(GtkWidget *container);
        void addSearchInput(GtkWidget *container);
    public:
        GUI(int * argc, char * argv[]);
        ~GUI();

        static GtkWidget *searchContainer;
        static GtkWidget *spinner;
        void setTitle(std::string title);
        void setDefaultSize(unsigned int width, unsigned int height);
        void setWidth(unsigned int width);
        void setHeigth(unsigned int heigth);
        static void addAddon(std::string name, std::string version, std::string supported, std::string image, std::string downloads);
};

#endif
