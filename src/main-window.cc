
#include <glibmm/miscutils.h>
#include <giomm/file.h>
#include <giomm/fileenumerator.h>
#include <list>
#include "main-window.hh"

namespace toyfb {

    using std::list;
    using std::string;

    namespace {

        bool
        open_file_with_xdg(const string& file)
        {
            bool   result;
            gchar* quoted_filename = g_shell_quote (file.c_str());
            gchar* command = g_strconcat ("xdg-open ", quoted_filename, NULL);

            g_free (quoted_filename);
            result = g_spawn_command_line_async (command, NULL);
            g_free (command);

            return result;
        }

    } // anonymous namespace

    MainWindow::MainWindow()
    {
        // Getting a pixbuf from Images created from stock icons
        // does not use the supported storage type
        pixbuf_file_ = image_file_.render_icon(Gtk::Stock::FILE,
                Gtk::ICON_SIZE_LARGE_TOOLBAR);
        pixbuf_dir_ = image_dir_.render_icon(Gtk::Stock::DIRECTORY,
                Gtk::ICON_SIZE_LARGE_TOOLBAR);

        set_border_width(10);
        set_size_request(400, 300);

        add(container_);

        scrolled_window_.add(icon_view_);

        scrolled_window_.set_policy(Gtk::POLICY_AUTOMATIC,
                                    Gtk::POLICY_AUTOMATIC);

        container_.pack_start(scrolled_window_);

        // list/treemodel->iconview magic
        list_model_ = Gtk::ListStore::create(columns_);
        list_model_->set_sort_column(columns_.col_description,
                                     Gtk::SORT_ASCENDING);


        icon_view_.set_model(list_model_);
        icon_view_.set_markup_column(columns_.col_description);
        icon_view_.set_pixbuf_column(columns_.col_pixbuf);
        //

        icon_view_.signal_item_activated().connect(
                sigc::mem_fun(*this, &MainWindow::on_item_activated));

        show_all_children();
        change_dir(Glib::get_home_dir());
    }

    MainWindow::~MainWindow()
    {
    }

    void
    MainWindow::on_item_activated(const Gtk::TreeModel::Path& path)
    {
        Gtk::TreeModel::iterator iter = list_model_->get_iter(path);
        Gtk::TreeModel::Row row = *iter;

        const string filename = row[columns_.col_filename];
        const string file_path = current_dir_ + G_DIR_SEPARATOR + filename;

        Glib::RefPtr<Gio::File> item = Gio::File::create_for_path(file_path);
        Glib::RefPtr<Gio::FileInfo> info = item->query_info();

        Gio::FileType type = info->get_file_type();

        if (type == Gio::FILE_TYPE_DIRECTORY) {
            change_dir(file_path);
        } else if (type == Gio::FILE_TYPE_REGULAR) {
            open_file_with_xdg(file_path);
        }
    }

    void
    MainWindow::clear_items()
    {
        Gtk::TreeModel::Children children = list_model_->children();

        while (children.size() > 0) {
            list_model_->erase(children.begin());
            children = list_model_->children();
        }
    }

    void
    MainWindow::add_parent_dir()
    {
        Gtk::TreeModel::Row row = *(list_model_->append());
        row[columns_.col_filename] = "..";
        row[columns_.col_description] = "↩";

        row[columns_.col_pixbuf] = pixbuf_dir_;
    }

    void
    MainWindow::add_item(const Glib::RefPtr<Gio::FileInfo>& info)
    {
        Gio::FileType type = info->get_file_type();
        string name = info->get_name();
        bool render = true;
        Glib::RefPtr<Gdk::Pixbuf> pixbuf;

        if (name[0] == '.') return;

        if (type == Gio::FILE_TYPE_DIRECTORY) {
            pixbuf = pixbuf_dir_;
        } else if (type == Gio::FILE_TYPE_REGULAR) {
            pixbuf = pixbuf_file_;
        } else {
            g_debug("%s is neither a file or directory",
                    name.c_str());
            render = false;
        }


        if (! render) return;

        Gtk::TreeModel::Row row = *(list_model_->append());
        row[columns_.col_filename] = name;
        row[columns_.col_description] = name;

        row[columns_.col_pixbuf] = pixbuf;
    }

    void
    MainWindow::change_dir(const string& dir_path)
    {
        Glib::RefPtr<Gio::File> dir = Gio::File::create_for_path(dir_path);

        try {
            Glib::RefPtr<Gio::FileEnumerator> enumerator =
                dir->enumerate_children(); // TODO: async

            clear_items();
            add_parent_dir();

            while (true) {
                Glib::RefPtr<Gio::FileInfo> info = enumerator->next_file();
                if (! info) break;
                add_item(info);
            }

            current_dir_ = dir_path;
            
        } catch (Glib::Exception& ex) { // could be a Gio:Error as well
            g_warning("Failed to get contents of %s",
                      dir_path.c_str());

            Gtk::MessageDialog dialog(*this,
                    "Failed to get contents of " + dir_path,
                    false, // no markup
                    Gtk::MESSAGE_ERROR);

            dialog.run();
        }
    }

} // namespace toyfb
