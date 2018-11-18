#include <gtk/gtk.h>
#include <locale.h>

gchar *utf8(char *string) {
    return g_convert(string, -1, "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
}

static void fechaDialogo(GtkDialog *dialog, gint response_id, gpointer user_data) {
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void janelaSobre(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
    GtkWidget *dialogo_sobre = gtk_about_dialog_new();
    const gchar *autores[] = {"Gustavo Silva Medeiros", NULL};

    gtk_window_set_modal(GTK_WINDOW(dialogo_sobre), 1);
    gtk_window_set_position(GTK_WINDOW(dialogo_sobre), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialogo_sobre), gdk_pixbuf_new_from_file("./img/icon-128.png", NULL));
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialogo_sobre), "Sistema Hotel Campina Confort Premium");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialogo_sobre), "1.0.0");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialogo_sobre), utf8("Projeto de conclusão da disciplina de Laboratório de Programação I, ministrada pelo professor Danilo Abreu, do curso de Ciências da Computação da Universidade Estadual da Paraíba - UEPB."));
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialogo_sobre), "Copyright \xc2\xa9 2018 Gustavo Silva Medeiros");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialogo_sobre), "Github");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialogo_sobre), "https://github.com/ghustavosm/hotelcampinaconfort");
    gtk_about_dialog_add_credit_section(GTK_ABOUT_DIALOG(dialogo_sobre), "Criado por:", autores);
    g_signal_connect(GTK_DIALOG(dialogo_sobre), "response", G_CALLBACK(fechaDialogo), NULL);

    gtk_widget_show(dialogo_sobre);
}

static void janelaPrincipal(GtkApplication *app, gpointer user_data) {
    GtkWidget *janela_principal;

    janela_principal = gtk_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(janela_principal), 600, 600);
    gtk_window_set_position(GTK_WINDOW(janela_principal), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_title(GTK_WINDOW(janela_principal), "Sistema Hotel Campina Confort Premium");

    gtk_widget_show_all(janela_principal);
}

static void sairPrograma(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
    GApplication *application = user_data;
    g_application_quit(application);
}

static void barraDeMenu(GApplication *app, gpointer user_data) {
    GMenu *menu;
    GSimpleAction *botao_sobre;
    GSimpleAction *botao_sair;

    /* cria o menu */
    menu = g_menu_new();
    g_menu_append(menu, "Sobre", "app.sobre");
    g_menu_append(menu, "Sair", "app.sair");

    botao_sobre = g_simple_action_new("sobre", NULL);
    g_signal_connect(botao_sobre, "activate", G_CALLBACK(janelaSobre), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(botao_sobre));

    botao_sair = g_simple_action_new("sair", NULL);
    g_signal_connect(botao_sair, "activate", G_CALLBACK(sairPrograma), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(botao_sair));

    gtk_application_set_app_menu(GTK_APPLICATION(app), G_MENU_MODEL(menu));
}

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
