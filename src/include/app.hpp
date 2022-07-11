#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include "automata.hpp"

class App {
    private:
        Fl_Double_Window *win;
        Fl_Menu_Bar *menubar;
        Fl_Text_Display *disp;
        Fl_Button *validate_btn;
        Fl_Return_Button *build_btn;


    public:
        Automata afd;
        Fl_Input *regex_in;
        Fl_Input *line_in;
        Fl_Text_Buffer *buff;

        App();
        void menu_actions();
};

void menu_callback(Fl_Widget*w, void*data);
void build_re_cb(Fl_Widget * w, void* data);
void validate_cb(Fl_Widget * w, void* data);

#endif
