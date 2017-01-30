// Description: New Class
// Created by Jonas Knarbakk (11:21:12-30/01/2017)

#include "GUI.hpp"
#include <iostream>

GUI::GUI(int * argc, char * argv[]){
    gtk_init(argc, &argv);
    
    m_Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    m_Width = 500;
    m_Height = 500;
    setDefaultSize(m_Width, m_Height);
    setResizable(true);

    addTabbedView();
    addTabSearch();
    addTabInstalled();
    addTabSettings();
    addSearch();
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

void GUI::addTabbedView(){
    m_TabView = gtk_notebook_new();
    gtk_widget_set_name(GTK_WIDGET(m_TabView), "tabview");
    gtk_container_add(GTK_CONTAINER(m_Window), m_TabView);
}

void GUI::addTabSearch(){
    m_SearchContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    GtkWidget * scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(m_SearchContainer), scrolledWindow, false, false, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(m_TabView), m_SearchContainer, gtk_label_new("Search"));
}

void GUI::addTabInstalled(){
    m_InstalledContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    GtkWidget * scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(m_InstalledContainer), scrolledWindow, false, false, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(m_TabView), m_InstalledContainer, gtk_label_new("Installed"));
}

void GUI::addTabSettings(){
    m_SettingsContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    GtkWidget * scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(m_SettingsContainer), scrolledWindow, false, false, 0);
    gtk_notebook_append_page(GTK_NOTEBOOK(m_TabView), m_SettingsContainer, gtk_label_new("Settings"));
}

void GUI::addSearch(){
    GtkWidget * hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new("Search:"), false, false, 0);
    GtkWidget *searchEntry = gtk_entry_new();
    // TODO: Bind this signal to the search function that should be in its own backend class
    g_signal_connect(G_OBJECT(searchEntry), "activate", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), searchEntry, false, false, 0);
    gtk_box_pack_end(GTK_BOX(m_SearchContainer), hbox, false, false, 0);
}
