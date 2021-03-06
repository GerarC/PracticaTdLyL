#include "include/app.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Window.H>
#include <FL/fl_ask.H>
#include <cstdio>
#include <string>

using std::string;

App::App(int width, int height, string title){
    win = new Fl_Double_Window(width, height, title.c_str());
    menubar = new Fl_Menu_Bar(0, 0, win->w(), 25);
    regex_in = new Fl_Input(70, 40, width - 190, 25, "Regex: ");
    build_btn = new Fl_Return_Button(width - 115, 40, 95, 25, "Construir");
    line_in = new Fl_Input(70, 70, width - 190, 25, "Prueba: ");
    validate_btn = new Fl_Button(width - 115, 70, 95, 25, "Validar");
    buff = new Fl_Text_Buffer();
    disp = new Fl_Text_Display(20, 120, width-40, height-120-20, "Información del Autómata");

    disp->buffer(buff);
    build_btn->callback(build_re_cb, (void*) this);
    validate_btn->callback(validate_cb, (void*) this);

    menubar->add("Archivo/Cerrar", FL_CTRL + 'q', menu_callback, (void*)this);
    menubar->add("Ayuda/Ayuda", 0, menu_callback, (void*)this);
    menubar->add("Ayuda/Acerca de", 0, menu_callback, (void*)this);
    win->end();
    win->show();

}

// Callback method with class access
void App::menu_actions() {
    char picked[80];
    menubar->item_pathname(picked, sizeof(picked)-1);

    if ( strcmp(picked, "Archivo/Cerrar") == 0 ) exit(0);
    if ( strcmp(picked, "Ayuda/Ayuda") == 0 ){
        string help;

    }
    if ( strcmp(picked, "Ayuda/Acerca de") == 0 ){
        string text = "Esta aplicación fue hecha para la práctica I\n";
        text += "de la materia Teoría de Lenguajes y Laboratorio.\n";
        text += "\nHecho por Gerardo E. Castillo.";

        fl_message("%s", text.c_str());
    }
}

void build_re_cb(Fl_Widget * w, void* data){
    string regex;
    string info;
    App * temp = (App *) data;

    regex = temp->regex_in->value();
    
    temp->af = Automata(regex);
    info = temp->af.getInfo();

    info += temp->af.getTable();

    printf("%s", info.c_str());
    temp->buff->text(info.c_str());
}

void validate_cb(Fl_Widget * w, void* data){
    string line;
    bool isValid = false;

    App * temp = (App *) data;

    line = temp->line_in->value();

    isValid = temp->af.validate(line);

    if (isValid) fl_message("La linea de caracteres es valida\n");
    else fl_message("La linea de caracteres no es valida\n");
}

// Static menu callback
void menu_callback(Fl_Widget*w, void*data) {
    App *o = (App*)data;
    o->menu_actions();
}

