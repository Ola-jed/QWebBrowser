# QWebBrowser  [![CodeFactor](https://www.codefactor.io/repository/github/ola-jed/qwebbrowser/badge)](https://www.codefactor.io/repository/github/ola-jed/qwebbrowser)


QWebBrowser is a simple web browser written in c++ with qt
## How to build ?

You need to have [qt](https://www.qt.io/download-qt-installer) installed  to build the app. (Don't forget to add qmake to the path)

```bash
# Compile in debug mode
git clone https://github.com/Ola-jed/QWebBrowser.git
cd QWebBrowser
cmake --build cmake-build-debug --target QWebBrowser -- -j 3
cmake-build-debug/QWebBrowser
```
Just import the project if you use an qt creator or visual studio.
