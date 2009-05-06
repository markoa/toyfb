
#include <iostream>
#include <gtkmm/main.h>
#include "main-window.hh"

int
main(int argc, char** argv)
{
    Gtk::Main kit(argc, argv);

    toyfb::MainWindow main_window;

    Gtk::Main::run(main_window);

    return 0;
}
