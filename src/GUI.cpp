// Description: New Class
// Created by Jonas Knarbakk (11:21:12-30/01/2017)

#include "GUI.hpp"
#include "Core.hpp"
#include <iostream>

extern "C" void searchEntryActivated(GtkWidget *widget, GtkWidget *entry){
    Core c;
    const gchar *entry_text;
    entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
    std::string searchText = entry_text;
    c.search(searchText);
}

GtkWidget * GUI::m_SearchContainer;

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
    // Add the vertical container to the Search Tab
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_notebook_append_page(GTK_NOTEBOOK(m_TabView), vbox, gtk_label_new("Search"));

    addTopLabels(vbox);
    addScrollArea(vbox);
    addSearchInput(vbox);
}

void GUI::addTabInstalled(){
    m_InstalledContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_notebook_append_page(GTK_NOTEBOOK(m_TabView), m_InstalledContainer, gtk_label_new("Installed"));
}

void GUI::addTabSettings(){
    m_SettingsContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_notebook_append_page(GTK_NOTEBOOK(m_TabView), m_SettingsContainer, gtk_label_new("Settings"));
}

void GUI::addTopLabels(GtkWidget *container){
    // Create the top labels for the search tab
    GtkWidget *topLabels = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    GtkWidget *lImage = gtk_label_new("Image");
    GtkWidget *lName = gtk_label_new("Name");
    GtkWidget *lVersion = gtk_label_new("Version");
    GtkWidget *lSupported = gtk_label_new("Supported");
    GtkWidget *lDownloads = gtk_label_new("Total Downloads");

    gtk_box_pack_start(GTK_BOX(topLabels), lImage, true, false, 0);
    gtk_box_pack_start(GTK_BOX(topLabels), lName, true, false, 0);
    gtk_box_pack_start(GTK_BOX(topLabels), lVersion, true, false, 0);
    gtk_box_pack_start(GTK_BOX(topLabels), lSupported, true, false, 0);
    gtk_box_pack_start(GTK_BOX(topLabels), lDownloads, true, false, 0);

    // Add the topLabels container to the search tab
    gtk_box_pack_start(GTK_BOX(container), topLabels, false, false, 0);
}

void GUI::addScrollArea(GtkWidget *container){
    m_ScrolledWindow = gtk_scrolled_window_new(NULL, NULL);

    m_SearchContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(m_ScrolledWindow), m_SearchContainer);
    gtk_box_pack_start(GTK_BOX(container), m_ScrolledWindow, true, true, 0);

}

void GUI::addSearchInput(GtkWidget *container){
    GtkWidget * hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new("Search: "), false, false, 0);
    GtkWidget *searchEntry = gtk_entry_new();
    // TODO: Bind this signal to the search function that should be in its own backend class
    g_signal_connect(G_OBJECT(searchEntry), "activate", G_CALLBACK(searchEntryActivated), searchEntry);
    gtk_box_pack_start(GTK_BOX(hbox), searchEntry, true, true, 0);
    gtk_box_pack_end(GTK_BOX(container), hbox, false, false, 0);
}

void GUI::addAddon(std::string name, std::string version, std::string supported, std::string image, std::string downloads){
    // Container
    GtkWidget * hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

    // Components
    printf("Trying to load image: %s\n", image.c_str());
    GtkWidget * iThumbnail = gtk_image_new_from_file(image.c_str());
    GtkWidget * lName = gtk_label_new(name.c_str());
    GtkWidget * lVersion = gtk_label_new(version.c_str());
    GtkWidget * lSupported = gtk_label_new(supported.c_str());
    GtkWidget * lDownloads = gtk_label_new(downloads.c_str());

    // Configure components
    gtk_image_set_pixel_size(GTK_IMAGE(iThumbnail), 1);

    // Add too container
    gtk_box_pack_start(GTK_BOX(hbox), iThumbnail, true, true, 0);
    gtk_box_pack_start(GTK_BOX(hbox), lName, true, true, 0);
    gtk_box_pack_start(GTK_BOX(hbox), lVersion, true, true, 0);
    gtk_box_pack_start(GTK_BOX(hbox), lSupported, true, true, 0);
    gtk_box_pack_start(GTK_BOX(hbox), lDownloads, true, true, 0);

    // Add too search tab
    gtk_box_pack_start(GTK_BOX(m_SearchContainer), hbox, false, false, 0);
    gtk_widget_show_all(m_SearchContainer);
}
