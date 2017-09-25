#-------------------------------------------------
#
# Project created by QtCreator 2017-04-23T18:03:41
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#INCLUDEPATH += -I/usr/local/include/opencv -I/usr/local/include/tesseract -I/usr/local/include/leptonica
#LIBS += -L/usr/local/lib -llept -ltesseract -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -lopencv_videoio -lopencv_imgcodecs -lopencv_video



#라즈베리에 서되 는설정
INCLUDEPATH += /usr/local/include/opencv /usr/local/include/leptonica /usr/local/include/tesseract
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -llept -ltesseract

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    client_thread.cpp \
    bath_room.cpp \
    kitchen.cpp \
    living_room.cpp \
    multipurpose_room.cpp \
    room1.cpp \
    room2.cpp \
    base_room.cpp \
    ect.cpp \
    entrance.cpp \
    graph.cpp \
    graph_thread.cpp \
    database.cpp \
    living_room_table.cpp \
    kitchen_table.cpp \
    room1_table.cpp \
    room2_table.cpp \
    bath_room_table.cpp \
    multipurpose_room_table.cpp \
    entrance_table.cpp \
    etc_table.cpp \
    vehicle_registration_table.cpp \
    vehicle_registration.cpp \
    add_vehicle_dlg.cpp \
    modify_vehicle_dlg.cpp \
    login_registration.cpp \
    add_login_dlg.cpp \
    modify_login_dlg.cpp \
    login_registration_table.cpp \
    open_cv.cpp \
    open_cv_thread.cpp

HEADERS  += mainwindow.h \
    server.h \
    client_thread.h \
    living_room.h \
    kitchen.h \
    room1.h \
    room2.h \
    bath_room.h \
    multipurpose_room.h \
    base_room.h \
    etc.h \
    graph.h \
    graph_thread.h \
    database.h \
    living_room_table.h \
    kitchen_table.h \
    room1_table.h \
    room2_table.h \
    multipurpose_room_table.h \
    bath_room_table.h \
    entrance_table.h \
    etc_table.h \
    vehicle_registration_table.h \
    vehicle_registration.h \
    add_vehicle_dlg.h \
    modify_vehicle_dlg.h \
    add_login_dlg.h \
    login_registration.h \
    login_registration_table.h \
    modify_login_dlg.h \
    open_cv.h \
    open_cv_thread.h

FORMS    += mainwindow.ui
