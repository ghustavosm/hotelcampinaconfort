#include <gtk/gtk.h>
#include <locale.h>
#include "interface.h"

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Portuguese");

    int status;
    GtkApplication *app;
    GList *icons = NULL;

    /* icones das janelas */
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-16.png", NULL));
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-32.png", NULL));
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-48.png", NULL));
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-64.png", NULL));
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-128.png", NULL));
    gtk_window_set_default_icon_list(icons);

    /* inicia o app */
    app = gtk_application_new("hotel.campina.confort", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(janelaPrincipal), NULL);
    g_signal_connect(app, "startup", G_CALLBACK(barraDeMenu), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
