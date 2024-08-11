#include "SceneBackend.hpp"
#include <QGuiApplication>
#include <QtQml>
#include <QtQuick/QQuickView>
#include <iostream>
#include <string>
#include "arg.hpp"
#include <signal.h>

int main(int argc, char** argv) {
    argparse::ArgumentParser program("scene-viewer");
    setAndParseArg(program, argc, argv);

    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<scenebackend::SceneObject>("scenetest", 1, 0, "SceneViewer");
    QQuickView view;
	{
		auto [w_width, w_height] = program.get<Resolution>(OPT_RESOLUTION);
		view.setWidth(w_width);
		view.setHeight(w_height);
		view.setResizeMode(QQuickView::SizeRootObjectToView);
		view.setSource(QUrl("qrc:///pkg/main.qml"));
	}

    QObject*                   obj = view.rootObject();
    scenebackend::SceneObject* sv  = obj->findChild<scenebackend::SceneObject*>();
    if (program.get<bool>(OPT_VALID_LAYER)) sv->enableVulkanValid();
    if (program.get<bool>(OPT_GRAPHVIZ)) sv->enableGenGraphviz();
    sv->setProperty("assets", QUrl::fromLocalFile(program.get<std::string>(ARG_ASSETS).c_str()));
    sv->setProperty("source", QUrl::fromLocalFile(program.get<std::string>(ARG_SCENE).c_str()));
    sv->setProperty("fps", program.get<int32_t>(OPT_FPS));
    sv->setAcceptMouse(true);
    sv->setAcceptHover(true);

    char *fd_str = getenv("PIPE_FD");
    if (fd_str != nullptr) {
        int fd = static_cast<int>(strtol(fd_str, nullptr, 10));
        unsigned long window_id = view.winId();

        write(fd, &window_id, sizeof(window_id));
        close(fd);

        raise(SIGSTOP);
    }

    std::cout << "show view..." << std::endl;

    view.show();
    return QGuiApplication::exec();
}
