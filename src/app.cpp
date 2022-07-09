#include "include/app.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Window.H>
#include <cstdio>
#include <string>

using std::string;

App::App() {
    // Make the app window and menu bar w/callbacks
    win = new Fl_Double_Window(440, 540, "ER a AFD");
    menubar = new Fl_Menu_Bar(0, 0, win->w(), 25);
    regex_in = new Fl_Input(70, 40, 250, 25, "Regex: ");
    build_btn = new Fl_Return_Button(325, 40, 95, 25, "Construir");
    line_in = new Fl_Input(70, 70, 250, 25, "Prueba: ");
    validate_btn = new Fl_Button(325, 70, 95, 25, "Validar");
    buff = new Fl_Text_Buffer();
    disp = new Fl_Text_Display(20, 120, 440-40, 540-120-20, "Tabla");

    disp->buffer(buff);
    build_btn->callback(build_re_cb , (void*)this);

    menubar->add("Archivo/Cerrar", FL_CTRL + 'q', menu_callback, (void*)this);
    menubar->add("Ayuda/Ayuda", 0, menu_callback, (void*)this);
    menubar->add("Ayuda/Acerca de", 0, menu_callback, (void*)this);
    win->end();
    win->show();
}

// Static menu callback
void menu_callback(Fl_Widget*w, void*data) {
    App *o = (App*)data;
    o->menu_actions();
}

// Callback method with class access
void App::menu_actions() {
    char picked[80];
    menubar->item_pathname(picked, sizeof(picked)-1);

    if ( strcmp(picked, "Archivo/Cerrar") == 0 ) exit(0);
    if ( strcmp(picked, "Ayuda/Ayuda") == 0 ) printf("Help goes here\n");
}

void build_re_cb(Fl_Widget * w, void* data){
    string regex;
    string table;
    App * temp = (App * ) data;

    regex = temp->regex_in->value();
    
    temp->afd = Automata(regex);
    table = temp->afd.getTable();

    printf("%s", table.c_str());
    temp->buff->text(table.c_str());

}
