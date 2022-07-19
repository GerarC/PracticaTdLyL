#include "include/app.hpp"

#define WIDTH 440
#define HEIGHT 540
#define TITLE "ER a AF"

int main() {
    App app = App(WIDTH, HEIGHT, TITLE); // Llamada a la aplicación
    return(Fl::run());
}
