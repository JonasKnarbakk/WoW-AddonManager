// Description: New Class
// Created by Jonas Knarbakk (11:21:12-30/01/2017)

#include "GUI.hpp"

GUI::GUI(int * argc, char * argv[]){
    gtk_init(argc, &argv);
    
    m_Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    m_Width = 500;
    m_Height = 500;
    setDefaultSize(m_Width, m_Height);
    setResizable(false);

    // Bind events
    g_signal_connect(G_OBJECT(m_Window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Show all and start the gtk main loop
    gtk_widget_show_all(m_Window);
    gtk_main();
}

GUI::~GUI(){
}

void GUI::setTitle(std::string title){
    gtk_window_set_title(GTK_WINDOW(m_Window), title.c_str());
}

void GUI::setDefaultSize(unsigned int width, unsigned int height){
    gtk_window_set_default_size(GTK_WINDOW(m_Window), width, height);
}

void GUI::setWidth(unsigned int width){
    gtk_window_resize(GTK_WINDOW(m_Window), width, m_Height);
}

void GUI::setHeigth(unsigned int height){
    gtk_window_resize(GTK_WINDOW(m_Window), m_Width, height);
}

void GUI::setResizable(bool resizable){
    gtk_window_set_resizable(GTK_WINDOW(m_Window), resizable);
}
