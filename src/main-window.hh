
#ifndef __TOYFB_MAIN_WINDOW__
#define __TOYFB_MAIN_WINDOW__

#include <gtkmm.h>
#include <giomm/fileinfo.h>
#include <string>

namespace toyfb {

    class MainWindow : public Gtk::Window
    {
    public:
        MainWindow();
        virtual ~MainWindow();

    protected:

        // TreeModel for the simple directory view
        class ModelColumns : public Gtk::TreeModel::ColumnRecord
        {
        public:
            ModelColumns()
            {
                add(col_filename);
                add(col_description);
                add(col_pixbuf);
            }

            Gtk::TreeModelColumn<std::string> col_filename;
            Gtk::TreeModelColumn<Glib::ustring>  col_description;
            Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > col_pixbuf;
        };

        // widget signal handlers
        virtual void on_item_activated(const Gtk::TreeModel::Path& path);

        // our functions
        void change_dir(const std::string& dir);
        void clear_items();
        void add_parent_dir();
        void add_item(const Glib::RefPtr<Gio::FileInfo>& info);

        // widgets
        Gtk::VBox container_;
        Gtk::ScrolledWindow scrolled_window_;

        ModelColumns columns_;
        Glib::RefPtr<Gtk::ListStore> list_model_;
        Gtk::IconView icon_view_;

        Gtk::Image image_file_;
        Gtk::Image image_dir_;
        Glib::RefPtr<Gdk::Pixbuf> pixbuf_file_;
        Glib::RefPtr<Gdk::Pixbuf> pixbuf_dir_;

        std::string current_dir_;
    };

} // namespace toyfb

#endif // __TOYFB_MAIN_WINDOW__
