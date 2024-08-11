#include <string>
#include <cerrno>

#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#include <signal.h>
#include <unistd.h>
#include <poll.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: sceneviewer-wrapper <assets> <scene>");
        return 1;
    }

    setenv("GDK_BACKEND", "x11", 1);

    int fds[2];

    pipe(fds);

    pid_t pid = fork();
    if (!pid) {
        close(fds[0]);
        std::string fd_str = std::to_string(fds[1]);
        setenv("PIPE_FD", fd_str.c_str(), 1);
        execlp("./sceneviewer-qml", "./sceneviewer-qml", argv[1] , argv[2], nullptr);
        exit(1);
    } else {
        close(fds[1]);

        gtk_init(&argc, &argv);

        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "GTK Application");
	    gtk_window_set_default_size(GTK_WINDOW(window), 1600, 900);
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), nullptr);

        GtkWidget *socket = gtk_socket_new();
        gtk_widget_show(socket);
        gtk_container_add(GTK_CONTAINER(window), socket);
        gtk_widget_realize(socket);

        uint64_t window_id = 0;
        
        pollfd pfd {
            .fd = fds[0],
            .events = POLLIN
        };
        poll(&pfd, 1, -1);
        read(fds[0], &window_id, sizeof(window_id));
        close(fds[0]);

        printf("window id from pipe: %lu\n", window_id);

        gtk_socket_add_id(GTK_SOCKET(socket), window_id);

        kill(pid, SIGCONT);

        gtk_widget_show(window);
        gtk_main();
    }

    return 0;
}

