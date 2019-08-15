QT += network gui widgets concurrent

HEADERS += \
    downloader.h \
    downloadwindow.h \
    include/QtPromise \
    src/qtpromise/qpromise.h \
    src/qtpromise/qpromise_p.h \
    src/qtpromise/qpromiseconnections.h \
    src/qtpromise/qpromiseexceptions.h \
    src/qtpromise/qpromisefuture.h \
    src/qtpromise/qpromiseglobal.h \
    src/qtpromise/qpromisehelpers.h \
    src/qtpromise/qpromisehelpers_p.h \
    src/qtpromise/qpromiseresolver.h

SOURCES += \
    downloader.cpp \
    main.cpp \
    downloadwindow.cpp \
    src/qtpromise/qpromise.inl

FORMS += \
    downloadwindow.ui

INCLUDEPATH += include(include/QtPromise)
