#include <gtk/gtk.h>
#include <locale.h>
#include "interface.c"

int main(int argc, char **argv) {
    setlocale(LC_ALL, "Portuguese");

    int status;
    GtkApplication *app;
    GList *icons = NULL;

    if(carregarClientes()) {
        printf("Arquivo clientes carregado com sucesso!\n");
    } else {
        printf("Erro ao carregar arquivo clientes!\n");
    }

    if(carregarTipoQuartos()) {
        printf("Arquivo tipo_quartos carregado com sucesso!\n");
    } else {
        printf("Erro ao carregar arquivo tipo_quartos!\n");
    }

    if(carregarQuartos()) {
        printf("Arquivo quartos carregado com sucesso!\n");
    } else {
        printf("Erro ao carregar arquivo quartos!\n");
    }

    if(carregarServicos()) {
        printf("Arquivo servicos carregado com sucesso!\n");
    } else {
        printf("Erro ao carregar arquivo servicos!\n");
    }

    adicionarCliente("Gustavo", "75429843067", "Rua Manuel Leonardo Gomes");
    adicionarCliente("Larissa", "52901248020", "Rua Henry Leandro Leonardo Dias");
    adicionarCliente("Tiago", "17745320003", "Rua Levi César Aparício");
    adicionarCliente("João", "91646177002", "Rua Martin Caio Souza");
    adicionarCliente("Iris", "04513040041", "Rua Bruno Pedro Caleb Campos");
    adicionarCliente("José", "72389066003", "Rua Luís Bernardo da Mota");
    adicionarCliente("Lucas", "89773759083", "Rua Yago Otávio Lopes");
    adicionarCliente("Augusto", "19753735043", "Rua Aprígio Nepomuceno");
    adicionarCliente("Yago", "29753735043", "Rua Aprígio Nepomuceno");

    //imprimirClientes();



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
